# Game-internal file seams (ER-017)

Written Wed 18 Nov 2026 by ER-017 (Opus 5.5, high). Read-only research: no build, yaml or source change. Every number
is produced by `tools/seams.py`; per-function and per-boundary tables go to `build/seams/` (gitignored, derived
from game code). Section 7 lists what is Unconfirmed. **No seam here is Confirmed**: "Strong" means several kinds of
evidence agree and the result survives changed parameters, not that a split has been built on it.
**ER-022 (section 8) cut `.text` at 7 Strong seams** (S1, S9, S12, S15, S16, S17, S20) after checking their data
cuts; they stay Strong (a byte-identical build is no evidence for a seam).

## 1. Summary

- The game region (`0x001000C0`-`0x001CE130`, 2,830 functions incl. game-vu0 and game-unk) has **22 Strong, 10
  Medium and 24 Weak candidate seam windows** (section 4). A window is the range in which the first function of a
  new file lies; 8 Strong windows are a single function (the seam is fixed there by this evidence).
- The Strong windows cluster in **`0x001339F8`-`0x00150878`** (619 functions): 28 Strong or Medium windows cut it
  into 27 candidate files, most of 6-30 functions (range 2-195), using almost no strings (what this code does is
  Unconfirmed). Two more stand alone: `0x001C8EE0` (end of
  the memory-card save / load code) and `0x001CA8B0` (inside game-unk).
- Large stretches show **no seam at all**: `0x001000C0`-`0x0012B0D0` (main, boot and the first memory-card and
  voice-file code), `0x00151000`-`0x00187BD8` except one weak window, and `0x0019BA90`-`0x001B7230` apart from one
  Medium window. Either they are large files, or global data used by neighbouring files ties them together in this
  method (section 6).
- Two ideas from `docs/census.md` 4.3 are **ruled out** by test compiles with the project toolchain (section 3):
  `.text` objects are only 8-byte aligned, and `.rodata` padding to 16 bytes comes from jump tables inside one file,
  so neither marks an object start by itself.
- No source file names, `__FILE__` strings or asserts are left in the image (0 hits).
- The known census seams `0x001C9B18` (game-vu0) and `0x001CA0F0` (game-unk) fall inside a window the scan finds
  open (`0x001C9B18`-`0x001CA408`): the method does not contradict them.

## 2. Method

`tools/seams.py` (about 1 s):

```
$env:PATH = "C:\msys64\usr\bin;" + $env:PATH
make build/SLUS_200.15.syms.elf          # function symbols and splat's data labels (or reuse build/seams/symbols.json)
.venv\Scripts\python.exe tools/seams.py --md
.venv\Scripts\python.exe tools/seams.py --span 60 --k 20 --out build/seams/var_60_20     # one sensitivity run
```

Code and data bytes come from the original ELF (`disc/extract/SLUS_200.15`), symbols from the syms ELF. Data
references come from `tools/census.py`'s linear address tracker (`lui` / `addiu` / `ori` pairs, copies, `$gp`):
each reference is mapped to the splat data label that contains it.

**Inputs of the run behind this doc.** `asm/` was not read at all (ER-013's clean splits rewrite it). Bytes: the
original ELF, which no build changes. Symbols: `build/SLUS_200.15.syms.elf` as built at 00:42 Eastern on Wed 18 Nov
2026 by ER-013's last build (around commit `10593b2`, ER-013 closed), 3,609 units (the census total) and 5,585 data
labels. The list is cached in `build/seams/symbols.json`, which the tool falls back on when the syms ELF is missing.
Function symbols are named by address, so a later split changes their names only if a function moves.

| Evidence | What it uses | Kind |
|---|---|---|
| **O: source-order scan** (main) | For every boundary between two functions, the data labels (`.rodata`, `.sdata`, `.data`; `.sbss` / `.bss` as a second check) used by the K functions on each side. Labels used by functions further apart than `LOCAL_SPAN` are left out (widely shared globals, tracker misreads). **Binding**: a label used on both sides. **Inversion**: a label used only on the left lying above one used only on the right. **Static calls**: a call across the boundary to a function whose callers all lie within `LOCAL_SPAN`. A boundary is *open* when binding, inversions and static calls are all 0 in the primary sections | positive and negative |
| **C: shared strings** | GCC keeps one copy of an identical string per file (section 3), so a string used by several functions ties them into one file; the union of those intervals gives minimum file extents | negative (no cut inside) |
| **R: `.rodata` padding** | A label at a 16-byte boundary after 8+ zero bytes. Mapped to a text window through the functions using the labels on each side (median user per label; longest non-decreasing run in address order to drop misreads) | weak positive (section 3) |
| text padding, names | `nop` runs before functions; file-name-like strings | ruled out / none found |

Why O works (section 3): the compiler emits initialized variables and string literals **in source order**, each
function's strings just before it, so a file's data is a run in source order and files follow in link order. Inside
one file, a later function that uses a variable defined near the top (above an earlier function's strings) gives an
inversion, and file-static data shared by two functions gives a binding. Across a real seam neither can happen,
except through a global used by both files or a misread. Uninitialized globals become `COMMON` and land at the end
of `.bss` / `.sbss` in an order not tied to their file, which is why those two sections only serve as a second
check.

**Windows and grades.** Consecutive open boundaries form a window (the new file starts at one of its functions).
The tool grades a window Strong when it is at most 3 functions wide, has at least 3 one-sided labels on each side
and `.sbss` / `.bss` also show no binding or inversion; Medium when it is at most 8 wide with that much evidence;
Weak otherwise. The doc's final grade (section 4) then adds **stability**: the scan rerun with five other settings
(`LOCAL_SPAN` / K = 60 / 20, 240 / 40, 120 / 80, 40 / 40, 120 / 20; default 120 / 40). Final Strong = tool Strong
and open in at least 4 of the 5 variants; tool Strong with 3 of 5 drops to Medium; tool Medium stays Medium when
stable 4+ of 5 and its second check is clean, else Weak.

## 3. Toolchain facts checked for this card (test compiles, Confirmed)

Scratch C files (no game code) compiled with ee-gcc 2.9-ee-991111 `-O2 -G8` and assembled with ee-as 2.9
(`-G8 -O2 -mabi=eabi`), the project's own flags (`Makefile`):

| Fact | Consequence |
|---|---|
| Object `.text` alignment is 8 (`.align 3` per function); `.rodata` / `.sdata` / `.bss` get only the largest alignment of their items (16 / 8 / 8 in the test), `.data` 1 when empty | no forced 16-byte padding at object starts; measured over the game: before every function the padding is 0 or 1 word, never more (one 5-word gap, at the libkernl stubs) |
| Strings of 8 bytes or less (with the NUL) go to **`.sdata`**, longer ones to `.rodata`, each `.align 3` | `.sdata` holds short strings mixed with small variables |
| An identical string used twice in one file is emitted once (`$LC0` reused) | evidence C |
| Jump tables go to `.rodata` with **`.align 4`** | 16-byte padding inside a file before each jump table: 34 of the 42 game padding points have a jump table right after them |
| Initialized globals and statics are emitted where they are defined, interleaved with each function's strings (strings just before their function) | evidence O |
| Uninitialized globals: `.comm` (linker COMMON); uninitialized statics: the object's own `.sbss` / `.bss`, after its code | `.bss` / `.sbss` are partly out of file order |

## 4. Candidate seams (first function of the new file lies in the window)

Labels left / right = one-sided data labels within K functions on each side (more = more evidence). Stab. = open in
how many of the 5 variant settings. R = a `.rodata` padding point maps into the same window (its kind: RJ = a jump
table follows, so the padding alone is explained inside one file; R3 = the item before is not a string).

### 4.1 Strong (22)

Cut by ER-022 (section 8): S1, S9, S12, S15, S16, S17, S20. Not cut: the 13 multi-function windows (data does not
decide the start), S21 (a `.data` binding seen in the whole-file view), S22 (only `.bss` decides).

| # | Window | Funcs | Labels L / R | Stab. | Also |
|---|---|---|---|---|---|
| S1 | 0x001343E8 (fixed) | 1 | 23 / 8 | 4/5 | |
| S2 | 0x00134480-0x00134488 | 2 | 23 / 11 | 4/5 | |
| S3 | 0x00134BB0-0x00134DB0 | 2 | 10 / 15 | 4/5 | |
| S4 | 0x001359C0-0x00135A50 | 2 | 3 / 15 | 4/5 | |
| S5 | 0x00140AC0-0x00140C18 | 3 | 27 / 39 | 4/5 | |
| S6 | 0x001416A8-0x001416B8 | 3 | 32 / 33 | 4/5 | |
| S7 | 0x00142920-0x00142A68 | 3 | 34 / 19 | 5/5 | |
| S8 | 0x00144218-0x00144308 | 2 | 13 / 29 | 5/5 | |
| S9 | 0x00145120 (fixed) | 1 | 14 / 44 | 5/5 | |
| S10 | 0x001464A8-0x00146598 | 3 | 33 / 38 | 5/5 | R RJ 0x003549B0 |
| S11 | 0x00147018-0x00147100 | 2 | 44 / 36 | 4/5 | R RJ 0x003549D0 |
| S12 | 0x0014A968 (fixed) | 1 | 19 / 14 | 5/5 | R RJ 0x00354A50 (wide) |
| S13 | 0x0014B2C0-0x0014B3A0 | 3 | 16 / 16 | 5/5 | R RJ 0x00354A50 (wide) |
| S14 | 0x0014BF40-0x0014BF48 | 2 | 14 / 19 | 5/5 | R RJ 0x00354A50 (wide) |
| S15 | 0x0014C840 (fixed) | 1 | 19 / 28 | 5/5 | R RJ 0x00354A50 (wide) |
| S16 | 0x0014D930 (fixed) | 1 | 23 / 29 | 5/5 | R RJ 0x00354A50 (wide) |
| S17 | 0x0014E2E8 (fixed) | 1 | 26 / 23 | 5/5 | R RJ 0x00354A50 (wide) |
| S18 | 0x0014F680-0x0014F690 | 2 | 20 / 16 | 5/5 | R R3 0x00354A90 (wide) |
| S19 | 0x0014FF38-0x0014FF78 | 3 | 23 / 23 | 5/5 | R R3 0x00354A90 (wide) |
| S20 | 0x00150878 (fixed) | 1 | 14 / 52 | 5/5 | R R3 0x00354A90 (wide) |
| S21 | 0x001C8EE0 (fixed) | 1 | 9 / 53 | 5/5 | R RJ 0x00356AD0; last game file before game-vu0 starts at 0x001C9B18 |
| S22 | 0x001CA8B0-0x001CA8D8 | 2 | 53 / 70 | 5/5 | inside game-unk (census region 0x001CA0F0-0x001CE130) |

### 4.2 Medium (10)

| # | Window | Funcs | Labels L / R | Stab. | Note |
|---|---|---|---|---|---|
| M1 | 0x001339F8-0x00134088 | 6 | 26 / 3 | 4/5 | |
| M2 | 0x00134690-0x00134958 | 7 | 13 / 13 | 4/5 | |
| M3 | 0x0013F4C0-0x0013F580 | 2 | 21 / 45 | 3/5 | tool Strong, less stable |
| M4 | 0x00143358-0x00143968 | 8 | 33 / 15 | 5/5 | |
| M5 | 0x00144E68-0x00144F90 | 4 | 14 / 44 | 5/5 | |
| M6 | 0x00146EF0-0x00146FC0 | 4 | 44 / 28 | 4/5 | R RJ 0x003549D0 |
| M7 | 0x0014FC68-0x0014FE10 | 5 | 23 / 22 | 5/5 | R R3 0x00354A90 (wide) |
| M8 | 0x001501D8-0x00150350 | 7 | 21 / 35 | 5/5 | R R3 0x00354A90 (wide) |
| M9 | 0x0019B9B0-0x0019BA90 | 3 | 25 / 9 | 3/5 | tool Strong, less stable |
| M10 | 0x001AEC58-0x001AED88 | 3 | 12 / 19 | 3/5 | tool Strong, less stable |

### 4.3 Weak (24)

Open but thin (fewer than 3 one-sided labels on a side) or wide (the seam, if any, could be anywhere inside):

`0x0012B0D0-0x0012B1F0`, `0x0012B540-0x0012B6B0`, `0x0012BCE0`, `0x0012F1D8`, `0x0012F740-0x0012FD70` (24 funcs),
`0x00130180-0x001316A8` (54; R R3 0x00354570 and RJ 0x003546A0 map here), `0x00148360-0x00148F40` (15; RJ x2),
`0x001498D8-0x0014A238` (13), `0x00158C70-0x0015B250` (47), `0x00187BD8-0x00189200` (13), `0x001898F0-0x0018A4D8`
(11), `0x0018AAB8-0x001902C8` (57), `0x00190688-0x00191440` (6; tool Medium, stable 5/5, but one `.bss` label
binds, so downgraded), `0x001B7230-0x001B7358`, `0x001B75E0-0x001B7B90` (20), `0x001B8258-0x001B8608`,
`0x001B8630-0x001B9918` (39), `0x001B99B0`, `0x001C1580-0x001C1630`, `0x001C17D0`, `0x001C1880-0x001C1928`,
`0x001C9B18-0x001CA408` (32: contains the census seams 0x001C9B18 and 0x001CA0F0; the VU0 helpers use no data, so
the scan has nothing to say there), `0x001CB218-0x001CB750` (16; `.bss` disagrees), `0x001CBBC0-0x001CBE90` (tool
Medium; `.bss` shows 7 bindings and 20 inversions, so downgraded).

### 4.4 `.rodata` padding points (evidence R)

54 labels sit at a 16-byte boundary after 8+ zero bytes; 42 of them map into the game. By kind: 34 RJ (a jump table
follows: explained inside one file), 4 R3, 2 R2, 2 R1 (string, padding, string: the only kind 8-byte alignment
cannot explain). Both R1 points (`0x00354F50`, `0x00355060`) are referenced on both sides by the same function, so
even string-to-string 16-byte padding happens inside one file (cause not found; perhaps a 16-aligned array between
the strings: Unconfirmed). R therefore only adds weight where O already finds a window (column "Also"); on its own
it grades nothing above Weak. Table: `build/seams/seams.csv`, every label: `build/seams/rodata.csv`.

### 4.5 Minimum file extents (evidence C: do not cut inside)

60 strings are used by more than one game function; 51 of them by functions within 120 of each other. They form 10
clusters, each of which must lie inside one file (if the references are not misreads):

| Cluster (first to last function) | Funcs | Shared strings |
|---|---|---|
| 0x00103518-0x0010B188 | 51 | 1 |
| 0x0013DCF0-0x0013EBA8 | 15 | 1 |
| 0x00147F78-0x00148210 | 2 | 1 |
| 0x001541C0-0x00156178 | 36 | 1 |
| 0x00157C58-0x00157E40 | 3 | 1 |
| 0x00163840-0x001746C0 | 134 | 34 |
| 0x00191AF8-0x001969F8 | 72 | 1 |
| 0x0019EBC0-0x001A9948 | 97 | 1 (the two ends are a strict clone pair, census 5) |
| 0x001ABC48-0x001AE078 | 19 | 1 |
| 0x001C2448-0x001C7418 | 97 | 9 |

No window of any grade falls inside a cluster. This is a consistency check, not independent evidence: the shared
strings also count as binding in the scan O.

## 5. Candidate files (Unofficial labels; readings Unconfirmed)

From the windows above (the start of each file is the window's first candidate; where a window is wide the file
boundary is open). Subsystem readings come from the themes of the strings each stretch uses (paraphrased; no game
text copied) and are **Unconfirmed**.

| File (Unofficial) | From | To (next window) | Funcs | Reading (Unconfirmed) |
|---|---|---|---|---|
| g_boot | 0x001000C0 | 0x0012B0D0 (Weak) | 339 | main, boot, early memory-card messages, voice-file loading; no seam found inside, likely several files |
| g_12b | 0x0012B0D0 | 0x001339F8 (M1) | 225 | several Weak windows; no strings |
| g_134a .. g_150 | 0x001339F8 | 0x00150878 (S20) | 619 in 27 files | one file per window S1-S20 / M1-M8; almost no strings (one stretch with ring-making messages near 0x001359C0-0x0013F4C0); small data-driven units |
| g_151 | 0x00150878 | 0x00158C70 (Weak) | 163 | few strings (element / rate labels) |
| g_ui | 0x00158C70 | 0x00187BD8 (Weak) | 488 | menu / status / equipment screens (character, attributes, equipment strings); cluster 0x00163840-0x001746C0 (134 funcs, 34 shared strings) is one large file |
| g_187 | 0x00187BD8 | 0x00190688 (Weak) | 86 | no strings |
| g_190 | 0x00190688 | 0x0019B9B0 (M9) | 141 | contains cluster 0x00191AF8-0x001969F8 |
| g_19b | 0x0019B9B0 | 0x001AEC58 (M10) | 180 | contains cluster 0x0019EBC0-0x001A9948 (clone pair) |
| g_1ae | 0x001AEC58 | 0x001B7230 (Weak) | 82 | a level-up message |
| g_1b7 | 0x001B7230 | 0x001C1580 (Weak) | 231 | several Weak windows |
| g_save | 0x001C1580 | 0x001C8EE0 (S21) | 168 | memory-card save / load (load / save failure and card-check messages, icon file path); cluster 0x001C2448-0x001C7418 |
| g_1c8 | 0x001C8EE0 | 0x001C9B18 | 3 | the last game file before the VU0 helpers |
| game-vu0, game-unk | 0x001C9B18 | 0x001CE130 | 105 | census regions; S22 splits game-unk at 0x001CA8B0-0x001CA8D8 |

Function counts are from the first candidate of one window to the first candidate of the next (a window's own
width can move up to 8 functions across a boundary). The file count inside the run `0x001339F8`-`0x00150878` is the
number of Strong / Medium windows there; Weak windows inside it are not counted as files.

## 6. What would confirm or refute

- **Data splits line up (strongest test).** For a Strong seam, every one-sided label on the left must lie below
  every one on the right in `.rodata`, `.sdata` and `.data`; a follow-up can compute the data cut address per
  section and check that splat can split each section there. If a section's left and right labels interleave at a
  real seam, the seam is wrong.
- **First matched functions.** A static function (called only inside its file) must sit on the same side as all its
  callers; `static` data must stay with its users. Matching around a seam will show it: a function that needs a
  file-static variable from the other side refutes the cut.
- **Literal pools.** `.lit4` gives no seam (census 4.3), but the order of `.lit4` entries is function order; a seam
  cut inside a function's literal run would break the build.
- **Tracker misreads.** The address tracker is linear (no control flow): a stale base register can invent a
  reference, which can bind or invert a real seam away (false negatives) far more often than it can open a false
  one. The large seam-less stretches (section 1) are where this, or globals shared by neighbouring files, may hide
  seams; a control-flow-aware tracker (or m2c output) would sharpen the scan.
- **Globals shared by neighbours.** A global used only by two adjacent files looks like file-static data and binds
  them; `LOCAL_SPAN` 40 opens more windows (99), 240 fewer (43). Seams that appear only at small spans are
  candidates, not findings.
- **SDK check.** Running the same scan over the SDK libraries, whose object boundaries are partly known
  (`docs/census.md` 4.1), would measure how often the method finds a known seam; not done in this card.

## 7. Not verified, Unconfirmed and Unofficial items

- **Every seam and file here is a candidate**, graded by agreement of evidence; none is confirmed. ER-022 built
  7 of them (section 8): the build proves only that the cut links, not that it is the original file boundary.
- ER-022 (section 8): the far labels behind the full-view inversions are read as globals of other files or
  tracker misreads (Unconfirmed); the 3 `.rodata` labels near the end of `.rodata` may be SDK data (Unconfirmed);
  `0x0034D1B0`-`0x0034D1C4` as one object (Unconfirmed); the data cut addresses in 8.2 are where the referenced
  labels put them, not checked by a data split.
- **Unconfirmed readings**: all subsystem readings in section 5 (from string themes); "small data-driven units" for
  the string-less run; the cause of the two R1 paddings (perhaps a 16-aligned array).
- **Unofficial labels**: the file labels `g_*` in section 5; the evidence names O, C, R, RJ, R1-R3; "window",
  "binding", "inversion", "open".
- **Assumptions**: the original source was compiled with the same compiler and flags as our test compiles
  (`docs/toolchain.md`: Confirmed for the compiler family and `-G8`, the exact flags are Unconfirmed); source files
  define variables before or between functions in the usual C way (a file defining all variables at the end would
  look different).
- The grade thresholds (width 3 / 8, 3 labels, stability 4 of 5) and `LOCAL_SPAN` / K defaults are our choices, not
  measured optima.
- Function counts in section 5 move by up to a window's width (section 5 note).

## 8. The split at the Strong seams (ER-022, Wed 18 Nov 2026)

ER-022 (Opus 5.5, high) checked the data cut of every Strong window (`tools/seams.py --cuts`), cut `.text` at the 7
seams that pass and left the other 15 uncut. `make regress` is green on the split (whole-ELF SHA-1 and all 3,606
functions; the 125 C functions of the old game file sit in their new files and still match).

### 8.1 The check

For a set of text cuts (the census seams `0x001C9B18` / `0x001CA0F0` plus the seams under test), each cut is
checked per data section: the labels used only by the file before it (L) must all lie below the labels used only
by the file after it (R), and no label may be used by both (a binding). Labels count as in the scan O (referencers
within `LOCAL_SPAN` = 120 functions). Two views: **full** (the two whole neighbouring files) and **local** (only the
labels used within K = 40 functions of the cut, the scan's own view). The data cut lies in (end of the highest L
label, lowest R label]; `n` = how many splat labels start in that range (1 = pinned to one address). A window
candidate is "decided by data" when some candidates fail and others pass. Output: `build/seams/cuts_<set>.csv`;
`--cut-set all` (every Strong seam at its window's first candidate, every candidate tried), `fixed`, `cut` (the
seams ER-022 cut, `CUT_STARTS` in the tool).

```
.venv\Scripts\python.exe tools/seams.py --cuts --cut-set all
.venv\Scripts\python.exe tools/seams.py --cuts --cut-set cut
```

**Why the full view fails where one side is a long uncut stretch.** If one side is several hundred functions (the
first game file before S1, the stretch after S20), it is many real files, and a handful of labels that lie far
from the seam's data and are used only far (> 40 functions) from the cut give inversions with everything on the
other side. Every full-view inversion at the cut seams was traced to such labels (`tools/seams.py` plus a scratch
listing of the pairs):

| Seam | Full-view inversions | All of them come from |
|---|---|---|
| S1 | `.rodata` 99, `.sdata` 284, `.data` 15 (pairs) | 3 `.rodata` labels near the end of `.rodata` (`0x00357DD8`, `0x00357E70`, `0x00358188`, above all game `.rodata`; perhaps SDK data: Unconfirmed) used by `func_001000C0`, `func_00101F20`, `func_00119DF0`; 2 `.sdata` labels (`0x0035C188`, `0x0035BF1C`) used by `func_00119DF0`, `func_001316A8`; 1 `.data` label (`0x00351378`) used by `func_00127868` |
| S20 | `.sdata` 21, `.data` 45 | 1 `.sdata` label `0x0035B8D8` (near the start of `.sdata`) used by `0x00176BA0`-`0x00177098`; 9 `.data` labels in `0x001F6BB0`-`0x00205B70` (the start of `.data`) used by functions from `0x001583F8` to `0x001C16C0` |

In every case the label sits below (or above) the whole data run of both files and is used only by code at least
40 functions away: a global defined in some other file (or a tracker misread), not data of either file at the
seam. The local view is clean at all 7 cut seams, and no cut seam has a binding in either view.

### 8.2 Seams cut (7)

| Seam | First function (file) | Funcs in file | Data cut: `.rodata` / `.sdata` / `.data` | Note |
|---|---|---|---|---|
| S1 | 0x001343E8 (`text_001343E8.c`) | 338 | `0x003546D8` / none / `0x0032FD20` (local view) | full view: far labels only (8.1); the file runs to S9 because S2-S8 stay uncut |
| S9 | 0x00145120 (`text_00145120.c`) | 105 | `0x00354980`-`0x00354990` (2) / `0x0035BC94` / `0x0032FF20` | clean in both views |
| S12 | 0x0014A968 (`text_0014A968.c`) | 52 | none / `0x0035BDD8` / `0x0032FF90` | clean |
| S15 | 0x0014C840 (`text_0014C840.c`) | 30 | none / `0x0035BE3C` / `0x0032FFB0` | clean |
| S16 | 0x0014D930 (`text_0014D930.c`) | 23 | none / `0x0035BE90` / none | clean |
| S17 | 0x0014E2E8 (`text_0014E2E8.c`) | 59 | `0x00354A30`-`0x00354A50` (2) / `0x0035BEC8` / none | clean |
| S20 | 0x00150878 (`text_00150878.c`) | 1,542 | `0x00354A90`-`0x00354AA0` (2) / `0x0035BF2C` (local) / `0x00330208` (local) | full view: far labels only (8.1); the file runs to the census seam 0x001C9B18 |

"none" = one side uses no label of that section near the cut, so the data says nothing there. `.sbss` / `.bss`
(second check) show no binding at S9-S20; at S1 the full view has 24 `.bss` inversions (`.bss` mixes COMMON,
whose order is not tied to files; local view clean). **Link order**: per section, the data cuts of the 7 seams rise in the
same order as the text cuts (`.sdata` `0x0035BC94` < `BDD8` < `BE3C` < `BE90` < `BEC8` < `BF2C`; `.data`
`0x0032FD20` < `FF20` < `FF90` < `FFB0` < `0x00330208`; `.rodata` `0x003546D8` < `4980` < `4A30` < `4A90`): the tool
prints "in order".

The old game file `text_001000C0.c` keeps `func_001000C0` to `func_00134358` (576 units, 22 of them C).

**ER-014 matches next to the cut seams (Wed 18 Nov 2026): neutral, no evidence for or against.** Nine functions
right at a seam were matched and placed: `func_00134460` (2nd function after S1), `func_001450F8` (last before S9),
`func_0014A940` (2nd-last before S12), `func_0014C810` / `func_0014C820` (last two before S15), `func_0014D918`
(last before S16), `func_0014E2C8` (last before S17), `func_00150810` / `func_00150818` (last two before S20).
None of them needs a file-static variable or a static function in a way the code can show:

- At `-O2` this ee-gcc does not inline non-`inline` functions, so a `static` helper and a global one compile to
  the same `jal` / `j`; a file-static variable and a global one give the same `$gp` access. The matched bytes are
  the same either way.
- `.data` / `.sdata` / `.rodata` / `.lit4` are still one subsegment each (8.4), so the C files define no data:
  every global is `extern`. A match that defines its own data (or literals or jump tables, which the C file's
  object would carry: `docs/decomp_recipes.md` 2.2) cannot be placed until the data is split, and that is where
  real seam evidence will come from.
- Call locality (all `jal` callers of each helper lie in its own file: `func_0014C810` from `0x0014C380` /
  `0x0014C3F8`, `func_0014D918` from `0x0014D7A0`, `func_0014E2C8` from `0x0014E120` / `0x0014E160`,
  `func_00134460` from `0x001343E8`, `func_001450F8` from `0x00144F90` / `0x00145038`, `func_00150810` /
  `func_00150818` from `0x00150350` / `0x00150400`) agrees with the cuts but is the scan's own "static calls"
  evidence (O), not new. `func_00150810` / `func_00150818` read `D_0035BF20` (`.sdata`, 100000.0f), below
  S20's `.sdata` cut `0x0035BF2C`: consistent, same evidence kind.

Grades unchanged.

### 8.3 Seams not cut (15)

| Seams | Why not |
|---|---|
| S2-S8, S10, S11, S13, S14, S18, S19 (13 windows of 2-3 functions) | every candidate start passes the check in both views, so the data does not decide which function opens the file; the card cuts a window only where data decides. The seams themselves line up (link order in order with `--cut-set all`) |
| S21 (0x001C8EE0, fixed) | **binding**: the `.data` label `0x0034D1B0` is used by the save / load code (`0x001C4FF0`, `0x001C5120`, `0x001C53D8`, `0x001C56F0`) and by `0x001C92D0` and `0x001C9680` in the would-be new file (the labels `0x0034D1B8` and `0x0034D1C4` just after it are used only by those two; one object: Unconfirmed); the scan's K = 40 view misses it because they are more than 40 functions apart. It is either a global shared by two files (then the seam stands) or the save / load file reaches to the VU0 helpers; matching will tell. Also 4 full-view `.rodata` inversions from labels at `0x00356B58`-`0x00358630` used by code at `0x001750F0`-`0x001A9948` (far labels, as in 8.1) |
| S22 (0x001CA8B0-0x001CA8D8, inside game-unk) | only `.bss` decides the start (4 `.bss` labels, `0x01B31D80`, `0x01B31DB0`, `0x01B31DB4`, ..., bind `0x001CA8B0` to the functions before it, so the file would start at `0x001CA8D8`); `.bss` mixes COMMON (global, order not tied to files), so this is not enough on its own; the primary sections have only one `.data` label near the cut |

### 8.4 Data not split

No file between two cuts has a pinned cut at both ends in all three primary sections (every file has at least one
"none" or a 2-label range at one end), so by the card's rule the data stays as ER-003 left it: one subsegment per
section. The cut addresses in 8.2 are where each file's data starts where the data shows it. Two further reasons to
wait: a plain `rodata` / `sdata` subsegment at those addresses would not attach the data to the C file (that needs
splat's `.rodata`-style subsegments named after the file, which change how splat emits the asm for INCLUDE_ASM
functions: untested), and the uncut window seams inside S1's and S20's files would leave those files' data runs
open anyway.

### 8.5 Grades

No grade changes. The byte-identical build is no evidence for a seam: objects are 8-aligned and every function's
asm starts `.align 3`, so a cut on any function start links unchanged (`docs/census.md` 4.3). The data line-up of
8.1 is a stricter use of the same evidence O (the same labels and tracker), not independent evidence. The 7 cut
seams therefore stay **Strong** (now "cut"); evidence that would confirm them: a matched function needing a
file-static variable or a static function that stays on its own side (section 6), or a data split that matches.
S21 stays Strong in the table of 4.1 with the binding noted here; whether it should drop to Medium is left to the
seam-method follow-up (ER-023).
