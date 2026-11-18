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

### 2.1 First real code (ER-014, Wed 18 Nov 2026; 17 functions proven by `make regress`, commit `9ac95fd`)

Every item below is **Confirmed**: the named function is placed in `src/` as C and the whole-ELF SHA-1 plus
`tools/func_check.py` pass. 15 of the 17 matched on the first or second try with plain C; nothing compiler-steering
is used.

| Lesson | Proved by |
|---|---|
| **`-O2 -G8` holds on real code**: branches, loops, `$gp` globals, `%hi` / `%lo`, COP1 arithmetic, calls, stack buffers all match with the Makefile's flags | all 17 |
| **Write the C plainly; gcc does the delay slots.** gcc fills `jr` / branch delay slots itself (`.set noreorder` blocks) and uses branch-likely (`bnel`) with the load in the slot for `if (p == NULL) return 0; return p->x ...` | `func_00134460`, `func_001450F8` |
| **Sibling calls (`j func`) come only from a call whose value is not used** in tail position: a `void` callee called last. A call whose value is returned (`return f(x);`) always gets a frame and `jal` (also tested on scratch code: `int g(int a){ return h(a+1); }`). So a game function that returns a value on early exits but ends with `j func` was written as `if (...) return -1; ... f(...); }` with **no return after the call** (falling off the end, legal C89) | `func_00194D08` (two separate `if (...) return -1;` give the layout; `if (a \|\| b) return -1;` gives a different branch shape); plain void tail calls: `func_0014A940`, `func_0014C820`, `func_00150810`, `func_00150818` |
| **Early-exit layout**: two separate `if (x) return -1;` statements give `bnel` + a shared `jr $31; li -1` block; one `if (a \|\| b) return -1;` gives `beqz` + a different order | `func_00194D08` |
| **Store order is not source order.** The scheduler moves stores; for three stores into one struct the game order (0x2C, 0xF, 0x24) came from source order 0x2C, 0x24, 0xF. When stores differ only in order, permute the source statements | `func_0014C810`; `func_00103848` matched only when written as indexed float stores `a0[0x24..0x2B]` in address order |
| **`0.0f` stores are `sw $0`**; `1.0f` (and any float whose low 16 bits are 0, e.g. `4.0f`) is `lui $1` + `mtc1`, **no `.lit4` entry** (gas expands `li.s`) | `func_00103848`, `func_00115000` |
| **Unsigned vs signed shows in constants**: a `u32` counter compared with -1 after `n--` gives `lui $3,0xFFFF; ori $3,0xFFFF` (`li 0xffffffff`), an `int` gives `addiu $3,$0,-1` | `func_00101200` (`while (n-- != 0)` with `u32 n`) |
| **`u8` vs `char`**: `char` is signed (`lb`), so a byte read as `lbu` needs `u8`; `s16` gives `lh`, `u16` `lhu` | `func_001450F8`, `func_00194D08`, `func_00134460` |
| **Loops**: gcc keeps a down-counting `bgez` loop when written so (`for (i = 0x20; i >= 0; i--) { ...; p += 0x180; }`); an up-counting index loop over the same records became a pointer-compare loop instead (`slt` / `bnez`). Up-counting loops with a variable bound stay up-counting (`slt` + `bnez`, increment in the delay slot) | `func_00125A00` (down), `func_00125CC0` (up), `func_0016E390` (down, indexed `p[8 + i]`) |
| **r5900 short-loop `nop`s come free**: ee-as pads loops of 6 instructions or fewer with `nop`s before the branch (the r5900 short-loop erratum); C needs nothing special | `func_00101200`, `func_0016E390`, `func_00125A00` |
| **`$gp` vs `%hi` / `%lo`**: `extern int x;` / `extern float x;` / `extern T *x;` (8 bytes or less) gives `$gp`-relative access under `-G8`; `extern float a[];` (incomplete array) gives `lui` / `addiu` (or `lui` / `lwc1 %lo`). ee-gcc emits its own `.extern sym,size` lines, which sit fine beside `gp_externs.inc` | `func_00115000` (both kinds), `func_00125A00`, `func_00150818` |
| **Small stack buffers**: `float v[4];` passed by address gives a 0x20 frame with `v` at `0($sp)` and `$ra` at `0x10($sp)` (`sd`/`ld`, 64-bit saves) | `func_00111548` |
| **Saved registers** are `sd` / `ld` (64-bit); argument copies into `$s` registers are `daddu $s, $a, $0` | `func_00150818` |
| **Float return values** stay in `$f0`; `float` arguments go in `$f12`, `$f13`, ... alongside integer arguments in `$a0`-`$a3` (EABI) | `func_0014D918`, `func_00111548`, `func_00150818` |

### 2.2 Blockers found by ER-014 (Confirmed; each parks a class of functions until fixed)

| Blocker | Evidence | Functions it parks | Fix (needs a card) |
|---|---|---|---|
| **Float literals (`.lit4`)**: gas, not gcc, turns `li.s` with a constant whose low 16 bits are not 0 into a `.lit4` entry **in the C file's own object**. `.lit4` is one asm data subsegment, so the new entry lands at the start of `.lit4` and shifts every literal by 4 | test build with `func_0014A948` written with `1500.0f`: map shows `.lit4 0x2c08` (was `0x2c04`), `text_00145120.o(.lit4)` 4 bytes at `0x00358B80`, func_check reports `D_00359DC4` and others MOVED (+4). Reverted, not committed | every function with such a float constant (`func_0014A948`, `func_001115E0` and, from the census, most COP1 functions) | split `.lit4` per C file (splat `.lit4` subsegments named after each C file) or move each file's literals out of the shared pool; the literals of one function are adjacent in the pool, and gas does not merge equal literals (0.01 appears twice in a row for `func_001115E0`) |
| **Jump tables (`.rodata`)**: a `switch` compiles to the right code (masked compare matched), but gcc emits its own table into the C file's `.rodata`, which would shift the shared `.rodata` subsegment the same way | masked compare: `func_0017D930` (89-entry table, 20 cases return 1) and `func_0014F618` (6 cases) match apart from the `%hi` / `%lo` of the table | every function with a jump table or string literal (66 game functions use `jr` on a register) | the same per-file data split for `.rodata` (`docs/game_seams.md` 8.4) |
| **FP-compare hazard `nop`**: when a `c.cond.s` is followed by `bc1t` / `bc1f` whose delay slot gcc leaves empty (gcc's output is in `.set reorder` there), our ee-as inserts a `nop` between them; the game has none (107 `bc1f` + 22 `bc1t` with an empty slot, all without the `nop`). Where gcc fills the slot itself, the game has the `nop` (1,069 cases), which our gas also gives (it comes from `mips_emit_delays` at `.set noreorder`). So Sony's assembler skipped this check for the r5900 in reorder mode only. In our source (`gas/config/tc-mips.c`, `append_insn`) the check is `! cop_interlocks`, and `cop_interlocks` lists only 4300 / 5400 | `func_001115E0`: our object has `c.lt.s; nop; bc1f; nop`, the game `c.lt.s; bc1f; nop` | the 129 game functions with that pattern | patch ee-as (see 2.3) |

### 2.3 Experimental patched assembler (ER-014; not used by the build)

A gas built from a copy of the source (`C:\rs\toolchain\er014\`: `src/` copy, `b/` build, `ee-as.exe`,
`er014_gas.patch`) with one change in `append_insn` (`gas/config/tc-mips.c`, the FP-condition-code check):

```
-               && ! cop_interlocks)
+               && ! cop_interlocks
+	       && ! (mips_cpu == 5900 && ! mips_opts.noreorder))
```

i.e. for the r5900, no hazard `nop` between `c.cond` and `bc1x` when the branch is assembled in reorder mode; in
a `.set noreorder` block (gcc filled the delay slot) the `nop` stays. Results (Confirmed):

- `func_001115E0` assembles to the game's code with it (masked compare; still parked for `.lit4`).
- A scratch function whose slot gcc fills (`c.lt.s; nop; bc1f; li`) keeps its `nop`, as the game's 1,069 such
  cases do. A first version that skipped the check in both modes dropped that `nop` too: wrong.
- The whole ELF built with it (`make AS=C:/rs/toolchain/er014/ee-as.exe`) is still byte-identical and
  func_check passes on all 3,606 functions (the asm files are `.set noreorder`, so only C is affected).

Not verified: that this is how Sony's assembler was written (only that it gives the same output on these cases);
other hazard checks that might differ (`mtc1` / `ctc1` then use, HI / LO) have not been met yet. The build still
uses the unpatched ee-as; switching is LuciDin's call (toolchain change: `tools/toolchain/build_ee_binutils.sh`,
`docs/toolchain.md`, Makefile path).

### 2.4 Parked by ER-014 (one-line causes)

| Function | File | Cause |
|---|---|---|
| `func_0014A948` | `text_00145120.c` | code matches with `1500.0f` as an extern variable, but the original was a literal in `.lit4` (2.2); the literal form shifts `.lit4` |
| `func_001115E0` | `text_001000C0.c` | `.lit4` literals (0.01 twice) and the FP-compare hazard `nop` (2.2) |
| `func_0017D930`, `func_0014F618` | `text_00150878.c`, `text_0014E2E8.c` | jump table in `.rodata` (2.2); code shape matches (masked compare) |
| `func_001C6F90` | `text_00150878.c` | epilogue scheduling: the game puts the first `$gp` store between `ld $31` and `ld $16`; ours puts both loads first. Tried 6 store orders and 5 loop forms (`while`, `do`, `for(;;)`, `u32`, `int` return); no plain-C variant found |

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
