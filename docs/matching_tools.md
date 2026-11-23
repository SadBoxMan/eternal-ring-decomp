# EE matching tools (ER-015)

Written Thu 19 Nov 2026 by ER-015 (Opus 5.5, medium). Tools for matching one function at a time on this game's
toolchain (ee-gcc 2.9-ee-991111, `-O2 -G8 -fno-strict-aliasing`, patched ee-as `-r2`; `docs/decomp_recipes.md` 1).
All run natively from PowerShell with the repo's venv, in the main checkout or a worktree (`docs/build.md` 6). They
need a split and the unstripped build (`build/SLUS_200.15.syms.elf`): any `make regress` leaves both.

A masked compare is a working aid, never proof (ruling `docs/decomp_recipes.md` 3.1). **The proof is the function
placed in `src/` and `make regress` green.**

| Tool | Job |
|---|---|
| `tools/cmpfunc.py` | compile C with the build's settings, compare functions with the original, report differences and relocations |
| `tools/place.py` | swap a function's `INCLUDE_ASM` line for C in the right `src/` file, and back; extract a placed function to a scratch file |
| `tools/clones.py` | list a function's strict clone family (`tools/census.py` keys) and try its placed C on every member |
| `tools/eematch.py` | the shared code (Makefile settings, compile, object reader, compare, report) |
| m2c (`mipsee` target) | first drafts from splat's asm (section 5); not in this repo, run from Twinblade's copy |
| `tools/m2c_bulk.py` | m2c in bulk (ER-050): draft, clean, compile and compare many functions, per-function result table (section 6) |

## 1. The loop

```
.venv\Scripts\python.exe -E -s "C:\Users\lukea\Claude\Projects\Project Twinblade\tools\m2c\m2c.py" -t mipsee-gcc-c asm\nonmatchings\text_001000C0\func_00131090.s
#   write build\scratch_in\func_00131090.c: #include "common.h", the externs, the function
.venv\Scripts\python.exe -I tools/cmpfunc.py build/scratch_in/func_00131090.c
#   edit until MATCH
.venv\Scripts\python.exe -I tools/place.py c func_00131090 build/scratch_in/func_00131090.c
.venv\Scripts\python.exe -I tools/regress.py
```

`build/scratch_in/` is gitignored (under `build/`), so scratch files never reach git. If regress fails, put the asm
back with `tools/place.py asm func_X`.

## 2. `tools/cmpfunc.py`

```
python -I tools/cmpfunc.py <file.c> [func ...] [--addr 0x...] [--full] [--context N] [--cflags "..."]
```

- **A scratch file** (anywhere outside `src/`) is compiled with exactly the Makefile's `CC`, `CFLAGS`, `CPPFLAGS`
  and `AS` / `ASFLAGS` (read through `make --eval`, so a flag change in the Makefile reaches the tool at once) into
  `build/scratch/`. With no names, every `func_XXXXXXXX` in it is compared.
- **A `src/` file** is built with the real rule (`make build/src/<file>.o`); name the function(s).
- `--addr` for a function not named after its address; `--full` prints every word with its relocation note;
  `--cflags` adds gcc flags to a scratch compile (experiments only: a flag in the build needs a ruling).
- Exit 0 = every compared function matches, 1 = a difference, 2 = compile error (gcc's message is printed).

**What it compares.** Our function's bytes (size from the symbol, trailing alignment zeros dropped) against the
original at its address, for the length of the game's function (its size from the syms ELF, so a length difference
shows). Words without a relocation must be equal. Where a relocation applies, the fixed part of the instruction
must be equal and the game's target is resolved and checked:

| Relocation | Check |
|---|---|
| `R_MIPS_26` (`jal`, `j`) | the game's target equals the symbol's address (`func_` / `D_` name, or a name of the syms ELF) |
| `R_MIPS_HI16` + `R_MIPS_LO16` | the game's `%hi` / `%lo` pair gives the symbol's address plus our addend. A `%lo` with no `lui` of its symbol pending before it (a `lui` shared between branches, `func_0014DDD8` +0x94) is paired with the last `HI16` of the same symbol seen, else the first one anywhere in the function; the note says "shares the HI16 at +0x..." (ER-065; before it, "no HI16 before it" and a false DIFF) |
| `R_MIPS_GPREL16` | `_gp` (0x00360B70) plus the game's offset equals the symbol's address (`.sdata` / `.data` symbols of an owned run are found by name in our object) |
| `R_MIPS_LITERAL` (`.lit4`) | the game's literal word has the same value as ours; its place in `.lit4` is judged by `make regress` |
| section-relative `.rodata` (jump tables, strings) | not checkable here; listed under "not checkable" with the game's address |

**Report.** `MATCH` or `DIFF` with the first difference and the count, then ours | game side by side in R5900
disassembly (rabbitizer), `!!` on each differing word, and a note under each relocation. Example (a wrong offset):

```
DIFF   func_0014D918 at 0x0014d918: ours 0x18 bytes, game 0x18: first difference at +0x8, 1 word(s) differ
     +0004  8c830130 lw          $v1, 0x130($a0)        | 8c830130 lw          $v1, 0x130($a0)
  !! +0008  c4410098 lwc1        $f1, 0x98($v0)         | c4410094 lwc1        $f1, 0x94($v0)
```

Checked (ER-015): every placed function tried (`func_0014D918`, `func_0014F618`, `func_00150818`, `func_001115E0`,
`func_0014A948`) reports MATCH with the jal, `%hi` / `%lo`, `$gp` and `.lit4` checks all ok; a deliberate wrong
offset is reported at the right word.

## 3. `tools/place.py`

```
python -I tools/place.py c   func_X scratch.c [--dry-run]   INCLUDE_ASM line -> the C of scratch.c
python -I tools/place.py asm func_X [--dry-run]             C definition -> INCLUDE_ASM line (folder from the file)
python -I tools/place.py extract func_X [-o out.c]          placed C (+ the extern lines it names) -> scratch file
python -I tools/place.py where func_X                        which src/ file and line, C or asm
```

- `c` drops the scratch file's `#include` lines and any single-line declaration (extern, prototype, typedef at
  brace depth 0, ending in `;`) that the target file already has **above** the insertion point, and puts the rest
  (declarations, comments, the definition) where the `INCLUDE_ASM` line was. A declaration the file has only
  **below** is moved up (see 3.1). Comment lines are never dropped. The scratch file must define that function and
  no other.
- `asm` removes the definition and the comment directly above it; extern lines above stay and are listed (other
  functions may use them).
- Files are written with LF. The tool edits text only: run `make regress` afterwards.
- Checked (ER-015): `extract`, `asm`, `c` round trip on four placed functions (with comments and externs) leaves
  `src/` byte-identical (`git status` clean).

### 3.1 ER-065 fixes (Sun 22 Nov 2026)

- **Extern below the insertion point (6.3, `D_001FFB06`, `func_00145760`, `func_0012FB48`).** `place.py c` counted a
  line as already present when it was anywhere in the file. Now only a copy **above** the insertion point counts; a
  declaration whose only copy is below is written above the new function and its lower copy is deleted (one
  declaration, nothing lost, every function in between still sees it). Checked on a scratch file for
  `src/text_0014A968.c` (extern `func_0014B0D8`, whose file copy is below `func_0014A968`'s `INCLUDE_ASM`): the old
  tool dropped the line, the new one writes it above and removes the lower copy.
- **Comment lines (ER-063).** The old filter dropped any scratch line equal to a line elsewhere in the file unless it
  started with `/*` or `*`: a `//` comment, or the body line of a multi-line `/* */` comment, that matched another
  comment was lost (the old tool left `/* multi` without its closing line: an unterminated comment, so the definition below it was swallowed). Now only a single-line
  declaration at brace depth 0 is deduplicated (found with the same comment / string blanking as the definition
  finder); nothing inside a comment or a struct / initialiser body is.
- **Round trip.** `extract`, `asm`, `c` on `func_0014DDD8`, `func_0014D918`, `func_0012FB48`, `func_0014FF38`,
  `func_00145238` leaves `src/` byte-identical (`git diff` empty). A moved extern is the one thing `asm` does not
  undo (extern lines stay by design), so place-then-unplace of a function whose extern was moved leaves the
  declaration in its new place above the function.

Data: a function whose `.rodata` splat migrated (jump tables, strings used by one function) needs nothing more: the
migrated copy lived in the function's asm file and the C emits its own (`docs/decomp_recipes.md` 2.5). A standalone
`INCLUDE_RODATA` symbol the function takes over is still dropped by hand (`docs/build.md` 3.2).

## 4. `tools/clones.py`

```
python -I tools/clones.py func_X                  the strict family: members, size, census category, src/ file
python -I tools/clones.py func_X --try            retarget func_X's placed C to each member, compile, compare
python -I tools/clones.py func_X --try --place    also place every member that matched (then make regress)
python -I tools/clones.py --matched               families with a placed member and members still in asm
```

Strict key (`tools/census.py`): the instruction words with address parts masked, so a family is the same code
calling other functions or using other globals. `--try` compiles the placed function once, reads its relocations and,
for each member, the game's words at the same offsets: each `jal` / `j` target, `%hi` / `%lo` address and `$gp`
address names the member's symbol (a real symbol of the build at that address, else `func_` / `D_` + address).
Those names replace the original's as whole words in the C text (externs included), the function is renamed, and the
result is compiled and compared. A different `.lit4` value, a section-relative access or two names for one symbol are
reported (edit by hand). Output: `build/scratch_in/clones/<member>.c`. The census is refreshed on each run (1.5 s;
`--no-refresh` to skip).

Checked (ER-015, Thu 19 Nov 2026; masked compares, nothing placed but the proof function):

| Family | Members | Result |
|---|---|---|
| `func_0014A940` (`j func`, 8 bytes) | 20 (1 C) | **19 of 19 asm members MATCH** with retargeted C (callee renamed, e.g. `func_0014FE10 -> func_0012EEB0`) |
| `func_00150810` (a sibling call with a `$gp` float argument) | 2 (1 C) | `func_00153CC0` **MATCH** (`func_0012FA18 -> func_00130B60`, `D_0035BF20 -> D_0035C068`) |

These are the only two families with a placed member today (`--matched`); the tool is ready for each new match.
Note: the member's file may already declare a callee with another prototype (compile error at placement; fix the
extern), and the member inherits the original's comment (edit it: names and types stay Unconfirmed).

## 5. m2c on EE code (test, Thu 19 Nov 2026)

m2c is not in this repo. Twinblade's copy (`Project Twinblade\tools\m2c`, commit `708d2d2`, 16 Sep 2026) has a
**`mipsee` target** (`-t mipsee-gcc-c`, its `MipseeArch`). It runs natively with our venv's Python, no install:

```
.venv\Scripts\python.exe -E -s "C:\Users\lukea\Claude\Projects\Project Twinblade\tools\m2c\m2c.py" -t mipsee-gcc-c <asm file> [--context ctx.h]
```

(`-E -s`, not `-I`: under `-I` Python does not put the script's folder on the path and `import m2c` fails.) It reads
splat's `asm/nonmatchings/<file>/<func>.s` as they are (numeric registers, `glabel`, `li.s`, migrated jump tables).

Six game functions tried, one per kind (census picks):

| Function | Kind | m2c output |
|---|---|---|
| `func_00100E00` | integer, `$gp` globals, 5 calls, loop | usable draft (loop as `goto`, externs and `unk` struct fields) |
| `func_00131090` | jump table (`jr` on a register) | clean `switch`; **after one fix (the callee takes `arg0` as well, which the report's `$a1` showed) it matches** under cmpfunc (masked; not placed) |
| `func_001BBEB8` | MMI-flagged, `$gp`, calls | usable draft |
| `func_00113920` | 128-bit `lq` / `sq` copy loop | usable draft (`s128` type) |
| `func_00111480` | COP1 with square root | draft with one `M2C_ERROR`: rabbitizer writes `sqrt.s` as `c1 0xc0004`, which m2c does not know; it still finds the NaN check and the `sqrtf` fallback call (`func_001E42E0`) |
| `func_0010DFD0` | VU0 macro (`lqc2`, `vmulax`, ...) | every instruction `M2C_ERROR`: m2c has no COP2. Such code is inline `__asm__` by ruling (`docs/decomp_recipes.md` 3.3) |

Verdict: **useful for integer, MMI, 128-bit and COP1 code**; COP1 square roots need the one line by hand, VU0 macro
code is out of m2c's reach. Not tried: a `--context` header (Twinblade's bulk tools generate one), larger functions,
how often a raw draft matches (that is ER-016's measurement). ER-050 did these: section 6.

## 6. `tools/m2c_bulk.py` (ER-050, Sat 21 Nov 2026)

```
python -I tools/m2c_bulk.py --file text_00145120                 every INCLUDE_ASM function of src/text_00145120.c
python -I tools/m2c_bulk.py func_00145208 func_00145428          named functions
python -I tools/m2c_bulk.py --list picks.txt                     one name per line (# comments); --file / names mix
   [--ctx none|src|harvest] [--single] [--depth 2] [--rounds 4] [--par 4] [--label NAME] [--m2c DIR] [--timeout 120]
   [--plain]                                                     also write placeable plain C for each MATCH (6.2)
   [--tag ER-NNN]                                                task code for the plain comment (6.6)
   [--gcc-timeout 120] [--no-passthrough]                        gcc / ee-as kill time; skip the pass-through retry (6.6)
   [--no-slot-move] [--no-const]                                 skip the tail-j slot rewrite / the const variant (6.10)
   [--no-store-sort] [--no-ruling-314]                           skip the offset-order store variant / the ruling 3.14 variant (6.12)
python -I tools/sdata_consts.py [--all] [--names D_X ...]        census of the never-stored .sdata floats (6.10)
python -I tools/m2c_bulk.py --rerun-diffs build/m2c_bulk/<label> --chunk 8 --plain    an earlier run's same-size DIFFs in small lists (6.6)
```

**`make regress` deletes all of `build/`, `build/m2c_bulk/` included**: copy a run's folder out (e.g. to a scratch
folder outside the repo) before placing from it and running regress (ER-054 lost its first run that way).

Nothing goes into `src/`. Each run writes `build/m2c_bulk/<label>/` (gitignored: the drafts are game code; the
label defaults to the file names plus the context mode, and the run clears its own `asm/`, `c/`, `obj/`,
`harvest/` first): `results.md` (summary tables, then one row per function), `results.csv`, `results.json`, and
per function `c/<func>.c` (the kept draft, compilable as it stands), `c/<func>.m2c.txt` (m2c's raw output),
`c/<func>.diff.txt` (the cmpfunc-style report) or `c/<func>.err.txt` (gcc's message). A MATCH draft goes in with
`tools/place.py c func_X build/m2c_bulk/<label>/c/func_X.c` and `make regress` (the masked compare is a working
aid, not proof; ruling `docs/decomp_recipes.md` 3.1). The raw draft includes m2c's headers and uses its macros,
which the build does not have: place the `--plain` form instead (`c/func_X.plain.c`, 6.2).

**What it does for each function**

1. **Asm rewrites** (a copy in `asm/`; splat's files are untouched), each one instruction for one, so delay slots
   stay where they are:

   | splat / rabbitizer writes | m2c gets | Why |
   |---|---|---|
   | `li.s $fN, 1.5e+03` (a `.lit4` load; `tools/split.py` writes them so) | `lwc1 $fN, (M2CLIT_44BB8000)`; the cleaner puts the literal back as `1500.0f` (shortest decimal giving the same float32 bits) | m2c cannot parse `li.s`: **every function with a `.lit4` float failed** before (e.g. `func_00145120`, `func_00145578`, `func_00145760`) |
   | `c1 0xC0004` | `sqrt.s $f0, $f12` | rabbitizer's R5900 square root (COP1 S funct 4, source in the `ft` field, `fs` 0); other `c1` words are flagged, not rewritten |
   | `.word 0x46001064 # cvt.w.s $f1, $f2` | `cvt.w.s $f1, $f2` | rabbitizer prints the R5900 `cvt.w.s` as a word with the instruction in a comment (422 sites in 241 functions of the game's asm); m2c then saw an unset `$f1` ("second half of f64") |
   | `j func_X` with an instruction in its delay slot (ER-084) | `insn; j func_X; nop` (not for a `nop` slot or the stack pop `addiu $29, $29, N`, which m2c reads as the epilogue already; `--no-slot-move` turns it off) | m2c drops the delay slot of a tail `j`: func_001501D8's `daddu $a1, $zero, $zero` became `arg1`, func_00146EF0's `swc1` was left out (6.10). Safe: `j` reads no register |

2. **m2c** (`-t mipsee-gcc-c --valid-syntax`, run natively from the m2c folder, default Twinblade's copy) with a
   context header. `--ctx`:
   - `none`: no context;
   - `src`: the prototypes of the C functions in `src/` (definitions, then `extern` prototypes; a clash between
     files is reported and the first kept) and the `extern` data lines of `src/`;
   - `harvest` (default): `src` plus a signature for every target and its callees to `--depth` levels, inferred by
     m2c from each callee's own asm, **in rounds** (each round uses the previous round's signatures as context;
     stops when none change, at most `--rounds`). One round is not enough: a wrapper that passes its arguments on
     to its callee comes out as `(void)` until the callee's parameters are known (text_00145120: 193 signatures,
     changes per round 193, 65, 26, 7).
   The context always leaves out the function's own prototype; `s128` / `u128` are declared for m2c with
   `__attribute__((mode(TI)))` (a plain `s128 *` made the context fail to parse). A `src/` declaration whose type is
   a file-local typedef (a struct type such as ER-092's `G390`) is left out and listed: one such line made m2c refuse
   the whole context, and every run from 22 Nov 2026 17:04 to ER-130 fell back to `none` (6.12). The run checks the
   context once and prints a WARNING when m2c cannot parse it.

3. **Fallback contexts.** Unless `--single`, each function is tried with `harvest`, then `harvest` without the
   harvested signatures that say `void` (a callee m2c thought returns nothing but whose `$v0` / `$f0` the caller
   uses: "Read from unset register"), then `src`, then `none`, until one matches; the best attempt is kept (match,
   compiles, fewest `M2C_ERROR`, nearest size, fewest differing words) and named in the Notes column.

4. **Cleaning** into a scratch file: `#include "common.h"`, a generated `m2c_extra.h` (`f64`, `s128`, `u128`), m2c's
   own `m2c_macros.h` from the m2c folder (included by path, **never copied into this repo**: m2c is GPL-3), and the
   context prototypes. Then:
   - `extern M2C_UNK func_X;` (m2c declares a function whose address is taken as data) becomes
     `extern void func_X();`: under `-G8` a 4-byte extern is read `$gp`-relative, the game uses `lui` / `addiu`;
   - `extern T D_X;` that the asm reaches through `%hi` / `%lo` gets `__attribute__((section(".data")))`, which makes
     ee-gcc use `%hi` / `%lo` for it (checked on a scratch file). **A draft aid only**: a placed function uses the
     recipe form `extern T D_X[];` (`docs/decomp_recipes.md` 2.6); a section attribute in `src/` would need a ruling;
   - undeclared stack variables (`sp10`, a bare `sp` for a stack buffer m2c lost) are declared so the draft compiles,
     and listed in Notes (such drafts are unlikely to match);
   - `M2C_ERROR(...)` stays (m2c's header makes it `0`): counted, and the row's class says so;
   - **byte offsets** (ER-097, 6.10): m2c prints pointer + offset with the raw byte count (its `evaluate.py`
     `add_imm` / `handle_add_real`), which C scales by the pointer's element type. A `%hi` / `%lo` base in index
     arithmetic (`(arg0 * 0x2C8) + &D_X`, `&D_X + 0x252`) that the draft declares itself becomes `extern char D_X[];`
     (`&D_X` -> `D_X`, a read of its value -> `(*(T *)D_X)`); one that `src/` declares gets a `(char *)` cast on the
     operand instead. A parameter or local `T *v` (element size 2, 4 or 8 known) in `v += N;` / `v - N` steps by
     `N / sizeof(T)` elements (byte arithmetic through `char *` when N is not a multiple). Notes: `byte base: D_X`,
     `N pointer step(s) in elements`. A dereferenced sum with such a base (`*((arg0 * 0xC4C) + D_X)`) gets m2c's type
     back as `*(T *)(...)`; where m2c had none (`M2C_UNK`), the width of the function's only non-stack memory access
     in the asm, if it has exactly one (ER-114, func_001BCF48: `lhu` -> `u16`; Notes `byte base: D_X (deref as u16)`).

5. **Compile** with exactly the Makefile's `CC` / `CFLAGS` / `CPPFLAGS` / `AS` / `ASFLAGS` (`eematch.settings()`)
   into `obj/`, **compare** with `eematch.compare_function` (the cmpfunc check: relocation targets checked).

6. **Two shape variants** on a compiled draft that does not match, both from confirmed recipes
   (`docs/decomp_recipes.md` 2.1):
   - **return-wrapper**: ours shorter than the game's and the draft is `void f(..) { ...; g(..); }`: try
     `M2C_UNK f(..) { ...; return g(..); }`. A call whose value is returned gets a frame and `jal`, a void tail call
     a sibling `j`; m2c gives the void form when it cannot tell that `$v0` is passed on;
   - **store order**: same size as the game's: first (ER-124) each run of 2 or more consecutive
     `M2C_FIELD(base, T *, off) = rhs;` lines at one indentation (inside blocks too) sorted by offset, in three modes
     (offset order, bytes last, bytes first; runs that read a field they write or call a function are left alone; at
     most 3 compiles); then the other orders of each run of 2 or more plain top-level assignments (every order up to
     4 statements, else reversed; at most 24 compiles); kept only on a MATCH (Notes: `store order by offset (asc)` /
     `store order permuted`);
   - **ruling 3.14** (ER-121): when our build of the draft has a sibling `j` (read from the compiled `.s`) and the
     game's function has no `j` out of itself, `(void)&arg0; /* ruling 3.14 */` after the declarations (the first
     parameter; none when the function has none), then the store orders of that form; kept only on a MATCH (Notes:
     `ruling 3.14 (void)&arg0`, else `ruling 3.14 tried, no match (best: ...)`; the Plain column says the ruling is
     used, for the list in recipes 3.14);
   - **const floats** (ER-101, 6.10): the draft compiled again with every `.sdata` float it reads that
     `tools/sdata_consts.py` finds never stored declared `extern const` (also in its prototypes header), then the
     store orders of that form; kept only on a MATCH (Notes: `const floats: D_X ...`). `--plain` declares those
     `extern const` and says in the Plain column when the target file declares one without `const`.

Columns of the table: function, file, address, instructions and size band (`tools/census.py` bands), m2c gave a
draft, draft compiles, exact match, differing words, sizes, class (MATCH / diff / compile error / m2c failed /
M2C_ERROR in draft) with detail (the diff's kind: size +-N instructions, same instructions with other operands
(registers, offsets), same instructions in another order (scheduling), different instructions), flags from the asm
(`div/sqrt (ER-034)`, `VU0`, `jump table`, unknown `c1` words), notes (cleaning and variant used, context kept).
Speed at `--par 4`, all fallbacks and the harvest rounds included: 148 functions in 102 s (68 s for 100, 34 s for 48).

### 6.1 First runs (Sat 21 Nov 2026; masked compares, nothing placed)

**`src/text_00145120.c`** (100 asm functions, 3 to 255 instructions; game code; 14 with `div.s` / square root, 5
with a jump table):

| Mode | Drafts | Compile | MATCH |
|---|---|---|---|
| `--ctx none` (m2c alone, with the asm rewrites, cleaning and variants) | 100 | 93 | 19 |
| `--ctx src` (falls back to none) | 100 | 97 | 23 |
| `--ctx harvest --single` | 100 | 99 | 29 |
| `--ctx harvest` (all fallbacks; the default) | 100 | 100 | **32** |

4 of the 32 are members of `func_0014A940`'s clone family (`j func`), which ER-049 places by clone anyway.

**`src/text_0014A968.c`** (48 asm functions): default mode, 48 compile, **22 match**.

Both files together (148 functions, default mode):

| Result | Functions | Note |
|---|---|---|
| MATCH | 54 (36%) | 48 with the full harvest context, 6 with void returns left out; 9 use `.lit4` literals, 7 the return-wrapper form, 6 a permuted store order, 5 a function-address extern, 3 a `%hi` / `%lo` data extern |
| size differs | 49 | mostly medium and large functions: m2c's control flow and temporaries differ from the original's (not analysed one by one) |
| has `div.s` / `sqrt.s` | 15 | none can match until ER-034 (two `nop`s before each in the game, none from our gcc) |
| same size, order differs (scheduling) | 8 | permuter / hand work |
| same size, 1-2 words differ | 7 | the nearest misses: hand work first |
| same size, different instructions | 7 | |
| same size, operands differ (register allocation, offsets) | 6 | |
| `M2C_ERROR` left in the draft | 2 | |

| Band | Functions | MATCH |
|---|---|---|
| 3-8 | 23 | 17 |
| 9-16 | 17 | 13 |
| 17-32 | 29 | 19 |
| 33-64 | 33 | 4 |
| 65-128 | 36 | 1 |
| 129-256 | 10 | 0 |

So **up to 32 instructions, 49 of 69 drafts match as drafted (71%)**; from 33 instructions on, 5 of 79 (6%).

Failure kinds seen on reading single diffs (examples, not counted over all rows; Unconfirmed as classes):
- a store in the delay slot of a tail `j` that m2c left out (`func_00146EF0`: the game's `swc1 $f1, 0x54($a0)` after
  `j func_0012B238` is missing from the draft);
- wrappers that m2c gives the wrong return shape (fixed by the return-wrapper variant where the callee's type allows);
- store order and the registers of address loads (`func_001464A8`, `func_00146FE0`: same instructions, the
  `lui` / `addiu` pairs in another order and registers).

### 6.2 `--plain` and the first bulk placement (ER-054, Sun 22 Nov 2026)

With `--plain`, after the run every MATCH draft is turned into a file `tools/place.py c` takes as it stands,
`c/<func>.plain.c`, which is **compiled and compared again** (Plain column of the table; only a plain MATCH is meant
for placing):

| Draft (m2c `--valid-syntax`) | Plain form |
|---|---|
| m2c's headers, `m2c_extra.h`, the context header | `#include "common.h"` only |
| `M2C_FIELD(e, T *, off)` | `*(T *)((char *)e + off)` (m2c's macro uses `s8 *`: same arithmetic); offset 0 -> `*(T *)e`; `&M2C_FIELD(..)` -> the pointer; parentheses only before a postfix operator |
| `M2C_UNK`, `M2C_UNK8/16/32/64`, `f64` | `s32`, `s8` / `s16` / `s32` / `s64`, `double` |
| `extern T D_X __attribute__((section(".data")));` (the draft aid for `%hi` / `%lo`) | `extern T D_X[];` (recipe 2.6), `D_X` -> `D_X[0]`, `&D_X` -> `D_X` |
| `T func_X(..); /* extern */` | `extern T func_X(..);` |

Declarations: one `extern` line per function / symbol the body names, taken in this order from (1) the target
`src/` file's own extern lines or C definitions, so `place.py` drops lines the file already has and the new function
agrees with the file; (2) the run's own matched functions (their final signature, e.g. after the return-wrapper
variant); (3) the draft's own extern lines; (4) the prototypes header it was compiled with. Skipped (listed in the
Plain column): another `M2C_` macro left (`M2C_ERROR`, `M2C_BITWISE`, ...), `s128` / `u128` (`common.h` has none).
At the end the tool lists names that two plain drafts of one file declare differently (they clash when both are
placed). The comment above each function: `ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and
offsets are Unconfirmed readings.` Left as m2c wrote it: arithmetic on `void *` (a GNU extension ee-gcc accepts),
`temp_` / `arg` / `sp` names, m2c's own casts.

**First placement** (default context, `--plain`; five files, 241 asm functions; nothing compiler-steering added).
Every file placed, then `make regress` (-j4): **78 functions placed, all green** (3,606 match, SHA-1 unchanged),
5,460 bytes.

| File | Asm functions tried | Masked MATCH | Plain MATCH | Placed (regress green) | Same size, 1-2 words differ (all matched by ER-062, 6.3) | Place + regress |
|---|---|---|---|---|---|---|
| `text_00145120` | 96 | 27 | 25 | 25 | 4 (`func_00145238`, `func_00145248`, `func_00148B00`, `func_0014A8B0`) | 46 s + one fix + 43 s |
| `text_0014A968` | 48 | 22 | 19 | 19 | 2 (`func_0014B420`, `func_0014C620`) | 45 s |
| `text_0014C840` | 26 | 8 | 8 | 8 | 1 (`func_0014D380`) | 49 s |
| `text_0014D930` | 16 | 7 | 6 | 6 | 1 (`func_0014DDD8`) | 51 s |
| `text_0014E2E8` | 55 | 21 | 20 | 20 | 3 (`func_0014FF38`, `func_0014FF58`, `func_001501D8`) | 44 s + one fix + 45 s |
| **All** | **241** | **85** | **78** | **78** | 11 | bulk run 3 min 22 s (0.84 s per function, par 4) |

Two placements needed declarations aligned by hand (the build failed with conflicting types; fixed, compared with
`cmpfunc` on the `src/` file, then regress green): `func_001507B8` (m2c left out the argument that passes through
in `$a0` to `func_00131088`, whose file prototype takes it; written `func_00131088(arg0) == (s32) func_00150350`
as `func_0014F210` does, `arg0` as `void *`, and the caller's extern changed to match) and `func_00149920` (two
drafts guessed `func_0012F8C8` differently; the `s32` one that `func_00147850` uses kept, the other dropped).

Not placed (masked MATCH, plain form failed; all still `INCLUDE_ASM`):

| Function | File | Cause |
|---|---|---|
| `func_001462D8` | `text_00145120` | `s128` in the draft (no 128-bit type in `common.h`) |
| `func_001492D0` | `text_00145120` | calls `func_0012B238` with arguments; the file declares it `(void)` (ER-049's clone prototype) |
| `func_0014FE10` | `text_0014E2E8` | the file's extern for it is `void func_0014FE10(void)` (ER-049), the draft's definition differs |
| `func_0014B3A0`, `func_0014B3E0`, `func_0014B4C8` | `text_0014A968` | `func_0014BC88` (placed, its own draft has 1 parameter) is called with 2 |
| `func_0014E120` | `text_0014D930` | `func_0014E2C8` (an ER-049 clone, `(void)`) is called with arguments |

All seven are declaration clashes, not code differences: the clone prototypes `(void)` of ER-049 and m2c's
parameter count for a pass-through callee. Skipped by nature: 22 functions with `div.s` / `sqrt.s` (ER-034; none
matched as drafts); no VU0 inline asm in these five files.

### 6.3 The 11 near misses by hand (ER-062, Sun 22 Nov 2026)

All 11 same-size near misses of 6.2 are placed (commit `a751ee2`), `make regress` (-j4) green after each placement:
708 bytes, nothing parked, nothing compiler-steering. Drafts from `m2c_bulk.py --plain` run on the 11 names only
(label `er062`, default `harvest` context, 8 s).

| Function | File | What it took |
|---|---|---|
| `func_00148B00`, `func_0014A8B0` | `text_00145120` | nothing: plain MATCH in the small run (DIFF in ER-054's file-wide run) |
| `func_0014FF38`, `func_0014FF58` | `text_0014E2E8` | nothing: plain MATCH in the small run (return-wrapper form) |
| `func_00145238`, `func_00145248` | `text_00145120` | `func_00130FE0(arg0, func_X)`: m2c dropped the pass-through `$a0` |
| `func_0014C620` | `text_0014A968` | `func_0012FD10(arg0, 0xA6, 2, 5)`: m2c dropped `$a0` and read the constant `5` as a parameter |
| `func_001501D8` | `text_0014E2E8` | `func_0012FB48(arg0, 0)` (pass-through `$a0`) |
| `func_0014D380` | `text_0014C840` | `func_0014D8F8(arg0, (s32) func_0014D3F0)` (pass-through `$a0`, written as `func_0014CC68` calls it) |
| `func_0014B420` | `text_0014A968` | `x6C > x74` instead of `x74 < x6C`: the game loads `+0x6C` first (recipes 2.7) |
| `func_0014DDD8` | `text_0014D930` | the plain draft as it stood; the one flagged word is a cmpfunc limit (below) |

**Why the small run matched 4 more.** ER-054's run gave `func_00148B00`, `func_0014A8B0`, `func_0014FF38` and
`func_0014FF58` as DIFF; the run on 11 names gave plain MATCHes with the same tool. The harvested signatures differ
between the two runs (they come from the callees m2c reaches from the targets); which signature made the difference
was not traced (Unconfirmed). Rerunning a file's remaining same-size DIFFs in small lists is cheap (8 s here).

**The pass-through signature.** A same-size DIFF where every argument sits one register higher in the game
(`lui $a1` / `addiu $a1` against our `$a0`) means the function passes its own `$a0` to the callee untouched, and
m2c left it out. Give the function a parameter and pass it (recipes 2.8, 2.10).

**Two tool limits met (both fixed by ER-065, Sun 22 Nov 2026: see 3.1; the text below is what was seen):**

- `place.py c` drops a scratch extern line that the target file already has, also when the file's copy is
  **below** the insertion point. The placed function then uses a name not yet declared: a build error for a data
  symbol (`D_001FFB06`) or a callee whose type conflicts (`func_00145760`), and **no message at all** for a `void`
  callee taken as an implicit declaration (`func_0012FB48`; the code still matched). Fix by hand: move the file's
  extern line above the new function (or add it there). Check after every placement that the scratch file's extern
  lines are above it.
- `cmpfunc` pairs an `R_MIPS_LO16` only with an `R_MIPS_HI16` that comes before it. When two branches share one
  `lui` (`func_0014DDD8`: the `lui $v0, %hi(D_0035BEB0)` at +0x44 serves the `%lo` at +0x94 in another case), the
  `%lo` is reported "no HI16 before it" and counted as a difference although the instruction's fixed part is equal.
  `make regress` proved the function.

### 6.4 The files cut by ER-051 (ER-072, Sun 22 Nov 2026)

`--plain`, default context, the five files `text_001388E8`, `text_0013A410`, `text_0013B748`, `text_0013C510`,
`text_0013DBC0` (237 asm functions, 186 s at `--par 4`; label `er072`): 229 compile, **83 masked MATCH, 73 plain
MATCH**. All 73 placed (`make regress` green per file or file group); the other 10 masked MATCHes placed after aligning
declarations by hand (the file's own extern for the function was `(void)` / `(char *)` from an address-taking user, or
a pass-through callee argument m2c dropped; recipes 2.13). Two `place.py` extern-below cases met again (6.3) and
fixed by hand. 9 same-size near misses (1-2 words) left: `func_001390F8`, `func_00139AF8`, `func_0013CA60`,
`func_0013E678`, `func_0013E758`, `func_0013F258`, `func_0013F288`, `func_001431A0`, `func_001449D0`. 31 functions
carry `div.s` / `sqrt.s` flags. m2c drafts of two-table functions fail to compile ("multiple default labels"); the
hand versions are in recipes 2.13.

`cmpfunc` limit seen again: a `%lo` of a function address (`.text`-relative) whose `%hi` sits in another block is
"no HI16 before it" (8 functions here); all proven by `make regress`.

**Rerun (ER-078, Sun 22 Nov 2026; ee-as r3, `-fno-edge-lcm`).** `--plain --tag ER-078` over the same five files (148 asm
functions left, 141 harvested signatures, 145 s at `--par 4`, label `er078`): 141 compile, 3 masked MATCH, all placed
(func_00138BE0 after aligning the file's extern, func_001390F8, func_0013CA60 by the pass-through retry); 7 same-size
DIFFs of 1-2 words, all ER-072's near misses (`docs/decomp_recipes.md` 2.22 for the hand work).

### 6.5 `text_00150878` (ER-060 part 1, Sun 22 Nov 2026)

`--plain` over the whole file (1,448 `INCLUDE_ASM` functions; default `harvest` context, 1,628 harvested
signatures, par 4), with the ER-034 toolchain. **230 functions placed, 13,260 bytes, `make regress` (-j4) green after
each of five batches and on a clean split at the end** (commit `af8f897` for 229, then `func_001BEF18`); 1,448 ->
1,218 `INCLUDE_ASM`. Bands placed: 3-8 91, 9-16 54, 17-32 66, 33-64 18, 65-128 1; 6 with `div.s` / `sqrt.s`
(`func_001528E0`, `func_0015A228`, `func_0015C6C0`, `func_0015D7C0`, `func_00176FF8`, `func_001A5360`).

| Stage | Functions |
|---|---|
| m2c gave a draft | 1,447 (all but `func_00179B30`, below) |
| draft compiles | 1,068 |
| masked MATCH | 234 |
| plain MATCH | 230 |
| placed, regress green | 229 + `func_001BEF18` by hand = 230 |
| same size, 1-2 words differ (near misses, not tried) | 48 |
| same size, 3-8 / 9+ words differ | 71 / 116 |
| size differs | 598 |

Comment above each: `ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed
readings.` (the tool writes `ER-054:`; the tag was changed in the copied drafts, the tool is unchanged).

**Declarations aligned by hand** (two drafts of one file guessed a callee differently; build error "conflicting
types"; no code change, each proven by regress):

| Callee | Kept | Note |
|---|---|---|
| `func_0012B890` | `(void **, s32, s32, s32, s16, s32, f32)` (as `text_00145120`) | the other draft had `s32` for a constant argument |
| `func_0012C360` | `void *(void *, s32, s32, s32, void *)` | |
| `func_00153380` | `s32 (void *)` | value unused at both calls |
| `func_001132B0` | **`s32`** `(void *)` | a `void` prototype changed `func_001BE7E8`'s registers (`$v0` instead of `$v1`): see recipes 2.15 |
| `func_001CB808`, `func_001E9D08` | `extern s32 f();` (no prototype) | called with 2 / 3 and 2 / 4 integer or pointer arguments (variadic: Unconfirmed) |
| `func_00131088` | `(void *)`; `func_00156F80` passes `arg0` (pass-through, recipes 2.10) | the draft called it with none |
| `func_0014FE10` | `(void *, s32)`; `func_001546F8` takes `arg1` too and passes both | ER-049's `(void)` clone prototype again (ER-061) |

`func_001BEF18`: its draft called `func_001E9A10` by address, which ER-041 named `fptodp` (link error after the
merge). Written as plain C, the conversion as the argument of `extern void func_001BE0B0(void *, double);`
(recipes 2.10); placed, regress green.

**Not placed:**

| Function | Cause |
|---|---|
| `func_00179B30` | **cc1 loops for ever** on every draft context of it (killed by hand after 6,457 s and 27,150 s of CPU; the run then finished). `m2c_bulk.py` has no compile timeout (follow-up) |
| `func_00161568` | plain form: parse error |
| `func_001C46C8` | plain form: too many arguments (declaration clash) |
| `func_0017D290`, `func_001BF5B0` | plain form: `s128` (no 128-bit type in `common.h`) |
| `func_00177098` | VU0 (the file's only VU0 function); not a MATCH |
| 33 jump-table functions | none a MATCH as drafts |
| ER-041's seven: `func_00178C60` (size -351 instr), `func_001BB2C8`, `func_001BB750` (compile error: m2c's macro), `func_0015C0E0` (too many arguments), `func_0015BE00` (+1 instr), `func_00160CE0` (-9), `func_00189810` (-1) | rerun with the ER-041 names (label `er060b`): drafts call `dpadd` / `sqrtf` ... by name; need plain double C by hand (recipes 2.11) |

Near misses (same size, 1-2 words; not tried): func_00150A30 func_00153DA0 func_00154BD0 func_00157B08 func_00159830
func_0015D3E8 func_0015E628 func_0015E988 func_00163C68 func_00164220 func_00164C08 func_00164C50 func_00168328
func_00168358 func_00169480 func_001766E0 func_0017A2E0 func_0017CC28 func_0017D3E8 func_0017D6F0 func_001835E0
func_00189198 func_0018C730 func_0019BA90 func_0019C078 func_001AB480 func_001AED88 func_001AEF90 func_001AF2E8
func_001B42B0 func_001B4BE8 func_001B5E68 func_001B77B8 func_001BBD40 func_001BBEB8 func_001BCD68 func_001BCF48
func_001BE1E0 func_001BF260 func_001BFEB8 func_001C0570 func_001C1310 func_001C1580 func_001C4F68 func_001C4FF0
func_001C53D0 func_001C8920 func_001C8928.

**Time.** Bulk run 03:25-03:53 ET for 1,447 functions (about 1.2 s per function at par 4), then stalled on the
`cc1` loop until 13:17; placement and five regress rounds 13:22-13:25, full regress and commit gate after. The run's
folder holds 32,000 object files: copy only `results.*` and `c/` out of `build/` (copying all of it took minutes).

### 6.6 `text_001000C0` (ER-060 part 2, Sun 22 Nov 2026)

`--plain`, default context, `--par 4`, the whole file (525 asm functions, 623 s; label `er060b`), with a watchdog
beside it that kills any `cc1.exe` of the worktree running over 2 minutes (none was needed: no draft looped). Results:
525 drafts, 400 compile, **58 masked MATCH, 55 plain MATCH**. All 55 placed in two batches (28 + 27), `make regress`
(-j4) green after each; the other 3 masked MATCHes placed after aligning declarations by hand (third regress green):
**58 functions, 4,752 bytes**, nothing compiler-steering. The comment above each says `ER-060: drafted with m2c ...
Unconfirmed readings` (the tool writes `ER-054`; changed in the copies before placing, the tool is untouched).

| Function | What it took |
|---|---|
| `func_0012B6B0` | calls `func_0012BAE0` without setting `$f12`, and `func_0012BAE0` reads `$f12` (another draft, `func_0012F958`, passes a float there): given an `f32 fparg0` parameter passed through; same code (recipes 2.16) |
| `func_0012DA68` | uses `func_001314C8`'s `$f0`: `func_001314C8` / `func_00131508` (placed in batch 2 as `s32`, sibling `j func_001E3EC8`) and the file's `func_001E3EC8` extern made `f32`; same code |
| `func_00130758` | returns `func_001114E8`'s float as it is (no `cvt.w.s` in the game): `float func_00130758(void *)` |
| `func_00132CF8` | the file's extern was `void func_00132CF8(void)` (an address-taking user, recipes 2.10 / 2.13): changed to `(s8 *)`; three declarations moved above it |

`place.py`'s extern-below limit (6.3) met 3 times (`D_0035B8A0`, `D_0035B8A8`, and `func_00133290` / `func_00132E80` /
`func_00133098`); fixed by moving the file's line up. A small checker run after each batch (every `extern` line of a
placed draft must be declared above the placed definition) found the first two before the build; the third was a
plain prototype line (no `extern`), caught by the compile. No implicit declarations in the build log.

**Skipped:** the 14 functions recipes 2.11 / 2.12 park or leave for ER-069 / ER-070 / ER-071 / ER-073 (none was a
MATCH anyway), and the 29 VU0-flagged functions (none matched; they need inline asm, ruling 3.3).

**Left in asm (not placed):**

| Kind | Functions |
|---|---|
| same size, 1-2 words differ (rerun as a list of 18, label `er060c`: none matched, unlike ER-062's run; **all 18 placed by ER-081**, Sun 22 Nov 2026: a rerun with `-fno-edge-lcm` in small lists gave `func_00130CE0` by the pass-through retry, the other 17 by hand, recipes 2.18) | `func_00101238`, `func_00103E30`, `func_0010CAF0` (div), `func_00111660`, `func_00129ED8`, `func_0012A140`, `func_0012B238`, `func_0012C778`, `func_0012F838`, `func_0012F870`, `func_00130A88`, `func_00130AD0`, `func_00130CE0`, `func_00130FE0`, `func_00131000`, `func_001320B8`, `func_001321D8`, `func_001328D8` |
| same size, 3-8 words differ | 33 |
| same size, 9+ words differ | 53 |
| size differs | 238 |
| compile error (mostly harvested signatures: `void value not ignored` 32, too few / many arguments 35, `invalid use of void expression` 18) | 125 (13 of them with `M2C_ERROR`) |

The run's `results.md` / `.csv` (game code: not in git) are in the scratch folder of the ER-060 part 2 session only.

### 6.7 Timeout, pass-through retry, small lists; the rerun of three ER-054 files (ER-066 + ER-079, ER-064, Sun 22 Nov 2026)

**Tool changes (ER-066, with ER-079 folded in):**

| Change | What it does | Checked on |
|---|---|---|
| **gcc / ee-as timeout** (`--gcc-timeout`, default 120 s) | each compile starts **suspended inside a Windows job object** with kill-on-close, is resumed, and runs under a timer; on expiry the job is terminated, which ends cc1.exe with the driver, and the attempt is classed `TIMEOUT` (row class, or a note when another context compiled); the run goes on. `taskkill /T` alone is **not** enough: the MSYS-built gcc.exe starts cc1.exe outside the Windows parent chain (cc1's parent PID was not under the driver's), and a first version that used it left every timed-out cc1 running (7 orphans from two test runs, killed by PID) | `func_00179B30` (text_00150878, the draft cc1 that looped for 9 h in ER-060; draft only, nothing placed): `--gcc-timeout 30`, all four contexts `TIMEOUT`, run finished in 124 s, **no cc1 left**; the same draft at 10 s: 1 cc1 while running, 0 after; a normal draft still compiles; smoke run on `text_0014C840` |
| **Results in completion order** | each row is printed when its function finishes, with `[k/n, seconds]`; the tables keep the input order (ER-060's log stopped at the first slow function, so a hang looked like no progress) | every run below |
| **`--tag ER-NNN`** | the plain comment reads `/* ER-NNN: drafted with m2c ... */` (no tag: `/* Drafted with m2c ... */`; it was hard-coded `ER-054:`) | ER-066's runs |
| **Pass-through retry** (on by default; `--no-passthrough` skips it) | for a function whose best draft compiles at the game's size but differs: if a differing word is the game writing `$a1`..`$a3` where ours writes the register one lower with the same instruction (relocated immediates not compared), or ours has a `nop` there, the callee is the game's `j` / `jal` holding that word in its delay slot, else the next one within 16 words. Its prototype is **widened in m2c's context** by a leading `void *` (padded with `s32` up to the highest `$aN` the game sets), and the function is drafted again, so m2c itself reads the untouched `$a0` as the function's own `arg0` (recipes 2.10). Kept only on a MATCH; the Notes say `pass-through retry: <prototype>` or `pass-through retry tried, no match`. `--plain` declares the widened prototype and says in the Plain column when the target file declares the callee narrower, or declares the function itself differently (its address-taking extern, recipes 2.10): align that line by hand before placing | below |
| **`--rerun-diffs RUN --chunk N`** | takes an earlier run's same-size DIFFs (from `results.json`) and runs them in lists of N, each with its own harvest (`chunkNN/` folders; `c/` files and the tables gathered in the run folder). `--chunk` also works on any name list | below |

**Reproduction on ER-062's five** (Sun 22 Nov 2026): `src/text_00145120.c`, `text_0014A968.c`, `text_0014C840.c`,
`text_0014E2E8.c` set back to their state before ER-062 (`a751ee2^`) in the worktree, split, the tool run on the
ER-062 names, the files restored afterwards (never committed):

| Function | Without the retry | With it |
|---|---|---|
| `func_00145238` | diff (`lui $a0` against the game's `lui $a1`) | **MATCH** (`func_00130FE0` widened to `(void *, s32 *)`); plain: compile error, and the Plain column names the cause: the file's own `extern void func_00145238(void);` (ER-062 changed it by hand) |
| `func_00145248` | diff (same shape) | **MATCH**, plain MATCH |
| `func_0014D380` | MATCH already (today's context has the callee's two parameters) | (not needed) |
| `func_0014C620` | diff: not this shape (`arg0` is there; m2c read the constant `5` as a parameter `arg3`) | not tried (no shifted register) |
| `func_001501D8` | diff | retry fired (`func_0012FB48` widened to `(void *, s32)`), still a diff: m2c then wrote `func_0012FB48(arg0, arg1)`; it **ignores the delay slot of a tail `j`** (the game's `daddu $a1, $zero, $zero` there), the same m2c limit as `func_00146EF0` in 6.1 |

So the retry reproduces the two pure pass-through cases; the other three need what ER-062 did by hand.

**ER-064: rerun of `text_00145120`, `text_0014A968`, `text_0014C840`** (`--plain`, default context, the ER-034
toolchain and ER-041 names; 107 asm functions, 125 harvested signatures, 117 s at par 4; label `er064`):

| File | Asm functions | Masked MATCH | Plain MATCH | Placed (regress green) |
|---|---|---|---|---|
| `text_00145120` | 64 | 6 | 5 | 5: `func_001462D8`, `func_00146F10`, `func_00147D90`, `func_00149880` (by the pass-through retry: `func_0012FB48(arg0, 0)`), `func_00149C18` |
| `text_0014A968` | 27 | 6 | 6 | 6: `func_0014AF58`, `func_0014B3A0`, `func_0014B3E0`, `func_0014B4C8`, `func_0014BC40`, `func_0014C220` |
| `text_0014C840` | 16 | 2 | 2 | 2: `func_0014CC10`, `func_0014D338` |
| **All** | **107** | **14** | **13** | **13, 1,312 bytes** |

- `func_001462D8` (skipped by ER-054 for `s128`) and `func_0014B3A0` / `func_0014B3E0` / `func_0014B4C8` (ER-054's
  `func_0014BC88` clash) now come out plain: 4 of ER-061's 7 are done. `func_001492D0` is still the `func_0012B238`
  `(void)` clash (plain: "too many arguments").
- **None of the 15 `div.s` / `sqrt.s` functions matched as drafts.** The divides are not the cause any more: the
  drafts differ in shape (12 by size, 3 same size with other instructions). Example `func_00147F78`: m2c lost a stack
  buffer (declared `sp[64]` by the cleaner), so the frame is 0x180 against the game's 0xD0; hand work.
- Small lists: the 26 same-size DIFFs rerun with `--rerun-diffs er064 --chunk 8` (4 chunks, 42 s): **no new
  match**, and none of them is a 1-2 word near miss (10 have 3-8 words, 16 have 9+). The pass-through retry fired
  on 4 more (`func_00145FE8`, `func_001464A8`, `func_00146FE0`, `func_001497D0`) without a match.
- `place.py`'s extern-below limit (6.3, ER-065) met in **5 of 13 placements, 7 extern lines** (`func_001469B8`, `func_0012FAE8`
  in `text_00145120`; `func_0014BC88`, `func_0014B7C0` in `text_0014A968`; `func_0014D8F8`, `func_0014D068`,
  `func_0014D890` in `text_0014C840`): each extern line moved above the new function before the build (a scratch
  check listed every name a placed body uses that is declared only below it).

### 6.8 The near misses of `text_00150878` in small lists (ER-080, Sun 22 Nov 2026)

ER-060's run folder was gone (`build/` is wiped by every regress), so `--rerun-diffs` had nothing to read: the 48 names
of 6.5 went in as a list instead, `--list near48.txt --chunk 8 --plain --tag ER-080` (6 chunks, 70 s at par 4, today's
toolchain with `-fno-edge-lcm`). **16 masked / 15 plain MATCH** (func_00164220 needed its caller's extern aligned):
all placed, `make regress` green. The other 31 came out as 1-2 word DIFFs again and were matched by hand (recipes
2.20). So the small-list effect of 6.3 held here (16 of 48 that the file-wide run had as DIFF), where ER-060 part 2's
list of 18 gave none (6.6).

Kinds among the 31 hand matches (recipes 2.20): byte strides m2c scaled by a typed base or pointer (8), tail-call
delay-slot arguments m2c dropped (4), callee return types (4), store order (11, most found by a search over all
orders), compare order (1), `?:` / `u64` shapes (2), a field copied as `f32` not `s32` (1). The first two look
mechanical enough for the tool (follow-up).

Rerunning a file's 3-8 word DIFFs the same way was not tried (71 in this file, 6.5).

### 6.9 The five ER-054 files with the current toolchain (ER-083 + ER-085, Sun 22 Nov 2026)

`--plain` runs with ee-gcc r1, `-fno-edge-lcm` (ruling 3.11) and ee-as r3 (r4 merged afterwards, regress still
green), default context, `--par 4`. Lessons in `docs/decomp_recipes.md` 2.20.

| Run | Files | Asm functions | Masked MATCH | Plain MATCH | Placed |
|---|---|---|---|---|---|
| `er083` (43 s) | `text_0014D930`, `text_0014E2E8` | 38 | 7 | 5 (+2 needing a declaration fix) | func_0014E120, func_0014E160, func_0014E740, func_0014E960 (`func_0014F298` returns `s32`), func_0014FE10 (clone prototype); func_0014F550 / func_00150350 were placed on main by ER-069 part 2 the same hour |
| `er083` rerun (`--rerun-diffs --chunk 8`, 13 s) | same | 8 same-size DIFFs | 0 | 0 | func_0014FBA0 by hand afterwards (store order) |
| `er085` (106 s) | `text_00145120`, `text_0014A968`, `text_0014C840` | 94 | 1 | 0 (+1 needing a declaration fix) | func_001492D0 (`func_0012B238` given its two parameters) |

By hand from the `er085` drafts (same-size DIFFs of 3-8 words, then div / sqrt): func_00146EF0, func_0014C748,
func_0014D050, func_00148360 (store order, `perm` script: 97 orders), func_00149DD8, func_0014C4D8, func_0014D3D0,
func_00146350, func_00149978, func_00145918, func_00147CF8, func_0014CCD0: 12, plus the 3 above and 4 from `er083`,
19 in all. A small store-order script (every order of N statements, cmpfunc per try; notes folder
`ER-085_c/perm.py`) found func_00148360 and func_0014FBA0 in under 100 compiles each.

### 6.10 Tail-j delay slots, byte offsets, const floats (ER-084 + ER-055, ER-097, ER-101, Sun 22 Nov 2026)

Three changes in `tools/m2c_bulk.py` (6, steps 1, 4 and 6) and a new census tool, `tools/sdata_consts.py`. Nothing
placed in `src/` by this card. Checked in worktree `C:\rs\wt\er084`; the reproductions set `src/text_0014E2E8.c` /
`text_00145120.c` back to `a751ee2^` and `text_00150878.c` back to `9fbc9a3^` (before ER-080), split, ran the tool,
and restored the files (never committed).

**ER-084 + ER-055: the tail `j`'s delay slot.** m2c drops the instruction in the delay slot of a `j func_X`; the asm
rewrite now writes `insn; j func_X; nop` (a `nop` slot and the stack pop are left alone).

| Check | Before (`--no-slot-move`) | After |
|---|---|---|
| func_001501D8 (`a751ee2^`) | diff | **MATCH**, drafted `func_0012FB48(arg0, 0)` (plain: the old file's own `extern void func_001501D8(void);`, which ER-062 aligned by hand) |
| func_00146EF0 (`a751ee2^`) | diff (the `swc1` missing) | **MATCH** (plain: `func_0012B238`'s old `(void)` clone prototype, ER-085's fix) |
| func_0019C078, func_001C8920 (`9fbc9a3^`; 2 of ER-080's 4 slot cases) | diff | **plain MATCH** |

**How often (ER-055)**, counted over the original ELF's code (every function, from the build's symbol sizes): 759
tail `j` (to another function); slots: 578 stack pop, 41 `nop`, **140 other instructions in 139 functions** (`addiu` 57,
`sw` 20, `daddu` 16, `lw` 11, `addu` 8, `sra` 7, `sb` 5, `and` 4, `sh` / `sd` / `swc1` / `lh` / `lwc1` 2 each, `lhu`
/ `ori` 1). Of them still in asm on main: 83 slots in **82 functions** (of 2,045 `INCLUDE_ASM`). The bulk tables of
6.1-6.9 are gone (`build/` is wiped by every regress), so the count is over the code, not the old tables.

**ER-097: byte offsets** (the cleaning step in 6.4). m2c prints pointer + offset with the raw byte count (its
`evaluate.py`: `add_imm` keeps the immediate, `handle_add_real` adds register and pointer), so a typed base or pointer
is scaled by C. Rerun of ER-080's examples (`9fbc9a3^`, 10 functions, no hand edits):

| Function | Shape | Result |
|---|---|---|
| func_00168358, func_0017CC28, func_001BFEB8 (card) | `(arg0 * N) + &D_X` -> `extern char D_X[];` | **plain MATCH** |
| func_0019C078, func_001C8920 (card) | tail-j slot (ER-084) | **plain MATCH** |
| func_0017D6F0 | `&D_X + 0x252` -> `extern char D_X[];` | **plain MATCH** |
| func_001BBD40, func_001BCD68 | `var += 4` on an `s32 *` -> `var += 1` | **plain MATCH** |
| func_001C1310 | `arg0 + 4` on an `s32 *` -> `arg0 + 1` | masked MATCH (context `none`); plain skipped: the draft declares `fptodp` (an ER-041 name), which the plain form does not take yet |
| func_001BCF48 | byte base, but m2c's `*(sum)` loses the game's `lhu` width | diff (needs `*(u16 *)` by hand, as ER-080 did) |

**ER-101: const `.sdata` floats.** `python -I tools/sdata_consts.py` scans every instruction of the original's
`.text` ($gp-relative accesses with gp 0x00360B70, absolute `lui` pairs) and every aligned data word: 665 `.sdata` /
`.sbss` symbols, 449 read by `lwc1`; **396 of the 445 `.sdata` floats are never stored and never have their address
taken**, 45 more are never stored but sit within 0x40 bytes after an address-taken symbol (not used: a pointer could
reach them; the window is a guess), 4 are stored; all 4 `.sbss` floats are stored. **The list does not tell the two
kinds of recipes 2.21 apart**: func_00145918's `D_0035BCF0` / `D_0035BCF4`, func_00147CF8's `D_0035BD58` /
`D_0035BD54` and func_00147F78's `D_0035BD40` (which must stay plain) are all never stored. So `const` is a variant
(6, step 6): tried on a draft that does not match, kept only on a MATCH. Rerun over the five ER-054 files: tried on 36
drafts, **no new match**; the three recipe functions stay diffs either way (their drafts are 4 / 4 / 1 instructions
off for other reasons, which ER-085 fixed by hand). In text_001000C0 no draft reads one of these floats (that file
uses `.lit4` literals).

**A/B on main** (`src/` as on main f714a26, default context, `--plain`, par 4; main's tool against this one):

| Files | Asm functions | Masked MATCH before -> after | Plain MATCH before -> after | Lost | Time |
|---|---|---|---|---|---|
| `text_00145120`, `text_0014A968`, `text_0014C840`, `text_0014D930`, `text_0014E2E8` | 111 | 0 -> **6** | 0 -> **5** | none | 216 / 212 s |
| `text_001000C0` | 446 | 4 -> **17** | 2 -> **13** | none | 1,085 / 1,167 s |

The 19 new masked MATCHes (16 plain): 17 by the slot move (func_001464A8, func_00148BB0, func_0014C630, func_0014D028,
func_0014DEA0, func_0014F5F8, func_00128928, func_0012B1F0, func_0012B208, func_0012B218, func_0012CAD8,
func_0012CAF0, func_0012E3B0, func_001329F0, func_00132CC0, func_001339F8, func_00134070: two of them with a store
order), 2 by byte offsets (func_00101160, func_0012C7B8). Four that main's tool matched as well are still unplaced in
text_001000C0 (func_0012F7E0, func_0012F808, func_0012F8A0, func_0012FEB8). Plain failures: func_0014DEA0,
func_0012CAD8, func_00134070, func_0012F8A0 (the file's own address-taking extern, recipes 2.10); func_0012F808 is a
plain MATCH that needs `func_0012B5F8` widened in the file. The drafts and both `results.md` are in the dispatcher's
notes folder `ER-084_c/` (game code: not in git); nothing was placed.

### 6.11 The whole of `text_00150878` in lists of 8 (ER-098, Sun 22 Nov 2026)

ER-060's run folder was gone, so the 71 + 116 same-size DIFFs of 6.5 had no list; the whole file went in instead:
`m2c_bulk.py --file text_00150878 --chunk 8 --plain --tag ER-098 --gcc-timeout 30` (label `er098all`; 146 chunks,
1,166 functions, 2,692 harvested signatures, **3,436 s** at par 4, worktree `C:\rs\wt\er095`). The tool was main's
before ER-084 / ER-097 / ER-101 (6.10) landed; their tail-j slot and byte-offset changes would have saved some of
the hand work below (byte strides came up 6 times). Results and every chunk's drafts (game code, not in git):
`C:\rs\wt\er098_runs\er098all\` (copied out before each regress; 189 MB with the drafts).

| Stage | Functions |
|---|---|
| m2c gave a draft | 1,165 (all but `func_00179B30`, every context `TIMEOUT`) |
| draft compiles | 807 |
| masked MATCH | 18 |
| plain MATCH | 17 (+ func_001C4750: the file's extern aligned, 2.25) |
| same size, 1-2 / 3-8 / 9+ words differ | 3 / 66 / 140 |
| size differs | 580 |

- **All 18 placed**, and 50 more by hand from the drafts (recipes 2.25): 68 functions, 7,348 bytes for ER-098.
  The small lists turned only 18 of the file's 1,166 into MATCHes (ER-080's 48 near misses had given 16), but they
  gave a near-miss list to work from: all of the 1-5 word ones and most of 6-8 are now placed.
- **Field order is the biggest single cause** (20 of the 50): the drafts list stores in the game's schedule order;
  sorting each run of consecutive stores by offset gives the game's schedule. A scratch script
  (`asc_sort.py` in the dispatcher's notes folder `ER-095_096_098_c/`) does it on the drafts (three modes: offset
  order, bytes last, bytes first; it skips runs where a store reads a field the run writes) and recompiles: over the
  173 same-size DIFFs not matched by then, **6 new MATCHes** (5 of them 5-8 words, func_001898F0 9+), each confirmed
  plain and placed. Worth adding to `m2c_bulk.py`'s store-order variant (follow-up): today's variant tries only
  permutations of plain top-level runs up to 4 statements, not offset order of longer runs or runs inside blocks.
- `--rerun-diffs` could not be used (no earlier `results.json`); `--file ... --chunk 8` cost about the same as
  ER-060's file-wide run (1.2 s against 1.2 s per function) and needs no list.

### 6.12 Broken context, own declarations, offset order, ruling 3.14, soft doubles (ER-130, ER-124, ER-121, ER-114, Mon 23 Nov 2026)

Changes in `tools/m2c_bulk.py`; nothing placed in `src/`. Worktree `C:\rs\wt\er114`, branch `er-114-m2c-bulk-2`. The
reproductions set `src/` files back to old states in the worktree (`text_001000C0` at `9527f54^`, the five ER-054
files at `87fc92e^` and at `554b0ee^`, `text_00150878` at `acb6a6a^`), split, ran main's tool and this one, and
restored the files (never committed). Drafts and tables (game code, not in git): dispatcher's notes folder `ER-114_c/`.

**ER-130 (b): the context was broken, not `--list`.** ER-092's `typedef struct {...} G390; extern G390 D_001FF390;`
in `text_001000C0.c` (commit `1b84924`, 22 Nov 2026, 17:04) went into m2c's context, which m2c then refused as a whole
("Syntax error when parsing C context"). From then on every harvest draft failed (`0 signatures, N without a draft`)
and every `harvest` / `src` attempt fell back to `none`, silently: ER-112's "0 harvested signatures", ER-115's 9 of 17
and func_0012C3E8's lost `$a0` all come from this. `--list` and `--file` harvest alike; no file-wide harvest was
needed. Fix: `src/` declarations whose type the context does not know are left out and listed (here `G390`, `Vec4`:
4 lines), and the run warns when m2c cannot parse the context. Since the working context changes many drafts, the
pass-through retry (6.7) now runs on each compiled attempt in score order (at most 3), not only on the best one:
func_0012F900 matched only through the `none` draft's retry.

**ER-130 (a): the file's own extern for the target.** In the plain form: the file's line is declared when it agrees
with the definition (an unprototyped `()` agrees on parameters); with the same parameter count the definition is
compiled with the file's types (kept on a plain MATCH: nothing in the file changes; Plain column `definition takes
the file's own prototype`); otherwise the line is left out of the plain file and the Plain column says to align it
before placing (the plain compile used to fail "conflicting types").

| Check (old state) | Main's tool | This tool |
|---|---|---|
| ER-112's 17 `text_001000C0` names, `--list` (`9527f54^`) | 0 signatures, 9 masked / 8 plain MATCH | 19 signatures, **17 masked / 17 plain MATCH** (func_0012B218, func_0012CAD8 take the file's prototype; func_00134070, func_0012F8A0 flagged to align) |
| ER-112's five ER-054 names (`87fc92e^`) | 0 signatures, 3 masked / 1 plain | **5 / 5 plain** (func_0014C630, func_0014D028 get func_0012C3E8's `$a0`; func_0014DEA0 flagged: the file's `(void)` extern) |

**ER-124: offset order** (6, step 6). Recipes 2.25's five examples, `text_00150878` at `acb6a6a^`, no hand edits:

| Function | Main's tool | This tool |
|---|---|---|
| func_00187F78, func_001918B8, func_001AC938, func_001B3440, func_001898F0 | size -1 instr (context `none`) | **plain MATCH**, `store order by offset (asc)` (the working context alone gives same-size 5-9 word DIFFs) |

**ER-121: ruling 3.14.** ER-100's eight ruling 3.14 functions (five files at `554b0ee^`): **func_0014C110 plain MATCH**
with `(void)&arg0`; func_0014A0B8 (7 words), func_00148BE0 (6), func_00148D28 (6) become same-size near misses
(listed in Notes, not kept); func_00149EE8, func_0014A500, func_001485A8, func_0014B528 stay off. That is ER-100's
"1 MATCH, near misses" picture.

**ER-114: soft doubles and the lost width.** The plain form takes named declarations (`sqrtf`, SDK names), and writes
the GOFAST calls m2c names as plain double C (recipes 2.11: never call them by name): `fptodp(x)` -> `((double) x)`,
`litodp` / `dptofp` / `dptoli` / `dptoul` -> casts, `dpadd` / `dpsub` / `dpmul` / `dpdiv` -> `+ - * /`, `dpcmp(a, b) OP 0`
-> `(a OP b)`; any other use is skipped with the reason. **func_001C1310: plain MATCH** (`func_001BE0B0(arg0 + 1,
((double) ((f32) (func_001E9C70() & 0xFFFF) / 65535.0f)))`). **func_001BCF48: plain MATCH** (the deref width, 6
step 4). Not done: byte arithmetic for a register added to a typed pointer (`v + i * 4`; no example to check it on).

**A/B on main** (`src/` as on main `a739927`, default context, `--plain --gcc-timeout 30`, par 4, the runs side by side):

| Files | Asm functions | Masked MATCH old -> new | Plain MATCH old -> new | Lost | Compile | Time old / new |
|---|---|---|---|---|---|---|
| the five ER-054 files | 74 | 0 -> 0 | 0 -> 0 | none | 68 -> 74 | 51 / 171 s |
| `text_001000C0` | 425 | 1 -> 2 | 0 -> 2 | none (func_0012F900 after the retry change, rerun alone) | 291 -> 312 | 291 / 904 s |
| `text_00150878` | 1,095 | 16 -> 21 | 12 -> 21 | none | 691 -> 740 | 812 / 2,304 s |

New plain MATCHes on main (none placed; drafts in `ER-114_c/`): `text_00150878` func_00162108, func_00165080 (ruling
3.14), func_00180200, func_00183930, func_0018AAB8 (working context), and func_00157A50, func_001AEC58, func_001B75E0
(masked MATCHes whose plain form failed "too few arguments" before), func_00164948 (file's `extern s32
func_00164948();` to align); `text_001000C0` func_0012DB88 (offset order), func_0012F900 (file's extern
`(char *, int, int, int, int, float)` to align: the draft drops the float, which passes through in `$f12`; the
file's six parameters are the likelier reading, Unconfirmed). **The tool is about 3 times slower** with a context
that parses (more attempts compile, harvest drafts succeed). `make regress` PASS after the runs (ELF unchanged).

### 6.13 `text_001000C0` rerun with ER-114's tool (ER-137 + ER-125, Mon 23 Nov 2026)

Worktree `C:\rs\wt\er125`, branch `er-125-text-001000c0`, main `b95ea9a` (after ER-137's two placements). Runs and
drafts (game code, not in git): `C:\rs\wt\er125_runs\` (`er125file`, `er125rr`; ER-137's two-function run `er137`
was wiped by a regress before it was copied out, its two drafts were rewritten by hand in `C:\rs\wt\er125_scratch\`).

| Run | Command | Functions | Result | Time |
|---|---|---|---|---|
| `er137` | `m2c_bulk.py --plain --tag ER-137 func_0012DB88 func_0012F900` (par 2) | 2 | 2 masked / 2 plain MATCH (func_0012F900 flagged: the file's extern to align) | 6 s |
| `er125file` | `m2c_bulk.py --plain --tag ER-125 --file text_001000C0 --par 4 --gcc-timeout 30` | 423 (443 harvested signatures) | **0 MATCH**; 310 compile; same size 74 (1-2 words 2, 3-8 words 27, 9+ 45); size differs 236; no compile 113 | 599 s |
| `er125rr` | `--rerun-diffs build/m2c_bulk/er125file --chunk 8` (9 chunks) | 71 | **0 MATCH, none closer** than in `er125file` | 161 s |

- The whole-file run found nothing new: ER-114's A/B had already shown this file's only two plain MATCHes
  (func_0012DB88, func_0012F900, ER-137's share, placed first). Lists of 8 (`--rerun-diffs --chunk 8`) gave the
  same drafts as the file-wide harvest here.
- **The near-miss list was the useful output**: 27 of the 74 same-size DIFFs placed by hand (2,556 bytes), most in
  minutes (recipes 2.29). The biggest single cause (13 functions) was m2c's `M2C_UNK sp[64]`: drafts that differ only
  in `$sp` offsets. A scan for that is cheap: a same-size DIFF whose `!!` lines all mention `$sp` (9 here; 3 of them
  string-literal functions). Worth adding to `m2c_bulk.py` (follow-up): size the stack locals from the game's frame
  (the lowest saved-register slot) and retry.
- Left in the file: 396 `INCLUDE_ASM` (of the run's 423, after 27 placed). Of them by the `er125file` draft: same size
  47 (3-8 words 11: func_00111700, func_0012D9B8, func_00101898, func_001033F0, func_00128940, func_00125790,
  func_00134090, func_00101740, func_00103470, func_0012E6B8, func_00130B18; 9+ words 36), size differs 236, no
  compile 113. Parked causes: recipes 2.29.
- `make regress` PASS after every batch (ELF unchanged, 3,606 functions match).

## 7. Not verified

- ER-130 / ER-124 / ER-121 / ER-114 (6.12): none of the new plain MATCHes is placed, so none is proven by `make
  regress`; the A/B runs used the tool before the per-attempt pass-through change (that change only adds attempts;
  func_0012F900 rerun alone with the final tool). The type filter of the context keeps a declaration only when every
  type word is a basic type (a typedef from `include/` would also be left out; none met). The offset sort handles
  `M2C_FIELD` stores only (not `*p = x` or array stores). The ruling 3.14 variant takes the first parameter only
  (ER-100 also used a local, func_0014B528's `g`); the soft-double rewrite was checked on func_001C1310 only
  (`fptodp`); `dpcmp` and the arithmetic forms are untested. The deref width needs exactly one non-stack memory
  access in the function.

- ER-084 / ER-097 / ER-101 (6.10): the slot move with a label on the delay-slot line (skipped by design, not met);
  the byte rewrites on a base declared in `src/` (the `(char *)` cast path; not met in the reruns), on a register
  added to a typed pointer (`v + i * 4`, not handled), and on struct-typed pointers (skipped); the census's lui
  pairing is per function and keeps the last `lui` per register (a pairing across a branch can be wrong, which can
  only add accesses). None of the 19 new matches is placed, so none is proven by `make regress` yet.

- The tools were checked on the functions named above only; relocation types other than the five handled
  (`R_MIPS_26`, `HI16`, `LO16`, `GPREL16`, `LITERAL`) are reported as "unhandled" (none met yet).
- `place.py c` on a function with `INCLUDE_RODATA` neighbours that it takes over: not met yet (the text edit works; the
  standalone symbol needs dropping by hand).
- m2c: the six-function sample above only.
- `m2c_bulk.py --plain` (ER-054): run on five game files (241 functions); its 78 plain MATCHes there are placed and
  proven by `make regress`. Not met yet: a plain draft with `M2C_BITWISE`, a struct-typed `%hi` / `%lo` extern, a
  `&M2C_FIELD` before a postfix operator (handled in code, untested).
- `m2c_bulk.py` (ER-050): its first run placed nothing (masked compares only); ER-054 placed the matches of these
  files. Drafts that
  match only with the `%hi` / `%lo` section-attribute extern need the recipe form before placement. Not tried on
  SDK code, VU0 / MMI functions, jump tables in `text_001343E8`, or functions over 255 instructions. The failure
  kinds in 6.1 come from reading a few diffs, not from a count.
- ER-066: the pass-through detection was checked on ER-062's functions and the ER-064 run only (one placed match,
  `func_00149880`); a callee reached through a register (`jalr`) is not looked for; a variadic callee prototype is
  left alone. The timeout's job-object kill was checked on `func_00179B30` only (Windows); outside Windows the
  tool kills the driver only (not tested). Why small lists matched 4 more drafts for ER-062 is still not traced
  (no new match from them in ER-064).
