# Census of the main EE ELF and Phase 1 cost estimate (ER-004)

Written Tue 17 Nov 2026 by ER-004 (Opus 5.5, medium). Every number here is produced by `tools/census.py` from the
ER-003 split; the per-function table and the evidence lists are generated into `build/census/` (gitignored, derived
from game code). Section 7 lists what is Unconfirmed or not verified. **Section 6 is an estimate, not a
measurement.**

## 1. How to rerun

```
$env:PATH = "C:\msys64\usr\bin;" + $env:PATH
make split
make build/SLUS_200.15.syms.elf
.venv\Scripts\python.exe tools/census.py --md     # about 1 s; --md prints the tables below
```

| Output (`build/census/`) | Content |
|---|---|
| `functions.csv` | one row per unit: address, size, band, kind (C / asm / crt0 / data-block), region, work category, three shape keys, family sizes, calls out / tail jumps / callers / pointer uses, jal targets, counts of MMI, COP1, COP2, COP0, `lq`/`sq`, pipeline-1 ops, `$gp` uses, `jr` on a register, `jalr`, syscalls (with the `$v1` number), spimdisasm's handwritten marks, `.word` lines |
| `families.csv` | strict and loose families with 2+ members, representative, members |
| `refs.txt` | per function: strings and data symbols it references (the evidence behind section 4) |
| `summary.json` | the totals quoted here |

## 2. Method

- **Units**: the 3,606 function symbols with a size in `build/SLUS_200.15.syms.elf` plus the 3 blocks splat
  labels as data (`func_001D73D0`, `func_001D7658`, `func_001DECA0`) = **3,609**, the same total as the tracker's
  `m-dec-funcs`. The 3 alternate entry points (`func_001D7700`, `func_001D77A8`, `func_001D7850`) lie inside
  `func_001D76D0`, `func_001D7778` and `func_001D7820` (three near-identical 41-42 instruction VU0 / COP1
  functions) and are not separate units. Code bytes come from the syms ELF (byte-identical to the original).
- **Decoding**: rabbitizer 1.16.2, R5900 mode. An address tracker follows `lui` / `addiu` / `ori` pairs, copies
  (`daddu rd, rs, $0`) and `$gp` (0x00360B70) to find data and code references. It is linear (no control flow), so a
  few references are misreads (for example `func_001C9680` / `func_001E1FD0` appear to point into the middle of a
  `.rodata` string); section 4 uses only evidence that does not rest on a single such reference.
- **Shape keys** (sha1 of the instruction stream, 12 hex digits):
  - *strict*: instruction words with address parts masked (jal targets; j targets outside the function, inside
    ones made relative; `lui` immediates in the image range; 16-bit offsets of instructions based on a tracked
    address register or `$gp`). Equal strict keys = the same code at other addresses (clone).
  - *regs*: opcode and registers kept, every immediate and target masked.
  - *loose*: opcode only (registers and immediates masked): near-identical shapes.
- **Size bands** in instructions (playbook bands, with 257-400 split from 401+). "Empty" = `jr $ra; nop`.
- **Regions** (section 4): string references, syscall stubs, the position of each function's referenced `.data`
  relative to the `PsII<lib><version>` tags in `.data`, the call graph, and object-end padding.

## 3. Census

### 3.1 By size band (all 3,606 function symbols; the 3 data blocks are left out of bands)

| Band (instr.) | Functions | Instructions | Already C | Asm | of which game | of which SDK |
|---|---|---|---|---|---|---|
| empty (jr ra; nop) | 127 | 254 | 127 | 0 | 125 | 2 |
| 3-8 | 533 | 2,444 | 0 | 533 | 289 | 241 (+3 crt0) |
| 9-16 | 397 | 4,920 | 0 | 397 | 300 | 98 |
| 17-32 | 690 | 16,186 | 0 | 690 | 573 | 119 |
| 33-64 | 739 | 34,025 | 0 | 739 | 570 | 168 (+1 crt0) |
| 65-128 | 640 | 58,203 | 0 | 640 | 537 | 103 |
| 129-256 | 316 | 55,153 | 0 | 316 | 287 | 29 |
| 257-400 | 96 | 30,045 | 0 | 96 | 89 | 7 |
| 401+ | 68 | 42,979 | 0 | 68 | 60 | 8 |

(The game / SDK columns also count the 3 data blocks, all SDK: 1 in 9-16, 2 in 17-32.) The largest function is a
game function of 1,659 instructions; the largest SDK ones have 1,350 and 1,139 (newlib `vfprintf` and `strtod`
shapes, judged from their strings).

### 3.2 Instruction features (functions using each at least once)

| Feature | Functions | Note |
|---|---|---|
| COP1 (single float) | 1,509 (1,458 game) | half the game code uses floats: float scheduling will be a main matching theme |
| `$gp`-relative access | 1,236 | small data and `.lit4`; ER-003's `gp_externs.inc` covers the build side |
| COP2 / VU0 macro | 113 | 26 in the game's own VU0 helpers, 33 in libvu0, rest scattered |
| MMI | 60 | mostly libmpeg (IDCT-like code), newlib string functions, a few game functions |
| `lq` / `sq` (128-bit) | 76 | |
| pipeline-1 ops (`mult1`, `div1`, `mflo1` ...) | 25 (13 game) | EE-specific scheduling by the compiler; worth a recipe once seen in C |
| `jr` on a register (jump tables) | 75 (66 game) | |
| `jalr` (calls through pointers) | 155 | |
| `syscall` | 140 | 138 in the libkernl stub table, 2 in crt0 |
| COP0 / `cache` / `sync` / `ei` / `di` | 39 | SDK only |
| spimdisasm "handwritten" | 188 (186 SDK + 2 crt0) | no game function is marked |
| `.word` lines in the asm | 490 lines in 245 functions | almost all are `cvt.w.s` (written as `.word` to keep the bytes); 238 game functions |
| words rabbitizer cannot decode | 6 words in 3 units | exactly the 3 data-labelled blocks (VU0 forms) |

### 3.3 Shapes (all functions except crt0 and the 3 data blocks)

| Key | Distinct shapes | Singletons | Families (2+) | Functions in families | Copies beyond the first |
|---|---|---|---|---|---|
| strict | 3,196 | 3,064 | 132 | 538 | 406 |
| regs | 2,911 | 2,705 | 206 | 897 | 691 |
| loose | 2,864 | 2,641 | 223 | 961 | 738 |

(These counts include the 127 empty C functions as one family of 127; excluding it, strict copies beyond the first
are **273**: 237 game, 36 SDK.) One image with no overlays means far less duplication than a PS1 overlay game:
**clones save about 8% of the functions** and about 2% of the instructions.

## 4. SDK library code and game code

### 4.1 Regions in link order

`.text` is crt0, then game objects, then SCE and GNU libraries. Seam window = the range in which the region's
first function lies when the evidence does not fix it (one address = fixed).

| Region | Kind | Start | End | Seam window | Grade | Functions | Instr. | Handwritten | VU0 / MMI | COP1 |
|---|---|---|---|---|---|---|---|---|---|---|
| crt0 | crt0 | 0x00100000 | 0x001000C0 | fixed | Confirmed | 4 | 46 | 2 | 0 | 0 |
| game | game | 0x001000C0 | 0x001C9B18 | fixed | Confirmed | 2,725 | 205,159 | 0 | 43 | 1,456 |
| game-vu0 | game | 0x001C9B18 | 0x001CA0F0 | fixed | Unconfirmed | 28 | 358 | 0 | 26 | 2 |
| game-unk | game | 0x001CA0F0 | 0x001CE130 | fixed | Unconfirmed | 77 | 4,075 | 0 | 1 | 0 |
| libgraph | sdk | 0x001CE130 | 0x001CFD40 | 0x001CE130-0x001CE270 | Confirmed | 18 | 1,781 | 0 | 6 | 0 |
| libkernl | sdk | 0x001CFD40 | 0x001D46E0 | 0x001CFAD8-0x001CFD40 | Confirmed | 260 | 4,658 | 158 | 0 | 3 |
| libdma | sdk | 0x001D46E0 | 0x001D5200 | 0x001D46E0-0x001D4718 | Confirmed | 21 | 699 | 0 | 0 | 0 |
| sdk-vudev | sdk | 0x001D5200 | 0x001D6280 | fixed (start) | Unconfirmed | 45 | 1,041 | 0 | 24 | 0 |
| libpad | sdk | 0x001D6280 | 0x001D7310 | 0x001D5868-0x001D6280 | Confirmed | 30 | 1,042 | 0 | 7 | 0 |
| libvu0 | sdk | 0x001D7310 | 0x001D8050 | fixed | Unconfirmed | 42 | 828 | 6 | 33 | 16 |
| libcdvd | sdk | 0x001D8050 | 0x001DAA88 | fixed | Confirmed | 44 | 2,684 | 0 | 1 | 0 |
| libmpeg | sdk | 0x001DAA88 | 0x001E2128 | 0x001DAA88-0x001DB2C8 | Confirmed | 131 | 7,520 | 18 | 19 | 0 |
| libipu | sdk | 0x001E2128 | 0x001E2788 | 0x001E1D78-0x001E2578 | Unconfirmed | 11 | 400 | 0 | 0 | 0 |
| libsdr | sdk | 0x001E2788 | 0x001E2D00 | fixed | Unconfirmed | 7 | 346 | 0 | 0 | 1 |
| libmc | sdk | 0x001E2D00 | 0x001E3DE0 | fixed | Confirmed | 22 | 1,070 | 0 | 0 | 0 |
| libm (newlib) | sdk | 0x001E3DE0 | 0x001E6508 | fixed | Confirmed | 21 | 2,496 | 0 | 0 | 20 |
| libgcc | sdk | 0x001E6508 | 0x001E9A50 | 0x001E64D8-0x001E6560 | Unconfirmed | 41 | 3,390 | 0 | 1 | 11 |
| libc (newlib) | sdk | 0x001E9A50 | 0x001F00E8 | 0x001E9A10-0x001E9A50 | Confirmed | 76 | 6,532 | 0 | 7 | 0 |
| libkernl-tail | sdk | 0x001F00E8 | 0x001F0354 | fixed | Unconfirmed | 6 | 150 | 4 | 0 | 0 |

Totals: crt0 4 functions; **game 2,830 functions (209,592 instr.)**; **SDK 775 units (34,637 instr.)**, of which
**623 in Confirmed regions** and 152 in Unconfirmed ones. "Confirmed" grades the region's library identity; the
exact first function of a region is Confirmed only where the seam window is a single address.

### 4.2 Evidence per region

| Region | Evidence |
|---|---|
| crt0 | SCE `crt0.s` shape (ER-003): clears `.bss` with `sq`, sets `$gp` and the stack, syscalls `0x3C` and `0x3D` (SetupThread / SetupHeap in ps2sdk's syscall numbering), `jal func_001D03E0` with `$a0 = 0` (the libkernl stub for syscall 0x64, FlushCache in that numbering), `ei`, `main`, then `j func_001CFD80` (stub for syscall 0x04, Exit). |
| game | `main` = `func_001000C0`; game-specific strings (memory card UI messages, `cdrom0:\%s;1` file paths, a `.PSS` movie name) up to `0x001C9680`; references into game `.data` (below the first `PsII` tag at 0x00351210). The call graph has a clean cut at `0x001C92D0`: no function above it calls anything below (the 2 functions from there to `0x001C9B18` are game by their data references). |
| game-vu0 | 28 leaf functions of 3-47 instructions, almost all COP2 (VU0 macro) or `lq`/`sq`; no strings or data; one shape is shared with libvu0 (`func_001C9B18` / `func_001C9B28` = `func_001D75B8`), the other 27 are not. They sit before libgraph in link order, so they belong with the game objects (a game-side VU0 helper file, or an SDK sample source compiled into the game: Unconfirmed). |
| game-unk | 77 functions with no strings and no `.rodata` at all; they reference game `.data` (0x0034D1D0) and large `.bss` buffers, create / use threads and semaphores through libkernl, one 449-instruction function uses MMI. Called from game code. Linked before libgraph, so with the game objects; game code or third-party middleware (sound or streaming manager?): **Unconfirmed**. |
| libgraph | `sceGs*` error strings (`sceGsPutDrawEnv`, `sceGsSyncPath`, `sceGsSetDefLoadImage`, `sceGsExecLoadImage`, `sceGsExecStoreImage`); its `.data` starts at `PsIIlibgraph1620` (0x00351210) and its `.rodata` directly after the game's. Seam: `func_001CE130` is called from game init and calls libgraph and libkernl but references no data, so it may be the last game-side function; `func_001CE270` is the first that touches libgraph `.data`. |
| libkernl | the syscall stub table from `0x001CFD40` (4-instruction handwritten stubs: `addiu $v1, N; syscall; jr $ra; nop`, N from 0 to 0x7F and negative numbers down to -120), `TTY:` strings, `rom0:UDNL` (IOP reboot / module loading), `.data` after `PsIIlibkernl1620`. Seam: the three functions `0x001CFAD8`-`0x001CFC28` before the stubs reference no data and are called from libgraph and game code: libgraph's tail or libkernl's head. |
| libdma | `libdma: sync timeout` and `D*: not installed yet` strings; `.data` around `PsIIlibdma  1620` (0x003512C0). Seam: `func_001D4630` (cop0-heavy, called by libkernl) ends at `0x001D46DC`, where a lone `nop` (a 1-instruction "function" to spimdisasm) pads to the next 8-byte boundary: an object end. |
| sdk-vudev | 45 functions writing VU / VIF control state (`cfc2` / `ctc2` on `$vi28`), two jump tables in `.rodata` between libdma's and libpad's strings; no version string. A VU / VIF device library (libdev-like) or part of libdma / libpad: **Unconfirmed**. |
| libpad | `libpad:` strings; `.data` after `PsIIlibpad  1630`. The seam with sdk-vudev is open from `0x001D5868` (a run of `lq`/`sq` functions with no data references). |
| libvu0 | 42 VU0 macro / COP1 functions (vector and matrix helpers; the three near-identical rotation-like functions with alternate entries; the 3 data-labelled blocks); `.data` between libpad's and libcdvd's tags; no `PsII` tag found for it. **Unconfirmed** as SCE libvu0 (no name evidence), Confirmed as library code by position. |
| libcdvd | `Libcdvd` / `sceCd*` strings, `.data` after `PsIIlibcdvd 1650` (0x003513C0). `func_001D8050` already uses libcdvd `.data`. |
| libmpeg | MPEG-2 decoder strings (picture / macroblock / motion type errors, `[MPEG ERROR]`), `.data` after `PsIIlibmpeg 1620`; 19 functions with MMI, 18 of them a run of MMI-heavy IDCT / motion-compensation-like shapes; 18 handwritten. Seam: `0x001DAA88`-`0x001DB258` use `.data` between libcdvd's and libmpeg's tags and lead into the PS demultiplexer: libcdvd's tail or libmpeg's head. |
| libipu | `func_001E2578` uses `.data` right after `PsIIlibipu 1620`; `0x001E2128`-`0x001E2510` are small register helpers called from libmpeg. `func_001E1D78` (handwritten, cop0) and `func_001E1E28` (a CSC error string) use `.data` just below the tag: libmpeg's tail or libipu's head. |
| libsdr | `.data` right after `PsIIlibsdr  1620`; `func_001E2788` is called from game init. No library-specific string: **Unconfirmed** by name, position only. |
| libmc | `libmc:` / `bind error libmc` strings, `.data` after `PsIIlibmc   1650`. |
| libm | newlib float wrappers carrying their own names (`acosf`, `asinf`, `atan2f`, `sqrtf`) and calling `__errno` (`func_001E9A50`); the two 58-60 instruction float functions before them are called 59 and 87 times from the game (sin / cos shapes: Unconfirmed). |
| libgcc | `func_001E6508` walks a table of function pointers until a null (GCC's global destructor list); from `0x001E8178` the soft-double routines of GCC's `fp-bit.c` (unpack both operands, operate, pack: `func_001E82A8` unpack, `func_001E8178` pack, `func_001E8A58` compare parts), called by game code. Identity by call structure only: **Unconfirmed**. ER-041: the fp-bit block is GCC's `fp-bit.c` built for GOFAST names (`dpadd`, `dpcmp`, `fptodp`, ...), Confirmed by the toolchain source and three callers proven under `make regress` (4.4). |
| libc | newlib: `vfprintf` (`bug in vfprintf: bad base`, `(null)`, hex digit tables), `strtod` (`Infinity`, `NaN`), `_impure_ptr` users, `__errno`, MMI string routines (word-at-a-time `0x01010101` tests), reentrant syscall wrappers (`errno = 0; call; if -1 ...`) calling libkernl's file stubs. |
| libkernl-tail | data-cache maintenance loops (`cache` + `mfc0 $28` TagLo), called from libpad: libkernl objects pulled in late by the archive order (libc before libkernl). **Unconfirmed**. |

Library order in `.data` (`PsII` tags, file `0x252210`-`0x253008`): libgraph 1620, libkernl 1620, libdma 1620,
libpad 1630, libcdvd 1650, libmpeg 1620, libipu 1620, libsdr 1620, libmc 1650. `.text` follows the same order,
with libvu0 (no tag) between libpad and libcdvd and an untagged VU / VIF block between libdma and libpad. These are
SDK runtime 1.6.x versions, consistent with ER-002.

### 4.3 File / object boundaries for ER-013

Cut points (first address of each new file), in address order. ER-004 gave the table with seam windows; **ER-013
(Wed 18 Nov 2026) cut `.text` at exactly these 19 addresses** (one `c` file per row, `docs/build.md` 3.1) and chose
each windowed seam from the evidence in the last column. `make regress` is green on the split.

| # | Cut at | File | ER-004 seam | Seam chosen by ER-013, and why | Grade |
|---|---|---|---|---|---|
| 1 | 0x00100000 | `asm/text_00100000.s` (crt0) | fixed | as given | Confirmed |
| 2 | 0x001000C0 | `src/text_001000C0.c` (game) | fixed | as given | Confirmed |
| 3 | 0x001C9B18 | `src/text_001C9B18.c` (game VU0 helpers) | fixed | as given | Unconfirmed |
| 4 | 0x001CA0F0 | `src/text_001CA0F0.c` (game-unk) | fixed | as given | Unconfirmed |
| 5 | 0x001CE130 | `src/sdk/libgraph.c` | 0x001CE130-0x001CE270 | 0x001CE130: it calls `func_001CE270`, a helper called only by libgraph functions (5 of them besides it) | Unconfirmed (judgement) |
| 6 | 0x001CFD40 | `src/sdk/libkernl.c` | 0x001CFAD8-0x001CFD40 | 0x001CFD40: the only 20-byte zero pad in `.text` ends here, on a 64-byte boundary (object alignment); `func_001CFB60` / `func_001CFC28` are called only from libgraph, so the three functions before the stubs stay in the libgraph file | Confirmed seam (padding); the 3 functions' library Unconfirmed |
| 7 | 0x001D46E0 | `src/sdk/libdma.c` | 0x001D46E0-0x001D4718 | 0x001D46E0: the lone `nop` at 0x001D46DC pads the previous object to 8 bytes | Confirmed seam (padding) |
| 8 | 0x001D5200 | `src/sdk/vudev.c` | fixed | as given | Unconfirmed |
| 9 | 0x001D6280 | `src/sdk/libpad.c` | 0x001D5868-0x001D6280 | 0x001D6280, the window's end: the first function with a `libpad:` string; the run before it is called only from the VU / VIF block and game code, with no link to libpad | Unconfirmed (judgement) |
| 10 | 0x001D7310 | `src/sdk/libvu0.c` | fixed | as given | Unconfirmed |
| 11 | 0x001D8050 | `src/sdk/libcdvd.c` | fixed | as given | Confirmed |
| 12 | 0x001DAA88 | `src/sdk/libmpeg.c` | 0x001DAA88-0x001DB2C8 | 0x001DAA88: its only caller `func_001DB008` calls libmpeg, and the window's functions share `.data` `D_00351430` with functions that call libipu (libcdvd would not) | Unconfirmed (judgement) |
| 13 | 0x001E2128 | `src/sdk/libipu.c` | 0x001E1D78-0x001E2578 | 0x001E2128: `func_001E1E28` and `func_001E1FD0` are called from libmpeg and `func_001E1FD0` calls libmpeg back, so 0x001E1D78-0x001E2124 stay with libmpeg; from 0x001E2128 small register helpers that call nothing outside themselves | Unconfirmed (judgement) |
| 14 | 0x001E2788 | `src/sdk/libsdr.c` | fixed | as given | Unconfirmed |
| 15 | 0x001E2D00 | `src/sdk/libmc.c` | fixed | as given | Confirmed |
| 16 | 0x001E3DE0 | `src/sdk/libm.c` | fixed | as given | Confirmed |
| 17 | 0x001E6508 | `src/sdk/libgcc.c` | 0x001E64D8-0x001E6560 | 0x001E6508: `func_001E64D8` is called only by libm's `func_001E6378`, so it stays in libm; whether the uncalled destructor walker `func_001E6508` opens libgcc or ends libm is not settled | Unconfirmed (judgement) |
| 18 | 0x001E9A50 | `src/sdk/libc.c` | 0x001E9A10-0x001E9A50 | 0x001E9A50: `func_001E9A10` calls the fp-bit routines `func_001E8DE8` / `func_001E8F80` (libgcc); `func_001E9A50` is `__errno`-shaped (reads `_impure_ptr`-like `D_003522FC`) | Confirmed seam (call graph) |
| 19 | 0x001F00E8 | `src/sdk/libkernl_tail.c` | fixed | as given | Unconfirmed |

**ER-022** (Wed 18 Nov 2026) cut the game file (row 2) further at 7 game-internal seams (`0x001343E8`,
`0x00145120`, `0x0014A968`, `0x0014C840`, `0x0014D930`, `0x0014E2E8`, `0x00150878`); evidence and grades in
`docs/game_seams.md` 8, layout in `docs/build.md` 3.1.

File names are address names or the census's region labels (`vudev`, `libkernl_tail`: **Unofficial**). Objects
are 8-aligned (every function's asm starts `.align 3`), so a seam on any function start links unchanged; the
padding words before a seam stay at the end of the previous function's `.s`.

**Data is not split** (ER-013): the `PsII` tags mark a position inside each library's `.data`, not its first byte
(libmpeg's window functions use `.data` below its tag, for example), so no `.data` / `.rodata` / `.sdata` / `.bss`
boundary is known exactly; the data subsegments stay as ER-003 left them.

Seen in passing (not settled): `func_001E1FD0` (libmpeg side) loads the address of the game `.rodata` string
`ERDEMO.PSS` (0x00354000) and uses game `.data`; either a game movie hook compiled into a libmpeg-side object or a
coincidental constant: **Unconfirmed**.

Not found by this card: **object boundaries inside the game region** (2,725 functions) and inside each library.
Tried and ruled out: alignment padding (functions are 8-aligned and the 0/4-byte gaps occur everywhere; one
20-byte gap only), `.lit4` (ee-as does not merge literals: 2,817 entries hold only 258 distinct values, laid out in
function order, so they give no positive seam). Candidates for a later card: monotone runs of each function's
`.rodata` / `.data` / `.sdata` references (each object's data is contiguous in link order), static-looking call
clusters, and string themes.

### 4.4 Names the compiler emits: `sqrtf` and the GOFAST soft-double routines (ER-041, Sun 22 Nov 2026)

gcc calls some library routines **by name** without the C naming them: double arithmetic, double comparisons and
float / double / int conversions become calls to libgcc's fp-bit routines (the EE FPU is single precision), and
`sqrtf()` becomes an inline `sqrt.s` plus a call to `sqrtf` when the result is NaN (`docs/decomp_recipes.md` 2.7).
A C function that does these links only if the symbols carry those names, so they are in `symbol_addrs.txt`.

**Which names: GOFAST, not `__adddf3` / `__ltdf2`.** A probe compiled with the build's ee-gcc (`-O2 -G8`) calls
`dpadd` (+), `dpsub` (- and unary minus, as `0.0 - x`), `dpmul`, `dpdiv`, **`dpcmp` for all six comparisons**
(`<`, `<=`, `>`, `>=`, `==`, `!=`; the caller tests the sign of `$v0`), `fptodp` (float -> double), `dptofp`
(double -> float), `litodp` (int -> double), `dptoli` (double -> int), `dptoul` (double -> unsigned), and
`__fixdfdi` / `__floatdidf` / `__fixunsdfdi` for 64-bit integers. The toolchain's own source says why (`C:\rs\toolchain\src-lf\ee\gcc\config`):
`mips/t-r5900` builds `fp-bit.c` and `dp-bit.c` whole as `LIB2FUNCS_EXTRA`, each with `#define US_SOFTWARE_GOFAST`
(and `NO_DENORMALS`); `mips/elf64.h` sets `INIT_SUBTARGET_OPTABS INIT_GOFAST_OPTABS`; `gofast.h` maps every DF
comparison libfunc to `dpcmp`; `fp-bit.c` lines 231-255 define `add` -> `dpadd`, ..., `df_to_sf` -> `dptofp`,
`sf_to_df` -> `fptodp`. The game agrees: only **one**
double compare routine is linked (`func_001E8B70`), and it has no NaN test, i.e. fp-bit's `compare` (`dpcmp`),
not one of `_lt_f2`, `_gt_f2`, ... (which return a fixed value for NaN). ER-035 / ER-041's working guesses
(`__extendsfdf2`, `__ltdf2`, `__subdf3`) were wrong for this toolchain.

| Address | Name | Evidence (all read from the game's code) | Grade |
|---|---|---|---|
| 0x001E42E0 | `sqrtf` | newlib `wf_sqrt.c` wrapper line by line: `__ieee754_sqrtf` (`func_001E5290`), `_LIB_VERSION == _IEEE_` (-1) at `D_003582C0`, `isnanf` (`func_001E6350`), `x < 0` -> `struct exception` with type 1 (DOMAIN), name = the string `"sqrtf"` at `D_00357DD0`, `arg1 = arg2 = (double)x`, retval 0.0 for `_SVID_` else `D_00357DD8` (`0x7FF8000000000000`, the folded `0.0/0.0`), `errno = EDOM` (33) through `__errno`-shaped `func_001E9A50`, `matherr` (`func_001E5F78`), `return (float)exc.retval`. **Proven:** `func_0012D568` (calls it through gcc's builtin) matched under `make regress` | Confirmed |
| 0x001E8588 | `dpadd` | unpack both (`func_001E82A8`), `_fpadd_parts` (`func_001E8348`), pack (`func_001E8178`) | Confirmed (code; no placed caller yet) |
| 0x001E85E0 | `dpsub` | as `dpadd` but flips b's sign (`xori 1` on the unpacked sign word) first. **Proven:** `func_001411E8` (`fabs` of a double) matched under `make regress` | Confirmed |
| 0x001E8648 | `dpmul` | unpack both, 64-bit partial products through `func_001E7608` (a `__muldi3`-like helper: the r5900 has no `dmult`), pack; source order after `sub` | Confirmed (code) |
| 0x001E88F0 | `dpdiv` | unpack both, shift-and-subtract loop, pack; source order after `multiply` | Confirmed (code) |
| 0x001E8A58 | (fp-bit `__fpcmp_parts_d`, left address-named) | NaN -> 1; both infinite -> `b->sign - a->sign`; zero, sign, exponent, fraction tests in fp-bit's order | Unconfirmed name, not needed |
| 0x001E8B70 | `dpcmp` | unpack both, `__fpcmp_parts_d`, no NaN test (fp-bit `compare`). **Proven:** `func_001399B8`, `func_001411E8` matched under `make regress` | Confirmed |
| 0x001E8BC0 | `litodp` | int -> unpacked number, pack only (fp-bit `si_to_float`) | Confirmed (code) |
| 0x001E8C78 | `dptoli` | unpack; NaN / zero -> 0; saturates to `0x7FFFFFFF` / `0x80000000` above exponent 30; negates for sign (fp-bit `float_to_si`) | Confirmed (code) |
| 0x001E8D10 | `dptoul` | unpack; saturates to `0xFFFFFFFF` above exponent 31, shifts left above 60 (fp-bit `float_to_usi`) | Confirmed (code) |
| 0x001E8E18 | `dptofp` | unpack double, `__make_fp` (`func_001E99E0`); called by the libm wrappers for `return (float)exc.retval` | Confirmed (code) |
| 0x001E9A10 | `fptodp` | unpack float (`func_001E8F80`), `__make_dp` (`func_001E8DE8`); called by the libm wrappers for `exc.arg1 = (double)x`. **Proven:** `func_001399B8`, `func_001411E8` | Confirmed |

**Proven** = placed in `src/` in ER-041's worktree with `make regress` green (whole-ELF SHA-1 and all 3,606
functions), then put back to INCLUDE_ASM before the commit, because their files belong to other wave 3 cards
(`text_001343E8.c` ER-031, `text_001000C0.c` ER-063); the C is in the dispatcher's notes folder, `ER-041_c/`, for
the file owners to place. Control: with the three proof functions placed and the names removed, the link fails with undefined `sqrtf`,
`fptodp` and `dpcmp`; with the names, `make regress` passes. The renames alone change no byte (whole-ELF SHA-1 and
all 3,606 functions unchanged).

**Layout of the libgcc fp-bit block (left address-named: gcc never emits these names for the EE; names from the
source's macros, the address of each by order and shape, none proven by a match):** double object `0x001E8178`-`0x001E8E70` in fp-bit's source order (pack, unpack, `_fpadd_parts`, add, sub,
multiply, divide, `__fpcmp_parts`, compare, `si_to_float`, `float_to_si`, `float_to_usi`, negate `func_001E8DB0`,
`__make_dp` `func_001E8DE8`, `df_to_sf`), then the single-float object `0x001E8E70`-`0x001E9A50` in the same order
(`fpadd` `func_001E9248`, `fpsub` `func_001E92A0`, `fpmul` `func_001E9308`, `fpdiv` `func_001E9500`, `fpcmp`
`func_001E9778`, `sitofp` `func_001E97C8`, `fptosi` `func_001E9880`, `fptoui` `func_001E9910`, `__negsf2`
`func_001E99A8`, `__make_fp` `func_001E99E0`, then `fptodp`). Functions nobody calls (negate, the float arithmetic)
are present because each file is one whole object (`LIB2FUNCS_EXTRA`, no `FINE_GRAINED_LIBRARIES`); internals are
`__pack_d` / `__unpack_d` / `__fpcmp_parts_d` (and `_f`), `_fpadd_parts` is static. `func_001E6D20` / `func_001E6E10` /
`func_001E6F08` (libgcc2 64-bit conversions calling the dp routines) are not identified.

**Not renamed:** `func_001E9B0C` (in ER-041's candidate list) is not fp-bit: it is libc's `memset` by shape (args
dst / byte / count, byte replicated with `pcpyh` / `pcpyld` and stored with `sq` when 16-aligned; 307 game callers).
Name Unconfirmed (no string), and game C calls it explicitly, which links under the address name. Likewise the other
libm wrappers (`acosf` / `asinf` / `atan2f`, `func_001E3FB8` / `func_001E40B8` / `func_001E41B8`, names from their
own `exc.name` strings) are called explicitly and stay address-named for now.

## 5. Clone families

132 strict families (2+ members) among the non-C functions: 105 game-only, 18 SDK-only, 9 spanning both (tiny
shared shapes: 3-11 instruction stubs, e.g. a `jr $ra` + return value, libvu0's vector copy). Family sizes: 85
pairs, 26 triples, 21 with 4-9 members, 5 with 10+ members.

Largest by instructions saved (copies x size), representative = lowest address:

| Representative | Region | Members | Instr. | Instr. saved |
|---|---|---|---|---|
| func_0017DBE8 | game | 25 | 17 | 408 |
| func_00186DD8 | game | 23 | 13 | 286 |
| func_0019EBC0 | game | 2 | 255 | 255 |
| func_0015E2C0 | game | 3 | 125 | 250 |
| func_00187BD8 | game | 2 | 232 | 232 |
| func_0017DB98 | game | 11 | 19 | 190 |
| func_0019E1D0 | game | 2 | 120 | 120 |
| func_001B0510 | game | 3 | 59 | 118 |
| func_0017D598 | game | 5 | 27 | 108 |
| func_001342C8 | game | 7 | 17 | 102 |
| func_001D0950 | libkernl | 5 | 25 | 100 |
| func_00133868 | game | 2 | 99 | 99 |
| func_001904F8 | game | 2 | 99 | 99 |
| func_0019D620 | game | 2 | 97 | 97 |

Loose (opcode-only) families add near-relatives: 223 families, 961 functions; the biggest is the libkernl syscall
stub table (137 stubs, 135 strict shapes differing only in the syscall number), then `func_0017DBE8`'s family (25),
`func_00158C38` (24, 13 instr., 2 strict shapes) and `func_0017DB98` (14, 4 strict shapes). A matched member of a
loose family is a template for its relatives (sibling first, as in the playbook).

## 6. Phase 1 cost estimate (ESTIMATE, not measured)

### 6.1 Work categories

Each unit gets the first category that applies (`tools/census.py`, `assign_categories`). Shapes = distinct strict
keys (the cost unit: one match per shape, copies by the clone pass).

| Category | Game funcs | Game shapes | Game instr. | SDK funcs | SDK shapes | SDK instr. | Route |
|---|---|---|---|---|---|---|---|
| matched (empty, already C) | 125 | 1 | 250 | 2 | 1 | 4 | done |
| crt0 | 4 | 4 | 46 | - | - | - | stays asm (needs a ruling, 6.3) |
| handwritten (spimdisasm mark) | 0 | 0 | 0 | 186 | 179 | 2,807 | needs a ruling (6.3) |
| clone-copy | 237 | 108 | 5,297 | 36 | 22 | 312 | clone pass after the representative matches |
| VU0 / MMI (any COP2 or MMI op, or data block) | 70 | 70 | 17,423 | 74 | 74 | 2,539 | needs a ruling on inline asm (6.3), then fan-out |
| small (<= 32 instr.) | 941 | 941 | 15,899 | 216 | 216 | 3,155 | bulk if m2c copes with EE code, else fan-out 2 per agent |
| medium (33-128) | 1,057 | 1,057 | 72,030 | 223 | 223 | 14,200 | fan-out, 1 per agent |
| large (129-400) | 356 | 356 | 73,075 | 30 | 30 | 6,350 | fan-out, 1 per agent, up to 5 rounds |
| huge (401+) | 44 | 44 | 25,618 | 8 | 8 | 5,270 | fan-out try, 2-lane tournament, finisher |

To match by hand or agent: **2,468 game shapes and 551 SDK shapes** (VU0/MMI + small + medium + large + huge), plus
the clone pass and the rulings.

### 6.2 Cost

**What it rests on.** No Ringsmith function beyond the empty ones has been matched yet, so every rate below is
borrowed from Project Twinblade (PS1, GCC 2.7.2 + maspsx; playbook `references/workflow-costs.md`), measured in
"5x points" (percent of a Max 5x weekly bar). Ringsmith's own cards so far ran about **1% of this account's weekly
all-models bar per hour of plain Opus work** (`docs/usage_log.md`); Twinblade's plain Opus stream burned about 1.3
5x points per hour. Equating the two hours gives **1 5x point = about 0.77% of this account's weekly bar** (a
conversion resting on one rough figure each side: Unconfirmed). Rates per shape include finishers:

| Category | Twinblade basis | Low | Likely | High |
|---|---|---|---|---|
| small | r178 fan-out <= 64, 2 per agent: 0.024-0.029 a shape | 0.02 | 0.029 | 0.05 |
| medium | r178 / r144 / r148: 0.03-0.05 a shape | 0.03 | 0.045 | 0.08 |
| large | r158 / r162: about 0.10 a shape | 0.07 | 0.10 | 0.18 |
| huge | r148 0.25 a try; renderers 1.6-2.1 with tournaments | 0.25 | 0.5 | 1.5 |
| VU0 / MMI | no basis (GTE renderers are the nearest) | 0.05 | 0.15 | 0.5 |

| | Low | Likely | High |
|---|---|---|---|
| Game shapes (5x points) | 90 | 143 | 297 |
| SDK shapes (5x points) | 19 | 34 | 83 |
| Coordinator share (+7 / 12 / 15%) | 8 | 21 | 57 |
| **Total, 5x points** | **116** | **199** | **437** |
| **Total, % of this account's weekly all-models bar** (x 0.77) | **about 90%** | **about 150%** | **about 340%** |

In words: the likely case is about **one and a half weeks of the whole weekly bar** spent on Ringsmith matching
alone; if Ringsmith gets about 40% of the bar beside the other projects (share still open, handoff question 2),
that is roughly **4 weeks**, with a range of about 2 to 8+ weeks. Cross-check: Twinblade's whole-unit run r216 took
about 32 functions per weekly point at Max 20x on easier leftovers; at that pace the 3,019 shapes here would take
about 95 weekly points, inside the range above.

High-side risks, not priced in: GCC 2.9-ee's float scheduling (1,458 game functions use COP1), EE 64-bit idioms
and pipeline-1 ops, whether m2c drafts EE code at all (if it does, bulk could take half the small shapes for almost
nothing: up to about 15 5x points saved), and parks that need Fable (Twinblade: 1.4-1.6 Fable-bar points a match
at 20x, a separate bar). Low-side chances: SDK library sources compiled with known flags may match in long runs.

### 6.3 Rulings needed before or early in Phase 1 (for LuciDin)

1. **Handwritten SDK code (186 functions, 158 of them the libkernl syscall stubs) and crt0 (4).** The originals were
   assembly sources; plain C cannot produce a `syscall` stub without `asm()`. Options: keep them as `.s` files
   (counted matched as "original asm"), or write them with `asm()`. Today's rule parks `asm()` without a ruling.
2. **VU0 macro code (game-vu0 28, libvu0 42, the 3 data blocks, other COP2 users).** SCE's own libvu0 is written
   with inline `__asm__` in C; matching it means `asm()` blocks. Needs a ruling for the 144 VU0 / MMI units.
3. **SDK libraries in scope?** The goal (true 100%) implies matching SDK library C too (775 units, 34,637 instr.,
   about 14% of the instructions and 18% of the shapes to match). The estimate includes them; say if they should be treated differently (for example last).

### 6.4 Plan of batches (sessions, model and effort)

| Step | Work | Sessions | Model / effort |
|---|---|---|---|
| 0 | ER-013 file split at the cuts in 4.3; ER-014 first real C matches | 2 cards | Opus, medium |
| 1 | EE matching tools: per-function compile and diff (cmp_func for ee-gcc), placement helper, clone tool (strict keys from `tools/census.py`), m2c try on EE code | 1-2 cards | Opus, medium |
| 2 | Pilot: a seeded stratified sample of about 40 game shapes (10 small, 12 medium, 10 large, 4 huge, 4 COP1-heavy) by fan-out, measuring rates and effort (agents at Opus high vs medium) on this toolchain; replaces the borrowed rates in 6.2 | 1 card | coordinator Opus medium, agents Opus high; Ultracode on for the card |
| 3 | small shapes (1,157), 2 per agent, workflows of about 110 shapes | about 11 batches | coordinator Opus medium, agents per the pilot (Opus high to start); Ultracode |
| 4 | medium shapes (1,280), 1 per agent, workflows of 50-60 | about 23 batches | as above |
| 5 | large shapes (386), up to 5 rounds | about 8 batches | agents Opus high |
| 6 | huge shapes (52): fan-out try, 2-lane tournament, finisher | about 5 sessions | Opus high, xhigh for a stubborn one; Fable for parks after two Opus rounds |
| 7 | VU0 / MMI (144) after the ruling | about 3 batches | Opus high |
| 8 | Clone pass after every batch; finisher waves on parks | inside each batch | Opus high (finishers) |

About 50 fan-out cards plus a few plain ones. Twinblade's fan-out workflows took 21-62 minutes of wall time each.

## 7. Not verified, Unconfirmed and Unofficial items

- **Unconfirmed regions**: game-vu0, game-unk, sdk-vudev, libvu0, libipu, libsdr, libgcc, libkernl-tail (identity by
  position and code style only); every seam window in 4.3.
- **ER-041 (4.4)**: the addresses of the fp-bit internals and single-float routines (names from the source, mapping
  by order and shape), `memset` for `func_001E9B0C`, and `__muldi3` for `func_001E7608` are Unconfirmed readings
  (left address-named); `dpadd`, `dpmul`, `dpdiv`, `litodp`,
  `dptoli`, `dptoul`, `dptofp` are confirmed from code but not yet by a placed caller.
- **Unconfirmed readings**: syscall names (SetupThread, SetupHeap, FlushCache, Exit) come from ps2sdk's public
  syscall numbering, not from this game; "sin / cos shapes", "vfprintf / strtod shapes", "IDCT-like" are readings of
  code style; the rotation-like libvu0 trio is not named.
- **Unofficial labels**: the region names `game-vu0`, `game-unk`, `sdk-vudev`, `libkernl-tail` and the file labels in
  4.3 are ours.
- The address tracker is linear: a few data references are misreads (2.). Counts of data references and `$gp` uses
  are approximate; instruction, shape, call and feature counts are exact decodes.
- `.text` is 3,609 units = 3,606 symbols + 3 data blocks: the tracker total `m-dec-funcs` 3,609 stands.
- The cost estimate (6) is borrowed from a PS1 project and converted with one rough figure; the pilot (6.4 step 2)
  is the way to replace it. The plan tier: `get_usage` reported "Ultra" on 17 Nov 2026 while `docs/usage_log.md`
  says Max; the conversion uses hours, not the tier.
- Object boundaries inside the game and inside each library: not found (4.3).
