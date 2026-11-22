# Decomp recipes (Eternal Ring, SLUS_200.15)

Compiler and matching lessons, each added **the moment it is confirmed** on this game's toolchain. Only confirmed
items go here; anything still a reading stays in `docs/toolchain.md` (graded Unconfirmed) until a match proves it.
Rulings by LuciDin on matching tricks (`asm()`, `volatile` keep-alives, dead code) are recorded in their own section.

Started Tue 17 Nov 2026 by ER-011.

## 1. Toolchain and the byte check

### 1.1 The working pipeline (ER-011, confirmed on 3 functions, 33 words)

```
C:\rs\toolchain\ee-gcc-2.9-991111-r1\gcc.exe -BC:\rs\toolchain\ee-gcc-2.9-991111-r1\ -O2 -G8 -fno-strict-aliasing -S file.c -o file.s
C:\rs\toolchain\ee-binutils-2.9-991111-r3\ee-as.exe -G8 -O2 -mabi=eabi file.s -o file.o
```

(`ee-gcc-...-r1` = with our compiler patch since ER-034 (2.8, ruling 3.7: `nop; nop; div.s`); the unpatched
`ee-gcc-2.9-991111` stays as a fallback. `ee-binutils-...-r3` = with all three gas patches since ER-034 (2.3, 2.7,
2.8; rulings 3.4, 3.5, 3.8); `-r2` has the first two, `-r1` the first only; the unpatched `ee-binutils-2.9-991111`
gives an extra `nop` after FP compares. `-fno-strict-aliasing` since ER-035, ruling 3.6.)

or in one step, with a folder that holds a copy of `ee-as.exe` named `as.exe`:

```
gcc.exe -B<gcc dir>\ -B<as dir>\ -O2 -G8 -fno-strict-aliasing -c file.c -o file.o
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
- **Matching tools (ER-015, Thu 19 Nov 2026): `docs/matching_tools.md`.** `tools/cmpfunc.py` compiles a scratch or
  `src/` file with the Makefile's own settings and compares functions with the original (side-by-side R5900 diff;
  jal, `%hi` / `%lo`, `$gp` targets and `.lit4` values checked); `tools/place.py` swaps INCLUDE_ASM and C;
  `tools/clones.py` tries a placed function's C on its strict clone family; m2c's `mipsee` target gives first drafts.
  `tools/m2c_bulk.py` (ER-050, Sat 21 Nov 2026) drafts, cleans, compiles and compares many functions at once
  (`docs/matching_tools.md` 6).
  ER-011's `er011_compare.py` is superseded (kept for its record).

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

**Status (ER-024, Wed 18 Nov 2026):** the `.lit4` and jump-table blockers are fixed by the per-file data split
(`docs/build.md` 3.2; matching side in 2.5), except for jump tables in `text_001343E8` (2.5). The FP-compare `nop`
is handled by the toolchain since ER-026 (Thu 19 Nov 2026; ruling 3.4, 2.3, 2.6): write plain C.

| Blocker | Evidence | Functions it parks | Fix (needs a card) |
|---|---|---|---|
| **Float literals (`.lit4`)**: gas, not gcc, turns `li.s` with a constant whose low 16 bits are not 0 into a `.lit4` entry **in the C file's own object**. `.lit4` is one asm data subsegment, so the new entry lands at the start of `.lit4` and shifts every literal by 4 | test build with `func_0014A948` written with `1500.0f`: map shows `.lit4 0x2c08` (was `0x2c04`), `text_00145120.o(.lit4)` 4 bytes at `0x00358B80`, func_check reports `D_00359DC4` and others MOVED (+4). Reverted, not committed | every function with such a float constant (`func_0014A948`, `func_001115E0` and, from the census, most COP1 functions) | split `.lit4` per C file (splat `.lit4` subsegments named after each C file) or move each file's literals out of the shared pool; the literals of one function are adjacent in the pool, and gas does not merge equal literals (0.01 appears twice in a row for `func_001115E0`) |
| **Jump tables (`.rodata`)**: a `switch` compiles to the right code (masked compare matched), but gcc emits its own table into the C file's `.rodata`, which would shift the shared `.rodata` subsegment the same way | masked compare: `func_0017D930` (89-entry table, 20 cases return 1) and `func_0014F618` (6 cases) match apart from the `%hi` / `%lo` of the table | every function with a jump table or string literal (66 game functions use `jr` on a register) | the same per-file data split for `.rodata` (`docs/game_seams.md` 8.4) |
| **FP-compare hazard `nop`**: when a `c.cond.s` is followed by `bc1t` / `bc1f` whose delay slot gcc leaves empty (gcc's output is in `.set reorder` there), our ee-as inserts a `nop` between them; the game has none (107 `bc1f` + 22 `bc1t` with an empty slot, all without the `nop`). Where gcc fills the slot itself, the game has the `nop` (1,069 cases), which our gas also gives (it comes from `mips_emit_delays` at `.set noreorder`). So Sony's assembler skipped this check for the r5900 in reorder mode only. In our source (`gas/config/tc-mips.c`, `append_insn`) the check is `! cop_interlocks`, and `cop_interlocks` lists only 4300 / 5400 | `func_001115E0`: our object has `c.lt.s; nop; bc1f; nop`, the game `c.lt.s; bc1f; nop` | the 129 game functions with that pattern | patch ee-as (see 2.3) |

### 2.3 Patched assembler (ER-014; adopted by ER-026, now the build's assembler)

**ER-026 (Thu 19 Nov 2026):** the patch is `tools/toolchain/patches/ee-as_r5900_fpcc_reorder.patch`, applied by
`tools/toolchain/build_ee_binutils.sh`; the Makefile uses `C:\rs\toolchain\ee-binutils-2.9-991111-r1\`
(`docs/toolchain.md` 4.2). The paragraph "The build still uses the unpatched ee-as" below is history.

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
| ~~`func_0014A948`~~ | `text_00145120.c` | matched by ER-024 (2.5) |
| ~~`func_001115E0`~~ | `text_001000C0.c` | matched by ER-026 (2.6) |
| ~~`func_001C12D0`~~ (ER-026) | `text_00150878.c` | matched by ER-034 (2.8: the `nop; nop; div.s` comes from the compiler, ruling 3.7) |
| `func_00142788` (ER-026, ER-034) | `text_001343E8.c` | the divides match since ER-034; the clamp tail does not: the game computes the clamped value in `$f0` and copies it with `mov.s $f2, $f0` (the `*p` register) before the store; about 30 C shapes tried (if / else, `?:`, temporaries, `*p` in place) |
| ~~`func_0017D930`, `func_0014F618`~~ | `text_00150878.c`, `text_0014E2E8.c` | matched by ER-024 (2.5) |
| `func_001C6F90` | `text_00150878.c` | epilogue scheduling: the game puts the first `$gp` store between `ld $31` and `ld $16`; ours puts both loads first. Tried 6 store orders and 5 loop forms (`while`, `do`, `for(;;)`, `u32`, `int` return); no plain-C variant found |

### 2.5 Functions with their own data (ER-024, Wed 18 Nov 2026; Confirmed by `make regress`, commit `8fcf48c`)

The game files own their `.lit4` and `.rodata` (five of them also `.sdata` / `.data`; `docs/build.md` 3.2), so a
function's own literals, jump tables and strings come from its C now.

| Lesson | Proved by |
|---|---|
| **Float literals: write them as literals.** `1500.0f` in C gives `li.s`, gas puts the word in the file's own `.lit4` in source order, between the asm functions' literals (which `tools/split.py` writes as `li.s` too). Nothing else to do; the `.lit4` words have no labels any more, so an `extern float` stand-in no longer links | `func_0014A948` (`func_0012FAE8(a0, 4, 0x7F, ... + 0x850, 1500.0f)`, a sibling call) |
| **Jump tables: a plain `switch`.** gcc emits the table into the file's `.rodata` (`.align 4`, 16 bytes); splat's migrated copy in the function's asm file is simply not included any more. Several case labels on one `return 1;` give one shared target; the default and the code after the `switch` follow the case bodies | `func_0017D930` (89-entry table, 20 cases return 1, else 0), `func_0014F618` (`switch` on a `u16` at offset 4, 6 cases return 0..5, else 6; the table is the last item of the file's `.rodata`, so the next file's 16-byte alignment makes the padding after it) |
| **Where the INCLUDE_RODATA lines go.** splat's order: a symbol it does not migrate sits just before the next function that has migrated rodata. `tools/split.py` checks that order on every split; when a C function takes over a standalone symbol, define it in C where the bytes need it and drop the INCLUDE line (the check allows that; `make regress` judges the place) | all game files (270 standalone symbols) |
| **No jump table in C in `text_001343E8` yet.** Its `.rodata` starts at 0x003546D8 (8-aligned), and a gcc table would make the object 16-aligned and move it. Park such functions there ("jump table in text_001343E8: needs the file cut", one line) until the file is cut where its real files meet (S3 waits for matching, LuciDin 22 Nov 2026). Since ER-051 that file ends at `0x001388E8`; the five files cut after it (`text_001388E8` ... `text_0013DBC0`) start 16-aligned and take C jump tables | the alignment rule (`docs/build.md` 3.2); func_0013D4D0 (ER-031, `text_0013C510`: 13-entry table 0..12, explicit `case 0:` / `case 12:` beside `default` give the 0-based range, since gcc indexes from the lowest case) |
| **Variables in owned `.sdata` / `.data`** can be read through `extern` in the same file (the INCLUDE_SDATA definition and gcc's `$gp` access agree) | `func_00150818` reads `D_0035BF20` (built and checked in `48f781a`) |

Not verified yet: a matched function that **defines** data itself (a string literal, a `static` variable, an
initialised global) in an owned run. The mechanism allows it (drop the INCLUDE line, define the symbol in C at the
right place), but no such function is placed yet, so the order rules of gcc's data output are not confirmed here.

### 2.6 FP compares with the patched assembler (ER-026, Thu 19 Nov 2026; Confirmed by `make regress`)

The c.cond / bc1 `nop` is now the toolchain's job (2.3): write the compare as plain C. Where gcc leaves the `bc1x`
delay slot empty (reorder), the patched ee-as gives the game's `c.lt.s; bc1f; nop`; where gcc fills it (noreorder),
the `nop` before the branch stays, as in the game. A control build with the unpatched ee-as fails (12 bytes longer).

| Lesson | Proved by |
|---|---|
| **A plain `if (x < y)` on floats** gives the game's reorder sites; an `if / else` that both store through the same pointer gives two `jr $31; swc1` tails | `func_001115E0` (steps `*a0` by 0.01 toward `a1`; the two 0.01 literals in the file's own `.lit4`), `func_0012EA28` (both kinds of site in one function: `bc1f; nop` and `c.lt.s; nop; bc1fl` with a filled slot) |
| **A variable read through `%hi` / `%lo`** although it lies in `.sdata` (`D_0035B94C`, -0.0): declared as `extern float D_0035B94C[];` (no size, so gcc does not use `$gp`). Reading only; how the original declared it is not known | `func_001115E0` |
| **Float constants with low 16 bits 0** (1.0, -1.0, 180.0, 360.0) come out as `lui $1; mtc1` from gas's `li.s`; others go to `.lit4` | `func_00148330` (two sibling calls `j func_00147F20` with 1.0 / -1.0) |
| **Short loops get 3 `nop`s before `bc1t` from gcc itself** (`.set noreorder; nop` x3; Unconfirmed reading: gcc's r5900 short-loop workaround, padding a loop to 6 instructions; the gcc source was not read), so `do { f -= 2pi; } while (pi < f);` matches as written | `func_0012E7B0`, `func_00131608` (wrap an angle into [-pi, pi]) |
| **Clamp with a return code**: `r = 0; if (hi < f) { f = hi; r = 1; } hi = -hi; if (f < hi) { f = hi; r = -1; }` | `func_0012EAB8` |

~~Still open (parked, 2.4): `mul.s; nop; nop; div.s` followed by `jr; nop`.~~ Solved by ER-034 (2.8): the
compiler emits every `div.s` / `sqrt.s` as `nop; nop; div.s`, an insn of length 3 that never goes in a delay slot
(ruling 3.7).

### 2.7 FP compares, batch 1 (ER-035, Thu 19 Nov 2026; Confirmed by `make regress`, commits `2fd12a0`, `187cff8`, `7707c5e`)

Two more toolchain facts came out of the first batch, each ruled by LuciDin (3.5, 3.6) and now in the build:
the patched ee-as `-r2` and `-fno-strict-aliasing`.

| Lesson | Proved by |
|---|---|
| **No `nop` between `mtc1` and the next instruction that uses its register.** The game never has one (1,454 game sites, 19 SDK, 0 with a `nop`); our ee-as added one in reorder mode (`gas/config/tc-mips.c`, the `INSN_COPROC_MOVE_DELAY` test, `cop_interlocks` lacks the r5900). Fixed in the assembler (`-r2`, ruling 3.5); write plain C. `mfc1` followed by a use of its GPR still gets the `nop` when the use is a branch, as in the game (`func_0012F550`); before a normal instruction the game has none (ER-034, 2.8, ruling 3.8) | `func_0012E908`, `func_0013DBC0`; the whole ELF unchanged |
| **Build with `-fno-strict-aliasing`.** This ee-gcc enables type-based aliasing at `-O2`; the game reloads a pointer after a store of another type (a float store, then `*a0` read again), which strict aliasing removes. With the flag the whole ELF is still byte-identical (all earlier C functions unchanged). Whether Sony passed the flag or its compiler defaulted to it is Unconfirmed | `func_0013DBC0` (matches only with it) |
| **`while (pi < f) f -= 2pi;` and `if (pi < f) do { ... } while (pi < f);` both exist and give different FP registers**: the `while` form keeps the constant in one register (`c.lt $f2,$f0` in loop and test); the `if` + `do` form copies it in the first branch's delay slot. Try both | `func_0012CD08` (`while`), `func_00130F50`, `func_0012E908` (`if` + `do`) |
| **Register numbers follow variables**: when the game uses different registers for the same value in two places, a fresh local for each use (instead of reusing one pointer variable) can give its allocation | `func_0013DBC0` (four `char *` locals) |
| **A value read back right after it is stored** (`*(float *)(e + 0x910) = 0.0f; z = *(float *)(e + 0x910);`) gives the game's `sw $0` + `lwc1` and a register kept for both later compares | `func_0013DBC0` |
| **`int` from a float difference, then a sign**: `i = f - old; if (i != 0) { r = 1; if (i < 0) r = -1; } else r = 0;` gives `slt` + `movz`; a `?:` gives another layout | `func_0012F550` |
| **`return a < b;` on floats** gives `c.lt; bc1t` over `li 1` / `move 0`; `if (x) return 1;` chains share one `jr $31; li 1` | `func_00123BA8` |
| **Branch layout: an `if / else if / else` chain, not early `return`s.** With `if (c) { f(); return; } g();` gcc put `g()` on the fall-through; the game has `f()` there, which the `else` chain gives (tails of the chain then share the epilogue) | `func_0013B840` |
| **Compare operand order picks the load order**: `a->x14 > a->x18` loads `+0x14` first, `a->x18 < a->x14` loads `+0x18` first (same `slt`) | `func_0013B840` |
| **`f = p->a + s * p->b; p->a = f;` then clamp `f` and store it again** gives the game's two stores and register use; `p->a += s * p->b;` or a reload of `p->a` gives other registers | `func_00133098` |
| **Two functions identical but for one callee and one `.sdata` constant** match with the same C (`func_0011B9D0` / `func_0011BAB0`): try the twin as soon as one matches | `func_0011BAB0` |

**`div.s` and `sqrt.s` always have two `nop`s before them in the game** (754 `div.s` and 59 square-root words that rabbitizer prints as `c1 0x...` in game code, 19 `div.s` in the SDK; none without). Our compiler and assembler give none, so every function with a float divide or square root is parked for ER-034 (cause not known: a gcc option, Sony's assembler, or a compiler build; nothing in cc1's strings names it). This widens ER-034's `mul.s; nop; nop; div.s` to all divides and square roots. `sqrtf()` compiles to the game's inline `sqrt.s` + `c.eq.s` NaN check + call to `func_001E42E0` (so that libm function is `sqrtf`), but needs the symbol name `sqrtf` and the `nop`s. **The `nop`s are solved by ER-034 (2.8, ruling 3.7); the `sqrtf` name by ER-041 (2.10).**

### 2.8 Bulk m2c placement (ER-054, Sun 22 Nov 2026; Confirmed by `make regress`, 78 functions)

| Lesson | Proved by |
|---|---|
| **m2c drafts in plain C match as placed**: unknown fields as `*(T *)((char *)p + off)`, unknown types as `s32`, `.lit4` literals written as float literals, `%hi` / `%lo` data as `extern T D_X[];` (`tools/m2c_bulk.py --plain`, `docs/matching_tools.md` 6.2) | the 78 ER-054 functions in `text_00145120`, `text_0014A968`, `text_0014C840`, `text_0014D930`, `text_0014E2E8` |
| **One prototype per callee per file.** Drafts made one at a time guess a callee's parameters differently; in one file they must agree. Where a callee's value is used, keep the declaration that returns it; an argument m2c dropped because it passes through untouched in `$a0` can be written explicitly with no change to the code | `func_00149920` (`func_0012F8C8` as `s32`), `func_001507B8` (`func_00131088(arg0)`) |
| **Arithmetic on `void *`** (m2c's `arg0 + 0x90`) compiles as byte arithmetic in ee-gcc (a GNU extension) and matches; kept as m2c wrote it | `func_0014FF98` |

### 2.8 Divides, square roots and mfc1 (ER-034, Sun 22 Nov 2026; Confirmed by `make regress`)

Two more toolchain facts, each ruled by LuciDin (3.7, 3.8) and now in the build: ee-gcc `-r1` and ee-as `-r3`
(`docs/toolchain.md` 4.1, 4.2). Evidence: notes folder `ER-034_evidence.md` (dispatcher's folder).

| Lesson | Proved by |
|---|---|
| **Every `div.s` / `sqrt.s` is `nop; nop; div.s`, from the compiler.** Census of the whole `.text`: 754 `div.s` and 59 `sqrt.s` in game code, 19 `div.s` in the SDK, each with exactly two `nop`s after the last real instruction (whatever it is: `lwc1`, `mtc1`, `mul.s`, another `div.s`, ...; 3 sites after `jal sqrtf; nop` and a label still have two after the label), and **none in a delay slot** (133 are followed by a branch / `jr`, always with a `nop` in its slot). gcc chooses delay slots itself, so an assembler alone cannot give that. The `divsf3` / `sqrtsf2` patterns of `mips.md` now output `nop\n\tnop\n\tdiv.s` with length 3 (`tools/toolchain/patches/ee-gcc_r5900_fdiv_nops.patch`), which also keeps them out of delay slots. Write plain C | `func_001C12D0` (control: the stock compiler gives `jr; div.s`, ELF SHA-1 fails), `func_00149330`, `func_001304C8`, `func_001536A0` |
| **No `nop` between `mfc1` and a normal instruction that uses its GPR.** The game has 58 such sites (45 game, 13 SDK) with the use directly after, none with a `nop`; the 11 sites whose user is a branch (gcc's noreorder blocks) keep the `nop`. Our ee-as added it in reorder mode (`INSN_LOAD_COPROC_DELAY` test, `cop_interlocks` lacks the r5900). Fixed in the assembler (`ee-as_r5900_mfc1_reorder.patch`, `-r3`); write plain C | `func_001B8BC0` and its byte-identical copies `func_001B8CD8`, `func_001B8FF0` (`(int)(f) & 0xFFF` table index; control: r2 fails) |
| **`(int)x * 2pi` and `(float)(int)` conversions** (`cvt.w.s` printed as `.word`, `mfc1`, `mtc1`, `cvt.s.w`) come from plain casts | `func_001304C8` (`f - (int)(f / 2pi) * 2pi`) |
| **A call's value used in a table index: put it in a local first.** `D[(int)(f(x) * k) & 0xFFF]` makes gcc compute the table address before the call (kept in `$s0`, bigger frame); `a = f(x); return D[...]` computes it after, as the game does | `func_001B8BC0` |
| **A sign as a nested `?:`**: `sign = (d == 0.0f) ? 0.0f : ((d < 0.0f) ? -1.0f : 1.0f); return (sign < 0.0f) ? -s : s;` gives the game's shared zero register and return copy; the `if` chain (ER-035's `func_0012F550` shape) does not here | `func_001304C8` |
| **Clamp into the bound itself**: `lim = (v < -lim) ? -lim : ((lim < v) ? lim : v);` gives the game's `b; mov.s` / `bc1fl; mov.s` pair with the result in `lim`'s register | `func_00149330` |
| **Angle wrap on memory**: `*a0 += d; while (pi < *a0) *a0 -= 2pi; while (*a0 < -pi) *a0 += 2pi;` gives the game's one store after each loop (gcc keeps `*a0` in a register in the loop) | `func_00149330` |

Parked by ER-034 (one-line causes): `func_00142788` (2.4); `func_0012BF08` (only `$s3` / `$s4` swapped between
the result flag and the `out` pointer; 6 orders of declarations and initialisation tried); `func_0019B6D0` (the angle
stays in `$f0` instead of `$f12` before the call, and `$s2` / `$s3` swapped for the struct base of `D_001FF390`; a
struct type for it fixed the rest; 6 tries); `func_00163158` (FP register choice in the first block and the three
add tails; 8 tries, close). `func_0012BDA8` (the twin of `func_0012BF08` with an extra call) not tried.

Still blocked after ER-034 (from ER-035's list of 46): 12 functions call `sqrtf` (`func_001E42E0` needs that name)
and 11 call the soft-double routines (libgcc names): both ER-041; 3 have VU0 code. **Names in since ER-041 (2.10).**

### 2.9 Near misses by hand (ER-062, Sun 22 Nov 2026; Confirmed by `make regress`, commit `a751ee2`, 11 functions)

| Lesson | Proved by |
|---|---|
| **Arguments one register higher in the game = a pass-through `$a0`.** When the game loads the callee's arguments into `$a1`, `$a2`, ... where ours use `$a0`, ..., the function hands its own `$a0` to the callee untouched and m2c dropped it. Give the function the parameter and pass it first: `void f(void *arg0) { g(arg0, func_X); }` gives the game's `lui $a1; j g; addiu $a1` (sibling call), with no move for `$a0`. A trailing constant m2c read as a parameter (`$a3` set in the game) goes back in as a literal | `func_00145238`, `func_00145248` (`func_00130FE0(arg0, func_X)`), `func_001501D8` (`func_0012FB48(arg0, 0)`), `func_0014C620` (`func_0012FD10(arg0, 0xA6, 2, 5)`), `func_0014D380` (inside an `if`, with `jal`) |
| **Compare operand order picks the load order** (2.7) holds in m2c drafts too: m2c wrote `x74 < x6C`, the game loads `+0x6C` first, `x6C > x74` matches (same `slt`) | `func_0014B420` |
| **A `%lo` sharing a `lui` with another branch** (one `lui $v0, %hi(D_X)` reaching two `lwc1 %lo(D_X)($v0)` in different cases) comes from plain C with `extern f32 D_X[];` read in both cases; gcc shares the `%hi` itself | `func_0014DDD8` (`D_0035BEB0`) |
| **A taken address needs the same prototype as the definition.** A file that only takes a function's address may have declared it `(void)`; once the function is placed with a parameter, that extern must be changed to match (no code change) | `func_00145238`, `func_001501D8` |

### 2.10 Doubles and `sqrtf` (ER-041, Sun 22 Nov 2026; Confirmed by `make regress`, commits `48f1dc6`, `34893a6`)

The names gcc emits for these are in `symbol_addrs.txt` since ER-041 (`docs/census.md` 4.4): `sqrtf`, and libgcc's
GOFAST soft-double routines `dpadd`, `dpsub`, `dpmul`, `dpdiv`, `dpcmp`, `litodp`, `dptoli`, `dptoul`, `dptofp`,
`fptodp` (not `__adddf3` / `__ltdf2`: this ee-gcc is configured for GOFAST). Write plain C with doubles; never call
these routines by name. Committed matches: `func_00145428`, `func_00148210` (`text_00145120.c`), `func_0014D510`
(`text_0014C840.c`), `func_0014DAC8` (`text_0014D930.c`). Also proven in place and put back for their files' owners
(C in the dispatcher's notes folder `ER-041_c/`): `func_001399B8`, `func_001411E8`, `func_0012D568`.

| Lesson | Proved by |
|---|---|
| **`sqrtf` from `extern float sqrtf(float);` at file scope** gives gcc's builtin: `nop; nop; sqrt.s`, `c.eq.s` NaN test, `jal sqrtf` only on NaN. A declaration inside the function body gives a plain `jal sqrtf` | `func_0012D568` (`len = sqrtf(x*x + y*y + z*z); if (max < len) { scale(max / len, v, v); len = max; } return len;`) |
| **A float compared with a double constant** (`f < 0.0`, a `fabs()` result against a float literal) is promoted: `jal fptodp`, then `jal dpcmp` with the constant in `$a1` (0.0 as `$0`, 200.0 built with `ori` / `dsll32`, others from `.rodata`), then a branch on the sign of `$v0`; every comparison operator calls `dpcmp` | `func_001399B8`, `func_001411E8` |
| **`fabs()` of a double** (`extern double fabs(double);` at file scope) gives `dpcmp(d, 0)`, `bgez`, `dpsub(0, d)` | `func_001411E8` (`if (fabs(v[0]) > 200.0f) return;`) |
| **A sign as `?:`**: `s = (f(x) < 0.0) ? -1.0f : 1.0f;` loads `1.0f` after the call, as the game does; `s = 1.0f; if (...) s = -1.0f;` hoists the load above the call | `func_001399B8` |
| **The same sign test with the values swapped** (`(x < 0.0) ? 1.0f : -1.0f`), and a sign flip on the return (`return (g() < 0x1F) ? s : -s;`, not `if (...) s = -s; return s;`) | `func_0014D510`, `func_00145428` |
| **`if (!(d < 0.0f)) v = -v;`** gives the game's `bc1fl` with `neg.s` in the likely slot; a `?:` in the call's argument gives `bc1t` over two moves | `func_0014DAC8` |
| **A variable used as a call argument in one branch shares that argument register**: a value computed in the other branch needs its own variable to get the game's registers (`v` in `$f12` in branch 1, `w` in `$f2` in branch 2) | `func_0014DAC8` |
| **A pointer used in both branches loaded once before the test**: `p = *(char **)a0;` ahead of the `if` gives the game's single `lw` above the branch; `*(char **)a0 + 0x90` in each branch reloads it | `func_00148210` |
| **`if (*(s8 *)(a0 + 9) == 0) *(u8 *)(a0 + 9) += 1;`** gives `lb` for the test and `lbu; addiu; sb` for the increment | `func_0014DAC8` |
| **A prototype narrower than the game's argument**: `func_0012F8C8`'s third parameter was `s16` (from m2c); the game passes a plain `lw` there, which an `s16` parameter would truncate. Widened to `s32`; the other caller's `lh` value compiles the same | `func_00148210` |
| **Each branch's own call, not one call with a chosen pointer**: `if (a && g(...)) { f(p, A); return 1; } if (...) ...` lets gcc cross-jump the shared `jal f; li 1` tail with `$a0` / `$a1` set in each branch, as the game has; `fn = A; ... f(p, fn);` gives one setup | `func_001371C0` (masked MATCH only, see below) |

Not proven: `func_001371C0` matches on the masked compare, but its double constant `D_00354718`
(`(double)0.17453293f`, 10 degrees) is gcc's anonymous `.rodata` entry in C, and `tools/split.py`'s order check
refuses dropping the `INCLUDE_RODATA` line (the C defines no symbol of that name). Placing it needs a decision on
that check, or the file cut (ER-031) first. Same blocker, not tried: `func_0014DF08` (`D_00354A28`, `(double)0.69813174f`,
40 degrees; the last `.rodata` item of `text_0014D930.c`) and `func_0014EEF0` (`D_00354A68`, `text_0014E2E8.c`).

### 2.10 Div / sqrt and FP compares, batch 2 (ER-063, Sun 22 Nov 2026; Confirmed by `make regress`)

| Lesson | Proved by |
|---|---|
| **Two `$s` registers swapped between a result flag and a pointer argument: try a second `return`.** `if (out == NULL) return r; f(out, v); return r;` gives the game's allocation where `if (out != NULL) f(out, v); return r;` swaps them (same code otherwise; the extra use of `r` changes its priority: a reading). Moving `r = 0`, a local copy of `out`, `if (out)` or `else r = 0` did not (ER-034 tried 6 more) | `func_0012BF08` (parked by ER-034), `func_0012BDA8` |
| **A field read twice is not a pointer local**: the game reads `a1 + 0x80` / `+0x88` straight off the argument register each time; a `float *p = a1 + 0x80` costs an extra `$s` register and frame slot | `func_0012BF08` |

**Seen only in masked compares (Unconfirmed until a placement proves them under `make regress`):**

| Shape | Seen in |
|---|---|
| **A 16-byte `.rodata` vector copied with `ld` / `ld` / `sd` / `sd` is a struct assignment** from a shared `const` (several functions read the same `D_` symbol, so it is not a local initialiser): `typedef struct { float v[4]; } __attribute__((aligned(16))) Vec4; extern Vec4 D_X; Vec4 a = D_X;`. Without the alignment gcc copies with four `lw` / `sw`. The `INCLUDE_RODATA` line stays | `func_00126F00` (masked compare, all but 6 words; not placed, see below) |
| **A float constant kept in a saved FPR across calls** (`lui; mtc1 $f21` long before its two uses) is a local variable assigned the constant (`h = 1200.0f;` then `% n - h` twice); an `int` constant in `$s2` likewise (`n = 2400;`) | `func_00126F00` (masked compare) |
| **`lui 0xBF80; mtc1; ... neg.s` (a run-time `-(-1.0)`)** is `sc = -1.0f;` set in an earlier block and `-sc` at the use: local-alloc moves the single-use constant next to the `neg.s`. Set in the same block, CSE folds it to `+1.0` | `func_00126F00` (masked compare) |
| **A `u16` counter decremented in place gives the game's `lhu; andi 0xFFFF`** (`if (*(u16 *)(a0 + 0x7A) != 0xFFFF) *(u16 *)(a0 + 0x7A) -= 1;`); ER-035 had parked `func_00131D40` on that `andi` | `func_00131D40` (head matches; parked for the GCSE reload below) |
| **Two calls cross-jumped into one `jal`**: put the branch that ends in the call first (`if (a || f() != 0) { ...; g(a0, fn); } else { ... }`), so both call sites end in a jump to the same label and gcc merges them | `func_00131D40` (masked compare of that part) |

**Two toolchain differences found (the effects are measured; the causes in the original toolchain are readings; neither adopted, each needs a ruling and a card):**

- **GCSE removes a reload the game keeps.** At a join after `if (*(u16 *)(a0 + 0xE8) & 1) {...} else {...}` the game reads `a0+0xE8` again (`lhu` in both arms, `b; lhu` in the delay slot); our ee-gcc reuses the first load. With `--cflags=-fno-gcse` (diagnostic only) the head of `func_00113C30` matches and its size is right, and the reload of the counter in `func_00131D40` matches too. But the whole ELF built with `-fno-gcse` is not identical: `func_0012F550` (ER-035) needs GCSE's copy propagation. So the original compiler ran GCSE but did not remove redundant loads across joins (reading: an older or differently configured `gcse.c`; not checked in the source). Parks `func_00113C30`, `func_00113ED8` (same head), `func_00131D40`.
- **ee-as does not swap a use of an `mtc1`-written FPR into a delay slot.** Where gcc leaves a `jal` in `.set reorder` (after a divide trap block, for example), gas fills the slot itself; ours refuses to swap `cvt.s.w $f12, $f12` after `mtc1 $a2, $f12` and emits `nop` (the swap check still applies the coprocessor-move delay that ruling 3.5 removed for `nop` insertion). Census of the game's asm: **288** sites with such a use in the slot after `mtc1; jal/j/branch` (265 `cvt.s.w`, 7 `mul.s`, 5 `mov.s`, 4 `add.s`, 3 `sub.s`, 4 `swc1`), **0** in the unswapped form (2 hits were `c.cond` before `bc1`, which cannot move). Script: notes folder `ER-063_drafts/census_mtc1_slot.py`. Parks `func_00126F00` (one word; 5 more words of `ld` order in its struct copies are still open after 7 shapes).

Parked by ER-063 (one-line causes; drafts in the notes folder `ER-063_drafts/`):

| Function | Size | Cause |
|---|---|---|
| `func_00113C30` | 676 | the GCSE reload of `a0+0xE8` (above); with it left aside: register numbering in both arms (an unused `float buf[16]` local gives the game's 0x80 frame and its `jal` instead of a sibling call: dead code, would need a ruling) |
| `func_00113ED8` | 760 | same head as `func_00113C30` (GCSE reload); not drafted further |
| `func_00126F00` | 476 | gas delay-slot swap (above) + 5 words of struct-copy load order |
| `func_00131D40` | 732 | GCSE reload of the `u16` counter at +0x7A; the six angle-wrap loops' pointer reloads not yet matched (2 shapes) |
| `func_00124BE8` | 1860 | not attempted in depth: m2c lost arguments and a stack variable; needs a dedicated round (also reads a `.rodata` vector like `func_00126F00`) |


### 2.11 Div / sqrt and FP compares, batch 2 (ER-063, Sun 22 Nov 2026, with the ER-041 names; Confirmed by `make regress`)

| Lesson | Proved by |
|---|---|
| **Two `$s` registers swapped between a result flag and a pointer argument: try a second `return`.** `if (out == NULL) return r; f(out, v); return r;` gives the game's allocation where `if (out != NULL) f(out, v); return r;` swaps them (same code otherwise; the extra use of `r` changes its priority: a reading). Moving `r = 0`, a local copy of `out`, `if (out)` or `else r = 0` did not (ER-034 tried 6 more) | `func_0012BF08` (parked by ER-034), `func_0012BDA8` |
| **A field read twice is not a pointer local**: the game reads `a1 + 0x80` / `+0x88` straight off the argument register each time; a `float *p = a1 + 0x80` costs an extra `$s` register and frame slot | `func_0012BF08` |
| **The vector being measured is read off the struct, not through the local pointer** that the calls got: `func_001C9FE8(v, v, m); len = sqrtf(*(float *)(a0 + 0x90) * ... );` with `v = a0 + 0x90` gives the game's `lwc1 0x94($s0)`; `v[1] * v[1]` reads through `$s2`. The clamp tail is `func_0012D568` written out (`if (max < len) { scale(max / len, v, v); len = max; }`) | `func_0012D470`, `func_0012D7C0` |
| **A local vector built from constants and then copied with signs** (`q = {0, 0, a, 0}; t = {-q[0], q[1], -q[2], 0}`) gives the game's `sw $0` stores, one `lwc1` of `q[0]` and that register stored for every zero of `t`: write it out like that, not `t = {0, 0, -a, 0}` | `func_0012D7C0` |

**Seen only in masked compares (Unconfirmed until a placement proves them under `make regress`):**

| Shape | Seen in |
|---|---|
| **A 16-byte `.rodata` vector copied with `ld` / `ld` / `sd` / `sd` is a struct assignment** from a shared `const` (several functions read the same `D_` symbol, so it is not a local initialiser): `typedef struct { float v[4]; } __attribute__((aligned(16))) Vec4; extern Vec4 D_X; Vec4 a = D_X;`. Without the alignment gcc copies with four `lw` / `sw`. The `INCLUDE_RODATA` line stays | `func_00126F00` (masked compare, all but 6 words; not placed, see below) |
| **A float constant kept in a saved FPR across calls** (`lui; mtc1 $f21` long before its two uses) is a local variable assigned the constant (`h = 1200.0f;` then `% n - h` twice); an `int` constant in `$s2` likewise (`n = 2400;`) | `func_00126F00` (masked compare) |
| **`lui 0xBF80; mtc1; ... neg.s` (a run-time `-(-1.0)`)** is `sc = -1.0f;` set in an earlier block and `-sc` at the use: local-alloc moves the single-use constant next to the `neg.s`. Set in the same block, CSE folds it to `+1.0` | `func_00126F00` (masked compare) |
| **A `u16` counter decremented in place gives the game's `lhu; andi 0xFFFF`** (`if (*(u16 *)(a0 + 0x7A) != 0xFFFF) *(u16 *)(a0 + 0x7A) -= 1;`); ER-035 had parked `func_00131D40` on that `andi` | `func_00131D40` (head matches; parked for the GCSE reload below) |
| **Two calls cross-jumped into one `jal`**: put the branch that ends in the call first (`if (a || f() != 0) { ...; g(a0, fn); } else { ... }`), so both call sites end in a jump to the same label and gcc merges them | `func_00131D40` (masked compare of that part) |

**Two toolchain differences found (the effects are measured; the causes in the original toolchain are readings; neither adopted, each needs a ruling and a card):**

- **GCSE removes a reload the game keeps.** At a join after `if (*(u16 *)(a0 + 0xE8) & 1) {...} else {...}` the game reads `a0+0xE8` again (`lhu` in both arms, `b; lhu` in the delay slot); our ee-gcc reuses the first load. With `--cflags=-fno-gcse` (diagnostic only) the head of `func_00113C30` matches and its size is right, and the reload of the counter in `func_00131D40` matches too. But the whole ELF built with `-fno-gcse` is not identical: `func_0012F550` (ER-035) needs GCSE's copy propagation. So the original compiler ran GCSE but did not remove redundant loads across joins (reading: an older or differently configured `gcse.c`; not checked in the source). Parks `func_00113C30`, `func_00113ED8` (same head), `func_00131D40`, and after ER-041's merge `func_0012D600` (with `-fno-gcse` all but 7 words match), `func_0012EBB0` and `func_0012EEB0` (both match up to their six angle-wrap loops; the loops differ with and without GCSE, and a `static inline` wrap helper did not help).
- **ee-as does not swap a use of an `mtc1`-written FPR into a delay slot.** Where gcc leaves a `jal` in `.set reorder` (after a divide trap block, for example), gas fills the slot itself; ours refuses to swap `cvt.s.w $f12, $f12` after `mtc1 $a2, $f12` and emits `nop` (the swap check still applies the coprocessor-move delay that ruling 3.5 removed for `nop` insertion). Census of the game's asm: **288** sites with such a use in the slot after `mtc1; jal/j/branch` (265 `cvt.s.w`, 7 `mul.s`, 5 `mov.s`, 4 `add.s`, 3 `sub.s`, 4 `swc1`), **0** in the unswapped form (2 hits were `c.cond` before `bc1`, which cannot move). Script: notes folder `ER-063_drafts/census_mtc1_slot.py`. Parks `func_00126F00` (one word; 5 more words of `ld` order in its struct copies are still open after 7 shapes).

Parked by ER-063 (one-line causes; drafts in the notes folder `ER-063_drafts/`):

| Function | Size | Cause |
|---|---|---|
| `func_00113C30` | 676 | the GCSE reload of `a0+0xE8` (above); with it left aside: register numbering in both arms (an unused `float buf[16]` local gives the game's 0x80 frame and its `jal` instead of a sibling call: dead code, would need a ruling) |
| `func_00113ED8` | 760 | same head as `func_00113C30` (GCSE reload); not drafted further |
| `func_00126F00` | 476 | gas delay-slot swap (above) + 5 words of struct-copy load order |
| `func_00131D40` | 732 | GCSE reload of the `u16` counter at +0x7A; the six angle-wrap loops' pointer reloads not yet matched (2 shapes) |
| `func_00124BE8` | 1860 | not attempted in depth: m2c lost arguments and a stack variable; needs a dedicated round (also reads a `.rodata` vector like `func_00126F00`) |
| `func_0012D600` | 448 | GCSE reload of `a[0]` / `a[1]` after each clamp; 7 words around the last call (`&m[8]` set before `sqrtf` in the game) also open |
| `func_0012EBB0` | 764 | matches to the angle wraps (same as `func_0012EEB0`); wrap loops: GCSE / load placement |
| `func_0012EEB0` | 676 | first 0x118 bytes match; the six wrap loops reload `out[n]` after each join in the game |
| `func_00120EF0`, `func_0011BB90`, `func_00119FE0` | 1348, 2068, 6636 | not attempted (large; ER-041 names now in place, `func_001E9B0C` is a plain call); best after the GCSE follow-up |
| `func_00106A38` | 4136 | VU0 inline asm (ruling 3.3) plus `sqrtf`; not attempted |


### 2.12 Jump tables in the files cut by ER-051, and a bulk pass (ER-072, Sun 22 Nov 2026; Confirmed by `make regress`)

86 functions placed in `text_001388E8` / `0013A410` / `0013B748` / `0013C510` / `0013DBC0` (commits `2bc2d39`,
`e9e8f1f`, `468ebef`, `cd450b0`, `0f3a1a8`), 7,564 bytes: 73 plain MATCHes of `tools/m2c_bulk.py --plain`, 10 more
after aligning declarations, `func_0013D550` by hand, and ER-041's proven `func_001399B8` / `func_001411E8`. Jump
tables placed: `func_00139B38`, `func_0013AD40` (bulk), `func_0013D550` (hand).

| Lesson | Proved by |
|---|---|
| **gcc needs five case "nodes" for a table; a run of adjacent values on one body is one range node that counts 2.** With four (0, 9, 11-12 as one range: 1 + 1 + 2) gcc builds a compare tree; one more label beside `default` (ruling 3.9) gives the game's 13-entry table. Reading (Unconfirmed mechanism; gcc's `expand_end_case` / `add_case_node` source not checked): `case 0: case 1: case 2:` on one body counts 2, which is why `func_0013C750`'s inner switch (0..2, 3, 4) is a compare tree in the game (masked compare) | `func_0013D550` (`case 10:` beside `default`; which value of 1..8 / 10 the original listed is Unconfirmed, all give the same bytes) |
| **splat's function regex reads `void *func_X(` as the name `*func_X`**, and `tools/split.py`'s INCLUDE order check then fails ("splat expects func_X"). Write a pointer return type as `void* func_X(` (no code change) | `func_00139B38` |
| **State handlers take (object, argument)** even when one ignores the argument: a caller that passes `$a1` on (`func_0013CB60`'s `func_0013D068(arg0, arg1)` in its switch) needs the callee's second parameter in the file's prototype; adding an unused parameter changes nothing in the callee | `func_0013D068`, `func_0013CB60` |
| **A `(void)` clone or a function whose address is taken, once its callee gets a parameter, passes it on**: `void f(void *arg0) { g(arg0); }` gives the same `j g` (2.9) | `func_00142600`, `func_00143F10` (ER-049 clones) |

**Seen only in a probe / masked compares (Unconfirmed until a placement proves them):**

- **A sibling `j` needs a jump to the epilogue after the call.** In a scratch probe (`build/t2.c`, not kept), a call in
  a block that falls straight into the epilogue got `jal` + epilogue, while the same call followed by more code (a
  later case) got `j`. `func_0013C750`'s last call is `j func_0012F9C0` in the game and `jal` in every shape tried (4):
  the original's block order or structure is not found yet.

**Not placed (one-line causes; C in the dispatcher's notes folder `ER-072_c/`):**

| Function | Cause |
|---|---|
| `func_00138A20`, `func_00139EE0` | masked MATCH; placed, `make regress` fails: the double after the table (`D_00354788`, `D_003547C8`) moves -4. In asm the table carried the pad word; gcc's table ends 4 bytes earlier and splat's `D_` file has no `.align 3` (`tools/split.py` `_rodata_align` handles 16-aligned items only) |
| `func_0013C750` | the last tail call (`j` in the game, `jal` ours, above) + register choice in case 0 |
| `func_0013D088` | one delay slot: the game's `beqz n, return` takes the epilogue's `ld $ra`, ours fills from the fall-through; then everything shifts (3 shapes) |
| `func_0013FCF8` | GCSE hoists the `u16` reload of `+0x6E` above the table (2.11's toolchain difference; with `-fno-gcse`, diagnostic only, only one block's store order is left); also needs the `.align 3` fix (`D_00354968`) |
| `func_0013AA28`, `func_0013BD90`, `func_0013DF90` | each reads a double constant from a standalone `INCLUDE_RODATA` (`D_00354800`, `D_00354848`, `D_00354928` / `D_00354930`): `tools/split.py`'s order check blocker of 2.10 (`func_001371C0`). Not drafted by hand |

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

### 3.4 ER-014 rulings (Wed 18 Nov 2026, about 16:20 Eastern, via DISP-01)

| Ruling | Item |
|---|---|
| **Accepted** | The patched GNU as (ER-014): no nop between `c.cond.s` and `bc1t` / `bc1f` in reorder mode, as Sony's assembler evidently did (129 game sites; in noreorder both assemblers agree). A documented reconstruction of the original assembler's behaviour for every file, not a per-function trick; added to the toolchain by ER-026. If the exact Sony binutils build is found (ER-028), it replaces the patch |
| **Accepted** | A non-void function that ends in a plain call with no return statement (falls off the end), when that is the only C shape that gives the original's tail jump and its semantics match the original (the caller gets what the callee left in `$v0`): func_00194D08. Natural old-style C, nothing added; marked with a `PORT NOTE` because a PC compiler treats the missing return as undefined, so the port supplies an as-shipped-equivalent return on its side only |

### 3.5 ER-035: ee-as, no nop after mtc1 (Thu 19 Nov 2026, 15:33 Eastern, LuciDin in the ER-035 session)

| Ruling | Item |
|---|---|
| **Accepted** | Second ee-as patch (`tools/toolchain/patches/ee-as_r5900_copmove_reorder.patch`): no coprocessor-move hazard `nop` between `mtc1` and the next instruction that uses its register, in reorder mode, for the r5900 (1,454 game sites, none with a `nop`). Same standing as 3.4; built into `ee-binutils-2.9-991111-r2` |

### 3.6 ER-035: -fno-strict-aliasing (Thu 19 Nov 2026, 17:51 Eastern, LuciDin in the ER-035 session)

| Ruling | Item |
|---|---|
| **Accepted** | `CFLAGS` = `-O2 -G8 -fno-strict-aliasing` for every file: the game reloads memory across stores of other types (func_0013DBC0), and the whole ELF stays byte-identical with the flag |

### 3.7 ER-034: ee-gcc, nop; nop before div.s / sqrt.s (Sun 22 Nov 2026, 02:04 Eastern, LuciDin in the ER-034 session)

| Ruling | Item |
|---|---|
| **Accepted** | Compiler patch `tools/toolchain/patches/ee-gcc_r5900_fdiv_nops.patch`: `divsf3` / `sqrtsf2` emit `nop; nop; div.s` (`sqrt.s`) as one insn of length 3, so never in a delay slot (832 of 832 sites in the game and SDK). Same standing as 3.4 / 3.5: a documented reconstruction of the original toolchain's behaviour for every file, not a per-function trick; built into `ee-gcc-2.9-991111-r1` |

### 3.8 ER-034: ee-as, no nop after mfc1 (Sun 22 Nov 2026, about 02:50 Eastern, LuciDin in the ER-034 session)

| Ruling | Item |
|---|---|
| **Accepted** | Third ee-as patch (`tools/toolchain/patches/ee-as_r5900_mfc1_reorder.patch`): no load-delay `nop` between `mfc1` and the next instruction that uses its GPR, in reorder mode, for the r5900 (58 sites, none with a `nop`; the 11 branch users in noreorder keep it). Same standing as 3.4 / 3.5; built into `ee-binutils-2.9-991111-r3` |

### 3.9 ER-031: explicit case labels beside default (Sun 22 Nov 2026, 04:08 Eastern, via DISP-02)

| Ruling | Item |
|---|---|
| **Accepted** | A `switch` that lists explicit `case` labels sharing the `default` body (func_0013D4D0: `case 0:` and `case 12:` beside `default:`), when that is the only C shape that gives the original's jump table (its base and entry count) and the listing reads as the original source naming those values. Natural C, not a matching trick; ER-072 may use the same shape on the other jump-table functions |

No ruling yet on matching tricks in C (`asm()`, `volatile` keep-alives, dead code): park such functions with a
one-line cause and ask.
