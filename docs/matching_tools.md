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
| m2c (`mipsee` target) | first drafts from splat's asm (section 4); not in this repo, run from Twinblade's copy |

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
| `R_MIPS_HI16` + `R_MIPS_LO16` | the game's `%hi` / `%lo` pair gives the symbol's address plus our addend |
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

- `c` drops the scratch file's `#include` lines and any line already present in the target file (an extern that is
  already there), and puts the rest (declarations, a comment, the definition) where the `INCLUDE_ASM` line was. The
  scratch file must define that function and no other.
- `asm` removes the definition and the comment directly above it; extern lines above stay and are listed (other
  functions may use them).
- Files are written with LF. The tool edits text only: run `make regress` afterwards.
- Checked (ER-015): `extract`, `asm`, `c` round trip on four placed functions (with comments and externs) leaves
  `src/` byte-identical (`git status` clean).

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
how often a raw draft matches (that is ER-016's measurement).

## 6. Not verified

- The tools were checked on the functions named above only; relocation types other than the five handled
  (`R_MIPS_26`, `HI16`, `LO16`, `GPREL16`, `LITERAL`) are reported as "unhandled" (none met yet).
- `place.py c` on a function with `INCLUDE_RODATA` neighbours that it takes over: not met yet (the text edit works; the
  standalone symbol needs dropping by hand).
- m2c: the six-function sample above only.
