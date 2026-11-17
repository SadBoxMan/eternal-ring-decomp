# Decomp recipes (Eternal Ring, SLUS_200.15)

Compiler and matching lessons, each added **the moment it is confirmed** on this game's toolchain. Only confirmed
items go here; anything still a reading stays in `docs/toolchain.md` (graded Unconfirmed) until a match proves it.
Rulings by LuciDin on matching tricks (`asm()`, `volatile` keep-alives, dead code) are recorded in their own section.

Started Tue 17 Nov 2026 by ER-011.

## 1. Toolchain and the byte check

### 1.1 The working pipeline (ER-011, confirmed on 3 functions, 33 words)

```
C:\rs\toolchain\ee-gcc-2.9-991111\gcc.exe -BC:\rs\toolchain\ee-gcc-2.9-991111\ -O2 -G8 -S file.c -o file.s
C:\rs\toolchain\ee-binutils-2.9-991111\ee-as.exe -G8 -O2 -mabi=eabi file.s -o file.o
```

or in one step, with a folder that holds a copy of `ee-as.exe` named `as.exe`:

```
gcc.exe -B<gcc dir>\ -B<as dir>\ -O2 -G8 -c file.c -o file.o
```

- Compiler: ee-gcc **2.9-ee-991111**, built from SSXModding/ps2-ee-toolchain `b595ded` under MSYS2
  (`docs/toolchain.md` 4.1). Assembler / linker: **binutils 2.9-ee-991111** from the same tree (`docs/toolchain.md`
  4.2). Both run natively from PowerShell.
- Flags **`-O2 -G8`** gave byte-identical code for `func_0012F7E0`, `func_001257E8` and `func_0012D3B8`
  (relocations masked, then exact after linking with `ee-ld`; `docs/toolchain.md` 4.3). `-O1` and `-G 0` give
  different code for them (ER-002). That `-O2 -G8` holds for the whole game is not yet shown: these are 3 small
  functions.
- **Assemble by hand with `-G8 -O2 -mabi=eabi`** (what the driver passes). Without `-mabi=eabi` the code is the same
  but e_flags lose `E_MIPS_ABI_EABI64`, so the object's header does not match the game's `0x20924001`.
- Use **Windows paths** for these MSYS2-built tools; Git Bash's `/c/...` path conversion breaks the driver's `-B`.

### 1.2 How to byte-compare a function (ER-011)

- Game file offset = vaddr - `0x000FF000` (one `PT_LOAD`; `docs/toolchain.md` 6).
- Drop trailing zero words after a function before comparing: ee-gcc aligns entries to 8 bytes, so a function of an
  odd word count is followed by one zero word, in our object and in the game alike.
- Unlinked object: mask `R_MIPS_26` to the top 6 bits (and check the game's `jal` target against the callee's
  address) and `R_MIPS_GPREL16` to the top 16 bits. Better: link with `ee-ld` and a small script that defines the
  callees and `_gp = 0x00360B70`, then compare exactly. `tools/toolchain/er011/er011_compare.py` does both for
  ER-011's three test functions (moved into the repo by ER-003).
- For placed functions the gate is `make regress` (ER-003): the whole ELF's SHA-1 plus `tools/func_check.py`,
  which compares every function of the linked build exactly, nothing masked (`docs/build.md`).

### 1.3 The 1999 assembler and splat's output (ER-003, confirmed by the whole-ELF match)

ee-as 2.9-ee-991111 is older than the gas splat targets. Each limit below broke the first build and is handled in
the build; confirmed because the whole ELF now matches byte for byte.

| Symptom | Cause | Fix in the build |
|---|---|---|
| `illegal operands` on every line with `$v0`, `$sp`, ... | ee-as knows numeric register names only | yaml `mips_abi_gpr: numeric`, `mips_abi_float_regs: numeric` |
| `Bad expression` on `lw $2, %gp_rel(sym)($28)` | no `%gp_rel` operator (its `%gprel` is MIPS16-only) | yaml `use_gp_rel_macro(_nonmatching): False` (splat writes `lw $2, (sym)`), plus `.extern sym, 4` for each such symbol (`tools/gen_gp_externs.py`): under `-G8` ee-as makes a macro gp-relative only if it knows the symbol is small (`tc-mips.c` `nopic_need_relax`). Without the hint it silently emits `lui` + `lw`: no error, wrong size |
| `Unknown pseudo-op: .aent` | no `.aent` | `alabel` macro writes the label only (3 alternate entries) |
| `Unknown pseudo-op: .incbin` | no `.incbin` | `.vudata` split as `data` (words), not `databin` |
| warnings on quoted labels, `\()` | splat's default `macro.inc` is for modern gas | our `include/macro.inc`; `nonmatching` is a no-op (no token pasting) |
| one word off by one: `.float 5.605193857e-45` (`0x00000004`) assembles to `0x00000003` | ee-as misreads denormal floats | `tools/split.py` makes spimdisasm write denormal floats / doubles as `.word` |

Also confirmed: MMI and VU0 macro mnemonics as rabbitizer 1.16.2 writes them assemble byte-identically with ee-as;
the GCC-style `.align 3` before functions and `.align 2` before branch labels (splat `EEGCC`) add nothing.

### 1.4 Linker facts (ER-003, confirmed)

- **`.reginfo` is link-once**: bfd keeps the first input object's `.reginfo` and drops the others (so masks are
  not OR-ed). crt0 must be the first object `ee-ld` opens: first on the command line, and the objects before
  `-T` (a script's own file names would be opened first).
- An output section's alignment is the largest of its inputs'; the game's alignments come from empty aligned
  input sections in `src/elf_layout.s`.
- An object on the command line that the script does not place becomes an orphan and lands at the end of a
  section of the same name (a dropped `pad_` bss object grew `.bss` by `0x14` until it was left out of the link).
- splat's `bss_size` counts from the end of the load image (`0x0035C410`), not from `.sbss`.

## 2. Code-shape lessons

- **Empty function** (ER-003, 127 functions): `void f(void) {}` at `-O2 -G8` gives `jr $31; nop` (8 bytes) and
  matches; splat writes these as C on its own. Confirmed by the whole-ELF match.

## 3. Rulings (LuciDin)

Standing rulings; each applies until LuciDin changes it.

### 3.1 The goal and what "matched" means (Tue 17 Nov 2026, about 15:20 Eastern, via DISP-01)

- **The goal is a true 100% decomp, as in Twinblade**: every function in C that compiles byte-identical, with no
  INCLUDE_ASM and no NON_MATCHING left (NON_MATCHING count 0).
- **A function counts as matched only when the byte-identical build proves it**: its C is placed in `src/`
  and `make regress` passes (whole-ELF SHA-1 plus `tools/func_check.py`, nothing masked). A per-function compare
  with relocations masked is a working aid, never proof. The tracker metric `m-dec-funcs` counts only such
  functions.

### 3.2 ELF structure pieces of the ER-003 build (Tue 17 Nov 2026, about 15:20 Eastern, via DISP-01)

| Ruling | Item |
|---|---|
| **Accepted** | `src/elf_layout.s`: hand-written, empty sections and alignment only, no game bytes (`docs/build.md` 3) |
| **Accepted** | The four VU1 DVP overlay sections (`.DVP.ovlytab`, `.DVP.ovlystrtab`, `.DVP.overlay.*`) taken from the original ELF as binary data at build time (`tools/gen_elf_extras.py`, output gitignored) until `ee-dvp-as` exists (ER-012) |
| **Accepted** | `tools/split.py` writing denormal floats (and doubles) as `.word` (1.3) |

### 3.3 Census rulings (Tue 17 Nov 2026, about 16:20 Eastern, via DISP-01; `docs/census.md` 6.3)

| Ruling | Item |
|---|---|
| **Accepted** | SDK units that were hand-written assembly in Sony's SDK (158 syscall stubs and 28 other routines, `docs/census.md`) and the 4 crt0 routines stay as faithful `.s` files and count as matched, listed apart from C (their original source was assembly) |
| **Accepted** | Inline `__asm__` only for VU0 macro / MMI instructions that C cannot express (Sony's libvu0 used it), in SDK and game code alike; each use marked with a comment and listed here; everything else in such functions stays plain C |
| **Scope** | The SDK libraries (775 units) are in the matching scope (true 100%), matched after the 2,830 game functions |

No ruling yet on matching tricks in C (`asm()`, `volatile` keep-alives, dead code): park such functions with a
one-line cause and ask.
