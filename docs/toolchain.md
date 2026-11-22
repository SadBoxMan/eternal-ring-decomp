# Toolchain (ER-002)

Written Tue 17 Nov 2026 by ER-002 (Opus 5.5, high); binutils and the first byte check (sections 4.2, 4.3,
appendix B) added the same day by ER-011 (Opus 5.5, medium). Source: the US main EE ELF `disc/extract/SLUS_200.15`
(SHA-1 6087A8CADB7538A849A4F5760502A39DAA6A12AF), read only; the IOP modules in `disc/extract/IOP/`; the GPL ee-gcc
source tree (SSXModding/ps2-ee-toolchain, commit b595ded). No game bytes, disassembly listings or strings dumps are
kept in the repo; the working files are in the session scratchpad only.

Grades: **Confirmed** = read from the binary or the source; **Unconfirmed** = inferred. **Unofficial** = a label we
invented.

## 1. Verdict

| Claim | Grade |
|---|---|
| Compiled with **GCC** (Cygnus r5900 / "ee" port), assembled with **GNU as** and linked with **GNU ld** from SCE's EE toolchain (with DVP / VU support) | **Confirmed** (section 2.4 fingerprints, the DVP section types, the SDK linker-script layout) |
| Not Metrowerks CodeWarrior, not SN ProDG | **Unconfirmed**, but nothing points to either: no `.mwcats` / `.mwo`, no SN strings or SN runtime, and every fingerprint is GNU as / GCC |
| Compiler build: **ee-gcc 2.9-ee-991111** (or a sibling 2.9-ee build, such as 990721) | **Unconfirmed**. Built here from the GPL source, it gives instruction-for-instruction the same code as the ELF for the three test functions (section 3.2); that fits, but other 2.9-ee builds may well give the same code for functions this small |
| ABI: **EABI, 64-bit registers** (`-mips3`, `E_MIPS_ABI_EABI64`), args in `$a0`-`$a3`, `$t0`-`$t3`, floats in `$f12`-`$f19`, single-float | **Confirmed** (e_flags; crt0 passes a 5th argument in `$t0`; float args in `$f12` / `$f13` in game code) |
| SDK: **SCE PS2 runtime library release 1.6.x** (EE libraries 1620 to 1650, IOP modules 1600 to 1660); libc is **newlib** | Tags and strings **Confirmed**; mapping the tags to a release number **Unconfirmed** |
| A Windows-native build of that compiler **runs**: `C:\rs\toolchain\ee-gcc-2.9-991111\` (MSYS2 host build, from PowerShell or MSYS2) | **Confirmed** (section 4) |
| Windows-native **EE binutils 2.9-ee-991111** (`ee-as`, `ee-ld`, `ee-objdump`, ...) from the same tree **run**: `C:\rs\toolchain\ee-binutils-2.9-991111\` | **Confirmed** (ER-011, section 4.2) |
| Compiler + assembler output is **byte-identical** to the ELF for the three test functions (33 words, relocations aside), and also after linking with `ee-ld` | **Confirmed** (ER-011, section 4.3) |
| Flags: `-O2` (or higher) with small data on (`-G` > 0, `-G 8` is the default) | **Unconfirmed** (3 functions; section 5) |

## 2. The ELF

### 2.1 Header (Confirmed)

| Field | Value |
|---|---|
| Class / data | ELF32, little-endian |
| Type / machine | EXEC (2), MIPS (8) |
| Entry | `0x00100008` (crt0 `_start`; two `nop`s at `0x00100000`) |
| e_flags | `0x20924001` = `EF_MIPS_ARCH_3` (0x20000000) + `E_MIPS_MACH_5900` (0x00920000) + `E_MIPS_ABI_EABI64` (0x00004000) + `EF_MIPS_NOREORDER` (0x1), decoded with the binutils `elf/mips.h` constants |
| Program headers | 1: `PT_LOAD` file offset `0x1000`, vaddr `0x00100000`, filesz `0x25C410`, memsz `0x1A7BAA0`, flags RWX, align `0x1000` |
| Sections | 19, section header table at `0x25EBAC` |
| Symbols / debug | **none**: no `.symtab`, `.strtab`, `.comment`, `.mdebug`, `.debug_*`, `.mwcats` or `.note`. The ELF is stripped |

### 2.2 Sections (Confirmed)

| # | Name | Type | Address | File offset | Size | Align |
|---|---|---|---|---|---|---|
| 1 | `.text` | PROGBITS, AX | `0x00100000` | `0x001000` | `0x0F0354` | 64 |
| 2 | `.vutext` | PROGBITS, AX | `0x001F0354` | `0x0F1354` | 0 | 1 |
| 3 | `.reginfo` | MIPS_REGINFO (not allocated) | `0x001F0354` | `0x25D494` | `0x18` | 4 |
| 4 | `.data` | PROGBITS, WA | `0x001F0380` | `0x0F1380` | `0x1623F0` | 128 |
| 5 | `.vudata` | PROGBITS, WA | `0x00352770` | `0x253770` | `0x1630` | 16 |
| 6 | `.rodata` | PROGBITS, A | `0x00353E00` | `0x254E00` | `0x4D18` | 16 |
| 7 | `.lit4` | PROGBITS, WA + GPREL | `0x00358B80` | `0x259B80` | `0x2C04` | 4 |
| 8 | `.sdata` | PROGBITS, WA + GPREL | `0x0035B800` | `0x25C800` | `0xC10` | 16 |
| 9 | `.sbss` | NOBITS, WA + GPREL | `0x0035C480` | | `0xEC` | 16 |
| 10 | `.bss` | NOBITS, WA | `0x0035C580` | | `0x181F520` | 128 |
| 11 | `.vubss` | NOBITS, WA | `0x01B7BAA0` | | 0 | 1 |
| 12 | `.DVP.ovlytab` | `0x7FFFF420` (SHT_DVP_OVERLAY_TABLE) | `0x01B7BAA0` | `0x25D4AC` | `0x30` (4 entries of 12) | 4 |
| 13 | `.DVP.ovlystrtab` | STRTAB | | `0x25D4DC` | `0x82` | 1 |
| 14-17 | `.DVP.overlay..0x2000.625463.19.0`, `..0x1000.625463.38.1`, `..0x0.625463.59.2`, `..0x3000.625463.83.3` | `0x7FFFF421` (SHT_DVP_OVERLAY) | 0 | `0x25D55E` on | `0x6E0`, `0x538`, `0x4A0`, `0x490` | 1 |
| 18 | `.shstrtab` | STRTAB | | `0x25EAA6` | `0x105` | 1 |

No `.lit8`, `.rdata`, `.gcc_except_table`, `.ctors` / `.dtors` or `.spad` section.

### 2.3 Linker script and crt0 (Confirmed, except where marked)

- **Section order and alignment are those of SCE's standard EE linker script** (`app.cmd`): `.text` at
  `0x00100000`, then `.reginfo`, `.data`, `.rodata`, `.lit4`, `.sdata`, `.sbss`, `.bss`, each 128-byte aligned (all
  addresses above are multiples of `0x80`). The VU sections (`.vutext`, `.vudata`, `.vubss`) sit where the SDK
  script puts them. That this is the SDK's own `app.cmd` file is **Unconfirmed** (the file itself is not in hand).
- **gp = `0x00360B70`** (`.reginfo` `ri_gp_value`, and crt0 loads the same value into `$gp`). It equals `.lit4`
  start `0x00358B80` + `0x7FF0`, i.e. the script's `_gp = ALIGN(128) + 0x7ff0` placed just before `.lit8` / `.lit4`.
- **crt0 is SCE's** (shape read from the code at `0x00100008`, Unconfirmed that it is byte-identical to a given SDK
  `crt0.s`): clears `0x0035C480` (`.sbss` start, `_fbss`) to `0x01B7BAA0` (`_end`) with `sq $zero`; syscall `0x3C`
  (SetupThread) with gp, stack `-1` (top of RAM), stack size `0x00100000`, args `0x0035C580`, root `0x001000B8`;
  `$sp` from the result; syscall `0x3D` (SetupHeap) with `_end`, heap size `-1`; calls `func_001D03E0` (library code,
  probably `_InitSys`: Unconfirmed); `ei`; `main` = **`func_001000C0`** with `argc`, `argv`; then `func_001CFD80`
  (probably `Exit`: Unconfirmed). `0x001000B8` is the `ExitThread` (syscall `0x23`) stub.
- `.reginfo` `ri_gprmask` = `0xB00001FE` (`$at`, `$v0`-`$t0`, `$gp`, `$sp`, `$ra`), exactly the registers crt0
  uses, and `ri_cprmask` all 0. The compiled objects added nothing to the mask. Why is **not known** (perhaps the
  compiled objects carry no `.reginfo`); it does not affect the split.
- **VU microcode:** the four `.DVP.overlay` sections are VU microprograms assembled by `ee-dvp-as` (DVP is the
  binutils name for the VU / VIF units). `.DVP.ovlytab` gives their load copies in `.vudata` (`0x003527A0`,
  `0x00352EB0`, `0x00353420`, `0x00353900`) and VU addresses `0x2000`, `0x1000`, `0x0`, `0x3000`. `.vutext` is empty.
  The overlay sections are not loaded (address 0); they repeat the `.vudata` copies for the debugger (Unconfirmed).
- **No other code modules:** the ELF has one `PT_LOAD`; `ERDATA.BIN` holds no ELF header and not a single
  word-aligned `jr $ra`, so it holds no uncompressed EE code (compressed code cannot be ruled out). The IOP side is
  nine IRX modules plus `IOPRP165.IMG` (see 2.5).

### 2.4 Compiler fingerprints in the code (Confirmed counts; readings marked)

Counts over all 246,997 words of `.text`, disassembled with rabbitizer 1.16.2 in R5900 mode (Twinblade's venv).

| Trait | Count | What it says |
|---|---:|---|
| Register saves `sd $sN/$fp/$ra, k($sp)` with a **16-byte slot stride**, frames multiples of 16 | 2,033 of the 2,099 prologues with 2+ saves (50 more use `sd` but save one slot twice on different paths; 16 mix `sd` and `sw`); 2,465 `addiu $sp, $sp, -N` in all | Cygnus r5900 GCC port: `STACK_BOUNDARY` 128 and 16-byte slots for 128-bit registers, 64-bit `sd` stores (matches `elf5900.h`) |
| `move` emitted as `daddu rd, rs, $zero` | 22,221 | GCC MIPS 64-bit `move` |
| `lui $at, hi; mtc1 $at, $fN` (float constants with zero low half) | 3,868 | GNU as `li.s` macro expansion |
| `lwc1 $fN, k($gp)` into `.lit4` (other float constants) | 3,384 | GNU as `li.s` with `-G` > 0 puts literals in `.lit4`; `.lit4` section present (0x2C04 bytes = 2,817 floats) |
| `lui $at, ...; ori $at, ...; mtc1` | 112 | the same macro without small data (code built with `-G 0`, e.g. SDK libraries: Unconfirmed) |
| Stores and FP loads through `$at` (`lui $at` + `sw`/`lwc1`/`ld`/...) | about 330 | GNU as expanding symbolic `sw $r, sym` (GCC 2.x emits no explicit `%hi` / `%lo`) |
| gp-relative accesses | 5,735 (lwc1 3,384, lw 1,969, sw 277, ...) | small data on (`-G 8`, the ee-gcc default `MIPS_DEFAULT_GVALUE`, is consistent; the exact value is Unconfirmed) |
| Branch-likely (`beql`, `bnel`, `bc1fl`, `bc1tl`, `bgezl`, ...) | 2,842 | GCC delay-slot filling with MIPS III branch-likely |
| 3-operand `mult rd, rs, rt` (R5900) | 521 of 530 `mult` | R5900-aware compiler (`mulsi3` pattern of the r5900 port) |
| Pipeline-1 ops (`mult1`, `div1`, `mflo1`) | 36 | rare; likely SDK or hand-written code (Unconfirmed) |
| MMI (`pcpyld`, `qfsrv`, `pnor`, `por`, `pextlw`, `paddh`, `psrlh`, ...) and `lq`/`sq` | 330 MMI `p*` ops, 408 `lq` / `sq` | mostly SDK (libvu0 / libmpeg / libipu) or inline asm (Unconfirmed) |
| VU0 macro (`vmulax`, `vmadday`, `lqc2`, `sqc2`, `vnop`, ...) | 2,222 | COP2 macro mode, as in the `sceVu0*` library and inline asm |
| Function entry alignment | 1,712 of 1,717 `jal` targets are 8-byte aligned; 897 have 1 zero word of padding before them | ee-gcc r5900 `DEFAULT_FUNCTION_ALIGN 3` (8 bytes) |
| `.word` (undecodable) inside `.text` | 430 | data or unusual opcodes in `.text`; ER-003 to look at |

Game code and SDK library code (library code appears from about `0x001C8000` up; the first `libgraph` string
reference is at `0x001CE898`; the exact boundary is ER-004's) have the **same prologue shape**, slot stride and
`move` idiom.

### 2.5 SDK and libraries (Confirmed strings; release mapping Unconfirmed)

SCE libraries embed a `PsII<lib><version>` tag. EE side, in `.data` near its end:

| Library | Tag |
|---|---|
| libgraph | `PsIIlibgraph1620` |
| libkernl | `PsIIlibkernl1620` |
| libdma | `PsIIlibdma  1620` |
| libpad | `PsIIlibpad  1630` |
| libcdvd | `PsIIlibcdvd 1650` |
| libmpeg | `PsIIlibmpeg 1620` |
| libipu | `PsIIlibipu  1620` |
| libsdr | `PsIIlibsdr  1620` |
| libmc | `PsIIlibmc   1650` |

Also linked (strings, no tag): libvu0-style VU0 macro code, SIF / TTY (`TTY: receive error`), and the **newlib**
C library (`bug in vfprintf: bad base` is newlib's `vfprintf.c` message; `asinf` from newlib libm). libvu0 has no
tag string, so its presence is a reading of the code (Unconfirmed).

IOP side (`.iopmod` name and version, plus the `PsII` tag):

| Module | `.iopmod` name | Version | Tag |
|---|---|---|---|
| `LIBSD.IRX` | Sound_Device_Library | 1.04 | `PsIIlibsd   1600` |
| `MCMAN.IRX` | mcman_tool | 2.0b | `PsIImcman   1650` |
| `MCSERV.IRX` | mcserv | 2.08 | `PsIImcserv  1650` |
| `MODHSYN.IRX` | SPU2_Synthesizer_Module | 1.04 | `PsIImodhsyn 1660` |
| `MODMIDI.IRX` | Midi_Sequencer_Module | 1.04 | `PsIImodmidi 1660` |
| `PADMAN.IRX` | padman | 2.06 | `PsIIpadman  1630` |
| `SDRDRV.IRX` | sdr_driver | 1.04 | `PsIIsdrdrv  1600` (and `sdr driver version 1.4.0 (C)SCEI`) |
| `SIO2MAN.IRX` | sio2man | 2.01 | `PsIIsio2man 1600` |
| `FSIOPSND.IRX` | fslibsnd_driver | 1.05 | none (a non-SCE module; "FS" = FromSoftware is Unconfirmed) |

**Reading (Unconfirmed):** the four digits are the SCE runtime library release, so `1620` = release 1.6.2. The
libraries span 1600 to 1660, with the EE libraries at 1620 to 1650; together with `IOPRP165.IMG` this points to
**SCE PS2 runtime library release 1.6.x** (a 1.6.5 / 1.6.6 era IOP set over 1.6.2 EE libraries).

## 3. Compiler

### 3.1 Family

All the evidence in section 2 points to GCC with the GNU binutils from SCE's EE toolchain:

- **GNU as macro expansion everywhere**: `li.s` float constants as `lui $at` + `mtc1` (3,868) or gp-relative loads
  from a `.lit4` section (3,384); stores through `$at`; `move` as `daddu`. GCC 2.x leaves exactly these to the
  assembler (Confirmed: the built `cc1` emits `lw $2,sym` and `s.s` macros, section 3.2). That a compiler with its
  own code emitter, such as CodeWarrior, would not produce a `.lit4` section is a reading (Unconfirmed).
- **GNU ld with Cygnus DVP support**: section types `0x7FFFF420` / `0x7FFFF421` (`SHT_DVP_OVERLAY_TABLE` /
  `SHT_DVP_OVERLAY`) and `.DVP.overlay..<vu addr>...` section names come from the Cygnus "sky" / dvp binutils (the same GPL
  tree has the `ee` = `mips64r5900-sky-elf` and `ee-dvp` targets in its `config.sub`: Confirmed). That SCE shipped
  exactly these as `ee-as` / `ee-dvp-as` / `ee-ld` is Unconfirmed.
- **The r5900 GCC port's own defaults** (read in the source, `gcc/config/mips/elf5900.h`): EABI by default,
  `DEFAULT_FUNCTION_ALIGN 3` (8-byte function entries: 1,712 of 1,717 call targets), `STACK_BOUNDARY` 128 (frames
  of 16, 32, ... bytes), 16-byte save slots, ISA 3. Game code and SDK code share the same shape.
- **Era**: the SDK libraries are release 1.6.x. That 2.9-ee-99xxxx was SCE's EE compiler of the 1.6 SDK era (before
  the 2.95-based releases) is Unconfirmed: no dated source was read for it.

### 3.2 Build check (sanity check only; matching proper starts in ER-003)

`C:\rs\toolchain\tests\er002_shapes.c` (our code, outside the repo) holds C guesses at three small game functions:
`func_0012F7E0` (argument-shuffling wrapper), `func_001257E8` (two gp-relative globals, a call, a store back) and
`func_0012D3B8` (two float arguments packed into a stack vector). Names and types in the test are Unofficial.

Compiled with the Windows build (`cc1 -O2 -G8`), **all three come out as the same instruction sequence as the
ELF**: the same instructions in the same order, the same registers, the same delay-slot fill (`move $5,$0` and
`sw $0,12($sp)` in the `jal` slot, `addu $sp` in the `j $31` slot), the same frame sizes and save offsets, and the
same gp-relative loads. The remaining step is mechanical: as macros, `move` = `daddu`, `subu $sp,N` = `addiu $sp,-N`,
`lw sym` with a 4-byte `.extern` under `-G 8` = `lw ..($gp)`. ER-002 could not assemble the `.s` (no `ee-as`
yet); **ER-011 assembled it and the bytes match** (section 4.3).

Variations: `-O1` puts all three in a different order (no instruction scheduling pass), and `-G 0` changes the
second (`%hi` / `%lo` pairs plus a saved `$s0`), so both are ruled out for this code. `-O3` gives the same as `-O2`
here (nothing to inline).

The `gcc.exe` driver (`gcc -B<dir>\ -O2 -G8 -S`) gives the same output as `cpp` + `cc1` by hand.

## 4. Windows-native toolchain

### 4.1 Compiler (ER-002)

**Working: ee-gcc 2.9-ee-991111 built from the GPL source as an MSYS2 program** (host `x86_64-pc-msys`, LP64),
the same approach Twinblade used for GCC 2.7.2 (`Project Twinblade/tools/build_old_gcc.sh`). No WSL, Docker, Wine,
hypervisor or prebuilt binary was used; only MSYS2's own GCC 15.3, bison, make and sed, already on the machine.

| Item | Value |
|---|---|
| Source | https://github.com/SSXModding/ps2-ee-toolchain, commit `b595ded606227e93b8c4a447446c1d2ac093827d` (14 Apr 2022, "Init (EE-GCC 2.9-ee-991111, IOP GCC 2.8.1)"); `gcc/version.c` says `2.9-ee-991111`; `gcc/README` says egcs 1.1. GPL (COPYING files in the tree) |
| Clone | `C:\rs\toolchain\src-lf`, **with `-c core.autocrlf=false`**. This machine has `core.autocrlf=true` globally; a CRLF checkout breaks bison 1.25 on `cexp.y` silently (exit 0, no output) and may affect the `.md` machine description |
| Build script | `C:\rs\toolchain\build_ee_gcc.sh` (appendix A), run with `MSYSTEM=MSYS C:\msys64\usr\bin\bash.exe -lc /c/rs/toolchain/build_ee_gcc.sh`; about 2 minutes at `-j4` |
| Output | `C:\rs\toolchain\ee-gcc-2.9-991111\` = `gcc.exe` (driver), `cpp.exe`, `cc1.exe`, `msys-2.0.dll` (copied so it runs outside MSYS2). `cc1 -version` prints `GNU C version 2.9-ee-991111 (ee) compiled by GNU C version 15.3.0.` `cc1.exe` SHA-1 `6575FD31384320FDFA55822BD97AC8492BF51162` (local build, not reproducible bit for bit) |
| Runs from | PowerShell directly, and MSYS2 bash |

What the build needed (all in the script):

1. `config.sub`: accept `x86_64-*` and the `-msys*` OS.
2. `gcc/configure`: an `x86_64-*-msys*` host entry with a new `gcc/config/xm-msys64.h` (LP64:
   `HOST_WIDE_INT` = `long`, 64 bits, which the r5900 target requires). The entry must go in the **main** machine
   case (the one starting `# Support site-specific machine types`), not the first `case $machine`, which only sets
   `cpu_type`.
3. Generated parsers: the tree ships no `c-parse.c` / `cexp.c`. MSYS2's bison 3.8 rejects the grammar (untyped
   mid-rule `$$`, tokens as an `enum` that clashes with the RTL code `RETURN`, no `YYLEX`). The tree's own **bison
   1.25** builds under MSYS2 and generates both parsers unchanged, so the grammar needs no edits.
4. `libiberty` built as a sibling of the gcc build directory (gcc links `../libiberty/libiberty.a`).
5. Host flags relax GCC 15's errors on 1990s C only (`-std=gnu89 -fcommon -Wno-implicit-int ...`, `-O0`).

**Patched build in use (ER-034, Sun 22 Nov 2026; ruling `docs/decomp_recipes.md` 3.7).** The build uses
`ee-gcc-2.9-991111-r1`: the same tree plus our machine-description patch.

| Item | Value |
|---|---|
| Patch | `tools/toolchain/patches/ee-gcc_r5900_fdiv_nops.patch` (`patch -p1` against `src-lf/ee`). In `gcc/config/mips/mips.md`, `divsf3` and `sqrtsf2` output `nop\n\tnop\n\tdiv.s` / `nop\n\tnop\n\tsqrt.s` with `length` 3 (was 1). `define_delay` takes only length-1 insns, so a divide or square root never goes in a delay slot, and it always has two `nop`s before it, as on all 832 sites in the game (754 `div.s` + 59 `sqrt.s` game, 19 `div.s` SDK; census in `decomp_recipes.md` 2.9). The tree already has an r5900 workaround of the same kind (`mips_r5900_lengthen_loops`, short loops). Why the chip needs it is not known (Unconfirmed: an FPU divide erratum). No `-m` option of this ee-gcc does it (`TARGET_SWITCHES` read) |
| Script | `tools/toolchain/build_ee_gcc.sh` now applies every `patches/ee-gcc_*.patch` to `C:\rs\toolchain\src-lf\ee` once (reverse dry run), checks the `Ringsmith ER-034` marker in `mips.md`, and builds into `b-gcc-r1` / `ee-gcc-2.9-991111-r1`, log `build_ee_gcc-r1.log` (about 2 minutes). Run: `$env:MSYSTEM = "MSYS"; C:\msys64\usr\bin\bash.exe -lc "'<repo>/tools/toolchain/build_ee_gcc.sh'"` |
| Output | SHA-1 `cc1.exe` `460DD93D37759A805EAC17A92BCF4731C544E78F`, `gcc.exe` `57E2CDB082982D2E124B6FEC1BF0ACE14A224504` (local builds) |
| Where the path lives | `Makefile`: `GCC_DIR := $(TOOLCHAIN)/ee-gcc-2.9-991111-r1` |
| Fallback | ER-002's unpatched build stays in `C:\rs\toolchain\ee-gcc-2.9-991111\` (`b-gcc`); `src-lf` now carries the patch (`patch -R -p1 -d /c/rs/toolchain/src-lf/ee < <patch>` takes it out) |
| Proof | `make regress` green with r1 (and ee-as r3); before any C function with a divide was placed, the whole ELF was already byte-identical with it (no C function had a divide). Control: with `func_001C12D0` placed, the stock compiler gives SHA-1 `65a10c55...` (FAIL), r1 gives `6087a8ca...` (OK) |

### 4.2 Binutils (ER-011, Tue 17 Nov 2026)

**Working: EE binutils 2.9-ee-991111 built from the same GPL tree as an MSYS2 program**, first try, same method as
the compiler. No prebuilt binary was downloaded (the decompals binutils v0.10 Windows build was not needed).

| Item | Value |
|---|---|
| Source | the same clone, `C:\rs\toolchain\src-lf` (commit `b595ded`, LF checkout, ER-002's `config.sub` patch already in it); `gas/configure.in` says `2.9-ee-991111` |
| Build script | `C:\rs\toolchain\build_ee_binutils.sh` (appendix B): top-level Cygnus `configure --target=ee --host=x86_64-pc-msys`, then `make -j4 all-gas all-ld all-binutils` with the same relaxed host `CFLAGS` as the compiler. No source edits beyond ER-002's `config.sub`. Run with `MSYSTEM=MSYS C:\msys64\usr\bin\bash.exe -lc /c/rs/toolchain/build_ee_binutils.sh`; a few minutes at `-j4`. Build dir `C:\rs\toolchain\b-binutils`, log `build_ee_binutils.log` |
| Output | `C:\rs\toolchain\ee-binutils-2.9-991111\` = `ee-as.exe`, `ee-ld.exe`, `ee-objdump.exe`, `ee-objcopy.exe`, `ee-nm.exe`, `ee-readelf.exe`, `ee-size.exe`, `ee-strings.exe`, `ee-ar.exe`, `ee-ranlib.exe`, `msys-2.0.dll`. `ee-as --version` = `GNU assembler 2.9-ee-991111`; `ee-ld --version` = `GNU ld 2.9-ee-991111`, emulation `elf32l5900`. SHA-1 `ee-as.exe` `EA33115B6FCE1B24C7D30EECDABCAD3DE357258E`, `ee-ld.exe` `2DE7435151749B8FA9E09896530EE3DC8C124B91` (local builds, not reproducible bit for bit) |
| Runs from | PowerShell directly (checked), and MSYS2 bash. **Not from Git Bash with `/c/...` paths**: Git Bash's path conversion confuses these MSYS2 programs (`gcc -B/c/...` could not find `cpp`); use Windows paths |
| Not built | the VU assembler `ee-dvp-as` (target `ee-dvp` = `dvp-elf`; needed only if the four VU microprograms are rebuilt from source rather than kept as binary) |

Notes:

- This 1999 `objcopy` has no `-j` (only `-R`); use `-R` to drop sections, or read `.text` from the ELF directly.
- `ee-as` prints `bfd assertion fail .../bfd/elf32-mips.c:2847` (`.reginfo` size) when its **input file is missing**,
  after the real error. It only happens on that failure path; an empty input or any real input assembles without it.
- The `gcc` driver calls `as` with `-G8 -O2 -mabi=eabi` (from `-O2 -G8`; specs `asm` / `gas_asm_spec`). Use the
  same flags when assembling by hand: without `-mabi=eabi` the object differs in one byte only, e_flags lacks
  `E_MIPS_ABI_EABI64` (`0x4000`; `0x20920001` instead of the game's `0x20924001`). With them, the hand-assembled
  object is byte-identical to the driver's.
  The driver finds `as` through `-B<dir>\` if the dir holds `as.exe` (a copy of `ee-as.exe`).
- **ee-as asserts on a gp-relative macro naming a labelled `.lit4` symbol** (`lwc1 $f, (D_x)` with `D_x` defined
  in `.lit4`): `gas/config/tc-mips.c`, `nopic_need_relax`. Write such loads as `li.s $f, <value>` and let gas build
  the pool (ER-024; `docs/build.md` 3.2, step 5).

**Patched build in use (ER-026, Thu 19 Nov 2026).** The build uses `ee-binutils-2.9-991111-r1`: the same tree plus
our one-line gas patch (ruling `docs/decomp_recipes.md` 3.4; why: 4.4).

| Item | Value |
|---|---|
| Patch | `tools/toolchain/patches/ee-as_r5900_fpcc_reorder.patch` (ours, in the repo; `patch -p1` against `src-lf/ee`). In `append_insn` the FP condition-code hazard test gains `&& ! (mips_cpu == 5900 && ! mips_opts.noreorder)`: no `nop` between `c.cond.s` and `bc1t` / `bc1f` in reorder mode; in `.set noreorder` the `nop` stays. Same change as ER-014's `C:\rs\toolchain\er014\er014_gas.patch`; only the comment text differs |
| Script | `tools/toolchain/build_ee_binutils.sh` applies every `patches/*.patch` to `C:\rs\toolchain\src-lf\ee` once (a reverse dry run spots an applied patch), then builds as before into the build dir `b-binutils-r1`, log `build_ee_binutils-r1.log`. Run: `$env:MSYSTEM = "MSYS"; C:\msys64\usr\bin\bash.exe -lc "'<repo>/tools/toolchain/build_ee_binutils.sh'"` (about 6 minutes at `-j4`) |
| Output | `C:\rs\toolchain\ee-binutils-2.9-991111-r1\` (the same eleven files as above). SHA-1 `ee-as.exe` `1E9C29A26D57E530F690D7936FAA28A77A7BB984`, `ee-ld.exe` `AA1C6D026F7A04898445BAFDF3986A49239AB812` (local builds, not reproducible bit for bit; `ee-ld` has no source change) |
| Where the path lives | `Makefile`: `BIN_DIR ?= $(TOOLCHAIN)/ee-binutils-2.9-991111-r1` |
| Fallback | the unpatched ER-011 build stays in `C:\rs\toolchain\ee-binutils-2.9-991111\` (`make BIN_DIR=C:/rs/toolchain/ee-binutils-2.9-991111 ...`). With ER-026's C functions in place it no longer builds the game (control run, `text_001000C0` assembled with it: ELF 12 bytes longer, SHA-1 FAILED, 3,350 functions differ). `src-lf` now carries the patch; `patch -R -p1 -d /c/rs/toolchain/src-lf/ee < <patch>` takes it out again |
| Proof | `make regress` green with the patched assembler (whole-ELF SHA-1, 3,606 of 3,606 functions), with six C functions that need it (`docs/decomp_recipes.md` 2.6); ER-014 had already shown the all-asm build unchanged by it |

**Second patch, build r2 (ER-035, Thu 19 Nov 2026; ruling `docs/decomp_recipes.md` 3.5).** The build now uses
`ee-binutils-2.9-991111-r2`: both patches in `tools/toolchain/patches/`.

| Item | Value |
|---|---|
| Patch | `ee-as_r5900_copmove_reorder.patch`: in `append_insn` the coprocessor-move hazard test (`INSN_COPROC_MOVE_DELAY`, `mtc1` and the like) gains `&& ! (mips_cpu == 5900 && ! mips_opts.noreorder)`: no `nop` between `mtc1` and the next instruction that uses its register, in reorder mode. The game has 1,454 such pairs (SDK 19) and none with a `nop`. The `mfc1` load delay (GPR use after `mfc1`) is untouched; the game has that `nop` |
| Script | `tools/toolchain/build_ee_binutils.sh` now builds into `b-binutils-r2` / `ee-binutils-2.9-991111-r2`, log `build_ee_binutils-r2.log` (slower than ER-026's 6 minutes this time; not timed); it checks both `Ringsmith ER-014/ER-026` and `Ringsmith ER-035` markers in `tc-mips.c` |
| Output | SHA-1 `ee-as.exe` `5B47B468705BCB27A60E72D880974315517C2004`, `ee-ld.exe` `CFF60F9F87B512FEF3AEBFCA62B42A1E1BAC0F74` (local builds) |
| Fallbacks | `-r1` (first patch only) and the unpatched ER-011 build stay; `src-lf` now carries both patches |
| Proof | `make regress` green with r2 (ER-035 commits `2fd12a0` on); func_0012E908 and func_0013DBC0 need it |

**Third patch, build r3 (ER-034, Sun 22 Nov 2026; ruling `docs/decomp_recipes.md` 3.8).** The build now uses
`ee-binutils-2.9-991111-r3`: all three `ee-as_*.patch` files in `tools/toolchain/patches/`.

| Item | Value |
|---|---|
| Patch | `ee-as_r5900_mfc1_reorder.patch`: in `append_insn` the load-delay test for `INSN_LOAD_COPROC_DELAY` (`mfc1`, `cfc1`, `dmfc1`) gains `&& ! (mips_cpu == 5900 && ! mips_opts.noreorder)`: no `nop` between `mfc1` and the next instruction that uses its GPR, in reorder mode. The memory load delay (`INSN_LOAD_MEMORY_DELAY`) is untouched. The game has 58 such pairs (45 game, 13 SDK), none with a `nop`; the 11 where the user is a branch (gcc's noreorder blocks; the `nop` comes from gas at `.set noreorder`) keep it. This corrects ER-035's note that the `mfc1` delay `nop` is always in the game: it is there only before branches |
| Script | `tools/toolchain/build_ee_binutils.sh` applies only `patches/ee-as_*.patch` now (the `ee-gcc_*` ones are `build_ee_gcc.sh`'s), checks the three markers, builds into `b-binutils-r3` / `ee-binutils-2.9-991111-r3`, log `build_ee_binutils-r3.log` |
| Output | SHA-1 `ee-as.exe` `7E7F70C78D3981286C64629ED17743F3BBEF0E74`, `ee-ld.exe` `4A335C2EA19656D0D945AC59E3EA167A467F287C` (local builds) |
| Fallbacks | `-r2`, `-r1` and the unpatched ER-011 build stay; `src-lf` now carries all three patches |
| Proof | `make regress` green with r3; control with every ER-034 function placed: r2 gives SHA-1 `949181d4...` (FAIL: func_001B8BC0 and its copies), r3 `6087a8ca...` (OK) |

**Fourth patch, build r4 (ER-070, Sun 22 Nov 2026; ruling `docs/decomp_recipes.md` 3.12).** The build now uses
`ee-binutils-2.9-991111-r4`: all four `ee-as_*.patch` files in `tools/toolchain/patches/`. Evidence and scripts:
notes folder `ER-070_drafts/` (dispatcher's folder; `ER-070_evidence.md`, `census_slots.py`).

| Item | Value |
|---|---|
| Cause | **This 1999 gas never filled a delay slot by swapping.** In `append_insn` the long "can not swap" test ends with `\|\| (prev_pinfo && INSN_SYNC)` (comment: "If the previous instruction is a sync ... we can not swap"): a logical `&&`, so it is true for every instruction that has any pinfo flag. Every branch gcc leaves in reorder mode (its own delay-slot filler found nothing, mips.c `%*` / `%/`) therefore got a `nop`, even after a plain `addu`. ER-063's reading (the swap test still applying the `mtc1` coprocessor-move delay) was not the cause: `cvt.s.w` has no `COD` flag |
| Patch | `ee-as_r5900_delay_swap.patch`: for the r5900 the test becomes `(prev_pinfo & INSN_SYNC) != 0` (only `sync`, `sync.l`, `sync.p` refuse, as the comment says), and a previous `div.s` / `sqrt.s` (opcode match `0x46000003` / `0x46000004`) is never moved into a slot. Other CPUs keep the old text. Swapping only happens in reorder mode, so `.set noreorder` code (all of `asm/`) is untouched |
| Why the `div.s` / `sqrt.s` exception | ee-gcc r1 emits them as `nop; nop; div.s` (ruling 3.7) and gcc leaves the following branch in reorder mode, so the bare typo fix moves the `div.s` into the slot: control build 12 bytes short (2 sites in `text_001000C0`, 1 in `text_00150878`). The game has none of its 832 `div.s` / `sqrt.s` in a slot. **Unconfirmed which tool did this originally**: the gas rule here, or the original gcc wrapping `nop; nop; div.s` in `.set noreorder` (then a later `div.s; Y; branch` would also keep Y out of the slot). No game site tells them apart (the one `div.s; Y; jal; nop`, 0x0012E708, has a label on the `jal`) |
| Census | Whole original `.text`: 28,567 game branches (27,655 slots filled); every nop slot is explained by gas's other refusal rules, gcc's noreorder blocks (a nop pad before the branch, a filled slot or divide trap just before the instruction), gcc's short-loop pads (`mips_r5900_lengthen_loops`: loops of 6 or fewer instructions; the pad becomes the slot `nop`, so these never reach gas) or `div.s` / `sqrt.s`, **except one**: `sw $v0, -0x4788($gp)` at 0x001C6E7C, a small-data store, which gcc writes as a `sw $2,sym` macro and gas keeps in a relaxable frag (never swapped). SDK: 11 left, 7 of them `ctc2.ni` (COD in gas; the census script missed the name), 4 not looked at. `mtc1` sites (ER-063's script): 285 with the use in the slot, 0 with a `nop` |
| Script | `tools/toolchain/build_ee_binutils.sh` checks the four markers, builds into `b-binutils-r4` / `ee-binutils-2.9-991111-r4`, log `build_ee_binutils-r4.log` |
| Output | SHA-1 `ee-as.exe` `BEF1A5A61EB44DEB6DD66278B1CAC54ABBC07D8A`, `ee-ld.exe` `682ECADBE657FB249F1FBEE16283B05ACF445913` (local builds; `ee-ld` has no source change) |
| Fallbacks | `-r3` (first three patches), `-r2`, `-r1` and the unpatched ER-011 build stay; `src-lf` now carries all four patches |
| Proof | `make regress` green with r4 on main at `9807646` (685 placed functions, `-fno-edge-lcm`): whole-ELF SHA-1 `6087a8ca...`, 3,606 of 3,606. Controls: the typo fix alone FAILS (above); r3 also passes (no placed function needs a swap yet); ER-063's draft of func_00126F00 is 4 bytes long with r3 (the `nop` after `cvt.s.w; jal`, 50 words shifted) and 4 words off with r4 (all struct-copy `ld` order; the slot matches) |
| Not adopted | An r5900 short-loop no-swap rule in gas (tried: harmless, but gcc already keeps such loops out of gas's hands, so nothing supports it) |

### 4.3 First byte check (ER-011)

Files (ours, outside the repo): `C:\rs\toolchain\tests\er011\` = `er002_shapes.c` (copy of ER-002's test),
`er011_compare.py` (ELF reader and compare), `er011_link.ld` (link test script), `asbin\as.exe` (copy of `ee-as`).

1. `gcc -B<gcc dir>\ -O2 -G8 -S er002_shapes.c` gives the same `.s` as ER-002 (file hash equal).
2. `gcc -B<gcc dir>\ -B<asbin>\ -O2 -G8 -c` (and by hand: `ee-as -G8 -O2 -mabi=eabi`) gives an `elf32-littlemips`
   object with **e_flags `0x20924001`, the same as the game ELF**.
3. `python -I er011_compare.py obj shapes_drv.o disc/extract/SLUS_200.15`: each function's `.text` words against
   the ELF at vaddr - `0x000FF000` (trailing zero alignment words dropped). **Relocations masked as follows**:
   `R_MIPS_26` (`jal`) compares the top 6 bits and checks the game's jump target equals the address in the callee's
   name (`func_0012B540`, `func_001139C8`, `func_0012D318`: all three right); `R_MIPS_GPREL16` compares the top 16
   bits (opcode, `$gp`, target register); the game's gp offsets (`-0x5328`, `-0x52D0`) cannot be checked without the
   variables' own addresses. Nothing else is masked.
4. `ee-ld -G8 -T er011_link.ld` links the object (callees at their named addresses, `_gp = 0x00360B70`, the two
   variables at `_gp` + the game's offsets), and `er011_compare.py elf` compares the linked words **exactly, no
   masking**: all match. This tests `ee-ld`'s relocation arithmetic; the two gp offsets are inputs taken from the
   game, so they are not independent evidence.

| Function (test name, Unofficial) | Game address | Words | Result |
|---|---|---:|---|
| `wrap_12f7e0` | `0x0012F7E0` | 9 | **Match** (1 `R_MIPS_26`) |
| `update_1257e8` | `0x001257E8` | 12 | **Match** (1 `R_MIPS_26`, 3 `R_MIPS_GPREL16`) |
| `pack_12d3b8` | `0x0012D3B8` | 11 | **Match** (1 `R_MIPS_26`) |

The words after each function in the game fit too: a zero word, then the next function, after `wrap` and `pack`
(our object pads `wrap` with one zero word to the next 8-byte entry); the next function starts at once after
`update` (`0x00125818`, already 8-aligned).

Not used, not needed: the decompals binutils v0.10 Windows prebuilt, the SN ProDG mirrors (the ELF shows no SN
marks), the ps2dev Windows tarball (GCC 15), the decompme packages (probably Linux binaries; unchecked).

Leftovers in `C:\rs\toolchain` from failed tries (safe to delete, LuciDin's call): `src\` (the CRLF clone),
`build-gcc\`, `build-bison\`, `build-bison2\`, `bisontest2\`.

### 4.4 Sony's assembler and the c.cond / bc1 nop (ER-028, Wed 18 Nov 2026)

Desk research, read-only: nothing built, run or downloaded (source text read only; the scratch copies of
`tc-mips.c` are outside the repo). The question: which EE binutils leaves **no `nop` between `c.cond.s` and
`bc1t` / `bc1f` in reorder mode** (the game's 129 sites; `docs/decomp_recipes.md` 2.2, 2.3, ruling 3.4).

**What the code says (Confirmed from source read this session).** The `nop` comes from the generic MIPS coprocessor
condition-code hazard: a `c.cond` writes the FP condition code, a following `bc1x` reads it, and on a CPU without
FP interlocks the assembler separates them. Whether the CPU "has interlocks" is the macro `cop_interlocks`.

| Tree | `cop_interlocks` | r5900 gets the `nop`? |
|---|---|---|
| SSXModding/ps2-ee-toolchain `b595ded` (our 2.9-ee-991111), `gas/config/tc-mips.c` | `mips_cpu == 4300 \|\| mips_cpu == 5400` | **Yes** (r5900 is in `hilo_interlocks` and `gpr_interlocks`, deliberately, per its `gas/ChangeLog` 1998 entries, but **not** here). Hazard check in `append_insn`, guarded by `mips_opts.isa < 4` |
| Upstream binutils with r5900 support (`ps2dev/binutils-gdb` mirror, tags `binutils-2_25`, `binutils-2_29`; none before 2.21) | isa not MIPS I/II/III, or R4300, or microMIPS; r5900 is `ISA_MIPS3` | **Yes** (same result, different code: `INSN_WRITE_COND_CODE` then `INSN_READ_COND_CODE` returns a 1-nop gap) |
| decompals/binutils-mips-ps2-decompals, branch `binutils-2_40` (the v0.10 Windows build, GPL-2.0) | same upstream macro | **Yes** (`tc-mips.c` read: same macro; its CHANGELOG lists only DVP relocs, `--align-section`, mwcc debug, `-mbranch-relocs`; nothing on hazards) |
| ps2dev/binutils-gdb, branch `ee-v2.45.1` (the current ps2dev EE binutils) | same upstream macro | **Yes** (macro text identical; the 2.40-to-2.45 diff was not read further) |

So **no public tree read gives the game's behaviour**, and ours already matches every one of them: the `nop` is
inserted by the GPL Cygnus tree, by upstream, and by every fork of upstream. Upstream r5900 support (Urban, 2012 to
2013; later `-mfix-r5900` short-loop work by Noring and Rozycki, 2018) never added the r5900 to `cop_interlocks`;
the 2013 patch mail was read and has nothing on FP-condition hazards (single source for the history; mail archive at
sourceware.org, whose robots.txt does not cover `/pipermail/`).

**Candidates, ranked**

| # | Candidate | Source public? | Licence | Evidence it behaves like the game |
|---|---|---|---|---|
| 1 | **SCE's own shipped `ee-as`** (the SDK 1.6.x binutils; "2.9-ee" series, possibly a later build than 991111) | **No** (binaries inside the licensed SDK; Sony's EE GCC / binutils source was released only in part, if at all, with the PS2 Linux kit: a 2001 NetBSD list post alleges files were withheld, **Unconfirmed**, one forum source, not fetched) | GPL source obligation, not honoured publicly as far as found | The game itself (129 sites: Confirmed in ER-014). Which exact build: **not identifiable**; the assembler leaves no version string in the ELF (stripped, no `.comment`) |
| 2 | **SN Systems ProDG assembler** (`ee-as` 2.9x or 2.95-era SN builds) | **No** (a 2000 binutils-list post says SN published its modified GCC source but not its binutils: single snippet, **Unconfirmed**) | proprietary | The ELF shows no SN marks (section 1), so unlikely to be the one used |
| 3 | A **different public Cygnus 2.9-ee snapshot** (990721, 991111-01, 991111b, ...) | Only the one vendored tree (`b595ded`, a 2022 single-commit upload) was found; no other dated EE binutils source tree turned up | GPL | The 991111 one does **not** match (this task). Other snapshots are not in public hands that I could find: **nothing found does not mean nothing exists** (GitHub raw + API only; code search needs sign-in; no web crawl beyond two search queries) |
| 4 | **Upstream / decompals 2.40 / ps2dev 2.45.1** | Yes | GPL-2.0 and GPL-3.0 parts | Do **not** match (same nop) |

**Our patch against upstream.** ER-014's patch (`decomp_recipes.md` 2.3) adds `&& ! (mips_cpu == 5900 && ! mips_opts.noreorder)`
to the hazard test in `append_insn`. Upstream has no equivalent change, and no upstream or ps2dev patch to port was
found. Making the r5900 an FP-interlock CPU outright (adding it to `cop_interlocks`) is **not** what the game shows:
it would also drop the `nop` in the 1,069 `noreorder` sites where gcc filled the slot, which the game keeps. That is
why ER-014's reorder-only form matches and the blunt form does not. Why Sony's assembler does this (a reorder-mode
rule inside `append_insn`, or the assembler never inserting hazard nops in reorder mode for `bc1x` after `c.cond`,
or scheduling the branch differently) is **not known**: it is a reading of the output only.

**Recommendation: keep the patch.** (Adopted: ruling 3.4; in the toolchain since ER-026, section 4.2.) No public source tree reproduces the behaviour, so there is nothing to switch to;
the patch is the smallest change that reproduces the game on 129 of 129 sites and still passes the whole-ELF and
`func_check` regressions (ER-014). Label it, as ruling 3.4 does, a reconstruction of the original behaviour, not
Sony's code. Re-open only if (a) an actual Sony or SN `ee-as` binary is supplied by LuciDin (it could be tested
by assembling the one function `func_001115E0`'s pattern, no source needed), or (b) a dated Cygnus / SCE EE binutils
source other than `b595ded` surfaces.

Not verified: that the 2.9-ee-991111 sources match the binary SCE shipped (the whole premise); the diff between
2.40 and 2.45.1 beyond the `cop_interlocks` text; what the PS2 Linux kit's binutils source contains; the NetBSD and
2000 binutils-list snippets (search results only, pages not opened). Sites: only `raw.githubusercontent.com` (no
robots.txt, 404), `api.github.com` and `sourceware.org/pipermail` were fetched; `github.com` robots.txt was read (the
tree pages were not used). Skipped as already known to forbid: gamefaqs, mobygames, pcsx2.net, codeberg.org,
decomp.me site, psdevwiki.com, tcrf.net.

## 5. Flags

| Flag | Status | Evidence |
|---|---|---|
| ISA / CPU defaults of the `ee` target (`-mips3`, R5900, EABI, single-float, 32-bit pointers) | **Confirmed** | e_flags, code; they are the target's defaults, so no flag is needed |
| `-O2` | **Unconfirmed** (fits 3 of 3 functions; `-O1` does not) | section 3.2 |
| `-G 8` (small data on) | **Unconfirmed** value; small data on is **Confirmed** (5,735 gp-relative accesses, `.sdata` / `.sbss` / `.lit4`) | `-G 0` gives different code |
| `-fno-strict-aliasing` | **In the build** (ruling `docs/decomp_recipes.md` 3.6, ER-035); whether Sony passed it or its compiler defaulted to it is **Unconfirmed** | this ee-gcc enables strict aliasing at `-O2`; func_0013DBC0 reloads a pointer after a float store, which only the flag gives; the whole ELF stays byte-identical with it |
| `-fno-edge-lcm` | **In the build** (ruling `docs/decomp_recipes.md` 3.11, ER-069, section 5.1); whether Sony passed it or its compiler predates the edge LCM is **Unconfirmed** | the game keeps loads after joins that this ee-gcc's default edge-based LCM removes; func_0012EEB0 matches only with it; the whole ELF stays byte-identical with it |
| `-fomit-frame-pointer`, `-g` | **Unknown** | SSX Tricky's release flags (research.md §4) are a hypothesis only; `$fp` is used as a saved register (`s8`) in `main`, as MIPS GCC does at `-O` |
| SDK libraries built with `-G 0` | **Unconfirmed** | 112 `lui $at; ori $at; mtc1` float constants (the no-small-data form of `li.s`) |
| C++ (`-fno-exceptions` etc.) | **No sign of C++**: no `.gcc_except_table`, `.ctors` / `.dtors`, or mangled names among the strings | |

### 5.1 `-fno-edge-lcm` (ER-069, Sun 22 Nov 2026; ruling 3.11)

**What the flag switches (Confirmed in the source, `gcc/toplev.c`, `gcse.c`, `lcm.c` of the b595ded tree).** This
ee-gcc has three GCSE switches, all on by default: `-fedge-lcm` (PRE by edge-based lazy code motion, `pre_edge_lcm`:
it may insert a computation on a CFG edge and delete a partially redundant one after the join), `-fedge-lm` (load
motion for `MEM`s at a `SYMBOL_REF` address) and `-fedge-sm` (store motion); the last two need the first. Off, PRE
uses the older block-based `pre_lcm` (insertions at block ends only) and neither motion runs. `gcc/ChangeLog`,
Mon Sep 27 1999, Andrew MacLeod (Cygnus): all three are new there, about six weeks before the 991111 snapshot.
GCSE itself (copy / constant propagation, PRE) still runs, which is why func_0012F550 (needs GCSE's copy propagation;
breaks with `-fno-gcse`) is unaffected.

**What it does to the game's code (Confirmed, RTL dump `-dG` of func_0012D600).** With edge LCM, the load of
`a[0]` after `if (a[0] < lo0) a[0] = lo0;` is partially redundant (the store kills it on one path only), so gcc
copies the value on the store's edge and deletes the reload after the join ("PRE/HOIST: edge (3,4), copying
expression 8"; 2 substs, 4 insns created). With `-fno-edge-lcm`: 0 substs, the reload stays, as in the game.
`-fno-edge-lm` or `-fno-edge-sm` alone change nothing in the parked functions: it is the LCM itself.

**Evidence** (notes folder `ER-069_evidence.md`; scratch files `C:\rs\toolchain\er069\`):

| Test | Current build before ER-069 | With `-fno-edge-lcm` |
|---|---|---|
| Whole ELF, every placed function | PASS | **PASS** (3,606 of 3,606) |
| func_0012EEB0 placed (scratch, put back), `make regress` | **FAIL** (SHA-1 `9b6ca991...`, only it differs) | **PASS** (`6087a8ca...`) |
| func_00113C30 / func_0012D600 / func_0013FCF8 (masked, ER-063 / ER-072 drafts) | 157 / 68 / 155 words, sizes wrong | 80 / 7 / 10 words, sizes right (same as `-fno-gcse`); the rest is C shape |
| m2c census, 693 functions with a compiling draft (6 files) | 97 MATCH | 100 MATCH: +3 (func_00144488, func_0014F550, func_00150350), **0 lost**; 157 drafts change |

**Reading (Unconfirmed):** either SCE's builds passed `-fno-edge-lcm` (SSX Tricky's SDK-era demo makefile does,
`docs/research.md` 4), or the game's compiler was a 2.9-ee snapshot from before 27 Sep 1999. In this tree both give
the same code. No compiler patch was needed, so `GCC_DIR` stays `ee-gcc-2.9-991111-r1` and no r2 was built.

**Not understood yet:** func_00145D08's wrap loops (a field through a loaded pointer reloaded and stored on every
pass, the pointer copied to `$v0` for the second loop) come out with neither setting in 4 source forms; with the
flag `loop.c` keeps the field in a register in the loop. Not a counterexample (neither matches), but open.

## 6. What ER-003 (splat split) needs

- splat: `platform: ps2`, `compiler: EEGCC` (research.md §4). Non-relocatable EXEC ELF, one `PT_LOAD`, so the split
  works on file offsets: **vram = file offset + `0x000FF000`** for every allocated section (`.text` `0x1000` ->
  `0x00100000`, `.data` `0xF1380` -> `0x001F0380`, ..., `.sdata` `0x25C800` -> `0x0035B800`).
- Segments in file order: `.text` (`0x001000`-`0x0F1354`), `.data` (`0x0F1380`-`0x253770`), `.vudata`
  (`0x253770`-`0x254DA0`), `.rodata` (`0x254E00`-`0x259B18`), `.lit4` (`0x259B80`-`0x25C784`), `.sdata`
  (`0x25C800`-`0x25D410`), then `.sbss` / `.bss` (NOBITS). Padding between them is alignment fill. After the load
  image: `.reginfo` (`0x25D494`), the DVP overlay table, strings and four overlay sections, and the section header
  table; these must be reproduced for a byte-identical ELF (**the whole-file SHA-1 needs them**, not only the load
  image).
- **gp = `0x00360B70`** (`_gp`). Entry `0x00100008`. `main` = `func_001000C0`. `_fbss` = `0x0035C480`,
  `_end` = `0x01B7BAA0`; stack size `0x00100000`; heap size `-1`.
- crt0 (`0x00100000`-`0x001000C0`) is SCE's `crt0.s` shape: keep as asm.
- Link order hint: game code first, SDK libraries last (library code from about `0x001C8000`; `PsII` version
  strings near the end of `.data`, file `0x252210`-`0x253008`; the exact boundary is ER-004's census).
- The 430 undecodable `.word`s inside `.text` (rabbitizer R5900) need a look: data in `.text`, or opcodes the
  disassembler does not know (COP2 / VU0 forms). splat / spimdisasm must disassemble MMI and VU0 macro opcodes
  correctly; test early (research.md §4).
- Section header names include `.vutext`, `.vudata`, `.vubss` and the `.DVP.*` sections; the SDK linker script
  (`app.cmd` form) will have to be rebuilt by hand to place them.
- Toolchain: `cc1` / `gcc` from `C:\rs\toolchain\ee-gcc-2.9-991111\` (`-O2 -G8` to start), `ee-as` / `ee-ld` /
  `ee-objdump` from `C:\rs\toolchain\ee-binutils-2.9-991111\` (section 4.2; assemble with `-G8 -O2 -mabi=eabi`, or
  let the driver call `as` through `-B`).
  Keep toolchain paths short and without spaces (`C:\rs\...`).

### 6.1 Result (ER-003, Tue 17 Nov 2026)

**Done: the whole ELF rebuilds byte-identical** (SHA-1 `6087A8CA...`, all 2,485,924 bytes: ELF and program
headers, the load image, `.reginfo`, the DVP sections, `.shstrtab` and the section header table), with every
function as INCLUDE_ASM (127 empty ones already in C). How: `docs/build.md`. What the list above turned into:

| Item | Outcome |
|---|---|
| splat `platform: ps2`, `compiler: EEGCC`, vram = file offset + `0xFF000` | Used as planned (splat 0.50.1); input is the PT_LOAD bytes (`tools/extract_rom.py`), as splat's own `create_config` does for PS2 ELFs |
| Linker script (`app.cmd` form) | Rebuilt by `tools/gen_ld.py`: every section 128-aligned except the VU ones, `_gp = ALIGN(128) + 0x7FF0` before `.lit4`; all addresses come out exactly. That this is SCE's own `app.cmd` text is still Unconfirmed; it is one that gives the same output with this `ee-ld` |
| `.reginfo` mask = crt0's registers only | **Explained (Confirmed in the bfd source)**: bfd marks `.reginfo` `SEC_LINK_ONCE` / `SEC_LINK_DUPLICATES_SAME_SIZE`, so `ld` keeps the first object's copy and drops the rest; crt0 was first on SCE's link line. `ri_gp_value` is filled in by `ld` |
| DVP overlay sections | `ee-ld` reproduces them (types by name, `sh_link`, entsize, addresses) from objects holding their bytes; the bytes come from the original ELF (`tools/gen_elf_extras.py`), since `ee-dvp-as` is not built |
| Empty `.vutext` / `.vubss`; section alignments (`.text` 64, `.data` / `.bss` 128, `.vudata` / `.rodata` / `.sdata` / `.sbss` 16) | Reproduced by `src/elf_layout.s` (empty input sections). Which original objects asked for those alignments is not known |
| MMI and VU0 macro opcodes | spimdisasm 1.42.4 / rabbitizer 1.16.2 write them in a form `ee-as` assembles byte-identically (the whole `.text` matches). 490 words in `.text` are written as `.word` (6 flagged invalid); 3 whole blocks are labelled as data: `func_001D73D0`, `func_001D7658`, `func_001DECA0` (COP2 / VU0-style words; what they are is ER-004's) |
| crt0 | Kept as asm (`asm/text_00100000.s`); spimdisasm splits it into 4 symbols (`func_00100000` = the two leading `nop`s, `func_00100008` = entry, `func_001000B0` = the tail after `main` returns, `func_001000B8` = the ExitThread stub) |
| Link order: game first, SDK last | Not split yet: `.text` after crt0 is one C file (`src/text_001000C0.c`); the game / SDK boundary and file split are ER-004's |

Assembler limits found on the way (each fixed in the build; `docs/decomp_recipes.md` 1.3): numeric register
names only; no `%gp_rel` (gp-relative macros plus `.extern` size hints instead); no `.aent`; no `.incbin`;
denormal `.float` values misread.

## 7. Not verified, Unconfirmed and Unofficial items

Not verified:

- The **exact compiler build**. 991111 fits 3 small functions; 990721, 991111-01 or another 2.9-ee build may give
  the same code. Larger functions in ER-003 / ER-004 will tell.
- That this LP64 MSYS2 host build generates the same code as SCE's own (32-bit host) 2.9-ee-991111 in every case.
  Old GCCs have occasionally differed between 32- and 64-bit host builds.
- Byte-level **compiler** output beyond the three test functions (33 words; ER-011 section 4.3) and the 127 empty
  functions ER-003 builds from C. The **assembler and linker** side is now shown for the whole ELF (ER-003, 6.1):
  every instruction form in the game, MMI and VU0 macro opcodes included, reassembles byte-identically; C
  functions with `.lit4` constants, `%hi` / `%lo` pairs and branches still wait for the first real matches.
- The gp offsets of the two variables in `func_001257E8` (masked; not checkable without the variables' addresses).
- That the LP64 MSYS2 host build of the binutils encodes everything as SCE's own `ee-as` / `ee-ld` did (fits the 33
  words; old binutils on 64-bit hosts have had host-width bugs).
- ~~Whether `ee-ld` reproduces the game's `.reginfo`, DVP overlay sections and section header table~~: it does
  (ER-003, 6.1).
- `ee-dvp-as` (VU assembler) is not built.
- That ER-026's gas patch is how SCE's assembler worked (it reproduces the output only, 4.4); other hazard rules that
  might differ (`mtc1` / `ctc1` then use, HI / LO) have not been met in C yet.
- That ER-070's swap fix (4.2, r4) is how SCE's assembler worked: it reproduces the game's slots only; whether the
  `div.s` / `sqrt.s` rule was gas's or gcc's is open (4.2).
- That the linker script is SCE's own `app.cmd` file, and that crt0 is byte-identical to an SDK `crt0.s`.
- What `func_001D03E0` and `func_001CFD80` are (`_InitSys` and `Exit` are guesses).
- ~~Why `.reginfo` `ri_gprmask` holds only crt0's registers~~: link-once `.reginfo`, crt0 linked first (ER-003, 6.1).
- Whether `ERDATA.BIN` holds compressed code.
- The four-digit `PsII` tags = runtime library release numbers.

Unconfirmed readings: listed inline above (the compiler build, flags, SDK release mapping, "FS" = FromSoftware,
libvu0 present, SDK libraries built with `-G 0`, DVP overlay sections as debugger copies).

Unofficial labels: the test-function names `wrap_12f7e0`, `update_1257e8`, `pack_12d3b8` and the variable names
`D_gp_5328`, `D_gp_52D0`, `vec4` in `C:\rs\toolchain\tests\er002_shapes.c`.

## Appendix A: `C:\rs\toolchain\build_ee_gcc.sh`

Now also in the repo as `tools/toolchain/build_ee_gcc.sh` (ER-003; same SHA-1; `docs/build.md` 2). Kept here because `C:\rs` is not backed up. Script SHA-1 `3A25FECA3644A0212BDEEC566A0DF6EC1FCE1A05`.

```bash
#!/bin/bash
# ER-002: build ee-gcc 2.9-ee-991111 (SSXModding/ps2-ee-toolchain commit b595ded, GPL source) natively
# on Windows under MSYS2, host x86_64-pc-msys (LP64), target "ee" = mips64r5900-sky-elf.
# Pattern taken from Project Twinblade's tools/build_old_gcc.sh (GCC 2.7.2 under MSYS2).
#
# Source must be cloned with LF line endings (this machine has core.autocrlf=true globally):
#   git -c core.autocrlf=false clone https://github.com/SSXModding/ps2-ee-toolchain.git /c/rs/toolchain/src-lf
# (Two case-colliding files, ee/libgloss/sparc/crt0.[sS] and ee/readline/{CHANGELOG,ChangeLog}, cannot
#  both exist on NTFS; neither is used by this build.)
#
# Run from an MSYS2 MSYS shell:  /c/rs/toolchain/build_ee_gcc.sh
# Output: /c/rs/toolchain/ee-gcc-2.9-991111/{gcc,cpp,cc1}.exe (+ msys-2.0.dll)
set -e
export PATH="/usr/bin:$PATH"
unset CC CXX

ROOT=/c/rs/toolchain
SRC=$ROOT/src-lf/ee
BUILD=$ROOT/b-gcc
OUT=$ROOT/ee-gcc-2.9-991111
LOG=$ROOT/build_ee_gcc.log

# Host compiler flags: GCC 15 rejects much 1990s C by default; these only relax diagnostics.
CFLAGS="-O0 -g0 -std=gnu89 -w -fcommon -Wno-int-conversion -Wno-implicit-function-declaration -Wno-implicit-int -Wno-return-mismatch -Wno-incompatible-pointer-types"
export CFLAGS

cd "$SRC"
# 1. config.sub: accept x86_64 CPUs and the msys OS (idempotent).
if ! grep -q 'ER-002 x86_64' config.sub; then
    sed -i 's/^\tmips64r5900-\* | mips64r5900el-\*) # CYGNUS LOCAL gavin\/r5900$/\tx86_64-*) # ER-002 x86_64\n\t\t;;\n&/' config.sub
    sed -i 's/      | -cygwin\* | -pe\* /      | -cygwin* | -msys* | -pe* /' config.sub
fi
grep -q 'ER-002 x86_64' config.sub
grep -q -- '-msys\*' config.sub

# 2. gcc/configure: an LP64 MSYS host entry at the top of the main machine case
#    (64-bit HOST_WIDE_INT, which the r5900 target requires: target_requires_64bit_host_wide_int).
if ! grep -q 'ER-002 msys host' gcc/configure; then
    awk '!done && /^\t# Support site-specific machine types\.$/ {print "\tx86_64-*-msys*) # ER-002 msys host"; print "\t\txm_file=xm-msys64.h"; print "\t\tfixincludes=Makefile.in"; print "\t\t;;"; done=1}
         {print}' gcc/configure > gcc/configure.tmp
    mv gcc/configure.tmp gcc/configure
    chmod +x gcc/configure
fi
grep -q 'ER-002 msys host' gcc/configure

cat > gcc/config/xm-msys64.h <<'EOF'
/* ER-002: host description for GCC running under MSYS2 (x86_64, LP64). */
#define HOST_BITS_PER_CHAR    8
#define HOST_BITS_PER_SHORT  16
#define HOST_BITS_PER_INT    32
#define HOST_BITS_PER_LONG   64
#define HOST_BITS_PER_LONGLONG 64
#define HOST_BITS_PER_WIDE_INT 64
#define HOST_WIDE_INT long
#define FALSE 0
#define TRUE  1
#define SUCCESS_EXIT_CODE 0
#define FATAL_EXIT_CODE 33
#define HAVE_VPRINTF
#define HAVE_STRERROR
#define POSIX
#define NO_STAB_H
#define BSTRING
#include "tm.h"
EOF

# 3. The tree ships no generated c-parse.c / cexp.c; MSYS2's bison 3.8 rejects the 1999 grammar.
#    Build the tree's own bison 1.25 and use it, with the grammar unchanged.
BISONB=$ROOT/b-bison
rm -rf "$BISONB"; mkdir -p "$BISONB"; cd "$BISONB"
"$SRC/bison/configure" --host=x86_64-pc-msys --build=x86_64-pc-msys > "$ROOT/build_bison.log" 2>&1
make CFLAGS="$CFLAGS" >> "$ROOT/build_bison.log" 2>&1
export BISON_SIMPLE="$SRC/bison/bison.simple" BISON_HAIRY="$SRC/bison/bison.hairy"
"$BISONB/bison.exe" --version

# 4. libiberty, as a sibling of the gcc build dir (gcc links ../libiberty/libiberty.a).
rm -rf "$ROOT/libiberty"; mkdir -p "$ROOT/libiberty"; cd "$ROOT/libiberty"
"$SRC/libiberty/configure" --host=x86_64-pc-msys --build=x86_64-pc-msys --target=ee > "$ROOT/build_libiberty.log" 2>&1
make -j4 CFLAGS="$CFLAGS" >> "$ROOT/build_libiberty.log" 2>&1
test -f libiberty.a

# 5. configure and build cpp, cc1 and the driver only (-j4, project rule).
rm -rf "$BUILD"; mkdir -p "$BUILD"; cd "$BUILD"
"$SRC/gcc/configure" --target=ee --host=x86_64-pc-msys --build=x86_64-pc-msys \
    --prefix=$ROOT/ee-prefix --with-gnu-as --with-gnu-ld --disable-nls > "$LOG" 2>&1
make -j4 cpp cc1 xgcc CFLAGS="$CFLAGS" LANGUAGES=c BISON="$BISONB/bison.exe" >> "$LOG" 2>&1

mkdir -p "$OUT"
cp cpp.exe cc1.exe "$OUT/"
cp xgcc.exe "$OUT/gcc.exe"
cp /usr/bin/msys-2.0.dll "$OUT/"
echo "Done: $OUT"
```

## Appendix B: `C:\rs\toolchain\build_ee_binutils.sh` (ER-011)

Now also in the repo as `tools/toolchain/build_ee_binutils.sh` (ER-003; same SHA-1; `docs/build.md` 2). Kept here because `C:\rs` is not backed up. Script SHA-1 `C0E8DF1E4472A63E2C41926F25A7FBD8997E2B73`.
**Superseded by ER-026:** the repo script now also applies `tools/toolchain/patches/` and writes to
`ee-binutils-2.9-991111-r1` (section 4.2). The repo copy is the current one; this is the ER-011 original.

```bash
#!/bin/bash
# ER-011: build the EE binutils (gas, ld, objdump, objcopy, ...) 2.9-ee-991111 from the same GPL source as
# ER-002's compiler (SSXModding/ps2-ee-toolchain commit b595ded), natively on Windows under MSYS2,
# host x86_64-pc-msys (LP64), target "ee" = mips64r5900-sky-elf.
#
# Needs the LF clone /c/rs/toolchain/src-lf with ER-002's config.sub patch (build_ee_gcc.sh step 1).
# Run:  MSYSTEM=MSYS C:\msys64\usr\bin\bash.exe -lc /c/rs/toolchain/build_ee_binutils.sh
# Output: /c/rs/toolchain/ee-binutils-2.9-991111/ee-{as,ld,objdump,objcopy,...}.exe (+ msys-2.0.dll)
set -e
export PATH="/usr/bin:$PATH"
unset CC CXX

ROOT=/c/rs/toolchain
SRC=$ROOT/src-lf/ee
BUILD=$ROOT/b-binutils
OUT=$ROOT/ee-binutils-2.9-991111
LOG=$ROOT/build_ee_binutils.log

CFLAGS="-O0 -g0 -std=gnu89 -w -fcommon -Wno-int-conversion -Wno-implicit-function-declaration -Wno-implicit-int -Wno-return-mismatch -Wno-incompatible-pointer-types"
export CFLAGS

grep -q 'ER-002 x86_64' "$SRC/config.sub"

rm -rf "$BUILD"; mkdir -p "$BUILD"; cd "$BUILD"
"$SRC/configure" --target=ee --host=x86_64-pc-msys --build=x86_64-pc-msys \
    --prefix=$ROOT/ee-binutils-prefix --disable-nls > "$LOG" 2>&1
make -j4 CFLAGS="$CFLAGS" all-gas all-ld all-binutils >> "$LOG" 2>&1

mkdir -p "$OUT"
cp gas/as-new.exe "$OUT/ee-as.exe"
cp ld/ld-new.exe "$OUT/ee-ld.exe"
for p in objdump objcopy nm-new size strings readelf ar ranlib; do
    if [ -f binutils/$p.exe ]; then cp binutils/$p.exe "$OUT/ee-${p%-new}.exe"; fi
done
cp /usr/bin/msys-2.0.dll "$OUT/"
echo "Done: $OUT"
```
