# Decomp recipes (Eternal Ring, SLUS_200.15)

Compiler and matching lessons, each added **the moment it is confirmed** on this game's toolchain. Only confirmed
items go here; anything still a reading stays in `docs/toolchain.md` (graded Unconfirmed) until a match proves it.
Rulings by LuciDin on matching tricks (`asm()`, `volatile` keep-alives, dead code) are recorded in their own section.

Started Tue 17 Nov 2026 by ER-011.

## 1. Toolchain and the byte check

### 1.1 The working pipeline (ER-011, confirmed on 3 functions, 33 words)

```
C:\rs\toolchain\ee-gcc-2.9-991111-r1\gcc.exe -BC:\rs\toolchain\ee-gcc-2.9-991111-r1\ -O2 -G8 -fno-strict-aliasing -fno-edge-lcm -S file.c -o file.s
C:\rs\toolchain\ee-binutils-2.9-991111-r4\ee-as.exe -G8 -O2 -mabi=eabi file.s -o file.o
```

(`ee-gcc-...-r1` = with our compiler patch since ER-034 (2.9, ruling 3.7: `nop; nop; div.s`); the unpatched
`ee-gcc-2.9-991111` stays as a fallback. `ee-binutils-...-r4` = with all four gas patches since ER-070 (2.3, 2.7,
2.9, 2.19; rulings 3.4, 3.5, 3.8, 3.12); `-r3` has the first three, `-r2` the first two, `-r1` the first only; the
unpatched `ee-binutils-2.9-991111` gives an extra `nop` after FP compares. `-fno-strict-aliasing` since ER-035, ruling 3.6.)

or in one step, with a folder that holds a copy of `ee-as.exe` named `as.exe`:

```
gcc.exe -B<gcc dir>\ -B<as dir>\ -O2 -G8 -fno-strict-aliasing -fno-edge-lcm -c file.c -o file.o
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
| ~~`func_001C12D0`~~ (ER-026) | `text_00150878.c` | matched by ER-034 (2.9: the `nop; nop; div.s` comes from the compiler, ruling 3.7) |
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

~~Still open (parked, 2.4): `mul.s; nop; nop; div.s` followed by `jr; nop`.~~ Solved by ER-034 (2.9): the
compiler emits every `div.s` / `sqrt.s` as `nop; nop; div.s`, an insn of length 3 that never goes in a delay slot
(ruling 3.7).

### 2.7 FP compares, batch 1 (ER-035, Thu 19 Nov 2026; Confirmed by `make regress`, commits `2fd12a0`, `187cff8`, `7707c5e`)

Two more toolchain facts came out of the first batch, each ruled by LuciDin (3.5, 3.6) and now in the build:
the patched ee-as `-r2` and `-fno-strict-aliasing`.

| Lesson | Proved by |
|---|---|
| **No `nop` between `mtc1` and the next instruction that uses its register.** The game never has one (1,454 game sites, 19 SDK, 0 with a `nop`); our ee-as added one in reorder mode (`gas/config/tc-mips.c`, the `INSN_COPROC_MOVE_DELAY` test, `cop_interlocks` lacks the r5900). Fixed in the assembler (`-r2`, ruling 3.5); write plain C. `mfc1` followed by a use of its GPR still gets the `nop` when the use is a branch, as in the game (`func_0012F550`); before a normal instruction the game has none (ER-034, 2.9, ruling 3.8) | `func_0012E908`, `func_0013DBC0`; the whole ELF unchanged |
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

**`div.s` and `sqrt.s` always have two `nop`s before them in the game** (754 `div.s` and 59 square-root words that rabbitizer prints as `c1 0x...` in game code, 19 `div.s` in the SDK; none without). Our compiler and assembler give none, so every function with a float divide or square root is parked for ER-034 (cause not known: a gcc option, Sony's assembler, or a compiler build; nothing in cc1's strings names it). This widens ER-034's `mul.s; nop; nop; div.s` to all divides and square roots. `sqrtf()` compiles to the game's inline `sqrt.s` + `c.eq.s` NaN check + call to `func_001E42E0` (so that libm function is `sqrtf`), but needs the symbol name `sqrtf` and the `nop`s. **The `nop`s are solved by ER-034 (2.9, ruling 3.7); the `sqrtf` name by ER-041 (2.11).**

### 2.8 Bulk m2c placement (ER-054, Sun 22 Nov 2026; Confirmed by `make regress`, 78 functions)

| Lesson | Proved by |
|---|---|
| **m2c drafts in plain C match as placed**: unknown fields as `*(T *)((char *)p + off)`, unknown types as `s32`, `.lit4` literals written as float literals, `%hi` / `%lo` data as `extern T D_X[];` (`tools/m2c_bulk.py --plain`, `docs/matching_tools.md` 6.2) | the 78 ER-054 functions in `text_00145120`, `text_0014A968`, `text_0014C840`, `text_0014D930`, `text_0014E2E8` |
| **One prototype per callee per file.** Drafts made one at a time guess a callee's parameters differently; in one file they must agree. Where a callee's value is used, keep the declaration that returns it; an argument m2c dropped because it passes through untouched in `$a0` can be written explicitly with no change to the code | `func_00149920` (`func_0012F8C8` as `s32`), `func_001507B8` (`func_00131088(arg0)`) |
| **Arithmetic on `void *`** (m2c's `arg0 + 0x90`) compiles as byte arithmetic in ee-gcc (a GNU extension) and matches; kept as m2c wrote it | `func_0014FF98` |

### 2.9 Divides, square roots and mfc1 (ER-034, Sun 22 Nov 2026; Confirmed by `make regress`)

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
and 11 call the soft-double routines (libgcc names): both ER-041; 3 have VU0 code. **Names in since ER-041 (2.11).**

### 2.10 Near misses by hand (ER-062, Sun 22 Nov 2026; Confirmed by `make regress`, commit `a751ee2`, 11 functions)

| Lesson | Proved by |
|---|---|
| **Arguments one register higher in the game = a pass-through `$a0`.** When the game loads the callee's arguments into `$a1`, `$a2`, ... where ours use `$a0`, ..., the function hands its own `$a0` to the callee untouched and m2c dropped it. Give the function the parameter and pass it first: `void f(void *arg0) { g(arg0, func_X); }` gives the game's `lui $a1; j g; addiu $a1` (sibling call), with no move for `$a0`. A trailing constant m2c read as a parameter (`$a3` set in the game) goes back in as a literal | `func_00145238`, `func_00145248` (`func_00130FE0(arg0, func_X)`), `func_001501D8` (`func_0012FB48(arg0, 0)`), `func_0014C620` (`func_0012FD10(arg0, 0xA6, 2, 5)`), `func_0014D380` (inside an `if`, with `jal`) |
| **Compare operand order picks the load order** (2.7) holds in m2c drafts too: m2c wrote `x74 < x6C`, the game loads `+0x6C` first, `x6C > x74` matches (same `slt`) | `func_0014B420` |
| **A `%lo` sharing a `lui` with another branch** (one `lui $v0, %hi(D_X)` reaching two `lwc1 %lo(D_X)($v0)` in different cases) comes from plain C with `extern f32 D_X[];` read in both cases; gcc shares the `%hi` itself | `func_0014DDD8` (`D_0035BEB0`) |
| **A taken address needs the same prototype as the definition.** A file that only takes a function's address may have declared it `(void)`; once the function is placed with a parameter, that extern must be changed to match (no code change) | `func_00145238`, `func_001501D8` |

### 2.11 Doubles and `sqrtf` (ER-041, Sun 22 Nov 2026; Confirmed by `make regress`, commits `48f1dc6`, `34893a6`)

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

Placed since (ER-077, 2.14): `func_001371C0`, `func_0014DF08`, `func_0014EEF0`. Before that, `func_001371C0`
matched only on the masked compare: its double constant `D_00354718`
(`(double)0.17453293f`, 10 degrees) is gcc's anonymous `.rodata` entry in C, and `tools/split.py`'s order check
refuses dropping the `INCLUDE_RODATA` line (the C defines no symbol of that name). Placing it needs a decision on
that check, or the file cut (ER-031) first. Same blocker, not tried: `func_0014DF08` (`D_00354A28`, `(double)0.69813174f`,
40 degrees; the last `.rodata` item of `text_0014D930.c`) and `func_0014EEF0` (`D_00354A68`, `text_0014E2E8.c`).

### 2.12 Div / sqrt and FP compares, batch 2 (ER-063, Sun 22 Nov 2026, with the ER-041 names; Confirmed by `make regress`)

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

- **GCSE removes a reload the game keeps.** At a join after `if (*(u16 *)(a0 + 0xE8) & 1) {...} else {...}` the game reads `a0+0xE8` again (`lhu` in both arms, `b; lhu` in the delay slot); our ee-gcc reuses the first load. With `--cflags=-fno-gcse` (diagnostic only) the head of `func_00113C30` matches and its size is right, and the reload of the counter in `func_00131D40` matches too. But the whole ELF built with `-fno-gcse` is not identical: `func_0012F550` (ER-035) needs GCSE's copy propagation. So the original compiler ran GCSE but did not remove redundant loads across joins (reading: an older or differently configured `gcse.c`; not checked in the source). **Solved by ER-069: the edge-based LCM, `-fno-edge-lcm` in `CFLAGS` since ruling 3.11 (2.17).** Parks `func_00113C30`, `func_00113ED8` (same head), `func_00131D40`, and after ER-041's merge `func_0012D600` (with `-fno-gcse` all but 7 words match), `func_0012EBB0` and `func_0012EEB0` (both match up to their six angle-wrap loops; the loops differ with and without GCSE, and a `static inline` wrap helper did not help).
- **ee-as does not swap a use of an `mtc1`-written FPR into a delay slot.** Where gcc leaves a `jal` in `.set reorder` (after a divide trap block, for example), gas fills the slot itself; ours refuses to swap `cvt.s.w $f12, $f12` after `mtc1 $a2, $f12` and emits `nop` (the swap check still applies the coprocessor-move delay that ruling 3.5 removed for `nop` insertion). Census of the game's asm: **288** sites with such a use in the slot after `mtc1; jal/j/branch` (265 `cvt.s.w`, 7 `mul.s`, 5 `mov.s`, 4 `add.s`, 3 `sub.s`, 4 `swc1`), **0** in the unswapped form (2 hits were `c.cond` before `bc1`, which cannot move). Script: notes folder `ER-063_drafts/census_mtc1_slot.py`. Parks `func_00126F00` (one word; 5 more words of `ld` order in its struct copies are still open after 7 shapes). **Solved by ER-070 (2.19, ruling 3.12): the cause was a typo in gas's swap test; ee-as r4 swaps.**

Parked by ER-063 (one-line causes; drafts in the notes folder `ER-063_drafts/`):

| Function | Size | Cause |
|---|---|---|
| `func_00113C30` | 676 | the GCSE reload of `a0+0xE8` (above); with it left aside: register numbering in both arms (an unused `float buf[16]` local gives the game's 0x80 frame and its `jal` instead of a sibling call: dead code, would need a ruling) |
| `func_00113ED8` | 760 | same head as `func_00113C30` (GCSE reload); not drafted further |
| `func_00126F00` | 476 | ~~gas delay-slot swap~~ (fixed by ee-as r4, 2.19) + struct-copy load order: 4 words with `-fno-edge-lcm` and r4 (ER-070) |
| `func_00131D40` | 732 | GCSE reload of the `u16` counter at +0x7A; the six angle-wrap loops' pointer reloads not yet matched (2 shapes) |
| `func_00124BE8` | 1860 | not attempted in depth: m2c lost arguments and a stack variable; needs a dedicated round (also reads a `.rodata` vector like `func_00126F00`) |
| `func_0012D600` | 448 | GCSE reload of `a[0]` / `a[1]` after each clamp; 7 words around the last call (`&m[8]` set before `sqrtf` in the game) also open |
| `func_0012EBB0` | 764 | matches to the angle wraps (same as `func_0012EEB0`); wrap loops: GCSE / load placement |
| `func_0012EEB0` | 676 | first 0x118 bytes match; the six wrap loops reload `out[n]` after each join in the game |
| `func_00120EF0`, `func_0011BB90`, `func_00119FE0` | 1348, 2068, 6636 | not attempted (large; ER-041 names now in place, `func_001E9B0C` is a plain call); best after the GCSE follow-up |
| `func_00106A38` | 4136 | VU0 inline asm (ruling 3.3) plus `sqrtf`; not attempted |


### 2.13 Jump tables in the files cut by ER-051, and a bulk pass (ER-072, Sun 22 Nov 2026; Confirmed by `make regress`)

88 functions placed in `text_001388E8` / `0013A410` / `0013B748` / `0013C510` / `0013DBC0` (commits `2bc2d39`,
`e9e8f1f`, `468ebef`, `cd450b0`, `0f3a1a8`, `b6690a2` (split.py), `979af8c`), 8,376 bytes: 73 plain
MATCHes of `tools/m2c_bulk.py --plain`, 10 more after aligning declarations, `func_0013D550`, `func_00138A20` and
`func_00139EE0` by hand, and ER-041's proven `func_001399B8` / `func_001411E8`. Jump tables placed: `func_00139B38`,
`func_0013AD40` (bulk), `func_0013D550`, `func_00138A20` (two tables), `func_00139EE0` (hand): 5 of 11.

| Lesson | Proved by |
|---|---|
| **gcc needs five case "nodes" for a table; a run of adjacent values on one body is one range node that counts 2.** With four (0, 9, 11-12 as one range: 1 + 1 + 2) gcc builds a compare tree; one more label beside `default` (ruling 3.9) gives the game's 13-entry table. Reading (Unconfirmed mechanism; gcc's `expand_end_case` / `add_case_node` source not checked): `case 0: case 1: case 2:` on one body counts 2, which is why `func_0013C750`'s inner switch (0..2, 3, 4) is a compare tree in the game (masked compare) | `func_0013D550` (`case 10:` beside `default`; which value of 1..8 / 10 the original listed is Unconfirmed, all give the same bytes) |
| **splat's function regex reads `void *func_X(` as the name `*func_X`**, and `tools/split.py`'s INCLUDE order check then fails ("splat expects func_X"). Write a pointer return type as `void* func_X(` (no code change) | `func_00139B38` |
| **A double right after a C jump table needs `.align 3`**: the asm table carried the pad word; gcc's table ends 4 bytes earlier, and the double moved -4 until `tools/split.py` gave 8-aligned items with 4 zero bytes before them `.align 3` (`docs/build.md` 3.2; dispatcher's yes, Sun 22 Nov 2026) | `func_00138A20` (`D_00354788`), `func_00139EE0` (`D_003547C8`) |
| **State handlers take (object, argument)** even when one ignores the argument: a caller that passes `$a1` on (`func_0013CB60`'s `func_0013D068(arg0, arg1)` in its switch) needs the callee's second parameter in the file's prototype; adding an unused parameter changes nothing in the callee | `func_0013D068`, `func_0013CB60` |
| **A `(void)` clone or a function whose address is taken, once its callee gets a parameter, passes it on**: `void f(void *arg0) { g(arg0); }` gives the same `j g` (2.10) | `func_00142600`, `func_00143F10` (ER-049 clones) |

**Seen only in a probe / masked compares (Unconfirmed until a placement proves them):**

- **A sibling `j` needs a jump to the epilogue after the call.** In a scratch probe (`build/t2.c`, not kept), a call in
  a block that falls straight into the epilogue got `jal` + epilogue, while the same call followed by more code (a
  later case) got `j`. `func_0013C750`'s last call is `j func_0012F9C0` in the game and `jal` in every shape tried (4):
  the original's block order or structure is not found yet.

**Not placed (one-line causes; C in the dispatcher's notes folder `ER-072_c/`):**

| Function | Cause |
|---|---|
| ~~`func_0013C750`~~ | **placed by ER-078 (2.22: an explicit `return;` at the end)**; was: the last tail call (`j` in the game, `jal` ours, above) + register choice in case 0 |
| `func_0013D088` | (ER-078: the slot comes from an `s32` return; now parked on aliasing, 2.22); was: one delay slot: the game's `beqz n, return` takes the epilogue's `ld $ra`, ours fills from the fall-through; then everything shifts (3 shapes) |
| ~~`func_0013FCF8`~~ | **placed by ER-087 (2.22)**; was: GCSE hoists the `u16` reload of `+0x6E` above the table (2.12's toolchain difference; with `-fno-gcse`, diagnostic only, only one block's store order is left); its double `D_00354968` has `.align 3` now |
| `func_0013AA28`, `func_0013BD90`, `func_0013DF90` | the order-check blocker is gone (ER-077, ruling 3.10); drafted by hand in ER-077 and parked on GCSE (2.14) |

### 2.14 Anonymous double constants (ER-077, Sun 22 Nov 2026; Confirmed by `make regress`, commits `2ed4058`, `78be147`)

Ruling 3.10 is in `tools/split.py` (`docs/build.md` 3.2): a C function's float compared with a double constant
that the split had as a standalone `D_` symbol may now drop that symbol's `INCLUDE_RODATA` line; gcc writes the
constant anonymously in the same place. Placed: `func_001371C0` (`D_00354718`, ER-041's C), `func_0014DF08`
(`D_00354A28`), `func_0014EEF0` (`D_00354A68` = 300.0), 1,328 bytes. Each split prints the symbols taken over.

| Lesson | Proved by |
|---|---|
| **Write the double literal as the source had it; delete the `INCLUDE_RODATA` line and nothing else.** `fabs(x) > 0.69813174f` (a float literal promoted) gives `ld` of `(double)0.69813174f` from `.rodata`; `300.0f` is loaded from `.rodata` too, where 200.0 is built with `ori` / `dsll32` (2.11; reading, Unconfirmed: 300.0's high half `0x4072C000` needs three instructions) | `func_0014DF08`, `func_0014EEF0` |
| **A clamp that reads the field three times** (`f = (f < -c) ? -c : (f > c) ? c : f;` with `f` the struct field itself, a CLAMP-macro shape) gives the game's `bc1tl` with the store of `-c` in the likely slot and `bc1fl` / `mov.s` for the upper bound; a local copy, an `if` / `else if` chain or a `lo` / `hi` pair gives other registers or 1 more word (5 shapes) | `func_0014DF08` |
| **Stack vectors in declaration order**: four `float[4]` and a `float[16]` declared in the order the game's offsets use (0x00, 0x10, 0x20, 0x30, 0x40) matched first time | `func_0014EEF0` |
| **Extern declarations above the first use**: `tools/place.py c` skips an extern line that the file already has, even when the file's copy sits below the placed function; add it to the placed block (an identical second declaration is legal C) | `func_0014EEF0` (`func_00131088`, `func_0014E608`) |

**Seen in drafts only (Unconfirmed until a placement proves them; drafts in the dispatcher's notes `ER-077_c/`):**

- **A `?:` with one double arm makes both arms double.** `w[1] += cond ? -r * 3.0f : r * 0.2;` gives the game's
  `func_0013DF90` code: `fptodp(w[1])` before the test, then `dpadd` in both branches (the float arm converted).
- **A byte variable read by absolute address** (`lui` / `lbu %lo`) needs `extern u8 D_001FF451[];`: a plain
  `extern u8` is reached through `$gp` under `-G8` (`func_0013DF90`).
- **A `static inline` max helper** (`return (b < a) ? a : b;`) gives `func_0013AA28`'s `bc1f` with the default in
  the delay slot where `?:` and `if` forms give the inverted branch; whether the original had such a helper is
  Unconfirmed (and not a ruled trick: it is ordinary C, but say so if it is ever used to place a function).

**Parked (one-line causes):**

| Function | Cause |
|---|---|
| `func_0013AA28` | GCSE (2.12): `a1 + 0x90` kept in a saved register across the switch because case 6 computes it too (one extra `sd` / `ld`); with `-fno-gcse` (diagnostic) 16 words left. Placing it also needs `_anon_takeovers` to accept `D_00354800`'s 8 zero bytes after the double (16-byte symbol; tried, reverted as unproven) |
| `func_0013BD90` | GCSE (2.12): case 1 re-reads `*(u16 *)(E0 + 0xA8C)` after the `if` in the game, ours reuses the value on one path; with `-fno-gcse` that matches, the cases' shared tail (`6C = 0; 6E = N`) is still cross-jumped with the wrong case |
| ~~`func_0013DF90`~~ | **placed by ER-087 (2.22: -fno-edge-lcm, ee-as r4, init store order)**; was: GCSE (2.12): the game re-reads `*(s8 *)(a0 + 8)` after `if (s != a0[8]) { a0[8] = s; ... }`, ours reuses it; with `-fno-gcse` 86 words left (init-block registers, a `ld` schedule) |

### 2.15 Bulk placement in `text_00150878` (ER-060, Sun 22 Nov 2026; Confirmed by `make regress`, 230 functions)

| Lesson | Proved by |
|---|---|
| **A callee's return type matters even when the value is unused.** With `extern void func_001132B0(void *);` a caller that calls it as a statement kept a later value in `$v0`; the game uses `$v1` (why: a reading, gcc treats `$v0` as set by an `s32` call). `extern s32 ...` matches in that caller and in the six other callers | `func_001BE7E8` (the other callers: func_00195E88 func_00196CE0 func_00197368 func_0019A3E0 func_0019BDC0 func_001BE7E8 func_001BE830) |
| **A callee called with different argument counts in one file** (a variadic-looking routine): declare it without a prototype, `extern s32 f();`; integer and pointer arguments compile the same | `func_001CB808`, `func_001E9D08` callers |
| **A float passed to a `double` parameter** gives the game's `jal fptodp` between the float call and the callee: write the plain call with the prototype `(void *, double)` | `func_001BEF18` |

### 2.16 Bulk pass over `text_001000C0` (ER-060 part 2, Sun 22 Nov 2026; Confirmed by `make regress`, 58 functions)

| Lesson | Proved by |
|---|---|
| **A sibling-call wrapper's return type does not show in its own code**: `return g(x);` compiled as `j g` is the same for `s32` and `f32`. m2c guesses `s32`; a caller that reads `$f0` after calling it (or a `cvt.w.s` that the game lacks) shows it is `f32`. Fix the wrapper, its callee's extern and the caller together; no byte changes in the wrapper | `func_001314C8`, `func_00131508` (`f32`, `j func_001E3EC8`), `func_0012DA68`, `func_00130758` |
| **A float passed through in `$f12`** works like the integer `$a0` of 2.10: the function never sets `$f12` before calling a callee that reads it, so give it an `f32` parameter and pass it on (no move is emitted) | `func_0012B6B0` (to `func_0012BAE0`) |

### 2.17 Reloads after joins: `-fno-edge-lcm` (ER-069, Sun 22 Nov 2026; ruling 3.11)

The first toolchain difference of 2.12 ("GCSE removes a reload the game keeps") is solved: it is this ee-gcc's
**edge-based LCM** (`-fedge-lcm`, on by default; Cygnus, 27 Sep 1999), which deletes a partially redundant load
after a join by copying the value on the other incoming edge. The game keeps those loads. `CFLAGS` now has
`-fno-edge-lcm` (`docs/toolchain.md` 5.1); GCSE still runs, so func_0012F550 is unaffected. Evidence: notes folder
`ER-069_evidence.md`.

| Lesson | Proved by |
|---|---|
| **Write the reloads as plain C.** A field read again after an `if` that stored it on one path (`if (a[0] < lo) a[0] = lo; if (hi < a[0]) ...`), a flag word tested before and after an `if / else`, and an angle wrapped in place on an argument pointer (`while (pi < out[0]) out[0] -= 2pi;`, six loops in a row) now compile with the game's reloads. No `volatile`, no copies | func_0012EEB0 (`make regress` with it placed, scratch; control without the flag fails); func_00144488, func_0014F550, func_00150350 (placed) |
| **`-fno-gcse` is no longer a diagnostic to reach for**: where it helped (2.11), `-fno-edge-lcm` gives the same code and keeps the whole ELF identical | func_00113C30, func_0012D600, func_0013FCF8 (masked compares: same words as `-fno-gcse`) |

Parked functions this moves (all in files owned by other cards in wave 4; listed for their owners; the
`text_001000C0.c` ones were taken by ER-086 in wave 5: three placed, the rest parked, 2.18):

| Function | File | With the flag (masked compare) |
|---|---|---|
| func_0012EEB0 | `text_001000C0.c` | **MATCH** with ER-063's C unchanged; placing it needs the `(void)` clone func_0012F158 and its extern changed to `(float *, float *)`, passing both on (2.13; same `j`) |
| func_0012EBB0 | `text_001000C0.c` | not re-tried (same wrap loops as func_0012EEB0) |
| func_0012D600 | `text_001000C0.c` | 7 words, right size: `&m[8]` goes to `$s0` before `sqrtf` in the game, and the order of the first call's `$a0` / `$a1` setup |
| func_00113C30 | `text_001000C0.c` | 80 words, right size: the register numbering in both arms (ER-063's note: an unused `float buf[16]` gives the 0x80 frame, dead code needs a ruling) |
| func_00113ED8, func_00131D40 | `text_001000C0.c` | not re-tried (same head as func_00113C30; the `u16` counter reload) |
| func_0013FCF8 | `text_0013DBC0.c` | 9-10 words, right size: case 0 (the game loads and negates `D_0035BB68` first and reuses `$f0` for `+0xE90` / `+0xE94`); all 120 orders of its five stores and 3 other forms tried |
| func_0013AA28, func_0013BD90, func_0013DF90 (ER-077) | `text_0013A410.c`, `text_0013B748.c`, `text_0013DBC0.c` | ER-077's drafts give the same words as its `-fno-gcse` diagnostic: 16 / 155 / 86 words left, all C shape (2.14 notes; func_0013AA28 also waits on `tools/split.py`'s 16-byte takeover) |
| func_00144488, func_0014F550, func_00150350 | `text_0013DBC0.c`, `text_0014E2E8.c` | **placed by ER-069** (m2c `--plain` drafts, census MATCHes only with the flag), `make regress` green on main 937b403 + the flag; control without the flag fails (`.text` 24 bytes longer). An 8-byte shift seen when all three were placed on the pre-merge tree did not come back |

**Seen, not understood:** func_00145D08 (`text_0013DBC0.c`) wraps `+0x190` of a loaded pointer: the game reloads
and stores the field on every loop pass and copies the pointer to `$v0` for the second loop. Neither setting gives
it in 4 source forms (with the flag `loop.c` keeps the field in a register in the loop, as func_00149330's in 2.9).

### 2.18 `text_001000C0` after the flag: GCSE-parked, near misses, large ones (ER-086 + ER-081 + ER-073, Sun 22 Nov 2026; Confirmed by `make regress`, 21 functions)

21 placed (2,996 bytes), commits `435ff7d`, `5d9343c`, `8833293`, `9dc6ca5`: func_0012EEB0, func_0012EBB0,
func_0012D600 (ER-086) and all 18 ER-060 part 2 near misses (ER-081; one plain MATCH of a `m2c_bulk.py` small-list
rerun, 17 by hand). Nothing compiler-steering.

| Lesson | Proved by |
|---|---|
| **The order of the argument moves shows the parameter order.** EABI puts the first float in `$f12` wherever it sits in the list, but gcc loads the argument registers in parameter order: the game's `$a0`, `$a1`, then `mov.s $f12` (in the `jal` slot) is a callee declared `(float *, float *, float)`. `func_001CA048` was `(float, float *, float *)` from an earlier draft; with the float last func_0012D600 matches and its four other callers (func_0012D470, func_0012D568, func_0012D7C0, func_00130B60) compile the same | func_0012D600 |
| **A pointer local set just before a call that needs it after a `sqrtf`**: `p = &m[8]; len = sqrtf(...); f(out, p, len);` gives the game's `addiu $s0, $sp, 0x30` in the NaN branch's slot, reusing `m`'s saved register; `&m[8]` written in the call, or `sqrtf` inside the call, computes it after | func_0012D600 |
| **`sqrtf` into a local when the other argument reads memory**: `len = sqrtf(...); out[1] = f(-r[2], len);` loads `r[2]` after the square root, as the game does; `f(-r[2], sqrtf(...))` computes `-r[2]` first and keeps it in a saved FPR (one more `swc1` / `lwc1` pair) | func_0012EBB0 |
| **An angle reduced in one variable that is the call argument**: `a = *(float *)(p + 0x50); a += pi; a -= (float)(s32)(a / pi2) * pi2; a -= pi; f(m, D, a);` keeps `a` in `$f12` throughout, as the game does; `a = field + pi` (load and add in one) or the reduction written inside the call use `$f0` / `$f1` | func_0010CAF0 (three times) |
| **A `u16` parameter is narrowed once on entry** (`andi $s1, $a1, 0xFFFF`) and the narrowed copy is tested and passed on; an `s32` parameter masked in the body tests the raw register | func_001320B8 |
| **A base pointer read once and stepped from a second pointer** needs its own variable: `t = *(s16 **)(a0 + 0x70); max = t[0]; p = t + 1;` gives the game's `lw $v0` then `addiu $a0, $v0, 2`; reading into the stepping pointer and incrementing it reuses one register | func_00111660 |
| **m2c's typed pointer plus byte offset**: drafts that wrote `s32 *p; p += 4;` or `D + 0x10` on an `s32 []` scale by the element size (+0x10 where the game has +4). Fix the type or the step, not the offset | func_00101238, func_00103E30, func_0012A140 (`D_001F58C0 + 4`), func_00129ED8 (`char *` record pointer) |
| **A `u8` argument loads with `lbu`**: m2c's `(s8)` cast gave `lb`; the callee's parameter is `u8` | func_0012F838, func_0012F870 |
| **A float passed through in `$f12` on a value-returning wrapper** (2.16 with 2.1): the wrapper has a frame and `jal` because it returns its callee's value: `s32 f(char *a, f32 x) { return g(a, *(s32 *)(a + 0x130), x); }`. Callers in other files declare it `void f(char *, float)`: same code | func_00130CE0 |
| **Two pass-through wrappers that return their callee's value** (`return func_0012E3C8(a0, a1, 1);`, 0x1C bytes with a frame) need the file's `extern void` for them changed to `s32`; the callers' code does not change | func_00130FE0, func_00131000 (callers func_00132890, func_001328D8 and the rest of the file unchanged) |
| **A `(void)` clone whose callee gains a parameter passes it on** (2.13 again): `func_0012B238(void *arg0) { func_0012B218(arg0, 0xFF); }` and its ER-049 clone `func_001336D8(void *arg0) { func_0012B238(arg0); }` | func_0012B238, func_001336D8 |
| **Two separate early returns** (2.1) again: `if (field test) return 0; if (g(a0) == 0) return 0; ...` gives the game's `daddu $a0, $s0` in the `jal` slot; one `\|\|` condition lets gcc reuse `$a0` and leaves a `nop` | func_001328D8 |

**Seen only in drafts / masked compares (Unconfirmed until a placement proves them):**

- **A 48-byte struct with 4-byte alignment copied by assignment comes out as `ldl` / `ldr` / `sdl` / `sdr` pairs**
  (gas's `uld` / `usd`), the game's code for `s = *(S48 *)(fc + 0xC);` (func_00120EF0: `typedef struct { s16 x0; s16 x2;
  s32 x4; s32 x8; s16 xC; s16 xE; s32 rest[8]; } S48;`, field layout Unofficial; only the size and alignment matter).
- **Struct globals change which base register gcc keeps**: `extern G390 D_001FF390;` with `D_001FF390.fc` keeps the
  symbol base in a register across the loop body as the game does (`lb 0x219($s6)` / `lw 0x288($s6)` for
  `D_001FF890`); `*(char **)(D_001FF390 + 0xFC)` on a `char []` builds `D + 0x219`-style addresses instead
  (func_00120EF0, 327 -> 277 differing words). Struct layouts Unofficial.
- **Registers follow variables** (2.7) at a larger scale: func_00113C30's draft used one `t` for the head and the
  loop; a separate local for the head moved 80 -> 49 differing words, and reading the loop bound in the `for`
  condition (`i < *(u16 *)(a0 + 4)`, hoisted by loop.c with the game's `$v0` -> `$t3` copy) 49 -> 45.

**Open shapes found (the causes are readings):**

- **Loops on a field of a loaded pointer are not promoted to a register in the game.** func_00131D40's six angle wraps
  (on `*(char **)a0 + 0xA0..0xA8`) load and store the field on every pass (`lwc1; sub.s; c.lt.s; swc1` in the loop)
  and copy the pointer for each loop (`daddu $2, $3, $0`); our `loop.c` (`load_mems`, which runs when a loop has one
  exit, no call and an invariant address) keeps the field in `$f0` and stores it after the loop. `while`, `if` +
  `do`, an explicit temporary and `static inline` helpers (per pair and per loop) all give the promoted form (6 shapes).
  Same as func_00145D08 (2.17). func_0012EEB0's wraps (on an argument pointer) are promoted in the game too.
- **A constant scratchpad address hoisted out of a loop.** `*(s32 *)0x70000010 = 0;` and the read after the call are
  constant addresses in the game (gas macros: `lui $at` for the store, `lui $v0` for the load); `memory_address`
  forces constants into a register for CSE, and our loop.c hoists that register (`lui $s6, 0x7000; ori $s6, 0x10`
  before the loop), which spills the loop bound and renumbers the saved registers (func_00120EF0). A
  `#define SPR ((Spr *)0x70000000)` struct form does not change it.

**Parked (one-line causes; drafts in the dispatcher's notes folder `ER-086_081_073_drafts/`):**

| Function | Size | Cause |
|---|---|---|
| func_00113C30 | 676 | **placed by ER-089 (2.23)**; was: 45 words: which variable gets `$a0` / `$a1` in the head and the loop (the game copies the loaded id from `$v0` into `$a1` at the join); the 0x80 frame needs an unused `float buf[16]` (nothing touches `sp+0..0x3F`): dead code, needs a ruling |
| func_00113ED8 | 760 | **placed by ER-115 (2.27)**; re-parked by ER-089 (2.23): cross-jumped found tails; was: same head as func_00113C30 and the same untouched 0x40 bytes of stack (unused buffer, ruling); not drafted further |
| func_00131D40 | 732 | wrap loops not promoted in the game (above); head and table call: register choice |
| func_00126F00 | 476 | **placed by ER-094 (2.23)**: `extern const` vectors |
| func_00120EF0 | 1,348 | **ER-091 (2.23): 71 words, right size, re-parked**; was: 265 of 337 words: the scratchpad constant hoisted in ours (above) and the `D_001FF390` base CSE; structure, struct copy and data match |
| func_0011BB90 | 2,068 | **placed by ER-092 (2.23)**, by hand (no jump table in it after all); was: the m2c draft is 27 instructions short |
| func_00119FE0 | 6,636 | surveyed only: 925 m2c lines, 53 calls, a jump table, 36 literals; m2c's draft does not compile (`M2C_BREAK`) |

### 2.19 Delay slots filled by the assembler: ee-as r4 (ER-070, Sun 22 Nov 2026; ruling 3.12)

The second toolchain difference of 2.12 ("ee-as does not swap a use of an `mtc1`-written FPR into a delay slot") is
solved, and it was wider than `mtc1`: **our 1999 gas never swapped anything into a reorder-mode delay slot**, because
its "previous instruction is a sync" test reads `prev_pinfo && INSN_SYNC` (true for any instruction). ee-as r4
(`docs/toolchain.md` 4.2) tests the flag properly for the r5900 and never moves a `div.s` / `sqrt.s`. Evidence: notes
folder `ER-070_drafts/` (`ER-070_evidence.md`, `census_slots.py`).

| Lesson | Proved by |
|---|---|
| **A branch gcc leaves in `.set reorder` now gets the game's slot**: gas moves the instruction before it into the slot (`mtc1 $a2,$f12; jal; cvt.s.w $f12,$f12`), unless that instruction feeds the branch, sits after a gcc noreorder block (a filled slot, a divide trap), has a label on the branch, is an `mtc1` / `mfc1` / `c.cond` itself, or is a `div.s` / `sqrt.s`. Write plain C | func_00126F00 (masked compare: the slot after the first divide block matches; r3 gives a `nop` there and the rest one word late); whole ELF identical with all 685 placed functions |
| **Short loops are gcc's business, not gas's**: gcc pads loops of 6 instructions or fewer (`mips_r5900_lengthen_loops`) and the pad lands in the slot as a noreorder `nop`; a word-copy loop written `while (n-- != 0) *dst++ = *src++;` with `u32 n` gives the game's `bne; nop` with any assembler | func_00101238 (masked MATCH with r3 and r4; `s32 n` gives a different loop counter; not placed: `text_001000C0.c` is ER-060 part 2's this wave) |

Unconfirmed: which original tool kept `div.s` / `sqrt.s` out of slots (gas, as r4 does, or gcc wrapping its
`nop; nop; div.s` in `.set noreorder`); a later `div.s; Y; branch` site would tell.

### 2.20 Near misses and doubles in `text_00150878` (ER-080, Sun 22 Nov 2026; Confirmed by `make regress`, 51 functions)

ER-060's 48 same-size near misses: 16 matched as `m2c_bulk.py --plain` drafts when rerun in lists of 8
(`docs/matching_tools.md` 6.8), 31 more by hand; 1 parked. Also 3 of the 4 plain-form failures and 2 of ER-041's
seven (plain double C). Each lesson below is proved by the named functions placed and `make regress` green.

| Lesson | Proved by |
|---|---|
| **m2c's typed data base scales an index by the element size.** `(arg0 * 0x2C8) + &D_X` with `D_X` declared `s32` adds `arg0 * 0xB20` (the game's `addiu 0x2C8` became `0xB20`); likewise `&D_X + 0x252` and a loop pointer stepping `+= 4` on an `s32 *` (0x10 bytes). Declare the base `extern char D_X[];` and index bytes, or step an `s32 *` by 1 | func_00168358, func_0017CC28, func_001BFEB8, func_001BCF48, func_0017D6F0, func_001BBD40, func_001BCD68, func_001C1310 (`arg0 + 1` on an `s32 *`, and the `double` parameter of 2.15) |
| **A field copied as a float is `lwc1` / `swc1`**: m2c's `M2C_UNK32` copy compiled to `lw` / `sw`; `*(f32 *)(a0 + 0x84) = *(f32 *)(a0 + 0x4C);` gives the game's FPU copy | func_0015D3E8 |
| **An argument set in a tail `j`'s delay slot is one m2c leaves out** (6.1, 6.7): the wrapper passes a field, a constant or an offset pointer. Write it: `g(arg0, *(s32 *)(arg0 + 4))`, `g(0)`, `g((char *)arg0 + 0x48, arg1)` | func_0019C078, func_001C53D0, func_001C8920, func_001C8928 |
| **A callee's return type picks `$v0` or `$v1` for the caller's next value** (2.15, both ways): a `void` callee lets the caller reuse `$v0` right after the call; an `s32` callee makes it take `$v1`. Read the game's register and set the file's extern to match; the other callers in the file stay byte-identical (`make regress`) | `void func_00162CB8`: func_001AF2E8, func_0018C730, func_001B5E68, func_001B4BE8 (func_00183E90, func_001B8258 unchanged); `void func_001C9B18`: func_0017D290, func_001BF5B0, func_00161568; `s32 func_0012F9C0` in this file: func_00154BD0 (other files declare it `void`); `void func_00164220`: its caller's `extern s32 func_00164220();` was aligned to the definition |
| **Store order: try every order of the plain top-level stores.** Where the game loads constants in an order no source order up to 4 statements gives, a search over all orders (5 statements: 120 compiles, 6: 720, about 1 s each) found it; the pattern here was floats first, then words, then the byte | func_00189198 / func_001AED88, func_001835E0, func_001AEF90 / func_001B77B8 / func_001B42B0, func_001B4BE8, func_0015E628 / func_0015E988 (the `x34` / `x38` stores), func_001AB480 (`x4` before `x10`) |
| **A store moved before a computed value**: `*(s8 *)(a0 + 2) = 0;` first, the copy `x0C = x50` read straight off the struct, then the divisor `(f32)(x50 - x54)` gives the game's `sb` before `sw` (no order of the m2c draft's statements did) | func_00159830 |
| **Compare operand order picks the load order** (2.7, 2.10) again: `x6C > x74` | func_00153DA0 (also a pass-through `$a0`: `func_00131000(a0, func_001539A8)`) |
| **A three-way result as a nested `?:`** (`return (a > 0) ? 0 : ((a == 0) ? 1 : 2);`) gives the game's `bgtzl` with `li 0` in the likely slot and `movz` for the rest; `if / else if` chains give `bgtz` | func_00169480 |
| **A byte widened into a `u64` local, then shifted** (`u64 v = *(u8 *)(p + 0x87); return v >> 7;`) gives `lbu; dsrl`; the same as one cast expression gives `srl` (gcc narrows it) | func_0017D3E8 |
| **A negation before a call goes in its delay slot**: `r = -*(f32 *)(a0 + 0x30); *(f32 *)(a0 + 0x34) = r * f(ang);` gives the game's `jal f; neg.s`; `-x * f(ang)` negates after the call | func_00189810 |
| **A float scaled by a double constant**: `(float)(f(x) * 0.75)` gives `jal fptodp`, 0.75 built with `ori 0xFFA0; dsll32 14`, `jal dpmul`, `jal dptofp` (2.11); `fabs()` of a float difference compared with two float fields gives `fptodp`, `dpcmp`, `dpsub`, `dptofp` | func_00189810, func_0015BE00 (three shapes of the wrap and `fabs` match: a local, the field itself, or the field assigned first) |
| **An angle wrap as one expression**: `v -= (pi < v) ? 2pi : ((v < -pi) ? -2pi : 0.0f);` gives the game's `bc1t` / `bc1fl` pair with `sub.s` in the likely slot and `mtc1 $0` for the zero arm | func_0015BE00 |
| **Twins** (2.7): functions that differ only in one stored constant; once one matches, change the constant and compare the other | func_0015E628 / func_0015E988, func_00189198 / func_001AED88, func_001AEF90 / func_001B77B8 / func_001B42B0 |

**Seen in masked compares only (Unconfirmed until a placement proves them):**

- **A product kept for the next call**: `t = r * cos(b); a1[1] = a0[4] - t * sin(e);` (a temporary, as m2c wrote it) gives
  the game's `mul.s` right after the first call; the one-line product schedules it later (func_00160CE0's tail; the
  function is parked, below).
- **`d[1] = d[2] = 0.0f;`** (a chained assignment) gives `mtc1 $0` and two `swc1`, where two plain statements give
  `sw $0` twice (func_0015C0E0, parked).
- **A constant used twice in one expression** (`r * k + k` with `k = -0.7853982f`) is one `.lit4` load when `k` is a
  local assigned after the last call; assigned before a call it goes to a saved FPR (func_0015C0E0).
- **A function widened to pass its arguments through** (`func_00175DD0` to 5 arguments for `func_00167B20`;
  `func_001776D8` to `(s32, s32, char *)`, the `sprintf`-like `func_001E9D08` taking `$a2` untouched) keeps its own
  bytes (masked MATCH) (func_001C46C8's draft).

**Parked (one-line causes; drafts in the dispatcher's notes folder `ER-080_c/`; all four rechecked after main gained ee-as r4 (ruling 3.12): the same words differ):**

| Function | Cause |
|---|---|
| func_00150A30 | **Placed by ER-096** (2.25: `func_0012B890`'s float is its sixth parameter). Was: 2 words: the first `func_0012B890` call's delay slot takes `mov.s $f12, $f20` in ours, `daddu $t1, $0, $0` in the game (6 shapes: literal or local float, its place, `s32` return for func_00151330, early returns) |
| func_001C46C8 | **Placed by ER-095** (ruling 3.4 applied, 2.25). Was: matches (masked) only if `func_001776D8` returns `s32` to it while its own code ends in the void tail call `j func_00177858`: an `s32` definition with no `return` (ruling 3.4's shape) or an implicit `int` declaration across the original's source files; **needs LuciDin's ruling** (and `func_00175DD0` / `func_001776D8` widened as above) |
| func_00160CE0 | **Placed by ER-096** (2.25: two temporaries for the two `sin` products). Was: right size; `$f21` / `$f22` swapped between the wrapped angle and the hoisted `-3pi/4` loop constant (27 words; 9 shapes: loop forms, declaration order, `if` chain, the condition on either variable, the constant in a local) |
| func_0015C0E0 | ER-123 (2.28): 33 words, still parked. ER-096 (2.25): 36 words. Was: right size (268 instructions); about 46 words of scheduling in two blocks: the stores to `D_00248880` / `D_002488A0` around `func_001E9C70() % 45` and the setup before `func_0017D968` (vector copies of `D_00354D80` / `D_00354D70` as a 16-aligned struct, recipe 2.12's unconfirmed shape); store-order search over the six global stores (720 orders): no match, best 46 words (a regress ran during part of the search, so a few orders may not have compiled) |
| func_001BB2C8, func_001BB750 | **func_001BB750 placed by ER-123** (2.28); func_001BB2C8 down to 2 words (2.28, parked). ER-096 (2.25): func_001BB2C8 drafted by hand from the asm, 284 words; func_001BB750 not started. Was: not attempted in depth (289 / 300 instructions, integer loops with `/ 100` and `sqrtf`); the m2c drafts do not compile (`M2C_BREAK` from the divide trap); need a dedicated round |
| func_00178C60 | not attempted by ER-123 either (2.28: needs its own card and a `volatile` ruling); not attempted by ER-096 (time went to ER-098). Was: not attempted (0xE18 bytes, `sqrtf`; the card asks Opus, high) |

func_0017D290 and func_001BF5B0, left by ER-060 for `s128`, no longer need it: with today's context their drafts call
`func_001C9B18` with plain arguments (placed above); func_00161568's parse error is gone the same way. No typedef was
added.

### 2.21 The five small game files, round 2 (ER-083 + ER-085, Sun 22 Nov 2026; Confirmed by `make regress`, 19 functions)

19 functions placed in `text_00145120` (9), `text_0014A968` (2), `text_0014C840` (4), `text_0014D930` (2),
`text_0014E2E8` (6 with func_0014FBA0 from ER-083's small-list rerun), 2,280 bytes; `make regress` (-j4) green after
each batch and after merging main (ee-as r4). Branch `claude/recursing-hellman-1bca67`. Runs: `m2c_bulk.py --plain`
over `text_0014D930` / `text_0014E2E8` (label `er083`, 38 functions, 7 masked MATCH) and over the other three
(`er085`, 94 functions, 1 masked MATCH), then hand work on the same-size DIFFs of 3-8 words and the div / sqrt drafts.

| Lesson | Proved by |
|---|---|
| **The file's small `.sdata` floats can be `extern const`.** Where the game loads two `$gp` floats ahead of the stores that follow (or loads one once for two uses across a store), gcc with `-fno-strict-aliasing` reloads a plain `extern float` after each store; `extern const float` lets it keep the loads early, as the game does. Reading (Unconfirmed): the original declared these tuning values `const`. Not every one is: func_00147F78's `D_0035BD40` is reloaded on both paths in the game (a `const` one gives 80 more differing words) | func_00145918 (`D_0035BCF0`, `D_0035BCF4`), func_00147CF8 (`D_0035BD58`, `D_0035BD54`); seen also in func_00145FE8 / func_00147F78 (parked) |
| **A branch-likely whose slot sets `$a0` from the copy in `$s0` means the next callee reads `$a0`.** func_00149978's `bc1tl ...; daddu $a0, $s0` over `jal func_001302C8`: with `func_001302C8()` gcc fills the slot without `likely` (`$a0` dead on the fall-through); with `func_001302C8(arg0)` (the pass-through of 2.10, no move emitted) `$a0` is live on both paths and gcc uses `bc1tl`. Checked against `func_001302C8`'s asm: it reads `0($a0)` | func_00149978 |
| **Parameter order in a prototype changes the scheduling of a call's argument setup.** `func_0012B890`'s float argument declared before the last `int` (`(void **, s32, s32, s32, s16, f32, s32)`) gives func_00149DD8's `mov.s $f12` before the `jal` with `daddu $t1, $zero` in the slot; float last gives the reverse. EABI passes both the same way, so the other four calls in `text_00145120` compile to the same bytes either way (only that file's prototype was changed; `text_0014A968` / `text_0014C840` keep the old order) | func_00149DD8 |
| **A callee that returns `s32`** moves a later temporary from `$v0` to `$v1` (2.15 again): `func_0012F9C0` (`extern s32`), func_00149DD8's counter `lhu $v1` | func_00149DD8 |
| **m2c reads a constant in a tail `j`'s delay slot as a parameter** (the `$t1` of `j func_0012B890; addiu $t1, $zero, 1`): write the constant | func_0014C4D8, func_0014D3D0 |
| **A two-state `switch` (0, 1) as the game's compare tree (`==1`, `<2`, `!=0`) needs `case 2:` listed beside the default** (ruling 3.9, extended by LuciDin in the ER-085 session). Without it gcc tests `==0` first | func_0014CCD0 |
| **`p = table; for (i = 0; i < 3; i++) if (x == *p++) ...`** gives the game's loop (load before the loop, `bnel` with the next load in the likely slot); a `do` / `while` with `i++` first, an index or `p` stepped in the `for` header give other code | func_00146350 |
| **A `(void)` clone's callee given its real parameters** (2.13): `func_0012B238(void *, void *)` in `text_00145120` (its clones func_00147D88 / func_00149C10 pass both on) let func_001492D0 in; `func_0014FE10` is `s32 (void *, s32)` in `text_0014E2E8` as in the other files, its clone func_0014FE08 passes both on. Two of ER-061's clashes | func_001492D0, func_0014FE10 |
| **m2c's `arg0 + 0x4C` on a `void **` scales by 4** (0x130 in our build): the drafts' `void **` arguments need `(char *)arg0 + off` before any pointer arithmetic | func_0014CCD0, func_00145FE8 (draft) |

ER-069's 8-byte ELF shift (func_00144488 / func_0014F550 / func_00150350 placed together) did **not** reproduce: on main
3967dfb plus this branch the three placed together (func_00144488 temporarily) gave `make regress` PASS. ER-069
part 2 placed all three on main the same day (52a553d), so they count there, not here. Its cause stays unknown.

Parked by ER-085 (one-line causes; C in the dispatcher's notes folder `ER-085_c/`):

| Function | Cause |
|---|---|
| func_00147F78 | 4 words: FP register choice in the `else` arm's `*= D_0035BD3C` pair (the game holds the constant in `$f1`; ours in `$f0` / `$f2`); about 20 shapes incl. `const` (m2c's lost stack buffer is solved: `float m[16], v[4], w[4]`, 0xD0 frame) |
| func_00145FE8 | 5 words: the top block's `lhu 0x8C` / `sw 0x2C` order and the register of the `0x94 + 0xB0` sum; needs `const` floats and `func_0012B890`'s new order; 20 shapes |
| ~~func_00149EE8, func_0014A500~~ | placed by ER-100 (2.24: store order and ruling 3.14) |
| func_0014D3F8 | 13 words: the angle constant chosen by the `200.0f` tests lands in `$f1`, the game's in `$f0` (shared with `-200.0f`); 3 branch shapes, 4 expression shapes, all 120 declaration orders. ER-100 (`-dl` dump): our `r = a * pi / 180` chain is tied into one register (pi, product and the block-local `r` all in `$f20`); the game keeps pi in `$f1`, the product in `$f0`, only `r` in `$f20` |
| ~~func_001459A8, func_001485A8, func_0014A0B8, func_00146DF8, func_0014B528~~ | placed by ER-100 (2.24) |
| func_00146598 | still parked: ER-119 got it to 2 words (2.26) |

ER-100 rechecked func_00147F78 and func_00145FE8 (ee-as r4 changes nothing): both are local-alloc ties. func_00147F78:
`K * x98` ties the product to K's register, while the game's product is in x98's, and `x98 * K` drops K to `$f2` (108
shapes: const or not, a local, order, operand order, `*=`, temporaries, a pointer local, a static inline helper).
func_00145FE8: the `lhu 0x8C` / `lw 0x28` order before the first call (720 orders of the top six statements: no change)
and the `x94 + xB0` sum register. Stock flags tried as diagnostics on all three (-fno-regmove, -fno-schedule-insns(2),
-fno-gcse, -fno-caller-saves, -fno-expensive-optimizations, -fno-peephole, -fno-cse-follow-jumps, -fno-thread-jumps,
-fno-strength-reduce): none fixes any of them.

### 2.22 ER-031's five files, round 2 (ER-078 + ER-087, Sun 22 Nov 2026; Confirmed by `make regress`, 11 functions)

11 placed in `text_001388E8` / `0013C510` / `0013DBC0`, 2,820 bytes, commits `01a795c`, `1351c2c`, `cd3a1ec`, `053ae27`,
`b071688` (branch `claude/wizardly-allen-6330f3`; `make regress` (-j4) green after each and after merging main with
ee-as r4): 3 plain MATCHes of `m2c_bulk.py --plain --tag ER-078` over the five files (148 asm functions, label
`er078`, 3 masked MATCH), 5 of ER-072's 9 near misses, and the jump-table functions func_0013C750, func_0013FCF8,
func_0013DF90 (6, 7 and 8 of the 11). Jump tables still open: func_0013AA28, func_0013BD90, func_0013D088.

| Lesson | Proved by |
|---|---|
| **An explicit `return;` as a void function's last statement gives a sibling `j` for the call before it.** func_0013C750's last case ends `if (x == 0x14) { func_0012F9C0(arg0, 4); return; } } break; }`; the game has the epilogue and `j func_0012F9C0`, then a second epilogue for the `break` paths. Without a statement after the `switch` the call block falls into the epilogue (`jal` + one shared epilogue, 3 words short); with `return;` after the `switch` gcc keeps a jump to the return label after the call and makes it a sibling call. ER-072's probe reading (2.13: "needs a jump to the epilogue after the call") is this | func_0013C750 |
| **A negated global taken into a local first** (`n = -D_0035BB68;` before a run of copies, `... = n;` last) gives the game's early `lwc1` / `neg.s` with the constant kept in its own FPR while `$f0` / `$f1` carry the copies; then the five stores in game order came from a search over all 120 orders (2.20). The same search without the local (ER-069, 120 orders) had not matched | func_0013FCF8 (case 0) |
| **A value-returning function whose `$v0` is live at the epilogue** fills a `beqz`'s slot from the epilogue (`ld $ra`) where a void one fills it from the fall-through: func_0013D088 is `s32` (the file's prototype already said so) and has no `return` value (ruling 3.4's shape); 4 bytes short as `void`, right size as `s32` | func_0013D088 (masked compare only, parked below) |
| **A callee that returns `s32` stops a cross-jump** (2.15, 2.20 again): with `void func_0013C468` the game's `li $v1, 5; sh $0, 0x6C; sh $v1, 0x6E` tail became `$v0` in ours and was merged with the other cases' `6C = 0; 6E = N` tails; `s32` gives `$v1` and the game's layout | func_0013BD90 (masked compare of the tails, parked below) |
| **m2c drops a constant in a tail `j`'s slot** (2.20) also in two-instruction wrappers: `j func_0012FB48; addiu $a1, 1` is `func_0012FB48(arg0, 1)` with the pass-through `arg0` (2.10); the callee's file prototype was `(void)` and the already placed clone `func_0013F280` passes both on (2.13) | func_0013E758, func_001449D0, func_0013F258 |

**Seen in probes and diagnostics only (Unconfirmed; evidence in the dispatcher's notes `ER-078_087_c/README.md`):**

- **Struct field access lets a global scalar's load move above a store through the struct.** A scratch probe with the
  build's flags: `*(float *)(a0 + 0x30) = ...; ... G ...` keeps `G`'s load after the store; the same with `S *a0;
  a0->x30` loads `G` first (gcc 2.95's fixed-scalar / varying-struct rule in `alias.c`; reading). This is likely why
  main's 2.21 `extern const float` works, and suggests the original source used struct types.
- **Pointer arguments scheduled as if they did not alias.** func_0013D088's last 4-12 words (an `arg1 + 0xA4` float
  against `arg0` floats) match only with `-fargument-noalias(-global)`; func_0013F288 matches and func_0013AA28 goes
  9 -> 5 words with `-fstrict-aliasing`, which ruling 3.6's func_0013DBC0 forbids. The whole ELF built with
  `-fargument-noalias` is 8 bytes longer (first moved function func_00144308; not traced). Diagnostics only, nothing
  adopted.

**Parked (one-line causes; C in the dispatcher's notes folder `ER-078_087_c/`):**

| Function | Cause |
|---|---|
| func_0013D088 | right size; 12 words, all `arg1 + 0xA4` accesses ordered against `arg0`'s fields (above); `extern const` floats fixed the `$gp` loads |
| func_0013AA28 | 9 words: case 2's `+0x30 + D_0035BA94` sum register (`$f2` in the game), two type-based reorders (above); `const` only for D_0035BA9C / D_0035BAA0 (all five const is worse); still needs `_anon_takeovers` widened for `D_00354800` (16 bytes) |
| func_0013BD90 | tails fixed (`s32 func_0013C468`); about 150 words: `p` in `$v1` (game `$v0`), case 2's vector setup and `cvt` order, case 5's `sqrtf` schedule, 8 bytes short (separate `p` locals: 125 words, 4 short) |
| func_001431A0 | 2 words: the tail's `daddu $a0, $s1` before `sw $0, 0x2C($s1)` in the game (8 shapes); block 1 needs `func_00131000` as `s32` (2.18) |
| func_0013F288 | 2 words: `addiu $v0, 0xA0` before `lw 0xE4` in the game (7 shapes; matches with the `-fstrict-aliasing` diagnostic) |
| func_00145D08 | not re-tried: the open shape of 2.17 / 2.18 (loops on a field of a loaded pointer are not promoted in the game) |

### 2.23 `text_001000C0`: register choice by variable reuse, const vectors, struct globals (ER-089 + ER-094 + ER-091 + ER-092, Sun 22 Nov 2026; Confirmed by `make regress`, 3 functions)

3 placed, 3,220 bytes, commits `6633092` (func_00113C30, 676), `bfe48ae` (func_00126F00, 476), `1b84924`
(func_0011BB90, 2,068); branch `er-089-text-001000c0`, `make regress` (-j4) green after each. First use of ruling
3.13 (func_00113C30's `float buf[16]`, commented; without it the frame is 0x40 and 129 words differ). Nothing else
compiler-steering. Drafts of the parked two and the two helper scripts: notes folder `ER-089_092_drafts/`.

| Lesson | Proved by |
|---|---|
| **One variable reused for two jobs sets the register of both.** func_00113C30's part index (`id`) and its later vertex-scan pointer share `$a1` in the game; written as one variable (`id = (s32)(q + 0x80)`, `*(float *)(id + 0x30)`, `id += 0x40`), its allocno spans the scan, conflicts with the `$a0` temporary there and inherits the call argument's `$a1` preference; the head, the loop's copy and the skip loop's `-1` / mask registers all follow (17 -> 7 words). Likewise the looked-up id read into `w` (the masked-word temporary) and the skip loop's quotient into `j` (the scan counter) closed the last 7. Separate variables for the head and the loop are much worse (71-87 words). Reading (Unconfirmed): the original reused a few general temporaries | func_00113C30 |
| **A struct's array member indexed by a variable is addressed base first** (`t->tab[t->sel]`: `addu $a0, $a0(t), $v0`), where any pointer-arithmetic form (`*(char **)(t + sel * 4 + 0x80)`, `((char **)(t + 0x80))[sel]`) gives index first; the address tie then moves the following registers (45 -> 33 words). Layout `Anm` Unofficial | func_00113C30 |
| **A sum written integer-first ties to the product**: `rec = (char *)(id * 0x50 + *(s32 *)(a0 + 8));` gives the game's `addu $v0, $v0(product), $v1`; `ptr + id * 0x50` in any pointer form ties to the base | func_00113C30 |
| **Load order of a record's fields picks which one reuses the record's register**: the last field read takes it (`m`, `cnt`, `p` in func_00113C30; func_00113ED8's twin wants `m`, `p`, `cnt`) | func_00113C30 (6 orders tried) |
| **`.rodata` vectors as `extern const`**: func_00126F00's three 16-byte `Vec4` copies (`D_003543C0` / `D_003543B0` / `D_00354390`, `INCLUDE_RODATA` in the file) issue each copy's `%lo` `ld` before its `+8` `ld` only when declared `const`; plain `extern Vec4` swaps two of the three pairs (4 words). Statement and declaration orders (320 tried) do not | func_00126F00 |
| **A global work area as a struct, not char-array offsets**: `D_001FF390.xB0` keeps the symbol as the base (`addiu $a0, $s4, %lo(D_001FF390)` per block, `%hi` in a saved register) where `*(s32 *)(D_001FF390 + 0xB0)` folds symbol + offset into one constant and bases later fields on it (`D+0x88`, offsets -8 / 0x3C) (532 -> 93 words). A declared array indexed by constants (`D_001FF340[5]`) behaves the same. Layout `G390` Unofficial (fields named by offset; `fc` and `bits` are func_00120EF0's) | func_0011BB90 |
| **A global read once into a local when the game keeps it across stores**: the pad word `D_0035B828` loaded once and tested eleven times from `$a2`; read directly, gcc reloads it after every store through `D_001FF390` | func_0011BB90 |
| **A counter stepped in place** (`D.xB0--; if (D.xB0 < -25) D.xB0 = -25;`, or `--D.xB0 < -25`, or `D.xB0 = D.xB0 - 1`): one register through load, add, store and test; a temporary (`x = D.xB0 - 1; D.xB0 = x;`) gives a second register (93 -> 9 words) | func_0011BB90 (all eight clamp blocks) |
| **Store order search again** (2.20): the six stores after func_0011BB90's `memset` matched in 3 of 720 orders (`v[3]`, `xC4`, `x80`, `x88`, `x90`, `x94`; the game's emitted order is the scheduler's); the three final negations in index order | func_0011BB90 |
| **A loop-local pointer to a global** (`g = &D_001FF390;` at the bit test, then `g->bits[i / 8]`, `g->fc`): the game materializes the base inside the loop instead of reusing the head's; and a function pointer read into a local before a store gives the game's `lw 0xD0` before the `sh` (265 -> 71 words, right size) | func_00120EF0 (masked; parked, below) |

**Parked (one-line causes; drafts in the notes folder `ER-089_092_drafts/`):**

| Function | Size | Cause |
|---|---|---|
| func_00113ED8 | 760 | **placed by ER-115 (2.27)**: `goto found;` to a call after the loop (sibcall USE stops the cross-jump), record pointer in `k`; was: right instructions, 4 words short: our post-reload cross-jumping merges the two `func_001C9F60` found tails (both blocks end `set $a1; call` at jump2, `-dS -dJ` dumps), the game keeps both; plus one `nop` before the game's loop top. 8 found-path forms tried |
| func_00120EF0 | 1,348 | **re-parked by ER-116 (2.27)**, 72 words with three regions fixed; was: right size, 71 words: the store block after the struct copy (the game re-materializes `&D_002029D0` / `&D_00202A50` / `&D_001FF8B0` more often than our CSE; about 700 statement orders searched, struct forms worse) and a `rec` / bit-index saved-register swap |

### 2.24 The five small game files, round 3 (ER-100, Sun 22 Nov 2026; Confirmed by `make regress`, 31 functions)

31 placed in `text_00145120` (15), `text_0014A968` (6), `text_0014C840` (2), `text_0014D930` (1) and `text_0014E2E8`
(7), 7,816 bytes. `make regress` (-j4) was green after each of five batches and after merging main. Branch
`er-100-five-files`, worktree `C:\rs\wt\er100`. A `m2c_bulk.py --plain --tag ER-100` run over the five files (label
`er100`, 111 asm functions) gave 0 masked MATCH; the 25 same-size DIFFs rerun in lists of 8 also gave 0. Then by hand:
19 here, and 12 by two helper agents working from the drafts (each one checked here before it was placed). Notes and
the C of the parked ones are in the dispatcher's notes, `ER-100_notes.md` and `ER-100_c/`.

| Lesson | Proved by |
|---|---|
| **The compiler has Cygnus' early sibling-call pass** (`gcc/sibcall.c`, `calls.c` "CYGNUS LOCAL sibcall/law"). It refuses every tail `j` in a function whose frame has a stack local (`frame_offset`), that uses ADDRESSOF (a local or parameter whose address is taken, later put back in a register), alloca or varargs. Per call, it also refuses when the call sits in a loop statement or is not followed by the exit. So a game function with `jal f` + epilogue, where plain C gives `j f`, had one of these in its source. Where the frame shows no local, an address-taken variable is the only blocker found: `(void)&arg0;` changes nothing else (ruling 3.14). A narrow local (`u16`) given `&` went to the stack instead: take a parameter's address | func_00149EE8, func_0014A500, func_0014A0B8, func_0014B528, func_00148BE0, func_00148D28, func_0014C110, func_001485A8 |
| **An unused local that sizes the frame also stops the sibling call** (ruling 3.13, and `frame_offset` above): func_0014E258's 16 spare bytes, func_001490B0's 0x20, func_0014FBD0's 0x40 | func_0014E258, func_001490B0, func_0014FBD0 |
| **A callee's real return type, read from its asm, changes the caller's FP register choice.** `func_0012E820` ends in `mov.s $f0, $f4`, so it returns `float`. Declared so in `text_00145120`, it gave func_001459A8's float-to-int conversion (`cvt.w.s` into the s16 argument of `func_0012B7C0`) the game's `$f1` / `$f0` pair; the file's other callers kept their bytes | func_001459A8 |
| **A float passed to an `s16` parameter** compiles to the game's `cvt.w.s; mfc1; sll 16; sra 16`; no cast is needed | func_001459A8 (`D_0035BCB0` to `func_0012B7C0`) |
| **A callee's real parameters, read from its asm**: `func_0012FA18` reads `$a1`, so it is `(void *, s32, float)`. The ER-014 wrapper func_00150810 then passes its second argument on (same bytes, recipes 2.10) | func_0014F728 |
| **An `s8` parameter is re-extended on entry; an `s16` argument is narrowed at the call.** func_0014F298's `sll 24; sra 24` at the top and `sll 16; sra 16` before the `jal` come from `(char *, s8, s32, s32, f32)`, with the `s32` passed to `func_0012B890`'s `s16` parameter | func_0014F298, func_0014F2E0 |
| **A pre-decrement test of a `u16` field**: `if ((s16)(*(u16 *)(p + 0x74) -= 1) < 0)` gives the game's `sll 16; bgez`, with the unextended value stored | func_00148BE0 |
| **A 64-bit flag set needs a 64-bit constant**: `*(u64 *)(p + 0x70) \|= (u64)0x8000 << 34` gives `ori; dsll32 2`, where an `int` shift overflows to `lui 2` | func_00148BE0 |
| **One local per branch** where the game uses `$v1` in one arm and `$v0` in the other: a shared local becomes one global pseudo | func_0014C560 |
| **Store order by search** again: permutations of up to 7 statements, or two stores inserted anywhere among the rest | func_0014A0B8, func_00149EE8 / func_0014A500, func_0014B528, func_00149C80, func_0014C968, func_0014A968, func_0014E2E8, func_001464A8, func_00146FE0, func_00148F40 |
| **A float parameter first in a callee prototype** changes the argument load order (recipes 2.21 again): `func_0012DA68` as `(void *, f32, s32, s32, f32)` in `text_0014A968` | func_0014B528 |
| **`case N:` beside the default** (ruling 3.9): func_001485A8's 5-entry jump table ends with `case 4: default:`, and func_0014B528's compare tree needs `case 2:` | func_001485A8, func_0014B528 |

**Parked by ER-100's helper (needs a ruling; Unconfirmed reading):** func_0014C840 (26 words) and func_0014D930 (18
words) match only with `-fstrict-aliasing`, as a diagnostic (ruling 3.6 forbids it). The game moves the `*arg0` load
above stores of other types and keeps those stores in source order; 120 orders and more were tried for each. This is
the same family as 2.22's func_0013F288 / func_0013AA28. Not attempted: func_00146598, func_00145578 (79 words; both taken by ER-119, 2.26) and
the 86 drafts whose size differs.

### 2.25 `text_00150878` round: whole-file small-list rerun, field order, ruling 3.4 (ER-095 + ER-096 + ER-098, Sun 22 Nov 2026; Confirmed by `make regress`, 71 functions)

71 functions placed in `text_00150878`, 8,284 bytes, `make regress` (-j4) green after each of three batches and
after merging main. Branch `er-095-text-00150878` (worktree `C:\rs\wt\er095`), commits `6d882d1` (ER-095),
`00617c8` (58), `db5bf40` (12). ER-095: func_001C46C8 (1). ER-096: func_00150A30, func_00160CE0 (2). ER-098: 68,
from a rerun of the whole file in lists of 8 (`docs/matching_tools.md` 6.11): 17 plain MATCHes, func_001C4750, and
50 by hand from the drafts (all same-size DIFFs of 1-5 words and most of 6-8; 6 from the 9+ band). Drafts of the
ones left are in the dispatcher's notes folder `ER-095_096_098_c/`.

| Lesson | Proved by |
|---|---|
| **Fields are stored in ascending offset order.** m2c drafts list the stores of a setter in the game's schedule order; the C that compiles to that schedule writes them in offset order (bytes and words alike), and the scheduler reorders them. Try ascending first, before any order search: a script that sorts each run of consecutive `M2C_FIELD(...) = ...;` stores of a draft by offset and recompiles found 6 matches outright (notes `asc_sort.py`; a tool follow-up). Where a store reads a field written in the run, keep that pair's order (func_00181210's `x58` first) | func_00153FD0, func_001522B8, func_0015A440, func_00187F78, func_0018F170, func_001882B8, func_0018D9C8, func_00190688, func_001918B8, func_001984E8, func_00181210, func_00199660, func_001535A8, func_001BFF20, func_001898F0, func_001AC938, func_001AD8F0, func_001B1198, func_001B3440, func_001B5298 |
| **Stores of a function's float parameters go in parameter order** (`p+0x10 = a; +0x14 = b; +0x18 = c; +8 = d` for `(s32, f32 a, b, c, d)`), not offset order | func_001652E8 |
| **A global's address kept in a register (`lui; addiu %lo`, then plain offsets) needs a base-pointer local**: `char *g = (char *)D_X; g[0x164] = 1;` or `s32 *p = D_X; p[0x68] = 0;`. With the symbol written in each expression gcc folds `%lo` plus the offset into each access | func_001663D0, func_00163790, func_001BE948, func_001BE218, func_001BFF20 |
| **gcc turns each simple `if (c) x = k;` into a `movz` / `movn`**; a run of the game's `movz` that all target one register is a run of independent `if`s, not an `else if` chain (branches, with a `movz` only at the end) or a nested `?:`. func_001BB2C8's selector: `if (m < 2 \|\| m > 21) sel = 5; if (m >= 2 && m <= 5) sel = 3; ...` gives the game's chain | func_001BB2C8 (draft), func_001BB750 (placed by ER-123, 2.28) |
| **`func_0012B890`'s float is its sixth parameter** (2.21's lesson, now in this file too): `(void **, s32, s32, s32, s16, f32, s32)`; with the float last, the first call's delay slot differs. The file's four other callers compile to the same bytes with their last two arguments swapped | func_00150A30 (func_00150988, func_001512E8, func_00156F28, func_00156F80 unchanged) |
| **One temporary per product**: `t = r * sin(b); ... t2 = r * sin(b);` gives the game's FPR allocation where reusing `t` swaps `$f21` / `$f22` between the wrapped angle and the hoisted loop constant (ER-080's 9 shapes all reused `t`). Same in func_001602A0 | func_00160CE0, func_001602A0 |
| **A chained assignment `a = b = x;`** stores `b` before `a` with one load (`x30 = x34 = x38;`) | func_00159578 |
| **One pointer local per branch** (`char *p` in the `if`, `char *q` in the `else`) gives the game's `$v1` / `$v0`; one shared local takes `$a1` | func_00154710 |
| **A loop strength-reduced from the top**: the game's pointer starting at `base + 0x10E`, stepping `-2`, with a counter from 63, is `for (i = 63; i >= 0; i--) arr[i] = 0;` with `arr` at `+0x90` (a pointer local also matches) | func_001663D0 |
| **A callee's parameter type narrows the caller's load**: `lhu` of an `s32` global as the first argument (`lw` of the same global later) means the parameter is `u16`: `func_0012F5F8(u16, ...)`; the `0xFFFF` store is `*(u16 *)p = 0xFFFF` (`ori`, not `addiu -1`) | func_00158130 |
| **A parameter converted at the call** (`sll` / `sra` after the branch, not on entry) is declared wider than the callee's: func_001512A0's third parameter is `s32`, passed on to `func_0012B890`'s `s16`. Its extern in the file changed (`s32` return as well: the `void` form loses the frame) | func_001512A0 |
| **`s16 *t = ...; t += i; return t[0xC8];`** gives `addu $v0, $v0, $a1` (the sum in the base register); every one-expression index form gives `addu $a1, $a1, $v0` | func_0017D788 |
| **A 16-byte constant copied to the stack and passed by address** is a struct copy (`ld` / `sd`): `typedef struct { f32 v[4]; } __attribute__((aligned(16))) Vec4A; Vec4A v = D_00354D70; f(x, &v);` (a two-`s64` struct matches too; the float vector is the reading kept: Unconfirmed). First aligned type in the repository | func_00158DC0 |
| **0x80 bytes of unused frame** with nothing stored there (`addiu $sp, -0x90`, `$ra` at `0x80`): ruling 3.13's frame-sizing local, `char buf[0x80];` with the ruling named in a comment | func_001668E0, func_001668A8 |
| **A 0x80-byte sprintf buffer** where m2c declared `sp[64]` (0x100): size the buffer from the game's frame | func_00165388 |
| **`if (f() == 0) { return ...; } return 0;`** gives the game's `bnel` with the zero in the likely slot (the `!= 0` early return gives `bnez`) | func_001C8758 (still 2 words, notes), func_001C8700 (notes) |
| **A `switch` that `break`s** gives a tail `j` in each case; `return` in the second case gives `jal` | func_00176388 |
| Recipes 2.10 / 2.20 again: byte strides (`char` bases, `p += 0xA0`), a pass-through `arg0` (func_00154050, with `func_00131140` made `s32` in the file), an unused first parameter (func_001C8C80's `(void *, char *)`), a field copied as `f32`, compare order (func_00176818: `arg0[7] <= arg0[6]`), twins (func_00199660 / func_00181210, func_001984E8 / func_001918B8), pointer returns written `char* func_X(` (2.13) | func_0017D0B8, func_0017CD20, func_001C2400, func_001C3030, func_001C3060, func_001602A0, func_001604C8 / func_0015DDB8, func_0017CA10, func_0017D3B8, func_0017D608, func_00150878 |

**ER-095 (ruling 3.4 applied).** `s32 func_001776D8(s32, s32, char *)` falls off the end after `func_00177858(...)`
(the sibling `j`), with the `PORT NOTE` 3.4 asks for; its third argument passes through to `func_001E9D08` in
`$a2`. `func_00175DD0` widened to pass five arguments on to `func_00167B20` (same bytes). func_001C46C8 then
matches: the `s32` callee makes it read `D_0035B828` into `$v1`.

**File declarations changed** (each proven by `make regress`): `func_0012B890` (float sixth; five callers' last two
arguments swapped), `func_00131140` `s32`, `func_001512A0` `s32 (void **, s8, s32, f32)`, `func_001C4750`
`(char *, s32, s32)` (its five callers pass the message strings without the `(s32)` cast), `func_00154050 (char *)`.

**Left (same size; drafts in notes `ER-095_096_098_c/`):** func_001C6F90 (3 words), func_001C8758 (2),
func_001C4838 (4), func_001C8700 (5), func_0016E368, func_0017D6B8, func_0018B8A8, func_0018C9E8, func_001540C0
(7), func_001636A0 (8, and a 0x10-byte frame difference), func_001C56F0 (calls three functions the file defines
`(void)` with an argument), func_001C6880 / func_001C6B18 / func_001C6BD8 (64-bit packing), func_001A7CA8,
func_001B34F8, func_001BE9C0, func_001C20B0, func_001C8600 (not tried); about 130 of 9+ words (only the sorter
tried them).

### 2.26 The five small game files, round 4 (ER-112 + ER-119, Sun 22 Nov 2026; Confirmed by `make regress`, 6 functions)

6 placed, 656 bytes, branch `er-119-five-files` (worktree `C:\rs\wt\er119`), commits `87fc92e` (ER-112: func_00148BB0,
func_0014C630, func_0014D028, func_0014DEA0, func_0014F5F8) and `f69bb83` (ER-119: func_00145578); `make regress`
(-j4) green after each and after merging main. ER-112's rerun (`m2c_bulk.py --plain --tag ER-112`, label `er112`, 5
functions) gave 3 masked MATCHes; func_0014C630 / func_0014D028 came out one instruction short this time (the run
harvested no callee signatures, so `func_0012C3E8` lost its pass-through `$a0`), and all three plain forms clashed
with their file's own declarations. All five were written by hand into each file's declarations from the drafts (ER-084's
drafts in the notes `ER-084_c/` agree) and each matched first try. func_0014DEA0 needed its file's
`extern void func_0014DEA0(void)` changed to `(char *)`.

| Lesson | Proved by |
|---|---|
| **A callee called with two arguments by one caller and one by another takes both** (recipes 2.10 / 2.24 again): func_00145578 sets `$a1` from its own `arg1` before each `jal func_00145F70`, which never reads `$a1`. `func_00145F70(void **, void *)` (second parameter unused) lets it compile; its other caller func_00145208 passes its own second parameter through (same bytes, no move emitted) | func_00145578, func_00145208, func_00145F70 |
| **The store order the game emits was the source order here**, not ascending offsets (2.25): case 1 `x30, x6C, x6E, xE`; the counter block `x10 -= 1, x30, x74, xD, x6E, x6C, xE`. Ascending was not tried first; the emitted order matched at once except the `x30` pair below | func_00145578 |
| **`extern const` again** (2.21): `D_0035BCEC` is loaded ahead of the `x10` decrement's store in the game; plain, gcc keeps the load after it (`x30` written first gives the load early but swaps the two stores) | func_00145578 |
| **A limit written out in each comparison** (as a `DEG2RAD(x)`-style macro would expand: `if (w < -(D * 3.1415927f / 180.0f)) w = -(...); else if (... < w) w = ...;`) gives the game's registers where a `lim` local puts the quotient in a fresh register (`$f3` against the game's `$f1`). Reading (Unconfirmed): the original used such a macro | func_00146598 (success branch; parked, below) |

**Parked: func_00146598** (0x2EC, right size, 2 words): one load pair in each of two blocks. The game loads the pi
literal before `D_0035BD28` in the `x6E == 0` block and after it in the clamp `else` block. In our build both orders
come from sched2 and both follow `D_0035BD28`'s const-ness: `const` (needed for the first block, where the loads of
`D_0035BD28` / `D_0035BD2C` must move above the `x10` store) gives pi first in both blocks; plain gives
`D_0035BD28` first in both. The sched2 dump shows why: with a plain extern the load gets an anti-dependence on the
later `x30` store, which raises its priority over the pi load (whose `$f1` is reused by the `D_0035BD24` load).
Tried: const on either or both globals, struct-typed stores in the first block (recipes 2.22's fixed-scalar rule,
with and without const), argument and statement orders, locals, 13 forms of the `else` block. C and the variant
script: notes `ER-119_c/`.

### 2.27 `text_001000C0` round 2: cross-jumped tails, ER-084's plain matches (ER-112 + ER-115 + ER-116, Sun 22 Nov 2026; Confirmed by `make regress`)

ER-112's share in this file: the 17 plain MATCHes of ER-084's A/B run (notes `ER-084_c/`), each rechecked with
`cmpfunc` on this branch and placed (944 bytes, commit `9527f54`). A fresh `m2c_bulk.py --plain --list` run on the 17
names harvested no signatures (`0 harvested signatures`; the list form does not harvest the file's callees) and gave 9
MATCHes, so the ER-084 drafts were used. Three needed the file's declarations aligned: `func_0012B5F8`'s third
parameter widened from `u8` to `s32` (func_0012F808 passes its own `arg1` there; func_0012F838 / func_0012F870 still
match), `func_00134070` declared with one parameter (its asm never reads `$a1`), `func_0012F8A0` declared `s32` with an
`s32` second parameter (it returns `func_0012B6B0`'s value; `src/text_0013DBC0.c` still declares the old prototype,
same code there). func_00113ED8 (ER-115, 760 bytes, commit `8196aa8`) by hand. ruling 3.13's `buf[16]` as in
func_00113C30; nothing else compiler-steering.

| Lesson | Proved by |
|---|---|
| **A call in tail position outside any loop keeps a `(use (const_int 0))` after it** when Cygnus' sibling-call pass refuses the sibcall (here for the 3.13 frame; 2.24), and that USE stops jump2's cross-jumping (`jump.c` `find_cross_jump` compares insn codes: USE against CALL_INSN fails). A call inside a loop statement is never a sibcall candidate and gets no USE, so two identical `call; jump epilogue` tails inside loops are merged (only the call and the `$a1` add must match). The game's func_00113ED8 keeps both `func_001C9F60` found tails: its first found path is `goto found;` to the call after its do-while (`found: func_001C9F60(...); return;`). The game's lone `nop` before the loop top was the compiler's own `.align 3` on the loop label, back in place once the 3 words of the tail returned | func_00113ED8 (func_00113C30's end call shows the same USE in `-dR` dumps) |
| **Order of passes after reload**: sched2 runs before jump2's cross-jump (`toplev.c`), so cross-jump sees the final instruction order without delay slots; a jump2 merge cannot be undone by statement order alone | func_00113ED8 (`-dS -dg -dJ -dR` dumps) |
| **Global allocation inherits preferences through deaths** (`global.c` `expand_preferences`): a pseudo set in the insn where another dies takes over its hard-register preferences if they do not conflict. In func_00113ED8 the part index (`$a1`, from `func_001C9B18(a2, id)`) passes `$a1` to the product, to the record pointer, then to whichever record field load carries the pointer's death after sched1, so the loop count took `$a1` (game `$a0`) or the loads came in the wrong order (sched1 prefers the load with the death, `INSN_REG_WEIGHT`). Holding the record pointer in `k` (the vertex count, `$v0` in the game too) makes it conflict with the count, so the preference stops: 2 words to MATCH. Recipes 2.23's variable reuse again | func_00113ED8 |
| **ER-084's drafts and the file's declarations**: an address-taking extern (`void f(s8 *, s32)` for a callback) or a narrowed callee parameter blocks a plain MATCH; read the callee's asm for the real parameters and return | func_0012CAD8, func_00134070, func_0012F8A0, func_0012F808 |

**Seen only in masked compares (Unconfirmed until a placement proves them), func_00120EF0:** call arguments read
from a record before a store to the scratchpad must be read into locals first (`w1 = ...0x118; w0 = ...0x114;
SPR->hit = 0; f(..., w0, w1, len)`): with `-fno-strict-aliasing` the store blocks the loads otherwise, and this also
gives `obj` the game's `$a1`; `byte = &(g = &D_001FF390)->bits[i / 8];` gives the game's `g + 0x140` first; the
`x219` read first in the compare gives the game's `bnel` operand order.

**Parked (one-line cause; drafts and notes in the notes folder `ER-115_116_c/`):**

| Function | Size | Cause |
|---|---|---|
| func_00120EF0 | 1,348 | right size, 72 words (the three fixes above in): `rec` / `bit` saved registers swap (global-alloc priority `bit` 2·6/19 = 0.632 against `rec` 5·47/358 = 0.656: the `w0` / `w1` copies lengthen `bit`'s range); the `&D_001FF890` `%hi` temp and the `lhu` / `lb` temps take `$v1` / `$v0` swapped; the store block after the struct copy, where the game re-materializes `%hi` of `D_002029D0` / `D_00202A50` / `D_001FF8B0` (reading: reload rematerializing unallocated HIGH pseudos, so register pressure; 1,500 statement orders plateau at 71-73 words, struct forms 115+) |

### 2.28 `text_00150878` hard ones: base pointers in loop bodies, `abs()`, one reused variable (ER-123, Sun 22 Nov 2026; Confirmed by `make regress`, 1 function)

func_001BB750 placed (1,200 bytes, commit `7e34c0d`), by hand from the asm; its twin func_001BB2C8 is down to 2 words
and func_0015C0E0 to 33 (parked below). Drafts, the register-blind diff (`sdiff2.py`), the order hill-climber
(`climb.py`) and the permutation search (`perm.py`) are in the dispatcher's notes folder `ER-123_c/` (our C only).
gcc's own pass dumps settled most of it: `gcc.exe -B<GCC_DIR>/ <CFLAGS> -I<include> -S -da file.c` writes one RTL
dump per pass (`.gcse`, `.loop`, `.cse2`, `.sched`, `.sched2`, ...); `-dS -dR -fsched-verbose-6` adds each scheduled
block's dependences, priorities and ready lists (the option is spelled with a dash, not `=`).

| Lesson | Proved by |
|---|---|
| **A global's base pointer assigned inside each loop body** (`for (...) { p = D_002029D0; q = D_00202A50; ... }`) gives the game's `lui $s5` / `$s6` / `$s7` at entry with each loop's `addiu %lo` from them. Assigned before the loop, the pointer keeps its own `%hi` and gcse inserts a second one at the end of the block for the later uses, so cse2 leaves a copy (`lui $v0; daddu $s7, $v0`) that cannot move above the first call; inside the loop, loop.c hoists the assignment after gcse, one `%hi` register remains, and it is scheduled above `jal func_001BBD40` (its value crosses calls). Function-wide base locals (2.25) hold whole addresses instead. The pointer is live code, not a steering trick (Unconfirmed as the original spelling; a loop-scope local reads the same) | func_001BB750 (func_001BB2C8's draft: prologue exact) |
| **`abs()` is gcc's `move; bgez; negu`** (expand_abs); `if (x < 0) x = -x;` becomes `slt` / `movz`. Inside a branch, `if (v < 0) { neg = 1; v = abs(v); }` gives the game's `bgez v; ...; addiu neg, 1; bltzl v; negu v`; two plain `if`s become two `movz` | func_001BB750 |
| **One variable for three values** (the scaled factor `w = w * k * 9 / 500 + k`, the loop-3 result `w = (s32)sqrtf(...) << 4`, the loop-4 sum) gives the game's `$a2` throughout; separate variables put `w` in `$v1` (probably because the shared pseudo then conflicts with loop 4's `a[i]` temporary in `$v1`: a reading, not traced in the dumps) | func_001BB750 |
| **A pointer local assigned twice loses its alias base** (alias.c forgets the base of a register set twice with unrelated values), so a load through it cannot move above an earlier store; a second local for the second assignment (`pF` at entry, `pG` before the late `D_001FF390` checks) lets the `lw 0x19C` rise above the `sh` to `D_00202AD0` as in the game | func_001BB750 |
| **A table read as `->p[i].chance` / `.value`** (eight `s16`, then eight `{s16 chance; s16 value;}` pairs; Unofficial names) gives `addu ptr, i*4` with the pointer first; `[8 + i * 2]` gives `addu i*4, ptr` (2 words) | func_001BB750 |
| **`rand % 256 <= v - 1`** gives the game's `addiu v, -1; slt; bnel` (the `< v` form gives `slt; beqz`) | func_001BB750 |
| **A scalar `extern s32 D_X;` is `$gp`-relative under `-G8`**; the game's `lui; lw %lo` needs `extern s32 D_X[];` and `D_X[0]` | func_001BB750 (`D_001FF530`) |

**Parked (one-line causes; drafts in notes `ER-123_c/`):**

| Function | Size | Cause |
|---|---|---|
| func_001BB2C8 | 1,156 | 2 words: in the last loop the game has `or` before `lw $a2, 0xC($s4)`, ours the reverse. sched1 gives the game's order; sched2 (after reload, where alias analysis no longer knows the two base registers `$s3` / `$s4`) adds a memory anti-dependence from that `lw` to the `sh` of the bit mask, which lifts the `lw` to the `or`'s priority (7) and it wins the tie on dependent count (3 against 2). About 120 spellings of the block (statement order, types, struct fields, `const`, pointer reassignment, loop shapes) leave the same 2 words; the twin func_001BB750 has the same order and matches because its block ends in a call. Everything else matches with the lessons above (w reused as the sum too; the `D_00202A50` base for the `[7]` test in one local and the loop's own copy in another) |
| func_0015C0E0 | 1,072 | 33 words (was 36: `d[3] = 1.0f` right after `f`). Left: before `func_0017D968` the game builds `D_00354D80`'s full address before its first `ld` and converts `(s32)f` last; after `% 45` the `10` and `%hi(D_002488A0)` registers are swapped. Tried: 360 orders of the first block, hill-climbs of both blocks, the conversion from `p[1]` / `a0 + 0x14`, `p` and `d` as `Vec4`, `f` sharing `y` / `k` / `r`; reading `D_002488A0` as part of `D_00248880` changes the size (separate symbols) |
| func_00178C60 | 3,608 | not attempted by ER-123: 902 instructions, 30 calls, stack records passed to `func_00178700` with 64-bit GIF-tag constants, and a busy-wait on hardware registers (`*(s32 *)0x1000A000 & 0x100`, `0x10003020`) that needs `volatile` reads (a ruling); the ER-098 m2c draft (run `er098all`, chunk47) is 366 instructions short (stores into the stack records lost). A card of its own |

### 2.29 `text_001000C0` round 3: m2c's stack arrays, parameters it drops, prologue ties (ER-137 + ER-125, Mon 23 Nov 2026; Confirmed by `make regress`, 29 functions)

29 placed (2,744 bytes), commits `a785de7` (ER-137: func_0012DB88, func_0012F900), `9f76d66`, `2d4eea2`, `c3c5fe4`,
`c22dc69`, `3f6597f`, `92232b1` (ER-125: 27 by hand from the near misses of a whole-file `m2c_bulk.py --plain` rerun
with ER-114's tool, `docs/matching_tools.md` 6.13). Rulings used: 3.13 (func_001198C0, func_00101ED0), 3.16
(func_001116D0, func_001116E0). Nothing else compiler-steering.

| Lesson | Proved by |
|---|---|
| **m2c writes every stack local as `M2C_UNK sp[64]`**, so the frame comes out 0x100 bytes too big and the draft differs only in `$sp` offsets. Size the locals from the game's frame: the lowest saved register's slot is the locals' size (`sd $s0, 0x10($sp)` = 16 bytes, a `f32 v[4]`; 0x40 = a `f32 m[4][4]`; 0x60 = a 0x60-byte packet). A word m2c reads at `sp+0x30` next to a 0x40 array is the matrix's last row (`m[3]`). Several locals are laid out in declaration order from `$sp` upward (func_0012CBF8: `m[4][4]` at 0, `r[4][4]` at 0x40, `v[4]` at 0x80) | func_0012C190, func_0012C718, func_0012FB90, func_0012D2D8, func_001307A0, func_0012DF30, func_0012EB60, func_0012CB08, func_0012CBF8, func_001124E8, func_0012FCA0, func_0012FD70, func_0012FE48 |
| **A frame bigger than the locals, with nothing read there** is ruling 3.13's unused local; any size within the 16-byte rounding gives the frame (func_001198C0: 0x101 to 0x110 bytes; func_00101ED0: 0x71 to 0x80), so the size and type are Unconfirmed | func_001198C0, func_00101ED0 |
| **Parameters m2c drops**: an argument register the function never touches but its callee reads is a pass-through parameter (`$a0` / `$a1` / `$a2` and `$f12`): func_0012F540 passes `$a0` / `$a1` on, func_0012FE10 `$a0` / `$a2`, func_0012F900 its float 6th parameter (the file's callers pass `0.1f`), func_0012EAF8 `$a0` | func_0012F540, func_0012FE10, func_0012F900, func_0012EAF8 |
| **A wrapper with a frame and `jal` returns its callee's value** (2.18 again): func_0012FE10 returns func_0012C500's value, which returns func_0012C360's; the file's `extern void` for func_0012C500 became `void *` (callers unchanged) | func_0012FE10, func_0012F900 |
| **A data symbol bigger than 8 bytes is not `$gp`-relative**: `extern s32 D_X;` gives `addiu $a2, $gp, ...` where the game has `lui` / `addiu`; `extern char D_X[];` (unsized) gives the game's pair | func_0012F540 (`D_0035B9A0`) |
| **The EE timer count register** (`0x10000000`, Timer 0 `T0_COUNT`) needs `volatile` (ruling 3.16) for the game's order of the `lui` and the `$gp` store and for the read after the `mult` | func_001116D0, func_001116E0 |
| **A clearing loop over records compares with `slt`** (signed) when written with an index (`for (i = 0; i < 0x200; i++) f(D + i * 0xA0, 0x28);`); a `char *` loop compares with `sltu` or changes shape | func_001194B8, func_00119510 |
| **m2c's copy of a parameter into a second variable** moved the parameter's move above the second register save; computing from the parameter itself gives the game's prologue. A `?:` on the result gives `movn` / `movz` where `if / return` gives a branch, and the operand order of `<` with `?:` picks `movz` over `movn` | func_0012E5C0, func_00131020 |
| **A `u8` field store of 0xFF**: m2c's `s8` store wrote `-1` (`addiu $a1, $zero, -1`); the field is `u8` (`addiu ..., 0xFF`) | func_0012B890 |
| **A loop count loaded from a `u16` but kept in an `s32`** has no `andi` after the decrement | func_00113238 |
| **Field store order inside a block**: three stores after a NULL test went out in the game's order only as `0x14, 0x30, 0x10` (offset order and the other four orders did not); `m2c_bulk.py`'s store variants miss runs inside an `if` | func_0012FF78 |
| **A 4x4 transpose** reads well as `f32 (*arg1)[4]` with `t[i][j] = arg1[j][i]`; the flat `arg1[j * 4 + i]` form is 4 words off and `t[i * 4 + j]` 1 word | func_0012EAF8 (its extern in the file now takes `f32 (*)[4]`; func_0012CBF8 and func_0012CD08 pass matrices already) |
| **splat's function regex needs `void* func_X(`** for a pointer return (`void *func_X(` is not seen as a function and `tools/split.py`'s order check fails); the repo already spells it that way | func_00131020, func_0012FE10 |

**Parked (one-line cause; the run's drafts in `C:\rs\wt\er125_runs\er125file\c\`, game code, not in git):**

| Function | Size | Cause |
|---|---|---|
| func_00111700 | 60 | prologue tie: the game saves `$s0`, moves `$a1` into it, then saves `$s1`; ours saves both first and gives `$a1` `$s1` (`$a0` passes through to func_00100EC0); 6 spellings of the locals and types |
| func_0012FEE0 | 84 | 2 words: the same prologue tie (the `s8` parameter's `sll` before the `$s1` save); 4 spellings |
| func_0012D9B8 | 96 | 4 words: the game schedules the `0.0f <= f` compare above the `sh` of 0x5C; 5 statement orders and types |
| func_0012E428 | 96 | 7 words: the game's NULL test falls through to `b` with the zero in its slot; 6 spellings of the root walk (a `static` helper inlines smaller) |
| func_00130B18 | 72 | 8 words: our float temporaries are `$f0`-`$f2`, the game's `$f1`-`$f3` (also with `f32` parameters) |
| func_001033F0, func_00103470, func_00112638, func_00112CC8 | | string literals ("TEXT0.PSS", "Cannot Read MI2 File\n", ...): a function that defines `.rodata` data is not verified yet (2.5); the drafts are frame-only or register-order near misses otherwise |

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
| **Accepted** (applies 3.4, Sun 22 Nov 2026, about 14:50 Eastern, LuciDin via DISP-02) | The same fall-off-the-end shape for func_001776D8 (`s32`, ends in a plain call with no `return`), so that its caller func_001C46C8 matches (ER-080, ER-095); same conditions and the same `PORT NOTE` as func_00194D08 |

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
| **Accepted** (extension, Sun 22 Nov 2026, about 14:34 Eastern, LuciDin in the ER-085 session) | The same for a compare tree: func_0014CCD0's two-state `switch` (0, 1) lists an empty `case 2:` beside the default, the only shape found that gives the game's `==1`, `<2`, `!=0` tests (2.20) |

### 3.10 ER-072: a double literal replaces its standalone .rodata constant (Sun 22 Nov 2026, about 05:35 Eastern, via DISP-02)

| Ruling | Item |
|---|---|
| **Accepted** | When a C function's own double (or float) literal is emitted by gcc as an anonymous `.rodata` constant that takes the place of a standalone `INCLUDE_RODATA` / `D_` symbol (func_001371C0, func_0013AA28, func_0013BD90, func_0013DF90, func_0014DF08, func_0014EEF0), `tools/split.py`'s order check may accept dropping that `INCLUDE_RODATA`: the literal is ordinary source and `make regress` still proves every byte. ER-077 implements it |

### 3.11 ER-069: -fno-edge-lcm (Sun 22 Nov 2026, 13:48 Eastern, LuciDin in the ER-069 session)

| Ruling | Item |
|---|---|
| **Accepted** (option A of three: flag; r2 compiler with the default changed; not adopted) | `CFLAGS` = `-O2 -G8 -fno-strict-aliasing -fno-edge-lcm` for every file: GCSE without the edge-based LCM, which removes loads after joins that the game keeps (func_0012EEB0; 4 more parked functions move; m2c census +3 matches, 0 lost); the whole ELF stays byte-identical. A stock flag of the same compiler (also in SSX Tricky's SDK-era makefile), not a patch; `GCC_DIR` stays r1 |

### 3.12 ER-070: ee-as, delay-slot swap (Sun 22 Nov 2026, 14:30 Eastern, LuciDin in the ER-070 session)

| Ruling | Item |
|---|---|
| **Accepted** | Fourth ee-as patch (`tools/toolchain/patches/ee-as_r5900_delay_swap.patch`): for the r5900, gas's "previous instruction is a sync" swap test checks the flag (`&` for the 1999 text's `&&`, which blocked every delay-slot swap), and a `div.s` / `sqrt.s` is never moved into a slot. The game's slots: 285 `mtc1` uses swapped, 0 not; every nop slot of 28,567 game branches explained. Same standing as 3.4 / 3.5 / 3.8: a documented reconstruction of the original assembler's behaviour, not Sony's code; built into `ee-binutils-2.9-991111-r4` |

### 3.13 ER-086: an unused local that only sizes the stack frame (Sun 22 Nov 2026, 14:46 Eastern, via DISP-02)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** | An unused local variable or array (func_00113C30, func_00113ED8: e.g. `float buf[16];`) whose only effect is the stack frame size, when nothing else in plain C gives the original frame and the rest of the function matches; read as a leftover from the original source (a removed or debug use). Each use carries a comment naming this ruling. Not a licence for other dead code: code that only steers register choice or scheduling, `volatile` keep-alives and `asm()` still need their own ruling. Used so far: func_00113C30 (ER-089, 2.23); three by ER-100 (2.24); func_001668E0, func_001668A8 (ER-095, 2.25: `char buf[0x80]`); func_00113ED8 (ER-115, 2.27); func_001198C0, func_00101ED0 (ER-125, 2.29) |

### 3.14 ER-100: an address-taken local that only stops a sibling call (Sun 22 Nov 2026, 16:40 Eastern, LuciDin in the ER-100 session)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** | `(void)&local;` (or `&` on an existing local or parameter), only where it is the sole blocker of a sibling call and the rest of the function matches: the game has `jal` + epilogue where plain C gives a tail `j`, because the compiler's Cygnus sibcall pass refuses sibcalls in a function that uses ADDRESSOF (2.24). Read as the original source taking some local's address. Each use carries the comment `ruling 3.14`. Used by ER-100: func_00149EE8, func_0014A500, func_0014A0B8, func_0014B528, func_00148BE0, func_00148D28, func_0014C110, func_001485A8 |

### 3.15 ER-095: a 16-byte aligned float vector type (Sun 22 Nov 2026, about 17:35 Eastern, LuciDin in the DISP-03 session)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** | `__attribute__((aligned(16)))` on a 4-float vector type (func_00158DC0: `typedef struct { f32 v[4]; } __attribute__((aligned(16))) Vec4A;` for `D_00354D70`), as a data layout: Sony's libvu0.h declares `sceVu0FVECTOR` as `float[4]` with `aligned (16)`, so the original source plausibly used such a type. Not for steering code generation; any other attribute needs its own ruling. Each use carries a comment naming this ruling. (A plain two-`s64` struct also matched; the float vector is the kept reading, Unconfirmed.) Used so far: func_00158DC0 (ER-095, 2.25) |

### 3.16 ER-132: volatile reads and writes of hardware registers (Sun 22 Nov 2026, about 18:50 Eastern, LuciDin in the DISP-03 session)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** (first ruled "not accepted" at about 18:45, reversed after the explanation) | `volatile` only on reads and writes of memory-mapped hardware register addresses (EE timers, DMA, GIF / VIF, GS privileged registers and the like: e.g. func_00178C60's busy-wait on `0x1000A000` / `0x10003020`), where the game's code re-reads the register each time as the hardware changes it. It states a fact about the location, as Sony's own register access macros do (Unconfirmed until checked against an SDK header). Each use carries a comment naming this ruling. **Never on ordinary variables**: `volatile` keep-alives that only force a load or store stay forbidden. LuciDin: the goal stays a true 100% decomp as in Twinblade (3.1); every use of the narrow rulings (3.13-3.16) is listed here so it can be reviewed later. Used so far: func_001116D0, func_001116E0 (ER-125, 2.29: EE Timer 0 count `0x10000000`) |

No other ruling yet on matching tricks in C (`asm()`, `volatile` keep-alives, other dead code; 3.13 covers only frame-sizing locals, 3.14 only the sibcall blocker): park such functions with a
one-line cause and ask.
