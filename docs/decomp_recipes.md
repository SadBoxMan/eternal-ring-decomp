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

**String literals: verified (ER-149, 2.33).** A C function's string literal lands in the file's owned `.rodata` at the
function's place (gcc's `.rdata`, 8-byte aligned): ER-060's func_00100EE0 (`"\\ERDATA.BIN;1"`) was the first, ER-149
placed four more. Still not verified here: a function-local `static` or an initialised global defined in C.

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
| func_00119FE0 | 6,636 | **parked by ER-093 (2.37) at 5 words, same size** (two tie-breaks: a sched2 class / dependent-count tie, a local-alloc `$v0` / `$v1`); was: surveyed only: 925 m2c lines, 53 calls, a jump table (in fact a compare tree), 36 literals; m2c's draft does not compile (`M2C_BREAK`) |

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
| **Field store order inside a block**: three stores after a NULL test went out in the game's order only as `0x14, 0x30, 0x10` (offset order and the other four orders did not); `m2c_bulk.py`'s store variants missed runs inside an `if` (they try them since ER-147, `docs/matching_tools.md` 6.18) | func_0012FF78 |
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
| func_001033F0, func_00103470, func_00112638, func_00112CC8 | | **placed by ER-149 (2.33)**; was: string literals ("TEXT0.PSS", "Cannot Read MI2 File\n", ...): a function that defines `.rodata` data was not verified yet (2.5) |

### 2.30 The five small game files, round 5 (ER-120, Mon 23 Nov 2026; Confirmed by `make regress`, 52 functions)

52 placed in `text_00145120` (24), `text_0014A968` (11), `text_0014C840` (5), `text_0014D930` (2) and `text_0014E2E8`
(10), 14,520 bytes; `make regress` (-j4) green after each batch and after merging main. Branch `er-120-five-files`,
worktree `C:\rs\wt\er120`, commits `4443a18`, `67f010f`, `111d5c1`. The fixed `m2c_bulk.py --plain --tag ER-120` (6.12) over the five files (74 asm functions,
labels `er120_text_*`) and again in lists of 8 (`er120_chunk8`) gave **no MATCH**: every draft compiled, only 3 were
the right size (all three belong to other cards: func_00145FE8 ER-122, func_0014C840 / func_0014D930 ER-118). All
52 came from the drafts by hand, 30 here and 22 by three helper agents working in scratch files (each one checked here,
its declarations aligned with the file, and placed). Runs (game code, not in git): `C:\rs\wt\er120_runs\`.

| Lesson | Proved by |
|---|---|
| **The m2c drafts of these state machines are usually right in content and wrong in shape**: most "size -N" drafts had lost a pass-through `$a0` / `$a1` (the callee takes the caller's own argument: func_00148FF8, func_001475E0, func_0014A238's `func_0014A948`), split one statement in two, or kept `goto`s. Written as `switch (*(s16 *)(p + 0x6E))` with fall-through cases, `+= 1` and early returns, most matched within a few tries | most of the round |
| **Store order: ascending offsets again** (2.25), pointer stores first and a byte store last; where not, a permutation (permg) found it. Sometimes the game's emitted order is the reverse of the source order (func_00147F20's `0xE` then `0xF`; func_0014E0A8's `0x234` before `0x194`) | func_001498D8, func_00147F20, func_0014E0A8, func_00145120 |
| **A callee's real return type moves later temporaries** (2.15 again): `func_0012FAA8` is `s32` (its definition in `text_001000C0`); `func_0012FB48` is `void`; `func_00130B60` returns `f32`; `func_00130BF8` returns a float (its result is compared) | func_0014C3F8, func_00147DE0, func_001488A0, func_0014BA78 |
| **A float parameter before the last int** in a prototype changes the argument setup order (2.21 again). `func_00145FA8`'s definition reordered to `(void *, f32, s32, f32)` (same bytes, no other caller); `text_0014A968` now declares `func_0012B890` as `(..., s16, f32, s32)` like `text_00145120` (all 51 C functions of the file still match) | func_00145260, func_0014AAA8 |
| **Locals loaded together**: `f32 k = D; f32 x = p->a; f32 y = p->b; p->a = x * k; p->b = y * k;` gives the game's two loads before both stores; separate temporaries per arm (`a = p->48; r = a - k ... b = p->4C; q = b + k`) give its registers | func_001493F8, func_00149700 |
| **`extern const` for never-stored `.sdata` floats** (2.21) only where the game hoists the load above a store: `D_0035BDB8` / `D_0035BDBC` (not `D_0035BDC0`), `D_0035BED0` / `D_0035BEE8` / `D_0035BEEC` (the census lists these three as near an address-taken symbol: Unconfirmed) | func_001493F8, func_0014EC58 |
| **A 64-bit flag cleared with the complement of the flag set**: `&= ~((u64)0x8000 << 35)` builds the game's `lui 0xFFFB; ori; dsll; ori; dsll; ori` | func_0014AAA8, func_0014AFA8 |
| **An explicit empty `case 0:` beside the default** (ruling 3.9) gives a compare tree `==1, <2, <5` for a switch on a call result; `case 3: case 4: default:` sizes a 5-entry jump table | func_00149170, func_001475E0 |
| **Helper findings**: two identical call tails written in each arm (jump2 merges them; one call with locals does not schedule the same: func_0014B688); an explicit trailing `return;` turns the last call into the game's tail `j` (2.22: func_0014CA60, func_00147DE0, func_00147A28); a function that returns its callee's value must be `void` with a plain call when the game has a tail `j` (func_0014E7A8); `i` used after a loop gives the unfolded `+0x50` index (func_00150658); the degree-to-radian expression written at each use (2.26: func_0014DC98) | helper agents A, B, C |
| **Ruling 3.14 twice** (the call after a nested `if` is `jal` + epilogue in the game, frame without locals) and **ruling 3.13 twice** (helper B: `s32 buf[4]` sizing a 0x40 / 0x50 frame) | func_00148FF8, func_00147018; func_0014C658, func_0014CF60 |
| **`a > b` rather than `b < a`** swaps the two loads of a compare (`lh 0x6C` before `lh 0x74`) | func_001475E0, func_00145260, func_0014CD88 |

Parked (one-line causes; C in the scratch folder, not in git):

| Function | Cause |
|---|---|
| func_00147708 | 3 words: `x94 + D_0035BD58` ties the sum to the constant's register (game: the field's); 5 expression shapes; the ER-100 FP-tie family |
| func_0014FC68 | 3 words: the game loads `0x6E` above a float store through `arg1`, which `-fno-strict-aliasing` code should not allow: the ER-118 family |
| func_0014E450 | 4 words (helper C): case 2's `sw $0, 0x90` against the `D_0035BEC8` load (720 store orders); and it needs `do { ... } while (0)` around case 4's two calls to stop the sibling call (recipes 2.24: a call in a loop statement): **ruling question**, reading (Unconfirmed) a macro; also `func_0014F2E0`'s float before its last int |
| func_00145D08 | 19 words (helper A; 32 with pi / 2pi in locals, both loops right): the game copies the pointer to `$v0` for the second wrap loop (`daddu $v0, $a1` in the `bc1f` slot); 10 loop shapes |
| func_0014BD70 | 30 words: case 1's loads of `v[0]` / `v[2]` before the fields, and the `-1 / 1 / 0` select's registers (`movn` / `movz` on the constant-1 register) |
| func_001486F0 | `func_0012C578`'s 5th argument: 16 bytes (`-x70, 1.0, -x78, 0`) written to the outgoing stack area with no pointer register; a struct (plain or 16-aligned vector) goes by reference in our build, varargs promotes to double. The other caller (func_0014B688) passes four words in registers: the callee's prototype is open |
| func_0014A238 | the game keeps `%hi(func_0014A3B0)` in a saved register (`$s0` / `$s3`) across four calls for two `func_00131140` calls; ours rematerialises it (4 more saved registers in the game's frame) |
| func_0014FA18 | the table pointer chosen by `0xF` is stored to the stack and reloaded (`sw` / `lw 0x10($sp)`): an address-taken or volatile local, not tried |
| func_0014D1E0, func_0014F328 | (clones) several 16-float matrices in overlapping stack slots (block-scoped temporaries); not tried in depth |
| func_001469B8, func_001494A0, func_0014E608 (jump tables), func_00147100, func_0014EAD0, func_0014F7F8 | not attempted (time); drafts in `C:\rs\wt\er120_runs\` |

Not this card: func_0014C840, func_0014D930 (ER-118), func_00145FE8, func_00146598, func_00147F78, func_0014D3F8 (ER-122).
C of the parked ones: notes `ER-120_c/` in the dispatcher's notes folder (game code, not in git).

### 2.31 `text_00150878` rerun: GS register packers, pointer-return spelling (ER-137 + ER-113, Mon 23 Nov 2026; Confirmed by `make regress`, 43 functions)

43 functions placed in `text_00150878`, 3,316 bytes, branch `er-113-text-00150878` (worktree `C:\rs\wt\er113`),
commits `870aaf8` (ER-137's 9), `51d70b9` (12 plain MATCHes of the rerun), `b18d16a` (13 by hand), `8b7a4c3` (2 by
hand), `91803bf` (7 with m2c's `sp[64]`). Runs: `docs/matching_tools.md` 6.15. ER-137's func_00162108 and func_00165080 use ruling 3.14 (listed there).

| Lesson | Proved by |
|---|---|
| **GS register packers: OR the fields in parameter order, left to right** (`a1 \| a2 << k2 \| a3 << k3 \| ...`, as Sony's `SCE_GS_SET_*` macros are written). m2c's drafts group the terms as the scheduler left them (`a \| d \| (b \| c)`), which compiles to the same instructions in another order (5-23 words); the flat parameter-order list matched all nine `func_001C6880` wrappers outright. A first search over every order and grouping found a right-nested form for two of them; the parameter order is the reading kept | func_001C68D0, func_001C6938, func_001C69D8, func_001C6A90, func_001C6B18, func_001C6B60, func_001C6BD8, func_001C6C70, func_001C6CE8, func_001C67D0 |
| **A stack-passed packer argument is `u32`** (`lwu` from the caller's frame); m2c's `u32 arg_sp0` with its `(s64)` cast is right, an `s64` parameter gives `ld` | func_001C6938, func_001C69D8, func_001C6B60, func_001C6C70 |
| **A 64-bit value stored as two words**: `(s32) (v & 0xFFFFFFFF)` and `(s32) (v >> 0x20)` give the game's `lui 0xFFFF; dsrl32; and` and `dsra32`; m2c's `0xFFFF0000U >> 0x20` mask does not. The stores then go in the order the game schedules (`+0xC`, `+0`, `+8`, `+4` for func_001C6880; a 24-order search) | func_001C6880, func_001C6828 (its 32-bit OR written `a5 \| (a2 \| (a3 \| a4))`: the parameter order gives 5 words) |
| **Pointer returns are written `void* func_X(`** (recipes 2.13): with `void *func_X(` the split step does not see the definition and stops on the function order | the 12 pointer-returning functions above |
| **An unused middle parameter**: the game reads `$a2` and never `$a1`, so the function has three parameters (the pass-through retry drafted `(void *, s32 arg2)`) | func_00152548 |
| **A pass-through second argument** to a callee another function in the file calls with two arguments: give the wrapper the parameter and pass it on (the one-argument draft conflicted with the file's other declaration) | func_00175D90 |
| **m2c's `M2C_UNK sp[64]` (2.29, ER-125): size the local from the game's frame** (`addiu $sp, -0x30` with `$s0` saved at `0x10`: 0x10 bytes of locals, `s32 buf[4]`; `0x60` / `0x40`: `f32 m[16]`). Four words passed as `&v[0]` ... `&v[3]` are one array (the game's `ori $a2, $sp, 4` needs the 16-byte-aligned array at `sp+0`). After the frame, the usual fixes: offset-order stores, the `NULL` arm first, one reused variable | func_001C89F8, func_001C8D58 (unused first parameter), func_0017D2D0, func_00157760, func_00192EE8 / func_00193440, func_001585C8 |
| Recipes 2.25 again: `if (p == NULL) return -1;` before the call gives the game's `bnel` with `-1` in the likely slot; a base-pointer local (`char *g = (char *)D_001FEC00;`) gives the game's single `%hi` / `%lo` pair with plain offsets | func_001A7CA8, func_001C20B0 |

**Parked (one-line causes; hand files in `C:\rs\wt\er113_runs\hand\`, game code not in git):**

| Function | Cause |
|---|---|
| func_001C8600 | 2 words: the `sw $a1, 0($a0)` store goes first in ours, second in the game; all 120 orders of the five stores, and a chained `x8 = xC = 0` (24 orders), leave it |
| func_0018C9E8 | 2 words: `li $v0, 1` and `sh $zero` swapped in the taken branch (the `x18` pointer read first, before the test, gives the game's delay-slot load; 5 spellings of the branch) |
| func_001B34F8 | 4 words (was 7): `x70` read into a local first and `x10 = x4 = 0xC9` fix two places; the `D_00248880` store keeps `$v0` / `$v1` swapped and the `sw` scheduled after the `addiu` |
| func_001BE9C0 | 8 words: the base pointer of `D_001FF340` takes `$v0` (game `$v1`) and the `%hi` register the other; 5 spellings (`f32 *`, `s32 *`, `char *`, assigned before / after the store) |
| func_0018B8A8 | 7 words: the game loads the `D_002488A0` float before the first store; none of the 720 orders of the six stores does |
| func_001C2FC8 | 1 word: `addu $v0, $v0, $s0` against the game's `addu $v0, $s0, $v0` for the `D_001FEC00[idx + 0x118]` address (frame fixed with `char buf[0x80]`); 8 spellings of the index (`char *` / `s32 *` base, array or pointer sum, either operand first) |
| func_001C56F0 | not tried: it calls `func_001C6158`, `func_001C7328`, `func_001C8648` with an argument, but the file defines them `(void)` |

### 2.32 The five small game files, round 6 (ER-148, Mon 23 Nov 2026; Confirmed by `make regress`, 14 functions)

14 placed in `text_00145120` (6), `text_0014A968` (1), `text_0014C840` (1) and `text_0014E2E8` (6), 6,452 bytes, of
ER-120's 15 left (2.30); `make regress` (-j4) green. Branch `er-148-five-files`, worktree `C:\rs\wt\er148`, commit
`826c224`. A fresh `m2c_bulk.py --plain --tag ER-148 --list` over the 16 (label `er148`, 30 s) gave 0 MATCH and no
same-size draft (so the `$sp`-only scan of `docs/matching_tools.md` 6.13 had nothing to take); all 14 came by hand
from those drafts and ER-120's parked C, by three helper agents in scratch files (each placed here, declarations
aligned, the whole file checked with `cmpfunc`). Runs (game code, not in git): `C:\rs\wt\er148_runs\`.

| Lesson | Proved by |
|---|---|
| **A callee's real return type, again** (2.30): `func_00130BF8` returns `f32` (now so in `text_00145120` as in `text_0014A968`): that alone fixed the 3 words parked as an FP tie. `func_0012FA18` (returns `func_00165170`'s `s32`) and `func_0012FAE8` (returns `lbu 0x79`) are `s32` in `text_0014E2E8` now (6 words of `$v0` / `$v1`); the other files still declare `func_0012FAE8` `void` (no caller there uses the value) | func_00147708, func_0014F7F8 |
| **A field read into a local first** moves its load above a store through another pointer, under `-fno-strict-aliasing`: `u16 s = *(u16 *)(p + 0x6E);` before `arg1->x94 = ...`, then `*(s16 *)(p + 0x6E) = s + 1;` (`u16`: the game's `lhu`) | func_0014FC68 (parked at 3 words in 2.30) |
| **A stack slot that looks address-taken can be a plain spill**: all 9 callee-saved registers in use; written like func_00150658 (`tbl = flag ? A : B`, loop with `i`, values used after the loop), it matched with no trick | func_0014FA18 |
| **Overlapping "temporaries" can be the rows of one matrix**: `f32 m[4][4]` whose row 1 is filled and row 2 normalised, then `func_001CA078(m, m[1], m[2])` builds `m` in place; locals in declaration order `m, b[4][4], r[4][4], v[4], out[4]` | func_0014D1E0, func_0014F328 (clones: three offsets differ) |
| **A 16-byte "stack argument" can be an unused local**: `func_0012C578` reads only `$a0`-`$a3` (its other caller passes four words); the four stores to `sp+0` are a local filled and never read, which gcc keeps (no dead-store removal for stack memory). **Ruling 3.18** | func_001486F0 |
| **Ruling 3.14 also keeps a `%hi` CSE**: `(void)&arg0;` blocks the last call's sibcall and with it the game's `%hi(func_0014A3B0)` held in `$s3` across four calls (ours rematerialised it without) | func_0014A238 |
| **Shape fixes**: case 3 and case 4 as two plain fall-through cases re-reading `->E0->8C` (not `case 4:` inside case 3's `if`); `*(f32 *)(p + 0x48) *= k;` rather than locals loaded together; `x6E = 3; break;` in each place instead of `goto`s (the compiler merges the tail); a 5-entry jump table from `case 0: case 1:`, `case 2`, `case 3` and an empty `case 4: break;`; `if (k == 1) ret = 1; if (k == 0x10) ret = -1;` for a `-1 / 1 / 0` select; `extern const` for never-stored `.sdata` floats the game hoists (`D_0035BD08`, `D_0035BD0C`, `D_0035BD38`, `D_0035BDF4`, `D_0035BF10`) | func_001469B8, func_001494A0, func_00147100, func_0014E608, func_0014BD70 |
| **Prototypes aligned with the definitions**: `func_0014BD70(char *, char *, s32)` (the caller passes `1` in `$a2`, the function never reads it: third parameter Unconfirmed), `func_0014D1E0(char *, f32)`, `func_001469B8(char *, char *)`; `func_0012EEB0(float *, float *)` declared first here (types Unconfirmed) | the files' other functions still match |

Parked (C in the dispatcher's notes folder `ER-148_c/` and `ER-120_c/`, not in git):

| Function | Cause |
|---|---|
| func_00145D08 | 19 words (17 more shapes, none closer): the game copies the pointer to `$v0` for the second wrap loop and reloads / stores the field every pass with the loop constants hoisted; a real `while` / `do` keeps the field in a register for the whole loop, a `goto` loop gets everything but the copy (32 words), every `q = p` placement is removed by CSE. Reading (Unconfirmed): an inlined helper |
| func_0014E450 | 4 words: case 2's `sw $0, 0x90` against the `D_0035BEC8` load, and case 4's two calls need a `jal` where plain C gives a sibling `j`. Ruling 3.17 forbids `do { } while (0)`; a ruling-3.14 form (`(void)&arg0` or `&arg1`) blocks every sibcall, but the game keeps case 3's tail `j func_00131000` (37 words) |

Not this card: func_0014C840, func_0014D930 (ER-118), func_00145FE8, func_00146598, func_00147F78, func_0014D3F8 (ER-122).

### 2.33 `text_001000C0`: string literals, a stub's parameters, func_00124BE8 by hand (ER-149 + ER-071, Mon 23 Nov 2026; Confirmed by `make regress`, 4 functions)

4 placed (480 bytes), commit `fa319b4` (its message says 424 bytes and credits the `"\\ERDATA.BIN;1"` proof to
func_00100EC0; it is 480 bytes and func_00100EE0), branch `er-149-text-001000c0` (worktree `C:\rs\wt\er149`). Ruling 3.13 for func_001033F0 and
func_00103470 (`s32 buf[40]`: 0xC0 / 0xD0 frames, nothing reads `sp+0..0x9F`). Nothing else compiler-steering. No
`tools/split.py` change was needed.

| Lesson | Proved by |
|---|---|
| **A string literal in C needs nothing special.** splat had migrated each function's own string into its asm file (no `INCLUDE_RODATA` line), and gcc emits the literal into the object's `.rodata` (`.rdata`, `.align 3`) while compiling that function, so it lands at the same offset: the `.asciz ""` pads spimdisasm shows after `"TEXT0.PSS"` are only that 8-byte alignment. Two strings in one function come out in the order of first use | func_001033F0, func_00103470 (`"TEXT1.PSS"`, `"STAFF.PSS"`), func_00112638, func_00112CC8 (and ER-060's func_00100EE0) |
| **Strings of 8 bytes or less (with the NUL) go to `.sdata`**, not `.rodata` (`-G8`; tested: `"abcdefg"` in `.sdata`, `"abcdefgh"` in `.rdata`, both still `lui` / `addiu`). `text_001000C0`'s `.sdata` is shared asm, so such a function cannot be placed there until that run is owned (Confirmed by compile only; no such function placed) | scratch compile |
| **The parameters of a stub callee pick the caller's saved registers.** func_00100EC0 only returns 1 (an unused 0x80 frame), so its parameters cannot be read from its body. Passing it `arg0, arg1` (already in `$a0` / `$a1`: no move) gives `arg1` four references, so global allocation takes it first and it gets `$s0` as in the game; with `arg0` alone `arg2` / `arg3` win (priority 2/7 against 3/11, `-dg` dump); all four arguments fix the registers but reorder the prologue copies. The parameter list is Unconfirmed | func_00112638, func_00112CC8 |

**func_00124BE8 (ER-071, 1,860 bytes): parked at 7 words, right size.** Draft and notes: notes folder
`ER-149_071_c/` (`func_00124BE8_7words.c`; scratch variants `C:\rs\wt\er149_runs\er071s\`). m2c
(`m2c_bulk.py --plain --list`) gave a draft that did not compile (an `sp+0x14` local it never declared); by hand from
the asm, 378 -> 7 words. Seen only in masked compares (Unconfirmed until a placement proves them):

- **Records indexed by a shifted id: an array of records, not char arithmetic.** `D_0035B8EC[id - 200].x3C` on a
  0x100-byte record type gives the game's `sll 8` + `lui/ori -0xC800` (gcc distributes the constant out of an array
  index); `ptr + (id - 200) * 0x100 + 0x3C` keeps `(id - 200) << 8`. Likewise `D_00210F30[i].x8 = 0` keeps the field
  offset in the `sh` (record size 0x30) where char arithmetic folds `+8` into the `%lo`. Layouts Unofficial.
- **A pointer chain read twice** (`*(s32 *)(*(char **)(obj + 0xFC) + 0x30)` before and after a join, no pointer
  variable) gives the game's `daddu $a2, $v1` copy: GCSE's PRE reuses the first `obj + 0xFC` load, and the field load
  stays because PRE runs one pass (the two addresses are different registers when it hashes them).
- **`field += d * k`** re-reads the field, and PRE turns the second read into a copy of the first load (`mov.s $f2, $f0`).
- **A variable reused for the result keeps the saved copy.** The angle blend `t = a; a -= cur; if (pi < a) a = a - pi -
  cur; else if (a < -pi) a = t + 2pi - cur;` gives the game's `mov.s $f3, $f1` and the difference in `a`'s own
  register; a separate `d = a - cur` (all 32 spellings of copies), an inline helper and nested inline helpers do not
  (CSE and copy propagation remove the copy whenever the original is not overwritten). The fold subtracts pi, not 2 pi:
  that is what the game computes (a reading: an original slip; Unconfirmed).
- **A global read into a local before a store** (`k = D_0035B824;` ahead of `*(u16 *)(obj + 2) |= 1;`) gives the game's
  load order: gcc cannot move the load above a store that may alias it (`-fno-strict-aliasing`), and the order at
  allocation time decides the registers of the whole block.
- **An index kept in the pointer variable** (`c = (s8 *)(id - 200); base = D_0021E730; ... c += (s32)base;`) gives the
  game's `addu $a0, $a0, $v1` (index register = result); four separate increment blocks are cross-jumped into one by
  jump2, which the game does not do (its first block keeps its own copy).
- **Still open (the 7 words):** the `%hi` temporaries and the base of the three counter tables take `$v0` in ours and
  `$v1` in the game, which never reuses the register of the preceding `slt` result there; about 30 forms (types,
  statement and declaration order, `base[i]`, per-branch pointers, split pointer variables) leave it. Same family as
  func_00120EF0's `%hi` registers (2.27).

### 2.34 ER-031's files and the game-unk files rerun with ER-114's tool (ER-154, Mon 23 Nov 2026; Confirmed by `make regress`, 76 functions)

76 functions placed in nine files, 7,676 bytes, branch `er-154-other-files` (worktree `C:\rs\wt\er154`), commits
`6c5614d`, `d5326c8`, `85a83c4`, `f43db66`, `58a8433` (bulk, 57 plain MATCHes + 7 by hand), `7a1f02e`, `42e974f`,
`ba45b19`, `9506bec`, `1aab8aa`, `01796d6`, `04e5cba` (12 by hand). Runs: `docs/matching_tools.md` 6.16. Ruling 3.14 for
func_00134618, func_00136D88, func_00137428 (`(void)&arg0`, drafted by the tool; listed there). Nothing else
compiler-steering.

| Lesson | Proved by |
|---|---|
| **A getter on a 2-D record array keeps the base address whole**: `char *p = D_01B72600 + ((a0 * 4) + a1) * 0x1C0; return *(T *)(p + off);` gives the game's `addiu %lo(D_01B72600)` and the field offset in the load; the one-expression form folds the offset into `%lo` (2 words); a `struct[][4]` or `struct[]` type gives other code (recipes 2.25's base pointer, now for a computed index) | func_001CB218, func_001CB240, func_001CB288, func_001CB2B0, func_001CB2D8, func_001CB500 |
| **A 16-bit counter compared after the increment is an `s16` written `x += 1; if (x >= N)`**: the game stores the raw sum (`sh $v0`) and compares a sign-extended copy (`sll` / `sra` into `$v1`); m2c's `temp = u16 + 1; field = temp; if (temp >= N)` sign-extends in place (3 words). `++x >= N` matches too | func_00138D50, func_0013A1A0, func_0013BBC8 |
| **The callback-table setters store `0xC4, 0xC8, 0xF8, 0xC0, 0xCC`** (with `0xF` first in func_0013F318), not offset order, in two functions; a third (func_001359C0) is offset order for its five function pointers, with its two float stores (0xE4, 0xB4) left as m2c wrote them. Found by searching every order of the run (120 / 24); `m2c_bulk.py`'s offset-order variant sorted the whole run (ER-160 traced it; its `by type` mode gets it, `docs/matching_tools.md` 6.18) | func_00139CA8, func_0013F318, func_001359C0 |
| **A float store ordered after a load**: the game reads `(*arg0)->0xA4` before storing `arg1` at `p->0x4C`; with `-fno-strict-aliasing` C must read first (a local) | func_001387C0 |
| **The if / else the other way round** gives the game's `beql` where m2c's order gives `bnel` (the likely slot takes the other arm's store) | func_001407F0, func_00140978 |
| Recipes 2.10 / 2.25 again: a pass-through `arg0` (`func_0013D480(arg0, 2)`, the file already said `(void *, s8)`); `func_0012B890`'s float is its sixth parameter (`text_0013C510`'s prototype changed to `(void *, s32, s32, s32, s16, f32, s32)`, its two other callers swap their last two arguments: same bytes); four float stores in one order of 24 | func_0013C6D8, func_0013D688, func_0013F128 |
| **A wrapper returning a callee's `f32` is `f32` itself** (2.18 again): func_00144B28 returns func_00144D18's value (placed as `f32`); as `s32` gcc adds `cvt.w.s` / `mfc1`. The tool's Plain column blamed a narrower `func_0012F8C8` prototype; that change was not needed | func_00144B28 |

**File declarations changed** (each proven by `make regress`): `text_0013DBC0` func_0013E818 prototype `(s32 *)` (a
non-`extern` prototype line, which the plain step does not check: the plain file compiled, the file did not);
`text_001343E8` func_00136058 / func_001360A0 `(void **, s8 *)`, `extern float sqrtf(float);` above func_00135958;
`text_001388E8` func_00139450 `(void **)` (the second argument is never read), func_00138D50's definition takes the
file's `(s8 *, void *)`; `text_0013B748` func_0013BBC8 `(void **, void *)`; `text_0013C510` func_0012B890 (above).
`text_001CB528`: the drafts' `extern s32 D_0035C554` conflicted with the file's `extern void *`; the sums are
written `(char *)D_0035C554 + ...` (same bytes).

**Parked (one-line causes; drafts in `C:\rs\wt\er154_runs\`, hand files in `C:\rs\wt\er154_scratch\`, game code not in git):**

| Function | Cause |
|---|---|
| func_0013B2B0, func_0013E760, func_0013F068 | **Placed by ER-158 (2.36: the callees' return types).** 3-5 words, one shape in three functions: `x94 += xB0` then `func_0012D9B8(arg0, x94)`; the game loads `x94` into `$f1`, `xB0` into `$f0` and sums into `$f1`; ours sums into `$f0` (8 spellings on func_0013B2B0: `+=`, operand order, a temp, the sum inside the call; `xB0 + x94` gets the loads right, 3 words left) |
| func_00137658 | **Placed by ER-159 (2.36: the call in both arms).** 4 words: the game stores `x74` (a call's result) before loading `x6E` for its increment; ours loads first (`+= 1`, `++` and m2c's form) |
| func_001CD680 | **Placed by ER-159 (2.36: `arg0 << 4`).** 4 words: `D_0035C554 + arg0 * 16` summed into `$v0` with `$ra` saved after the shift in the game; 2 spellings of the sum |

### 2.35 `text_00150878` hard ones 2: struct arrays, `?:` returns, arguments the callee reads, a syscall stub (ER-152 + ER-153 + ER-132, Mon 23 Nov 2026; Confirmed by `make regress`, 5 functions)

5 functions placed in `text_00150878`, 828 bytes, branch `er-132-text-00150878` (worktree `C:\rs\wt\er132`), commits
`78e6ef7` (ER-152), `287f64f` (ER-153). Fresh drafts: `m2c_bulk.py --plain --tag ER-153 --label er153` on the 13
near misses and func_00178C60 (20 s; 14 compile, 0 MATCH). Hand files, near-miss drafts, the func_00178C60 draft
and this card's scripts (register-blind score, region diff, per-block order climber, group reorder) are in the
dispatcher's notes folder `ER-132_152_153_c/` (our C only).

| Lesson | Proved by |
|---|---|
| **An empty callee keeps its bytes whatever its parameters**: func_001C6158, func_001C7328 and func_001C8648 (`jr $ra; nop`) are called with a value in `$a0`; widened to `(void *arg0)`, the caller then matched as plain C on the first draft | func_001C56F0 (func_001C6158, func_001C7328, func_001C8648 unchanged) |
| **`base->arr[base->i]` with the array a struct member** gives the game's `addu base, i*4`; every pointer-sum or `s32 *` index spelling gives `addu i*4, base` (2.31's 8 spellings). `D_001FEC00` read this way is a struct with a word array at 0x460 (Unofficial field names, element count Unconfirmed) | func_001C2FC8 |
| **`return (f(p) == 0) ? a + b * k : 0;`** as one expression gives the game's `bnel` with the zero in the likely slot and the product in the game's registers; the `if (...) return ...; return 0;` form (2.25) fixed the branch but left `$v0` / `$v1` swapped (5 words) | func_001C8700 |
| **A call that takes the caller's parameter** (`func_00154840(arg0)`, whose definition reads `$a0`): passing it costs no instruction and puts the parameter copies in the game's prologue order (`sd $s0` before `sd $s1`); the zero-argument call gives them swapped | func_001540C0 |
| **A syscall stub is `void`**: func_001D03E0 (`li $v1, 0x64; syscall`: FlushCache in the PS2 kernel, Unconfirmed name) declared `void` lets the caller reuse `$v0` after the call (2.20); the file's extern changed, its other callers unchanged, and func_001636A0's own later declaration (`s32`) aligned to `void` (func_00164760 unchanged) | func_001636A0 |
| **Rulings 3.13 + 3.14 together**: an unused array whose address is taken (`s32 buf[4]; (void)&buf;`) gives both the game's 0x30 frame (0x10 bytes of locals) and its `jal` + epilogue for the last call; a scalar `(void)&x` is optimised away (no frame, sibling `j`) | func_001636A0 |

**Seen only in the func_00178C60 draft (same size, not placed; Unconfirmed until a placement proves them):**

- one base-pointer local per region for a global struct (`g0`..`g10`, each assigned once): one local assigned many
  times loses its alias base (2.28), so gcc reloads its fields after every stack store; the separate locals took the
  draft from 3 instructions too long to the game's size;
- a base pointer for the entry read of `D_001FF340` keeps `%hi(D_001FF340)` in a saved register for a later use of the
  symbol (the game's `daddu $s6`);
- an angle written inline in each call pair (`f(K - base) * c1; g(K - base) * c2`) takes the first saved FPR; one
  reused variable takes the last;
- `D_002488C0 + i * 0x180` inside the loop gives the game's strength-reduced pointer, set after the entry test (a
  walking pointer is set before it);
- two flag tests as nested `if`s give the game's two `bnel`; `!(f & 4) && !(f & 1)` folds into one `andi 5`.

**Parked (one-line causes; drafts in notes `ER-132_152_153_c/`):**

| Function | Cause |
|---|---|
| func_00178C60 | ER-132: same size (0xE18), 394 words differ, 731 of 902 instructions equal register-blind; left: register allocation (`alpha` in `$s6`, the game's `$s5`) and the load / store order of most stack-record blocks, the `D_0035C548` block (the game loads `x4` / `x6` before its products), the second digit loop's pointer, the `hp * 0x8A / 100` product (before the `== 100` test in the game). The draft's busy-wait uses `volatile` reads under ruling 3.16. Draft and tools in notes |
| func_001C8600 | 2 words: the `sw $a1, 0($a0)` kills `$a1`, so sched1's register-weight tie-break puts it first; the game behaves as if `x0`'s value did not die there (all 120 orders of 3 shapes, chained zeros, `s16` / `u16` `arg1`) |
| func_0018C9E8 | 2 words: `li 1` / `sh` swapped; the game's schedule needs no memory dependence between the `sh` through the loaded `x18` pointer and the `sw` to `+4`, which `-fno-strict-aliasing` always adds (11 shapes: `?:`, comma, `if` / `else`, temporaries) |
| func_001C8758 | 2 words: the `xC` load wins sched2's dependent-count tie over `x8` (its register is rewritten by `mfhi` later); 13 index spellings, `?:`, typed element pointers |
| func_0017D6B8 | 3 words (was 7): `*arg2` read first fixes the argument registers; `sw arg0` against `lhu 0x1E4` left (840 orders) |
| func_001C6F90 | 3 words: the three `$gp` stores against `ld $s0` (sched2 gives the load priority 5, the stores 4); 4 loop shapes x 6 orders, `s32` with `return 1` |
| func_001B34F8 | 4 words: the `x70` load has three dependents (copy, call, anti-dependence on the `D_00248880` store) and wins the tie over the `x24` load |
| func_001BE9C0 | 4 words (was 8): the `D_001FF340[0]` store first gives the game's `$v0` / `$v1`; then `sw $0` falls after the `swc1` (720 orders, base-pointer spellings) |
| func_001C4838 | 4 words: `0x8000` / `1` in `$v0` / `$v1` swapped; the `1` is a QImode pseudo (size 1), so local-alloc ranks it below the SImode `0x8000` (8 spellings of the store) |
| func_0018B8A8 | 7 words: the float read first moves the `lwc1`; the registers then differ (720 orders of each of two shapes) |

### 2.36 ER-154's files: the `x94 + xB0` shape and the near misses (ER-158 + ER-159, Mon 23 Nov 2026; Confirmed by `make regress`)

13 functions placed in seven files, 2,508 bytes (ER-158: 3, 560 bytes; ER-159: 10, 1,948 bytes), branch
`er-158-nine-files` (worktree `C:\rs\wt\er158`), commits `4486a32` (ER-158), `16928e2`, `e977c6e`, `87901ad` (ER-159).
Fresh drafts (ER-154's run folder was gone): `m2c_bulk.py --plain --tag ER-159 --label er159` on the 12 ER-159
functions (32 s; 12 compile, 0 MATCH). Scratch, drafts, a variant-trial script (`tools/var.py`: one template, many
substitutions, cmpfunc on each) and two RTL dumps are in `C:\rs\wt\er158_runs\` (our C only). Nothing
compiler-steering: no ruling form used.

| Lesson | Proved by |
|---|---|
| **An ignored return value still steers register choice: declare the return type the caller saw.** The call insn sets `$v0` (or `$f0`) even when the caller drops the value, so local-alloc gives the next pseudo the second register. The `x94 + xB0` shape (2.34's parked three): with `func_0012E820` declared `float` (it ends `mov.s $f0, $f4; jr $ra` in the game) the later `x94` load and the sum take the game's `$f1`, `xB0` takes `$f0`; with `func_0012FAA8` declared `s32` (its placed C) the `x6E` counter after the call takes the game's `$v1`. The file's `void` declarations gave `$f0` / `$v0`. No spelling of the sum could fix it (ER-154's 8 tries). **It cuts both ways**: in func_001397E0, `func_00130CE0` (placed as `s32`) must stay `void` as the file declares it, or the `count >= 0x22` test moves from the game's `$v0` to `$v1`; so the type to use is the one the game's caller behaves as if it saw, which is usually but not always the callee's own | func_0013B2B0, func_0013E760, func_0013F068, func_001397E0 |
| **A call written in both arms of an `if` / `else` keeps a store and a later load apart in sched1**: func_00137658's `x74 = func_0012E568(a, b)` with `a, b` chosen by the `if` (m2c's form, ER-154's tries) lets sched1 hoist the `x6E` counter load above the `x74` store; `x74 = func_0012E568(0xA, 0x1E)` / `x74 = func_0012E568(0x32, 0x50)` in each arm gives the game's store-then-load (the arms are merged after reload: the game's shared `jal; nop`). Found by compiling with `-fno-schedule-insns` (experiment only) | func_00137658 |
| **`arg0 << 4` is not `arg0 * 0x10` inside one address expression**: `*(s32 *)((char *)D_0035C554 + (arg0 << 4) + 0x20C)` gives the game's `addu $v0, base, index` (and the `sll` before `sd $ra`); `* 0x10` sums the other way round. A base-pointer local also matches; the file writes its other sums `<< 5` | func_001CD680 |
| **A byte parameter stored raw and narrowed only for a call is a full word**: the game stores `arg1`'s low byte (`sb`) from a plain copy and sign-extends only the call's copy; an `s8` parameter is extended on entry (2 words). `s32 arg1` with `(s8)arg1` in the call matches (the caller in `text_00150878` already declares `(s32, s32)`) | func_001CC848 |
| **A struct with an array member at the offset** (2.35 again, now for a counter array): `arg0->unk10[func_00111660(arg1)]++` gives the game's `addiu base, 0x10; addu base, index`; every pointer-sum and `s32 *` index spelling is one word shorter (7 tried) | func_001346D0 |
| **Global word array indexed, not m2c's byte offset**: `D_01B31D80[12] = arg0[0]` (and a `char *` base local, or a struct) keeps one `%lo(D_01B31D80)` base; m2c's `*(s32 *)((char *)&D_01B31D80 + 0x30)` makes a second base at +0x30 (6 words) | func_001CA8B0 |
| **m2c sized a 16-byte local 256** (`M2C_UNK sp[64]`): `f32 v[4]` gives the game's 0x40 frame; nothing else differed | func_0013CD68 |
| **Stores at a join come out in the reverse of their C order**: func_00139650 writes `x1C` then `x20` and the game stores `x20` first (filled into the branches' delay slots); `state = 1` written after the call goes into the `jal` slot as in the game | func_00139650 |
| Recipes 2.34 again: `s16` counters written `x += 1; if (x >= N)` (func_0013CE28, both counters); callback stores written `0xC4` first, then the game's order (func_001416B8: the first of 8 positions tried); a constant written inline twice instead of a temp (func_001397E0: a temp moves its `li.s $f20` ahead of the `x48` load) | func_0013CE28, func_001416B8, func_001397E0 |

**File declarations changed** (each proven by `make regress`): `text_0013DBC0` `func_0012FAA8` `void` -> `s32`,
`func_0012E820` `void` -> `float` (moved above func_0013E760; the later callers unchanged), `func_0013E760` `(void)` ->
`(s8 *, s32)`; `text_001343E8` `func_00131140` `void` -> `s32` (its type in most other files; func_00137658 tests its
result; the file's other callers unchanged); `text_001388E8` `func_0012E820` `void` -> `float` (func_00139560
unchanged). Unofficial names added: the type `Count346D0` (func_001346D0's object: a word array at 0x10, element count
Unconfirmed) and the locals `state`, `count`, `mode`, `ret` in func_00139650 / func_001397E0. `D_01B31D80` is read
as a word array (Unconfirmed).

**Parked (one-line causes; scratch in `C:\rs\wt\er158_runs\s\`, game code not in git):**

| Function | Cause |
|---|---|
| func_00143230 | 3 words (was 8): the game copies the `xEC` pointer to `$a0` before the `x64` store and loads `xCC` through `$a0` after it. RTL dump: regmove rewrites the `xCC` load to use `$a0`, so the pointer dies at the copy; sched1's tie (equal priority and register weight, then insn order) keeps the store first and local-alloc ties the pointer to `$a0` (no copy). Reading `xCC` first gets the game's registers, the load then stays before the store (3 words). 29 spellings (temps, comma forms, struct type, callee and own return types). func_00140A20 (unplaced) has the same shape |
| func_00140C18 | 4 words (was 8): reading `D_0035BB90` into a local before the `x48` store fixed the first block; left: the `func_0012DA68` call loads `x6C` first (an `s16` argument is precomputed into a pseudo, RTL dump), the game loads `x30` first and puts the `lh` in the `jal` slot. A float-first prototype gets 3 words but contradicts func_0012DA68's definition `(void **, s32, s32, f32, f32)`; `s16`, `u16`, `s32` argument types and casts tried |

### 2.37 `text_001000C0`: func_00119FE0 by hand, a syscall stub's caller (ER-093 + ER-163, Mon 23 Nov 2026)

ER-163 (commit `39b555e`, branch `er-093-text-001000c0`, worktree `C:\rs\wt\er093`): `text_001000C0.c` declares
func_001D03E0 `void` like `text_00150878.c` (2.35); its five calls in the file (func_00104300 and four in
func_001124E8) compile to the same bytes. Confirmed by `make regress`.

**func_00119FE0 (ER-093, 6,636 bytes, 1,659 instructions): parked at 3 words, same size as the game** (5 at first; the 2-word spot fell to 2.36's lesson, below). By hand from
the asm with ER-073's m2c draft as a map (the draft did not compile: `M2C_BREAK` on the `% 5` divide check, and m2c
cannot read `li.s`; `m2c_bulk.py` gets past both). It is func_0011BB90's big sibling (2.23): the same D_001FF390 view
work area, counters and matrix tail, plus the DualShock 2 pressure bytes of the pad buffer D_001F6590 (8..0x13,
Unconfirmed reading), a slot-cycling selection on D_001FF890 and an item use through the 0x100-byte records at
D_0035B8DC. **No jump table** (the census's "jump table" is a four-case `switch` compiled as a compare tree: MIPS has no
`casesi`, so `CASE_VALUES_THRESHOLD` is 5). Draft, header (struct layouts, all Unofficial) and the scripts below in the
dispatcher's notes folder `ER-093_c/` (our C only). First draft: 1,320 / 1,659 instructions equal register-blind;
after the lessons below, 1,654 of the 1,659 words are equal. Nothing compiler-steering.

Seen only in masked compares (Unconfirmed until a placement proves them):

| Lesson | Effect here |
|---|---|
| **Spill slots follow declaration order.** The three pressure values that lose their register are stored at `sp+0xA4`, `0xA8`, `0xAC` in the order their locals are declared (reload allocates slots by pseudo number), after the spilled `btn` at `0xA0` | declaring `pA8up, pA8dn, pA0dn` first gave the game's slot map; with the next two rows, 1,543 -> 1,573 instructions equal register-blind |
| **`if (p >= 9) { ... } else { r = 0.0f; }`** gives the game's `slti; bnel ...; sw $0` (the single-store else-arm is placed last and reorg steals it into the likely slot); `if (p < 9) r = 0; else { ... }` gives `beqz` + `b` | eight pressure blocks (with the rows above and below) |
| **Duplicated code in both arms, merged by jump2.** The game's decay of `xB0` after `x70 = r[0]` / `x70 = 0`, and the `D_001FF340[1] = xE4 - xE0 - xE8` assignment under `if (!(xC7 & 1)) ... else if (x100 != 0) ...`, are written twice; cross-jumping merges only the tail that matches the fall-through block, so each arm keeps its head (two orphan `lui $a0` / `lui $a1` of `%hi(D_001FF340)` stay in the game) | the second copy of the assignment: 4 words short -> 2 short, 322 -> 211 differing words |
| **A flag set in both arms**: `if (btn & 0x4000) k = 0; else { func_001CA048(a, a, 2.0f); k = 1; }` keeps the game's `b join; daddu k, $0` (k = 0 in the slot). With `k = 1` before the call, jump.c hoists `k = 0` above the branch | 801 -> 322 differing words (everything after it realigned) |
| **`s32 fl; fl \|= (s16)(a & b);`** gives `and; sll 16; sra 16; or` (the game); an `s16 fl` accumulator ORs first and sign-extends after. Fixing it let loop.c hoist the third loop constant (0.6) and the frame got the game's fifth saved FPR (`$f24`) | frame 0x170 -> 0x180, prologue exact (1,573 -> 1,582 equal) |
| **A down-counting clear loop wants the pointer**: `bp = &D.bits[31]; for (i = 31; i >= 0; i--) *bp-- = 0;` gives the game's `sb; addiu -1; addiu -1; bgez`; `D.bits[i] = 0` keeps `addu i, base` in the loop | 211 -> 207 words |
| **`if (c) D.xC1 = 4; else D.xC1 = 1;`** (two stores, cross-jumped into one `sb`) gives the game's `beqz; b; li 4 / li 1; sb`; `D.xC1 = c ? 4 : 1` gives a filled-slot form; a variable for the value moves the register allocation (61 words) | same size as the game, 207 -> 153 words |
| **One variable per live range** (2.23 again): the 0x4000 flag, the two-shot loop counter, the decay copies (`x`) and the later length (`len`) each need their own local; sharing `k` / `t` put them in saved registers or `$f12` | `len`: 153 -> 133, `x`: 133 -> 94, the flag / counter split with the next row: 94 -> 56 |
| **The second `D_001F5910[D_0035B824 * 32]` test written `!= 0 ? D9E0 : CF80`** (the first `== 0 ? CF80 : D9E0`; gcc canonicalises the first either way) gives the game's cross-jump, where the second block keeps both calls and the first block's `func_0011D9E0` call is merged into it | 61 -> 56 words |
| **Statement order inside `switch` cases** decides the schedule of independent loads: one order shared by all four cases (`pA8up, pA8dn, pB4dn, pB4up, pA0dn, pB0up, pA0up, pB0dn`) found by a hill-climb from m2c's order plus one swap (`swperm.py`, 219 compiles, about 50 s) | the switch's 39 differing words -> 0 (56 -> 17 overall) |
| **Store order of five float stores into a struct global**: the game's offset order (`x90 .. xA4`), found by trying all 120 orders | 10 -> 5 words |

**The words left (two spots at parking time; the second is solved in the draft):**

- **+0x16DC, 3 words** (`sw $s4, 0x74($v0)` against `lui $a1, %hi(D_001FF890)` after the second `func_0017D968`
  call): in sched2 both have priority 11 and issue in the same cycle; `rank_for_schedule` picks the store first
  because its class is 3 (data dependence of cost 1 on the call) against the `lui`'s 2 (anti dependence on the call,
  cost 0 by `ADJUST_COST`), and it has 5 dependents against 1. The game issues the `lui` first. Same family as
  func_001C8758 / func_001B34F8 (2.35: dependent-count ties). 24 statement orders, struct / pointer spellings of the
  store, no `ret` variable: no change.
- **+0x195C / +0x1970, 2 words, solved in the draft with 2.36's lesson** (`lw $v0, 0xFC($s1)` / `lbu $a1, 3($v0)` for the first `func_00113ED8` call):
  local-alloc gives the `D.fc` pseudo the first free register `$v0`, the game `$v1` (the second call matches with
  `$v0`); no copy suggestion applies (`find_free_reg`, numeric `REG_ALLOC_ORDER`), so the game must have had
  something live in `$v0` there that our RTL lacks. Same family as func_00124BE8 / func_00120EF0 / func_001C4838
  (`$v0` / `$v1` swaps). 12 spellings (types of `fc` and `D_0035B8CC`, index forms like func_0011C858's
  `D_0035B8CC + idx * 32`, temporaries) and 200 random declaration orders: no change. **Cause found after ER-158's 2.36:** with func_00113238 declared `s32` in the caller's view, the preceding call becomes a call_value that takes `$v0`, and the `D.fc` pseudo gets `$v1` as in the game (3 words left). The game's func_00113238 leaves nothing meaningful in `$v0` (its matched definition is `void`), so the original caller most likely saw an implicit / K&R `int` declaration (a reading, Unconfirmed). Placing this needs a ruling first: the definition in the same file would have to become `s32` with no `return` (ruling 3.4's shape, but a new function and not a tail-call case). Declaring each other `void` callee `s32` in turn does not move the +0x16DC spot.

**Scripts** (notes `ER-093_c/tools/`, paths are this card's worktree): `sc.py` (compile + register-blind aligned diff,
relocations masked from the asm file's `%hi` / `%lo` / `gp_rel` / `li.s` lines), `try.py` (one line per variant, par
4), `swperm.py` (shared statement order across switch cases: climb or all), `perm.py` (every order of a line range),
`declperm.py` (random local declaration orders), `drift.py` (where the size drifts). One compile of this function
takes about 0.2 s at par 4.

### 2.38 Return-type sweep: callee declarations across `src/` (ER-164 + ER-156, Mon 23 Nov 2026; Confirmed by `make regress`)

Branch `er-164-return-types` (worktree `C:\rs\wt\er164`), commits `52b5d30` and the next. The full list (function, real type,
evidence, files) and the scripts are in `docs/matching_tools.md` 6.19. 4 functions placed, 732 bytes (func_0013F3B0,
func_001524F8, func_001C4838, func_00111700); 24 declarations in 11 files changed to the game's return type (5 of them ER-156's func_0012FAE8), plus one ruling-3.19 prototype.

| Lesson | Proved by |
|---|---|
| **Most wrong return types are neutral in matched code**: setting a callee's declaration to its real type changed no placed caller in 49 of 56 (file, callee) pairs, so a file's `void` for a value-returning callee usually does no harm once its callers match. It matters while a caller is being matched (2.36), so fix the declaration first, then draft | `declflip.py` over the list (6.19); 24 declarations changed, `make regress` PASS |
| **The caller's view decides, both ways, and the evidence is in the game's callers**: func_00130CE0 (a pass-through whose definition says `s32`) is read by none of its 108 call sites, and seven placed callers need it `void`; func_00111480 has 35 call sites that read `$f0`, yet func_001306C0 matches only with `s32` (what its file declares); func_001C4838 matches only with the `void` word copy func_00101238 declared `s32` (ruling 3.19). Read from the game: a callee whose callers read `$v0` / `$f0` returns a value; one no caller reads may have been declared `void` (or `int`) by the original source, whatever its definition does | func_00130CE0, func_001306C0, func_001C4838 |
| **A pass-through wrapper's definition says nothing about its return type**: `s32 f(..) { return g(..); }` and `void f(..) { g(..); }` both compile to the same tail `j`, so the `s32` of func_00131000, func_00130FE0, func_0014FE10, func_0012FA68, func_001C1560 is a reading (Unconfirmed). Use the callers (6.19) | the five wrappers |
| **An m2c draft's harvested prototypes can be wrong in both directions**: m2c infers `int` for a callee that leaves junk in `$v0` (func_001C9B18, a `lq; sq` 16-byte copy) and `void` for one whose value goes unused. `flip.py` (each callee prototype as `void` / `s32` / `f32`) on the 103 same-size near misses found one match; the other 102 do not move, so return types are not their cause | func_001C4838 |

Not compiler-steering: no new ruling form beyond 3.19 (asked and accepted in this card) and one use of 3.14
(func_0013F3B0, drafted with it by `m2c_bulk.py`).

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
| **Accepted, narrowly** | An unused local variable or array (func_00113C30, func_00113ED8: e.g. `float buf[16];`) whose only effect is the stack frame size, when nothing else in plain C gives the original frame and the rest of the function matches; read as a leftover from the original source (a removed or debug use). Each use carries a comment naming this ruling. Not a licence for other dead code: code that only steers register choice or scheduling, `volatile` keep-alives and `asm()` still need their own ruling. Used so far: func_00113C30 (ER-089, 2.23); three by ER-100 (2.24); func_001668E0, func_001668A8 (ER-095, 2.25: `char buf[0x80]`); func_00113ED8 (ER-115, 2.27); func_001198C0, func_00101ED0 (ER-125, 2.29); func_001033F0, func_00103470 (ER-149, 2.33); func_001636A0 (ER-153, 2.35: `s32 buf[4]`, with ruling 3.14's `(void)&buf`) |

### 3.14 ER-100: an address-taken local that only stops a sibling call (Sun 22 Nov 2026, 16:40 Eastern, LuciDin in the ER-100 session)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** | `(void)&local;` (or `&` on an existing local or parameter), only where it is the sole blocker of a sibling call and the rest of the function matches: the game has `jal` + epilogue where plain C gives a tail `j`, because the compiler's Cygnus sibcall pass refuses sibcalls in a function that uses ADDRESSOF (2.24). Read as the original source taking some local's address. Each use carries the comment `ruling 3.14`. Used by ER-100: func_00149EE8, func_0014A500, func_0014A0B8, func_0014B528, func_00148BE0, func_00148D28, func_0014C110, func_001485A8; by ER-137: func_00162108, func_00165080 (`(void)&arg0`, drafted by `m2c_bulk.py`, 2.31); by ER-148: func_0014A238 (`(void)&arg0`, 2.32); by ER-154: func_00134618, func_00136D88, func_00137428 (`(void)&arg0`, drafted by `m2c_bulk.py`, 2.34); by ER-153: func_001636A0 (`(void)&buf` on ruling 3.13's array, 2.35); by ER-164: func_0013F3B0 (`(void)&arg0`, drafted by `m2c_bulk.py`, 2.38) |

### 3.15 ER-095: a 16-byte aligned float vector type (Sun 22 Nov 2026, about 17:35 Eastern, LuciDin in the DISP-03 session)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** | `__attribute__((aligned(16)))` on a 4-float vector type (func_00158DC0: `typedef struct { f32 v[4]; } __attribute__((aligned(16))) Vec4A;` for `D_00354D70`), as a data layout: Sony's libvu0.h declares `sceVu0FVECTOR` as `float[4]` with `aligned (16)`, so the original source plausibly used such a type. Not for steering code generation; any other attribute needs its own ruling. Each use carries a comment naming this ruling. (A plain two-`s64` struct also matched; the float vector is the kept reading, Unconfirmed.) Used so far: func_00158DC0 (ER-095, 2.25) |

### 3.16 ER-132: volatile reads and writes of hardware registers (Sun 22 Nov 2026, about 18:50 Eastern, LuciDin in the DISP-03 session)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** (first ruled "not accepted" at about 18:45, reversed after the explanation) | `volatile` only on reads and writes of memory-mapped hardware register addresses (EE timers, DMA, GIF / VIF, GS privileged registers and the like: e.g. func_00178C60's busy-wait on `0x1000A000` / `0x10003020`), where the game's code re-reads the register each time as the hardware changes it. It states a fact about the location, as Sony's own register access macros do (Unconfirmed until checked against an SDK header). Each use carries a comment naming this ruling. **Never on ordinary variables**: `volatile` keep-alives that only force a load or store stay forbidden. LuciDin: the goal stays a true 100% decomp as in Twinblade (3.1); every use of the narrow rulings (3.13-3.16) is listed here so it can be reviewed later. Used so far: func_001116D0, func_001116E0 (ER-125, 2.29: EE Timer 0 count `0x10000000`) |

### 3.17 ER-150: `do { ... } while (0)` that only stops a sibling call (Mon 23 Nov 2026, about 12:05 Eastern, LuciDin in the DISP-03 session)

| Ruling | Item |
|---|---|
| **Not accepted** | Wrapping calls in `do { ... } while (0)` only so the compiler keeps a `jal` instead of a sibling `j` (func_0014E450, case 4; recipes 2.24 / 2.30). Park such functions; a ruling-3.14 form (an address-taken local) may be tried where it fits. |

### 3.18 ER-148: an unused local whose stores the game makes (Mon 23 Nov 2026, 13:02 Eastern, LuciDin in the ER-148 session)

| Ruling | Item |
|---|---|
| **Accepted, narrowly** | A local that the code fills and never reads, where the game's binary really contains those stores (func_001486F0: four stores `-x70, 1.0, -x78, 0` to a 16-byte local at `sp+0` before `func_0012C578`, which reads only `$a0`-`$a3`): gcc keeps dead stores to stack memory, so the C reproduces stores the original made. Reading (Unconfirmed): a leftover, or an argument dropped from the call. Not for stores the game does not have, nor for code that only steers registers or scheduling. Each use carries a comment naming this ruling. Used so far: func_001486F0 (ER-148, 2.32: `f32 s[4];`) |

### 3.19 ER-164: a cross-file prototype declared as the caller saw it (Mon 23 Nov 2026, 16:41 Eastern, LuciDin in the ER-164 session)

| Ruling | Item |
|---|---|
| **Accepted** | In a file that does not define the callee, an `extern` prototype with a return type other than the definition's, when the caller matches only with that type and does not use the value (func_001C4838 in `text_00150878`: `extern s32 func_00101238(...)`, whose definition in `text_001000C0` is a `void` word copy; the `int` return makes the call set `$v0`, which gives the game's `$v0` / `$v1` choice after it, 2.36 / 2.38). Read as the original source declaring the callee differently (or not at all: an implicit `int`). The definition keeps its own type. Precedent in placed code before the ruling: `text_00150878` declares the `void` func_00113238, func_0012E3B0, func_001321D8, func_001321E8 `s32` (neutral there). Each use carries a comment naming this ruling. **Not** covered: changing a definition's type, or a same-file declaration that contradicts the definition (func_00113238 for func_00119FE0 in `text_001000C0`, recipes 2.37: still open). Used so far: func_001C4838 (ER-164, 2.38) |

No other ruling yet on matching tricks in C (`asm()`, `volatile` keep-alives, other dead code; 3.13 covers only frame-sizing locals, 3.14 only the sibcall blocker, 3.18 only stores the game makes): park such functions with a
one-line cause and ask.
