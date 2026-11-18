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
   C:\msys64\usr\bin\bash.exe -lc "'/c/Users/<you>/Claude/Projects/Project Ringsmith/tools/toolchain/build_ee_gcc.sh'"
   C:\msys64\usr\bin\bash.exe -lc "'/c/Users/<you>/Claude/Projects/Project Ringsmith/tools/toolchain/build_ee_binutils.sh'"
   ```
   (or copy the two scripts to `C:\rs\toolchain\` and run them there, as ER-002 / ER-011 did). Run
   `build_ee_gcc.sh` first: it patches `config.sub`, which the binutils script checks for. Results:
   `C:\rs\toolchain\ee-gcc-2.9-991111\` (`gcc.exe`, `cpp.exe`, `cc1.exe`) and
   `C:\rs\toolchain\ee-binutils-2.9-991111\` (`ee-as.exe`, `ee-ld.exe`, ...). Details and pitfalls:
   `docs/toolchain.md` 4.1 / 4.2. Another toolchain folder: `make TOOLCHAIN=D:/path ...`.
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
| `slus_200.15.yaml` | one segment `main` (vram `0x00100000`, rom = PT_LOAD bytes): crt0 kept as `asm`, the rest of `.text` as 25 `c` files of INCLUDE_ASM (census regions, the game region cut further at 7 seams by ER-022; 3.1), `.data`, `.vudata` (kept as data words), `.rodata`, `.lit4`, `.sdata`, `.sbss`, `.bss`; `pad` subsegments for the alignment gaps between sections |
| `tools/split.py` | runs splat, writing denormal floats as `.word` (ee-as 2.9 misreads them; `docs/decomp_recipes.md` 1.3) |
| `include/macro.inc`, `labels.inc`, `include_asm.h`, `common.h` | splat's macros rewritten for ee-as 2.9 syntax; `macro.inc` also includes the generated `gp_externs.inc` |
| `tools/gen_gp_externs.py` | `build/include/gp_externs.inc`: `.extern sym, 4` for every symbol reached through `$gp`, so ee-as's `lw $2, sym` macros come out gp-relative (ee-as has no `%gp_rel`) |
| `tools/gen_elf_extras.py` | the sections after the load image that the whole-file SHA-1 needs: `.DVP.ovlytab`, `.DVP.ovlystrtab`, four `.DVP.overlay.*` (bytes from the original; `ee-dvp-as` is not built) |
| `src/elf_layout.s` | hand-written, no game bytes: the empty `.vutext` / `.vubss` and empty input sections that carry the original section alignments (`.text` 64, `.data` / `.bss` 128, others 16) |
| `tools/gen_ld.py` | rewrites splat's one-section script into SCE's layout: `.text .vutext .reginfo .data .vudata .rodata .lit4 .sdata .sbss .bss .vubss .DVP.ovlytab .DVP.ovlystrtab`, each 128-aligned except the VU ones, `_gp = ALIGN(128) + 0x7FF0` before `.lit4`, one RWX `PT_LOAD` |
| link order | crt0's object first and all objects before `-T`: ld keeps only the first `.reginfo` (link-once), which must be crt0's |
| `tools/func_check.py` | every function symbol of the unstripped link compared with the original bytes, and every `func_` / `D_` name checked against its address |

### 3.1 Source file layout (ER-013, ER-022, Wed 18 Nov 2026)

`.text` after crt0 is cut into one `c` subsegment per region of `docs/census.md` 4.1, seams on function starts
(census 4.3 has each seam's evidence and grade); ER-022 cut the game region further at the 7 Strong seams of
`docs/game_seams.md` whose data cuts line up (section 8 there). Each file's INCLUDE_ASM points at its own folder
`asm/nonmatchings/<file>/`. Data is not split: `.data` / `.rodata` / `.lit4` / `.sdata` / `.sbss` / `.bss` stay
one subsegment each (why ER-022 left it so: `docs/game_seams.md` 8.4).

| File | Starts | Units (C) | Region |
|---|---|---|---|
| `src/text_001000C0.c` | 0x001000C0 | 576 (22) | game: main, boot and everything up to the first Strong seam |
| `src/text_001343E8.c` | 0x001343E8 | 338 (8) | game, seam S1 (`docs/game_seams.md` 8.2) |
| `src/text_00145120.c` | 0x00145120 | 105 (2) | game, seam S9 |
| `src/text_0014A968.c` | 0x0014A968 | 52 (2) | game, seam S12 |
| `src/text_0014C840.c` | 0x0014C840 | 30 (2) | game, seam S15 |
| `src/text_0014D930.c` | 0x0014D930 | 23 (6) | game, seam S16 |
| `src/text_0014E2E8.c` | 0x0014E2E8 | 59 (0) | game, seam S17 |
| `src/text_00150878.c` | 0x00150878 | 1,542 (83) | game, seam S20, up to the VU0 helpers |
| `src/text_001C9B18.c` | 0x001C9B18 | 28 | game VU0 helpers (Unconfirmed) |
| `src/text_001CA0F0.c` | 0x001CA0F0 | 77 | game-unk (Unconfirmed) |
| `src/sdk/libgraph.c`, `libkernl.c`, `libdma.c`, `vudev.c`, `libpad.c`, `libvu0.c`, `libcdvd.c`, `libmpeg.c`, `libipu.c`, `libsdr.c`, `libmc.c`, `libm.c`, `libgcc.c`, `libc.c` (2 C), `libkernl_tail.c` | 0x001CE130 on | 775 | SDK libraries in link order |

A new game file is cut by adding a `c` line to the yaml and moving its INCLUDE_ASM lines (with the folder renamed)
into a new `src/` file; splat never overwrites an existing `.c`. `make regress` re-splits from clean, so stale
`asm/nonmatchings` folders do no harm.

Generated, gitignored (game data or derived from it): `asm/`, `linker/`, `baserom/`, `assets/`, `build/`,
`build_logs/`. Committed: the yaml, `symbol_addrs.txt`, `reloc_addrs.txt`, `src/`, `include/`, `Makefile`, the
tools.

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

## 5. Backups (ER-025, Wed 18 Nov 2026)

`python tools/backup.py` writes `F:\Claude\Project Ringsmith\backups\ringsmith_<YYYYMMDD>_<HHMM>_<tag>.zip` (tag `main`
for the main checkout, else the worktree folder name) and rewrites `repo.bundle` (all refs; `git clone repo.bundle`
restores every commit) in the same folder. The zip holds our own work only: `src/`, `include/`, `tools/`, `docs/`,
`Programs/`, the yaml, `symbol_addrs.txt`, `reloc_addrs.txt`, `Makefile`, `requirements.txt`, `CLAUDE.md`,
`.gitignore`, `.gitattributes`. The file list comes from git (tracked plus untracked-not-ignored), and the run refuses
(exit 2, nothing written) if any selected path is git-ignored or is game data or build output (`disc/`, `asm/`,
`assets/`, `baserom/`, `linker/`, `build/`, `build_logs/`, `.venv/`, `*.iso`, `*.elf`, `*.irx`, `*.bin`). It keeps the
last 48 zips per tag, and a lock file (`repo.bundle.lock`) stops two runs writing the bundle at once. The bundle is
written to a temporary file, verified, then moved into place. It prints one line: zip path, KB, file count, bundle
verify result.

**Only the dispatcher runs it**, after every 3-5 completed cards and at block end; work sessions never do.
