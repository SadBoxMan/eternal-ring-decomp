# Building SLUS_200.15 (ER-003)

Written Tue 17 Nov 2026 by ER-003 (Opus 5.5, high). The build turns the splat split of the US main EE ELF back
into a file whose **SHA-1 equals the original's: the whole ELF, headers and non-loaded sections included**
(`6087A8CADB7538A849A4F5760502A39DAA6A12AF`, 2,485,924 bytes). Everything runs natively on Windows (MSYS2 tools
and Windows Python; no WSL, Docker or hypervisor).

## 1. Daily use

From the repo root, in PowerShell (or an MSYS2 shell without the `$env:PATH` line):

```
$env:PATH = "C:\msys64\usr\bin;" + $env:PATH
make split       # once after a clone, and after any change to the yaml or symbol files
make -j4         # build/SLUS_200.15.elf
make check       # "build/SLUS_200.15.elf: OK" = byte-identical
make regress     # the gate: clean split + clean build + SHA-1 + per-function check (about 30 s)
```

| Target | What it does |
|---|---|
| `make split` | `tools/extract_rom.py` (PT_LOAD bytes to `baserom/`), `tools/split.py` (splat), `tools/gen_elf_extras.py` (DVP sections) |
| `make` / `make all` | builds the stripped `build/SLUS_200.15.elf` (the comparison target) |
| `make build/SLUS_200.15.syms.elf` | the same link with symbols, for tools and the per-function check |
| `make check` | `sha1sum -c` of the built ELF against `tools/regress_baseline.txt` |
| `make regress` | `tools/regress.py`: 1 original still pinned, 2 clean split, 3 clean build, 4 SHA-1, 5 `tools/func_check.py` |
| `make clean` / `make distclean` | remove `build/` / also `asm/`, `linker/`, `baserom/`, `assets/` |

`tools/regress.py --no-split` keeps `asm/` (still a clean build); `--staged` is the commit-gate mode. The log of
the last run is `build_logs/regress.log`.

**Commit gate:** `tools/hooks/pre-commit` and `pre-merge-commit` run `tools/regress.py --staged`, which exits at
once unless a staged path can change the build (`src/`, `include/`, `asm/`, the yaml, symbol files, `Makefile`,
`requirements.txt`, the build tools under `tools/`). Enable once per clone: `git config core.hooksPath tools/hooks`
(done in this clone by ER-003). The gate builds the working tree, so commit whole files. Skip only on purpose with
`RINGSMITH_SKIP_REGRESS=1` and say so in the commit message. A fast-forward merge runs no hook: run `make regress`
after it.

## 2. Fresh-machine setup

Needs: Windows, MSYS2 at `C:\msys64`, Python 3.12 (python.org build, `py` launcher), Git for Windows, and the
dumped disc (ER-001, `docs/disc_layout.md`).

1. MSYS2 packages: `pacman -S --needed make gcc bison sed coreutils findutils gawk diffutils`. ER-002 / ER-011
   used MSYS2's own GCC 15.3, bison, make and sed (already installed here); the build itself uses make, bash,
   find, sort, awk and sha1sum. The exact package list on a new machine is not verified.
2. The EE toolchain from the GPL source, at a short path without spaces:
   ```
   git -c core.autocrlf=false clone https://github.com/SSXModding/ps2-ee-toolchain.git C:\rs\toolchain\src-lf
   git -C C:\rs\toolchain\src-lf checkout b595ded606227e93b8c4a447446c1d2ac093827d
   ```
   The scripts are in this repo (`tools/toolchain/`) but write to `/c/rs/toolchain` (their `ROOT`), so the repo
   path with its space never reaches the 1999 configure scripts. From PowerShell:
   ```
   $env:MSYSTEM = "MSYS"
   C:\msys64\usr\bin\bash.exe -lc "'/c/Users/<you>/Claude/Projects/Project Ringsmith/Decomp/tools/toolchain/build_ee_gcc.sh'"
   C:\msys64\usr\bin\bash.exe -lc "'/c/Users/<you>/Claude/Projects/Project Ringsmith/Decomp/tools/toolchain/build_ee_binutils.sh'"
   ```
   Run `build_ee_gcc.sh` first: it patches `config.sub`, which the binutils script checks for. Run the binutils
   script from the repo (not a copy): it applies our gas patch from `tools/toolchain/patches/` (ER-026; the c.cond /
   bc1 `nop`, ruling `docs/decomp_recipes.md` 3.4) to `src-lf` before building. Results:
   `C:\rs\toolchain\ee-gcc-2.9-991111\` (`gcc.exe`, `cpp.exe`, `cc1.exe`) and
   `C:\rs\toolchain\ee-binutils-2.9-991111-r1\` (`ee-as.exe`, `ee-ld.exe`, ...; the Makefile's `BIN_DIR`). The
   unpatched assembler cannot build the game any more. Details and pitfalls: `docs/toolchain.md` 4.1 / 4.2.
   Another toolchain folder: `make TOOLCHAIN=D:/path ...`.
3. Python venv in the repo root:
   ```
   py -3.12 -m venv .venv
   .venv\Scripts\python.exe -m pip install -r requirements.txt
   ```
   (`requirements.txt` pins splat64 0.50.1, spimdisasm 1.42.4, rabbitizer 1.16.2 and their dependencies.)
4. The disc files under `disc/extract/` (ER-001; `disc/` is gitignored). The build reads only
   `disc/extract/SLUS_200.15`; another place: `make BASEELF=path ...` (the regress tool expects the default).
5. `git config core.hooksPath tools/hooks`, then `make split`, `make regress`: it must end `regress: PASS`.

Toolchain binaries, the venv, the disc and all split / build output stay out of git (`.gitignore`).

## 3. How the build works

```
disc/extract/SLUS_200.15 --extract_rom--> baserom/SLUS_200.15.rom (PT_LOAD bytes, 0x25C410)
                         --split.py (splat)--> asm/ (crt0, data, nonmatchings), linker/ (splat .ld, undefined syms)
                         --gen_elf_extras--> asm/elf/elf_extras.s (the 6 DVP overlay sections, as .byte)
src/**/*.c --gcc -O2 -G8 -S--> build/src/**/*.s --ee-as--> .o     (INCLUDE_ASM pulls in asm/nonmatchings)
asm/**/*.s, src/elf_layout.s --ee-as -G8 -O2 -mabi=eabi--> .o
linker/*.splat.ld --gen_ld.py--> build/SLUS_200.15.ld (SCE app.cmd layout)
all objects, crt0 first --ee-ld -s--> build/SLUS_200.15.elf  --sha1--> OK
```

| Piece | Why |
|---|---|
| `slus_200.15.yaml` | one segment `main` (vram `0x00100000`, rom = PT_LOAD bytes): crt0 kept as `asm`, the rest of `.text` as 32 `c` files of INCLUDE_ASM (census regions, the game region cut further at 7 seams by ER-022 and 7 by ER-051; 3.1), `.data`, `.vudata` (kept as data words), `.rodata`, `.lit4`, `.sdata`, `.sbss`, `.bss`; the game files' own `.rodata` / `.lit4` (and for ten files `.sdata` / `.data`) as dotted subsegments named after the C file (ER-024, 3.2), the rest of each data section as shared asm; `pad` subsegments for the alignment gaps between sections |
| `tools/split.py` | runs splat, writing denormal floats as `.word` (ee-as 2.9 misreads them; `docs/decomp_recipes.md` 1.3), plus the per-file data steps of 3.2 (rodata order check and INCLUDE_RODATA files, `.lit4` loads as `li.s`, `.align 4` items, INCLUDE_SDATA / INCLUDE_DATA files) |
| `include/macro.inc`, `labels.inc`, `include_asm.h`, `common.h` | splat's macros rewritten for ee-as 2.9 syntax; `macro.inc` also includes the generated `gp_externs.inc` |
| `tools/gen_gp_externs.py` | `build/include/gp_externs.inc`: `.extern sym, 4` for every symbol reached through `$gp`, so ee-as's `lw $2, sym` macros come out gp-relative (ee-as has no `%gp_rel`) |
| `tools/gen_elf_extras.py` | the sections after the load image that the whole-file SHA-1 needs: `.DVP.ovlytab`, `.DVP.ovlystrtab`, four `.DVP.overlay.*` (bytes from the original; `ee-dvp-as` is not built) |
| `src/elf_layout.s` | hand-written, no game bytes: the empty `.vutext` / `.vubss` and empty input sections that carry the original section alignments (`.text` 64, `.data` / `.bss` 128, others 16) |
| `tools/gen_ld.py` | rewrites splat's one-section script into SCE's layout: `.text .vutext .reginfo .data .vudata .rodata .lit4 .sdata .sbss .bss .vubss .DVP.ovlytab .DVP.ovlystrtab`, each 128-aligned except the VU ones, `_gp = ALIGN(128) + 0x7FF0` before `.lit4`, one RWX `PT_LOAD` |
| link order | crt0's object first and all objects before `-T`: ld keeps only the first `.reginfo` (link-once), which must be crt0's |
| `tools/func_check.py` | every function symbol of the unstripped link compared with the original bytes, and every `func_` / `D_` name checked against its address |

### 3.1 Source file layout (ER-013, ER-022, Wed 18 Nov 2026; ER-051, Sun 22 Nov 2026)

`.text` after crt0 is cut into one `c` subsegment per region of `docs/census.md` 4.1, seams on function starts
(census 4.3 has each seam's evidence and grade); ER-022 cut the game region further at the 7 Strong seams of
`docs/game_seams.md` whose data cuts line up (section 8 there), and ER-051 at seven more (the five exact seams inside
S1's file, S22 and `0x001CB528` in game-unk; section 9.10 there). Each file's INCLUDE_ASM points at its own folder
`asm/nonmatchings/<file>/`. ER-022 left the data as one subsegment per section (`docs/game_seams.md` 8.4); since
ER-024 the game files own their data where the references pin it (3.2).

| File | Starts | Units (C) | Region |
|---|---|---|---|
| `src/text_001000C0.c` | 0x001000C0 | 576 (22) | game: main, boot and everything up to the first Strong seam |
| `src/text_001343E8.c` | 0x001343E8 | 83 (3) | game, seam S1 (`docs/game_seams.md` 8.2); S3 inside, uncut (waits for matching) |
| `src/text_001388E8.c` | 0x001388E8 | 41 (2) | game, exact seam (ER-051, `docs/game_seams.md` 9.10) |
| `src/text_0013A410.c` | 0x0013A410 | 19 (1) | game, exact seam (ER-051) |
| `src/text_0013B748.c` | 0x0013B748 | 16 (2) | game, exact seam (ER-051) |
| `src/text_0013C510.c` | 0x0013C510 | 33 (2) | game, exact seam (ER-051); func_0013D4D0 is the first C jump table here |
| `src/text_0013DBC0.c` | 0x0013DBC0 | 146 (11) | game, exact seam (ER-051), up to S9 |
| `src/text_00145120.c` | 0x00145120 | 105 (2) | game, seam S9 |
| `src/text_0014A968.c` | 0x0014A968 | 52 (2) | game, seam S12 |
| `src/text_0014C840.c` | 0x0014C840 | 30 (2) | game, seam S15 |
| `src/text_0014D930.c` | 0x0014D930 | 23 (6) | game, seam S16 |
| `src/text_0014E2E8.c` | 0x0014E2E8 | 59 (0) | game, seam S17 |
| `src/text_00150878.c` | 0x00150878 | 1,542 (83) | game, seam S20, up to the VU0 helpers |
| `src/text_001C9B18.c` | 0x001C9B18 | 28 | game VU0 helpers (Unconfirmed) |
| `src/text_001CA0F0.c` | 0x001CA0F0 | 9 | game-unk (Unconfirmed) |
| `src/text_001CA8D8.c` | 0x001CA8D8 | 19 | game-unk, seam S22 (ER-051; `.bss` decides) |
| `src/text_001CB528.c` | 0x001CB528 | 49 | game-unk, seam `0x001CB528` (ER-051; `.bss` decides) |
| `src/sdk/libgraph.c`, `libkernl.c`, `libdma.c`, `vudev.c`, `libpad.c`, `libvu0.c`, `libcdvd.c`, `libmpeg.c`, `libipu.c`, `libsdr.c`, `libmc.c`, `libm.c`, `libgcc.c`, `libc.c` (2 C), `libkernl_tail.c` | 0x001CE130 on | 775 | SDK libraries in link order |

A new game file is cut by adding a `c` line to the yaml and moving its INCLUDE_ASM lines (with the folder renamed)
into a new `src/` file; splat never overwrites an existing `.c`. `make regress` re-splits from clean, so stale
`asm/nonmatchings` folders do no harm.

Generated, gitignored (game data or derived from it): `asm/`, `linker/`, `baserom/`, `assets/`, `build/`,
`build_logs/`. Committed: the yaml, `symbol_addrs.txt`, `reloc_addrs.txt`, `src/`, `include/`, `Makefile`, the
tools.

### 3.2 Per-file data (ER-024, Wed 18 Nov 2026)

A C file's own float literals, jump tables, strings and variables land in its own object (ER-014's blocker,
`docs/decomp_recipes.md` 2.2). So each C file now **owns** the run of each data section that its functions use:
the linker places `build/src/<file>.o(<section>)` where that run lies, and the data around it stays shared asm.
`make regress` is green with all of it (whole-ELF SHA-1 and all 3,606 functions).

**What is owned** (cuts from the references splat / spimdisasm resolve in the asm; `docs/game_seams.md` 8.6):

| Section | Owners | Still shared asm |
|---|---|---|
| `.lit4` | all 13 game files that use it: `text_001000C0` 0x00358B80, `text_001343E8` 0x003592C4, `text_001388E8` 0x003593F8, `text_0013A410` 0x00359460, `text_0013B748` 0x003594D0, `text_0013C510` 0x00359504, `text_0013DBC0` 0x0035957C, `text_00145120` 0x003597CC, `text_0014A968` 0x00359970, `text_0014C840` 0x003599D4, `text_0014D930` 0x00359A1C, `text_0014E2E8` 0x00359A4C, `text_00150878` 0x00359ACC to 0x0035B784 | nothing (no SDK file uses `.lit4`) |
| `.rodata` | `text_001000C0` 0x00353E00, `text_001343E8` 0x003546D8, `text_001388E8` 0x00354750, `text_0013A410` 0x003547D0, `text_0013B748` 0x00354830, `text_0013C510` 0x00354850, `text_0013DBC0` 0x00354910, `text_00145120` 0x00354990, `text_0014D930` 0x00354A28, `text_0014E2E8` 0x00354A30, `text_00150878` 0x00354A90 to 0x00356AF0 (`text_0014A968`, `text_0014C840` have none) | `rodata_00356AF0` (the SDK, libgraph on) |
| `.sdata` | `text_001388E8` 0x0035BA08, `text_0013A410` 0x0035BA68, `text_0013B748` 0x0035BAB0, `text_0013C510` 0x0035BAC8, `text_0013DBC0` 0x0035BB08, `text_00145120` 0x0035BC94, `text_0014A968` 0x0035BDD8, `text_0014C840` 0x0035BE3C, `text_0014D930` 0x0035BE90, `text_0014E2E8` 0x0035BEC8 to 0x0035BF2C | `sdata_0035B800` (`text_001000C0`, `text_001343E8`), `sdata_0035BF2C` (`text_00150878` on) |
| `.data` | `text_001388E8` 0x0032FE68, `text_0013DBC0` 0x0032FEA0, `text_00145120` 0x0032FF20, `text_0014A968` 0x0032FF90, `text_0014C840` 0x0032FFB0, `text_0014E2E8` 0x0032FFE0 to 0x00330208 (`text_0013A410`, `text_0013B748`, `text_0013C510`, `text_0014D930` have none) | `data_001F0380` (`text_001000C0`, `text_001343E8`), `data_00330208` (`text_00150878` on) |

`.sdata` / `.data` of `text_001000C0`, `text_001343E8` and `text_00150878` are not owned yet: those files are each
several real files (uncut seams), their runs mix globals used across files, and the ends are not checked symbol by
symbol (ER-051: `text_001343E8`'s runs are now pinned at both ends, `.sdata` 0x0035B9F0-0x0035BA08 and `.data`
0x0032FD20-0x0032FE68, `docs/game_seams.md` 9.10; left for ER-032). The new game-unk files (`text_001CA0F0`,
`text_001CA8D8`, `text_001CB528`) use no data of the owned sections. `.sbss` / `.bss` are not owned (NOBITS; `.bss` mixes COMMON).

**How a file gets its own data** (the recipe):

1. Find the run: list the section's labels with the files whose functions use them (spimdisasm's references, as in
   the asm). The file's run starts at its first label and ends where the next file's first label starts; every label
   inside must be used by this file only or by no function (data-only or unreferenced labels between two of the
   file's labels are its own by position). A label used only by a far file inside the run is a global defined here
   (noted in `docs/game_seams.md` 8.6). If an end is not pinned, leave the section shared: do not guess a cut.
2. In the yaml, give the run a dotted subsegment named after the C file (`[rom, .rodata, text_X]`, `.lit4`, `.sdata`,
   `.data`), and start the shared asm again after it with a plain subsegment (`[rom, rodata, rodata_<vram>]`).
   rom = vram - 0x00100000.
3. `.rodata` only: run `.venv\Scripts\python.exe -I tools/split.py slus_200.15.yaml --fix-includes` once (after
   `rm -rf asm linker`). splat migrates each function's own `.rodata` (jump tables, strings, floats used by one
   function) into its INCLUDE_ASM file; the other symbols (used by several functions, only from data, or not at
   all, and const tables spimdisasm does not migrate) need `INCLUDE_RODATA(folder, sym);` lines in the C file, in
   splat's order. `--fix-includes` inserts them (each just before the next function that has migrated rodata). The
   normal split then checks the order on every run and fails with the first difference.
4. `.sdata` / `.data`: the same `--fix-includes` run writes an `INCLUDE_SDATA` / `INCLUDE_DATA` block (every symbol
   of the run, in address order) after the `#include`; later splits check it.
5. `.lit4`: nothing to edit. `tools/split.py` writes each INCLUDE_ASM `lwc1 $f, (D_x)` of an owned literal back as
   `li.s $f, <value>`: gas then builds the object's literal pool in source order, the asm functions' literals and the
   C functions' together, exactly as the original compile did (each literal word is used by one instruction, never
   merged). A labelled word in `.lit4` cannot be used instead: ee-as asserts when a gp-relative macro names a symbol
   defined in `.lit4` (`gas/config/tc-mips.c`, `nopic_need_relax`).
6. `make regress`, commit the yaml and the C files.

**Alignment.** Each object's section is as aligned as its most aligned item, and the linker starts each object on
that boundary. gcc emits jump tables with `.align 4` (16 bytes); spimdisasm writes `.align 3`. `tools/split.py`
writes `.align 4` before every jump table and before every label on a 16-byte boundary that has 8+ zero bytes before
it in the file's run (8-byte alignment cannot explain such a gap), so when the item before it comes from C (without
the zeros), the padding still comes out. While everything is asm the directive changes nothing. Not in
`text_001343E8` (its `.rodata` starts at 0x003546D8, not 16-aligned): there a 16-aligned item would move the whole
object, so **a C jump table cannot be placed in `text_001343E8`** until the file is cut where its real files meet
(`docs/game_seams.md` 8.6; seam S3 waits for matching, LuciDin 22 Nov 2026). The five files ER-051 cut from it
start 16-aligned and take C jump tables (func_0013D4D0, `docs/game_seams.md` 9.10). `text_0014D930`'s run is one 8-byte item, so nothing to do.

**The same rule for 8 bytes (ER-072, Sun 22 Nov 2026).** A label on an 8-byte boundary with 4 zero bytes before it in
the file's run, not already 16-aligned by the rule above, gets `.align 3` where spimdisasm wrote none (it already
writes `.align 3` before most doubles). Needed when the item before is a jump table that comes from C: the asm table
carried the pad word, gcc's table ends 4 bytes earlier, and the double after it moved -4 (`D_00354788`, `D_003547C8`
after func_00138A20 / func_00139EE0). Same file limit (16-aligned runs only). With no `src/` change it leaves the ELF
identical (`make regress` PASS); the split adds the directive to 19 generated files: `text_001000C0` `D_00354360`;
`text_001388E8` `D_00354788`, `D_003547C8`; `text_0013DBC0` `D_00354928`, `D_00354968`; `text_0014E2E8`
`D_00354A68`; `text_00150878` `D_00354AF8`, `D_00354D70`, `D_00354EA0`, `D_00354FD0`, `D_00354FE0`, `D_00354FF8`,
`D_003551F8`, `D_00355A50`, `D_00355D80`, `D_003568C0`, `D_003568D0`, `D_00356900`, `D_00356930`. Like the 16-byte
rule this is evidence from zero bytes, not proof that the original was aligned: a real zero word before an 8-aligned
item looks the same (Unconfirmed per item until a C neighbour proves it).

**Anonymous constants from C (ER-077, Sun 22 Nov 2026; ruling 3.10).** When a C function's float or double
literal (a `float` compared with a double constant, `fabs(x) <= 0.17453293f`) was a standalone `D_` symbol in the
split, gcc writes it as an anonymous `.rodata` constant (`$LCn`) in the function's own place (the game has such a
double right after the function's jump table: `jtbl_003547E0` then `D_00354800` for func_0013AA28, likewise
func_0013BD90 and func_0013DF90). There is no C name to define, so the order check lets that `INCLUDE_RODATA` line go, but only if
all of these hold (`_anon_takeovers` in `tools/split.py`): the symbol is standalone and 4 or 8 bytes long; exactly
one function references it, from code, and that function is a C definition in this file; no data refers to it; and
between that function and the symbol splat's order has only functions without `.rodata` of their own (or other
symbols the same function takes over), so the constant lands where the symbol was. Anything else stays an error: a
dropped line whose user is still INCLUDE_ASM, two lines swapped (both tested, with the error text as before), a
symbol two functions share. Each split prints the symbols taken over ("anonymous C constants in place of (3.10)").
With no `src/` change the rule changes nothing (`make regress` PASS, ELF identical); `make regress` still proves the
bytes of every placement. Proven by three placements (`make regress` PASS after each): func_001371C0 (`D_00354718`),
func_0014DF08 (`D_00354A28`), func_0014EEF0 (`D_00354A68`), none with a jump table (`docs/decomp_recipes.md` 2.14). Not
covered: a symbol longer than 8 bytes (`D_00354800` is the double plus 8 zero bytes; func_0013AA28, parked, would need it).

**Gotchas.** Run a clean split after changing the yaml (`rm -rf asm linker`, or `make regress`): stale
`asm/data/*.s` files from an old cut would be linked twice. A C file that defines one of its data symbols itself
(`static const float D_x[4] = ...`) drops that INCLUDE line; the check accepts the gap and the byte check judges the
position. A float literal is written as a literal in C (`1500.0f`): owned `.lit4` words have no labels any more,
so an `extern float` stand-in (ER-014's workaround) no longer links. A variable of an owned `.sdata` / `.data` run
can still be used through `extern` from C in the same file (`func_00150818` reads `D_0035BF20` so).

## 4. Numbers (measured by ER-003, Tue 17 Nov 2026)

| Item | Value |
|---|---|
| Functions with a body (function symbols with a size in the linked ELF) | **3,606**: 4 in crt0 + 3,475 INCLUDE_ASM + 127 already in C |
| Other split units in `.text` | 3 blocks splat labels as data (`func_001D73D0`, `func_001D7658`, `func_001DECA0`: COP2 / VU0-style words rabbitizer does not decode, Unconfirmed); 3 alternate entry points (`func_001D7700`, `func_001D77A8`, `func_001D7850`) |
| INCLUDE_ASM lines in `src/` (all files) | 3,478 (3,475 functions + the 3 data blocks) |
| Functions splat wrote as C | 127 empty `void f(void) {}` (`jr $ra; nop`); they compile byte-identical |
| `.word` lines inside `.text` asm | 490 (6 marked "invalid instruction") |
| Functions spimdisasm marks "handwritten" | 186 |
| `.text` | `0xF0354` = 984,916 bytes (function bodies `0xEE7C4` = 976,836; the rest is alignment padding) |
| `.data` / `.vudata` / `.rodata` / `.lit4` / `.sdata` | `0x1623F0` / `0x1630` / `0x4D18` / `0x2C04` / `0xC10` bytes |
| `.sbss` / `.bss` (NOBITS) | `0xEC` / `0x181F520` bytes |
| Data symbols (`dlabel`) | 5,112 |
| Symbols reached through `$gp` | 3,371 |
| `make regress` | about 30 s (split 15 s, clean build 11 s, checks under 1 s) |
| `make regress` after the ER-013 file split | split 22 s, clean build 35 s (18 C files; one measurement, Wed 18 Nov 2026) |
| `make regress` after the ER-022 game split | split 22 s, clean build 34 s (25 C files; one measurement, Wed 18 Nov 2026) |
| `make regress` after the ER-024 data split | split 18 s, clean build 33 s (one measurement, Wed 18 Nov 2026) |

## 5. Backups (ER-025, Wed 18 Nov 2026)

`python tools/backup.py` writes `E:\Claude\Project Ringsmith\backups\ringsmith_<YYYYMMDD>_<HHMM>_<tag>.zip` (tag `main`
for the main checkout, else the worktree folder name) and rewrites `repo.bundle` (all refs; `git clone repo.bundle`
restores every commit) in the same folder. The zip holds our own work only: `src/`, `include/`, `tools/`, `docs/`,
`Programs/`, the yaml, `symbol_addrs.txt`, `reloc_addrs.txt`, `Makefile`, `requirements.txt`, `CLAUDE.md`,
`.gitignore`, `.gitattributes`. The file list comes from git (tracked plus untracked-not-ignored), and the run refuses
(exit 2, nothing written) if any selected path is git-ignored or is game data or build output (`disc/`, `asm/`,
`assets/`, `baserom/`, `linker/`, `build/`, `build_logs/`, `.venv/`, `*.iso`, `*.elf`, `*.irx`, `*.bin`). It keeps the
last 48 zips per tag, and a lock file (`repo.bundle.lock`) stops two runs writing the bundle at once. The bundle is
written to a temporary file, verified, then moved into place. It prints one line: zip path, KB, file count, bundle
verify result.

**The port repository too (ER-110, Sun 22 Nov 2026).** From the main checkout, when the sibling folder `..\Recomp`
is a git repository, the same run also writes `ringsmith_<YYYYMMDD>_<HHMM>_recomp.zip` (every file git sees there:
tracked plus untracked-not-ignored, under the same ignore / deny checks) and `recomp.bundle` (all its refs, written and
verified the same way, own lock file), and adds them to the one output line (`| recomp ...; recomp.bundle verify okay`).
A worktree run backs up only itself, as before.

**Only the dispatcher runs it**, after every 3-5 completed cards and at block end; work sessions never do.

## 6. Worktrees (ER-015, Thu 19 Nov 2026)

Decomp waves run several `src/` writers at once, each in its own git worktree on its own branch, merged into `main`
by the dispatcher one at a time. A worktree builds on its own; tested by ER-015 in
`.claude\worktrees\intelligent-lamport-969901` (a path with spaces): **`make regress` PASS, 51 s** (whole-ELF SHA-1
and all 3,606 functions); ER-034 was active in the main checkout then (whether it was building at that moment was not checked). Nothing in the main checkout
changed.

**Setup** (PowerShell, in the new worktree's root; `$m` is the main checkout):

```
$m = "C:\Users\lukea\Claude\Projects\Project Ringsmith\Decomp"     # the main checkout since ER-110 (22 Nov 2026)
cmd.exe /c mklink /J disc  "$m\disc"
cmd.exe /c mklink /J .venv "$m\.venv"
.venv\Scripts\python.exe -I tools/regress.py      # clean split + build + checks: must end "regress: PASS"
```

**The junctions point at the main checkout's own `disc/` and `.venv/`: read-only use only. Never write through them
from a worktree** (no `pip install`, no edits, no new files in `disc/`): any change lands in the main checkout and in
every other worktree at once.

That is all: the regress run makes the worktree's own `asm/`, `linker/`, `baserom/`, `assets/`, `build/` and
`build_logs/` (the first run splits from clean). Later, `make -j4` / `make check` work as in section 1.

| Item | Shared? | Why |
|---|---|---|
| `disc/` (the original ELF) | **junction** to the main checkout | read only; `tools/regress.py` pins it at `disc/extract/SLUS_200.15` |
| `.venv/` | **junction** | read only; the pre-commit hook and the Makefile call `.venv/Scripts/python.exe`. **Never `pip install` in a worktree**: it would change the main checkout's venv |
| `C:\rs\toolchain` | shared by path | the Makefile's `TOOLCHAIN`; nothing writes it |
| hooks | shared by git config | `core.hooksPath` is `tools/hooks`, relative (checked by ER-110: a relative hooks path resolves in each worktree's own root, so it survived the move to `Decomp` unchanged), and the hook runs `tools/regress.py --staged` of the worktree it commits in (ER-015's commit `829df8e`: the gate ran in the worktree, PASS 54 s) |
| `asm/`, `linker/`, `baserom/`, `assets/`, `build/`, `build_logs/` | **never shared** | each worktree writes its own; `tools/regress.py` deletes and rewrites them on every run, so a junction here would wipe another checkout's build. A stale `asm/` from another branch can also hold files this branch's yaml does not make |

**Findings**

- **The space in the path does not matter.** The Makefile and tools use paths relative to the checkout root, and the
  toolchain sits at `C:\rs\toolchain` (no space; the 1999 configure scripts are the only part that breaks on spaces,
  `docs/toolchain.md` 4). Both the main checkout and this worktree build with a space in their path. One exception
  to watch: `make BASEELF=...` with an absolute path that has a space breaks make's prerequisite list; the junction
  avoids that.
- **Line endings:** this machine's Git has `core.autocrlf=true` (system gitconfig), but `.gitattributes`
  (`* text=auto eol=lf`) forces LF, so a fresh worktree checks out LF files and gcc is happy (Twinblade's worktrees
  broke on this; Ringsmith's do not).
- **Removing a worktree is safe with the junctions in place** (tested on scratch folders, Thu 19 Nov 2026):
  `git worktree remove --force` deletes the folder's contents but leaves the junction itself (and the target intact);
  Python's `shutil.rmtree` and PowerShell 5.1 `Remove-Item -Recurse -Force` both remove the junction without touching
  the target. Still, unlink the junctions first: `cmd.exe /c rmdir disc` and `cmd.exe /c rmdir .venv` (rmdir on a
  junction removes only the link), then `git worktree remove`.
- **Timings (one measurement, ER-015):** clean split 33 s, clean build 18 s more, checks under 1 s: 51 s in all (another
  session may have been loading the machine). Main-checkout runs were 51 s for ER-024's layout
  (section 4).
- **Tools in a worktree:** `tools/cmpfunc.py`, `tools/place.py`, `tools/clones.py` and `tools/census.py` work there
  unchanged (they find the root from their own path); they need the worktree's own `build/SLUS_200.15.syms.elf`, which
  the first regress run leaves.

**Rules for waves** (from Twinblade's two-stream trial, `Project Twinblade/docs/decomp_pilot.md` part 3, and this test):

1. One worktree and one branch per writer; each writer owns whole `src/` files (no two writers in one file).
2. Shared tools (`tools/split.py`, `gen_ld.py`, the Makefile, the yaml) have one owner at a time; a change there goes
   to `main` first and the others merge `main` before their next batch.
3. Recipes, tracker and handoff lines conflict when two branches append to the same section: keep them in the shared
   notes folder (or one short block per card) and fold them in at merge time.
4. Merge often; a fast-forward merge runs no hook, so run `make regress` on `main` after each merge.
