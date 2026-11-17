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

## 5. Flags

| Flag | Status | Evidence |
|---|---|---|
| ISA / CPU defaults of the `ee` target (`-mips3`, R5900, EABI, single-float, 32-bit pointers) | **Confirmed** | e_flags, code; they are the target's defaults, so no flag is needed |
| `-O2` | **Unconfirmed** (fits 3 of 3 functions; `-O1` does not) | section 3.2 |
| `-G 8` (small data on) | **Unconfirmed** value; small data on is **Confirmed** (5,735 gp-relative accesses, `.sdata` / `.sbss` / `.lit4`) | `-G 0` gives different code |
| `-fno-edge-lcm`, `-fomit-frame-pointer`, `-g` | **Unknown** | SSX Tricky's release flags (research.md §4) are a hypothesis only; `$fp` is used as a saved register (`s8`) in `main`, as MIPS GCC does at `-O` |
| SDK libraries built with `-G 0` | **Unconfirmed** | 112 `lui $at; ori $at; mtc1` float constants (the no-small-data form of `li.s`) |
| C++ (`-fno-exceptions` etc.) | **No sign of C++**: no `.gcc_except_table`, `.ctors` / `.dtors`, or mangled names among the strings | |

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
