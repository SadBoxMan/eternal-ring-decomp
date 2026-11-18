# Game-internal file seams (ER-017)

Written Wed 18 Nov 2026 by ER-017 (Opus 5.5, high). Read-only research: no build, yaml or source change. Every number
is produced by `tools/seams.py`; per-function and per-boundary tables go to `build/seams/` (gitignored, derived
from game code). Section 7 lists what is Unconfirmed. **No seam here is Confirmed**: "Strong" means several kinds of
evidence agree and the result survives changed parameters, not that a split has been built on it.

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

- **Every seam and file here is a candidate**, graded by agreement of evidence; none has been built or confirmed.
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
