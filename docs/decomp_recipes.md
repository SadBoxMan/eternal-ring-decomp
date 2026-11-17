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
  callees and `_gp = 0x00360B70`, then compare exactly. `C:\rs\toolchain\tests\er011\er011_compare.py` does both
  (our tool, outside the repo until ER-003 brings tools in).

## 2. Code-shape lessons

(None confirmed yet beyond the flags above.)

## 3. Rulings (LuciDin)

(None yet.)
