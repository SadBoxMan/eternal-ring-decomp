# Recomp opcode correctness check (ER-020)

Written Thu 19 Nov 2026 by ER-020 (Opus 5.5, medium), 15:25 to about 16:00 Eastern. Analysis and a test plan only:
nothing was run, built or changed in `C:\rs\recomp` (the translator, the runtime and the generated code were read).

Question: which MMI, VU0 macro (COP2) and COP1 opcodes does `SLUS_200.15` use, and which of them does PS2Recomp
(commit `e5636981`, the one in `C:\rs\recomp`) translate wrongly? **Short answer: two COP1 opcodes are wrong for every
site in the game (`sqrt.s`, `cvt.w.s`); one runtime gap (vf0 not pinned in thread contexts) and the 128-bit
load/store alignment can break VU0 code; four VU0 ops are wrong on edge values; the MMI the game uses is right by
reading, and almost all of it sits in libmpeg / libc code that the current HLE config does not run.**

No game code is in this file or in git: opcode names, counts and addresses only.

## 1. Method and how to rerun

```
.venv\Scripts\python.exe tools\recomp\opcode_scan.py --md --cfg C:/rs/recomp/work/cfg_split.toml
```

- `tools/recomp/opcode_scan.py` (new, ours) decodes every function symbol of `build/SLUS_200.15.syms.elf` (3,606
  functions plus the 3 data-labelled blocks; byte-identical to the original ELF) with rabbitizer 1.16.2 in R5900
  mode, and counts per opcode: sites, functions, game vs SDK sites (regions from `tools/census.py`, ER-004;
  "game" = the `game`, `game-vu0` and `game-unk` regions), and with `--cfg` the sites inside functions that the
  PS2Recomp config binds to HLE stubs (their bodies are not run as recompiled code). It writes
  `build/recomp/opcode_sites.csv` (addresses only, gitignored). Run time about 2 s.
- rabbitizer names the EE `sqrt.s` (COP1, fmt S, funct 4) only `c1`; the script renames it. Its 59 words were
  checked by hand (fmt 16, funct 4).
- Families counted: MMI (major opcode 0x1C, all of MMI0-3 and `madd` / `mult1` / `div1` ...), the R5900 additions
  outside MMI (`lq`, `sq`, `mfsa`, `mtsa`, `mtsab`, `mtsah`, three-operand `mult` / `multu`), the variable shifts,
  COP2 (major 0x12 plus `lqc2` / `sqc2`), COP1 (major 0x11 plus `lwc1` / `swc1`) and COP0.
- Each opcode was then checked against PS2Recomp's translator (`ps2xRecomp/src/lib/*_translator.cpp`,
  `mmi_translation_helpers.cpp`, `vu_translation_helpers.cpp`), the runtime macros it emits
  (`ps2xRuntime/include/ps2_runtime_macros.h`), and the generated game code in `C:\rs\recomp\work\out_split`.
  `game_build` has the same translator and macros (`diff -q`: identical; the local ER-008 / ER-018 patches touch
  only the runtime's kernel, GS, MPEG and debug-panel files).
- Upstream issues and PRs were read through the GitHub REST API (`api.github.com`, public, unauthenticated; all
  open items, 232, plus the 100 most recently closed) on Thu 19 Nov 2026. Their claims about the hardware and PCSX2
  are theirs: where this file says "PCSX2 does X" it cites the issue, not PCSX2's source (not read here).

Status words: **OK** = right by reading the translator and macros for the values the game can feed it;
**suspect** = wrong for some inputs (evidence cited); **bug** = wrong for every site or for ordinary values;
**not run** = every site is inside an HLE-stubbed function in the current config.

## 2. Findings in short

| # | Finding | Sites in the game | Evidence | Severity |
|---|---|---|---|---|
| F1 | **`sqrt.s` takes the root of `$f0`, not of its operand.** The R5900 encodes `sqrt.s fd, ft` with fs = 0; the translator reads fs. All 59 sites have fs = 0 and ft != fd; the generated code has `FPU_SQRT_S(ctx->f[0])` 59 times | 59 sites, 44 functions, all game code, none stubbed | `fpu_translator.cpp:60-61`; issue text of PR #277 / #382 (open), #218 (closed, not merged) | **bug, high** |
| F2 | **`cvt.w.s` rounds to nearest instead of truncating**, and out-of-range values are host UB (MSVC gives 0x80000000 for both signs). `(int)f` in the game's C comes out one too high for about half of all fractional values | 423 sites, 241 functions (412 / 237 game); 4 sites in stubs | `ps2_runtime_macros.h:856` `FPU_CVT_W_S = (int32_t)nearbyintf(a)`; PR #215, #316, #383 (open) | **bug, high** |
| F3 | **vf0 is (0, 0, 0, 0) in every thread started by `StartThread`.** The `R5900Context` constructor memsets and does not pin vf0; only the main context (`PS2Runtime()`) and contexts returning from a VU0 microprogram set it. Any VU0 macro code that reads vf0.w (the constant 1.0) on such a thread gets 0 | 76 reads of vf0.w in 29 functions (56 / 14 game); 57 sites in 15 functions outside stubs (e.g. 24 `vdiv Q, vf0w, ...` reciprocals, 18 `vmaddw ..., vf0w` matrix products) | `EeScheduler.cpp:492` `target->context = R5900Context{}`; `ps2_runtime.h:147` (ctor); PR #187, #320 (open). Whether those functions run on a non-main thread: **Unconfirmed** | **suspect, high** if game code runs in a created thread |
| F4 | **`lq` / `sq` / `lqc2` / `sqc2` do not drop the low 4 address bits.** The fast path reads or writes 16 bytes at the unaligned address; the slow path (`PS2Memory::read128` / `write128`) throws. The EE forces 16-byte alignment | 213 `lq`, 195 `sq`, 369 `lqc2`, 249 `sqc2`; outside stubs 161 / 147 / 318 / 230 | `ps2_runtime_macros.h:454` (`Ps2FastRead128`), `ps2_memory.cpp:833`; PR #400 (open). Whether the game ever issues an unaligned address: **not known** (needs a runtime counter) | **suspect** |
| F5 | **VU0 FDIV unit**: `vdiv` gives 0 for a zero divisor (hardware: +-max), no exponent-0 flush; `vsqrt` gives 0 for a negative radicand (hardware: sqrt of the magnitude); `vrsqrt` ignores fs (this game's one site uses fs = vf0.w, so it only differs for ft <= 0) | `vdiv` 29 (24 game), `vsqrt` 1, `vrsqrt` 1 | `vu_translation_helpers.cpp:71-93`; PR #198, #317 (open) | **suspect** (edge values) |
| F6 | **`vftoi0` / `vftoi4` positive overflow gives 0x80000000** (`_mm_cvttps_epi32`); hardware saturates by sign | 49 sites (43 game) | `vu_translation_helpers.cpp:750`; PR #318, #383 (open) | **suspect** (large values) |
| F7 | **FPU compares and VU `vmax` / `vmini` use host IEEE rules**: an exponent-255 operand (a number on the PS2, NaN on the host, e.g. a `div.s` by zero result) makes `c.lt.s` / `c.le.s` / `c.eq.s` false and `_mm_max_ps` pick the second operand; two different exponent-0 patterns compare unequal on the host but are both zero on the PS2 | `c.lt.s` 1,279, `c.le.s` 189, `c.eq.s` 114, `vmaxx` 19, `vminix` 1 (stub) | `ps2_runtime_macros.h:860-872`, `vu_translation_helpers.cpp:258`; PR #321 (open) | **suspect** (rare inputs) |
| F8 | **No round-toward-zero for the EE FPU and VU0 macro code.** The PS2 FPU and VU round toward zero; the host rounds to nearest. `fesetround(FE_TOWARDZERO)` exists only around the VU1 interpreter | every `add.s` / `sub.s` / `mul.s` / `div.s` (8,829 sites) and VU0 FMAC op (about 1,000 sites) | `grep` of the runtime: only `ps2_vu1_core.cpp:1633`; the macro header (lines 143-152) does not mention rounding; ER-008 noted it as Unconfirmed. The PS2 rounding rule is from upstream #383 / #422 and general knowledge (PCSX2's default EE / VU rounding is "Chop"): not checked in PCSX2 here | **suspect** (last-bit differences that can add up) |
| F9 | **VU0 macro `vcallmsr` reads `ctx->vi[27]`**, 11 elements past the end of `uint16_t vi[16]`, instead of CMSAR0 | 1 site (`0x001D53AC` in `func_001D53A0`, sdk-vudev), not stubbed | `vu_translation_helpers.cpp:165-178`; issue #262 (open); generated code has `ctx->vi[27]` once | **bug** if the game starts VU0 microprograms through it |
| F10 | **`vlqi` / `vsqi` address RDRAM at `vi << 4` instead of VU0 data memory.** The upstream operand swap of `vsqi` (#309) does no harm here (both fields are register 1) | 1 + 1 sites (`vsqi` `0x001D566C`, `vlqi` `0x001D5694`, sdk-vudev), not stubbed | `vu_translation_helpers.cpp:768-799`; PR #310 (open) | **bug** if reached |
| F11 | **MAC flags are never set** (`cfc2 vi17` reads 0) | 1 `cfc2 vi17` (SDK, sdk-vudev); no game read | PR #292 (open) | low |
| F12 | **Delay slot past a function's end is replaced by a NOP.** One case: `func_001D46DC` is a lone `jr $ra` whose slot is the first word of libdma (`func_001D46E0`). Nothing in our split references it | 1 | `out_split/func_001D46DC_0x1d46dc.cpp`; issue #370, PR #304 / #415 (open) | low (dead code by our reading) |

Upstream open bugs that the game does **not** hit: PSLLH / PSRLH / PSRAH with sa 16-31 (#434, #443: the game's
halfword shifts use sa 1, 2 and 15 only); PADDUH / PSUBUH / PABSW / PABSH / PSLLVW / PSRLVW / PSRAVW (#397: not
used; the "variable word shifts" of `docs/research.md` §3 are these MMI ops, and the plain `sllv` / `srlv` / `srav` /
`dsllv` / `dsrlv` the game uses are right by reading); PROT3W and the other permutations of #291 / #435 (not used);
BC0F / BC0T / BC0FL / BC0TL (#389: no COP0 branch in the ELF); VRNEXT / VRGET / VRINIT / VRXOR (#439: not used);
the PMULTW / PMADDW / PDIVW / PMFHL / PMTHL family (#322: not used). Already fixed in e563698 and used by the
game: COP2 control register 22 = Q (#193 is in the code: `VU0_CR_Q = 22`).

## 3. Opcode tables (counts from `opcode_scan.py`, statuses from section 2)

Columns: sites, functions, game sites / functions, SDK sites / functions, sites / functions inside HLE-stubbed
functions (config `cfg_split.toml`), status.

### 3.1 MMI: 26 opcodes, 411 sites, 60 functions

| Opcode | Sites | Funcs | Game | SDK | In HLE stubs | Regions | Status |
|---|---|---|---|---|---|---|---|
| `paddh` | 74 | 15 | 0 / 0 | 74 / 15 | 0 / 0 | libmpeg | OK |
| `psrlh` | 54 | 14 | 0 / 0 | 54 / 14 | 0 / 0 | libmpeg | OK (sa 1, 2, 15 only) |
| `qfsrv` | 42 | 16 | 0 / 0 | 42 / 16 | 0 / 0 | libmpeg | OK (with `mtsab`; SA kept in bits) |
| `pextlb` | 36 | 16 | 0 / 0 | 36 / 16 | 0 / 0 | libmpeg | OK |
| `pcpyld` | 28 | 16 | 0 / 0 | 28 / 16 | 16 / 8 | libmpeg 12, libc 12, libvu0 4 | OK |
| `mult1` | 26 | 22 | 10 / 10 | 16 / 12 | 11 / 8 | game 10, libpad 7, libgraph 4, other SDK 5 | OK |
| `pnor` | 21 | 17 | 0 / 0 | 21 / 17 | 9 / 5 | libmpeg 12, libc 9 | OK |
| `por` | 19 | 15 | 2 / 2 | 17 / 13 | 4 / 4 | libmpeg 12, libgraph 3, game 2, other 2 | OK |
| `pextub` | 18 | 8 | 0 / 0 | 18 / 8 | 0 / 0 | libmpeg | OK |
| `pand` | 16 | 5 | 0 / 0 | 16 / 5 | 16 / 5 | libc | not run (OK) |
| `pcpyud` | 13 | 7 | 0 / 0 | 13 / 7 | 13 / 7 | libc 10, libvu0 3 | not run (OK) |
| `pcgth` | 12 | 8 | 0 / 0 | 12 / 8 | 0 / 0 | libmpeg | OK |
| `psubb` | 8 | 5 | 0 / 0 | 8 / 5 | 8 / 5 | libc | not run (OK) |
| `div1` | 5 | 4 | 5 / 4 | 0 / 0 | 0 / 0 | game | OK (divide-by-zero rule as coded; to test) |
| `mflo1` | 5 | 4 | 5 / 4 | 0 / 0 | 0 / 0 | game | OK |
| `pextlw` | 4 | 2 | 0 / 0 | 4 / 2 | 4 / 2 | libvu0 | not run (OK) |
| `pextuw` | 4 | 2 | 0 / 0 | 4 / 2 | 4 / 2 | libvu0 | not run (OK) |
| `pmaxh` | 4 | 2 | 0 / 0 | 4 / 2 | 0 / 0 | libmpeg | OK |
| `pminh` | 4 | 2 | 0 / 0 | 4 / 2 | 0 / 0 | libmpeg | OK |
| `psllh` | 4 | 4 | 0 / 0 | 4 / 4 | 0 / 0 | libmpeg | OK (sa 1 only) |
| `psrah` | 4 | 4 | 0 / 0 | 4 / 4 | 0 / 0 | libmpeg | OK (sa 15 only) |
| `madd` | 3 | 3 | 3 / 3 | 0 / 0 | 0 / 0 | game 2, game-unk 1 | OK |
| `pcpyh` | 2 | 2 | 0 / 0 | 2 / 2 | 2 / 2 | libc | not run (OK) |
| `ppacb` | 2 | 2 | 0 / 0 | 2 / 2 | 0 / 0 | libmpeg | OK |
| `psubw` | 2 | 1 | 0 / 0 | 2 / 1 | 2 / 1 | libc | not run (OK) |
| `pxor` | 1 | 1 | 0 / 0 | 1 / 1 | 1 / 1 | libc | not run (OK) |

The libmpeg MMI code (about 330 sites, the IDCT / motion-compensation-like run of ER-004) is not in a stubbed
function, but it sits under `sceMpegGetPicture`, which the config binds to the runtime's FFmpeg decoder (ER-018).
So it probably never runs in the current build (**Unconfirmed**: a static call graph cannot follow the indirect
calls; see section 6).

### 3.2 R5900 additions outside MMI, and variable shifts

| Opcode | Sites | Funcs | Game | SDK | In HLE stubs | Status |
|---|---|---|---|---|---|---|
| `mult` / `multu` (3-operand) | 521 | 252 | 398 / 209 | 123 / 43 | 46 / 18 | OK |
| `lq` | 213 | 63 | 108 / 25 | 105 / 38 | 52 / 16 | **suspect** (F4) |
| `sq` | 195 | 72 | 110 / 27 | 85 / 45 | 48 / 18 | **suspect** (F4) |
| `mtsab` | 32 | 16 | 0 / 0 | 32 / 16 | 0 / 0 | OK (libmpeg; `rs ^ imm` & 15, imm 0) |
| `sllv` | 88 | 38 | 24 / 23 | 64 / 15 | 10 / 1 | OK |
| `srlv` | 55 | 14 | 0 / 0 | 55 / 14 | 9 / 1 | OK |
| `srav` | 30 | 15 | 17 / 11 | 13 / 4 | 0 / 0 | OK |
| `dsllv` | 12 | 7 | 0 / 0 | 12 / 7 | 5 / 2 | OK |
| `dsrlv` | 5 | 5 | 0 / 0 | 5 / 5 | 0 / 0 | OK |

Not used: `mfsa`, `mtsa`, `mtsah`, `dsrav`, `pmfhl`, `pmthl`, and every MMI op not listed in 3.1.

### 3.3 COP2 (VU0 macro mode): 45 opcodes, 2,223 sites, 113 functions

| Opcode | Sites | Funcs | Game | SDK | In HLE stubs | Status |
|---|---|---|---|---|---|---|
| `vnop` | 496 | 36 | 483 / 33 | 13 / 3 | 13 / 3 | OK |
| `lqc2` | 369 | 86 | 304 / 56 | 65 / 30 | 51 / 20 | **suspect** (F4) |
| `sqc2` | 249 | 79 | 188 / 52 | 61 / 27 | 19 / 16 | **suspect** (F4) |
| `vmulax` | 172 | 41 | 161 / 30 | 11 / 11 | 11 / 11 | OK (F8) |
| `vmadday` | 170 | 39 | 161 / 30 | 9 / 9 | 9 / 9 | OK (F8) |
| `vmaddaz` | 148 | 37 | 140 / 29 | 8 / 8 | 8 / 8 | OK (F8) |
| `vmaddw` | 148 | 37 | 140 / 29 | 8 / 8 | 8 / 8 | OK (F8); 18 game sites read vf0.w (F3) |
| `cfc2` | 49 | 26 | 0 / 0 | 49 / 26 | 8 / 6 | OK (registers used: VI 0-15, 16, 17 (F11), 18, 20, 21, 22, 26, 28, 29) |
| `ctc2` | 42 | 19 | 1 / 1 | 41 / 18 | 5 / 5 | OK (VI 0-15, 16, 17 ignored as read-only, 18, 20, 21, 22, 27, 28, 31; game writes I) |
| `vsub` | 40 | 21 | 15 / 7 | 25 / 14 | 23 / 12 | OK (F8) |
| `vmulq` | 39 | 15 | 34 / 10 | 5 / 5 | 5 / 5 | OK (F8); 9 game sites read vf0.w (F3) |
| `vdiv` | 29 | 14 | 24 / 9 | 5 / 5 | 5 / 5 | **suspect** (F5); 24 game sites read vf0.w (F3) |
| `vmul` | 28 | 13 | 23 / 9 | 5 / 4 | 4 / 3 | OK (F8) |
| `vftoi4` | 27 | 12 | 24 / 9 | 3 / 3 | 2 / 2 | **suspect** (F6) |
| `qmtc2` | 25 | 18 | 6 / 3 | 19 / 15 | 14 / 11 | OK (no write to vf0) |
| `vftoi0` | 22 | 8 | 19 / 5 | 3 / 3 | 2 / 2 | **suspect** (F6) |
| `vmaddz` | 22 | 6 | 21 / 5 | 1 / 1 | 1 / 1 | OK (F8) |
| `vmaxx` | 19 | 5 | 18 / 4 | 1 / 1 | 1 / 1 | **suspect** (F7); all 18 game sites are `max(x, vf0.x)`, a clamp at 0 |
| `vmove` | 16 | 6 | 0 / 0 | 16 / 6 | 16 / 6 | not run |
| `vmulx` | 13 | 6 | 6 / 3 | 7 / 3 | 5 / 1 | OK (F8) |
| `vaddy` | 11 | 8 | 2 / 2 | 9 / 6 | 9 / 6 | OK (F8) |
| `vwaitq` | 10 | 8 | 3 / 2 | 7 / 6 | 7 / 6 | OK (no timing modelled; not needed) |
| `vaddx` | 9 | 6 | 2 / 2 | 7 / 4 | 7 / 4 | OK (F8) |
| `vaddw` | 7 | 5 | 0 / 0 | 7 / 5 | 7 / 5 | not run |
| `vopmsub` | 6 | 6 | 5 / 5 | 1 / 1 | 0 / 0 | OK (lane order checked) |
| `vopmula` | 6 | 6 | 5 / 5 | 1 / 1 | 0 / 0 | OK (lane order checked) |
| `qmfc2` | 5 | 4 | 0 / 0 | 5 / 4 | 2 / 2 | OK |
| `vadd` | 5 | 5 | 2 / 2 | 3 / 3 | 2 / 2 | OK (F8) |
| `vmr32` | 5 | 3 | 0 / 0 | 5 / 3 | 5 / 3 | not run |
| `vsubx` | 5 | 4 | 0 / 0 | 5 / 4 | 5 / 4 | not run |
| `vmaddq` | 4 | 2 | 4 / 2 | 0 / 0 | 0 / 0 | OK (F8) |
| `vmula` | 4 | 2 | 4 / 2 | 0 / 0 | 0 / 0 | OK (F8); reads vf0.w (F3) |
| `vaddi` | 3 | 1 | 3 / 1 | 0 / 0 | 0 / 0 | OK (F8) |
| `vaddz` | 3 | 3 | 0 / 0 | 3 / 3 | 3 / 3 | not run |
| `vmulw` | 3 | 2 | 0 / 0 | 3 / 2 | 3 / 2 | not run |
| `vaddq` | 2 | 2 | 0 / 0 | 2 / 2 | 2 / 2 | not run |
| `vitof0` | 2 | 2 | 1 / 1 | 1 / 1 | 0 / 0 | OK (host rounding above 2^24: Unconfirmed) |
| `vitof4` | 2 | 2 | 1 / 1 | 1 / 1 | 0 / 0 | OK (same) |
| `vmaddx` | 2 | 2 | 0 / 0 | 2 / 2 | 2 / 2 | not run |
| `vcallmsr` | 1 | 1 | 0 / 0 | 1 / 1 | 0 / 0 | **bug** (F9) |
| `vlqi` | 1 | 1 | 0 / 0 | 1 / 1 | 0 / 0 | **bug** (F10) |
| `vminix` | 1 | 1 | 0 / 0 | 1 / 1 | 1 / 1 | not run (F7) |
| `vrsqrt` | 1 | 1 | 1 / 1 | 0 / 0 | 0 / 0 | **suspect** (F5) |
| `vsqi` | 1 | 1 | 0 / 0 | 1 / 1 | 0 / 0 | **bug** (F10) |
| `vsqrt` | 1 | 1 | 1 / 1 | 0 / 0 | 0 / 0 | **suspect** (F5) |

Most of libvu0 (30 `sceVu0*` functions) is bound to runtime HLE stubs, so for those the stubs' C++ decides the
result, not the opcodes (one upstream report on them: `sceVu0MulMatrix` operand order, #361 / #363, open; not
checked here). Three more units hold 6 words rabbitizer cannot decode, all in the 3 data-labelled blocks: two
`vsqrt` encodings with non-zero `fsf` bits inside `func_001D73D0` / `func_001D7658` (bound to the HLE stubs
`sceVu0Normalize` / `_sceVu0ecossin`, analyzer names Unconfirmed) and 4 data words `0x00FF00FF` in
`func_001DECA0` (libmpeg; the analyzer binds a stub named `_copyRefImage` there and the recompiler also emitted an
`entry_1deca0` file for it; not looked into).

### 3.4 COP1: 20 opcodes, 43,538 sites, 1,509 functions

| Opcode | Sites | Funcs | Game | SDK | In HLE stubs | Status |
|---|---|---|---|---|---|---|
| `lwc1` | 11,133 | 1,306 | 11,029 / 1,287 | 104 / 19 | 25 / 6 | OK (bit copy) |
| `swc1` | 9,732 | 1,112 | 9,593 / 1,087 | 139 / 25 | 73 / 6 | OK |
| `mtc1` | 6,148 | 1,124 | 5,941 / 1,095 | 207 / 29 | 55 / 11 | OK |
| `mul.s` | 3,765 | 667 | 3,596 / 656 | 169 / 11 | 70 / 5 | OK for range (#272 is merged) (F8) |
| `sub.s` | 2,150 | 546 | 2,063 / 531 | 87 / 15 | 44 / 7 | OK (F8) |
| `add.s` | 2,141 | 642 | 2,014 / 626 | 127 / 16 | 30 / 8 | OK (F8) |
| `cvt.s.w` | 1,696 | 457 | 1,688 / 455 | 8 / 2 | 2 / 1 | OK (host rounding above 2^24; PCSX2 rule not checked) |
| `mov.s` | 1,664 | 549 | 1,596 / 532 | 68 / 17 | 14 / 5 | OK |
| `c.lt.s` | 1,279 | 372 | 1,269 / 363 | 10 / 9 | 3 / 3 | **suspect** (F7) |
| `div.s` | 773 | 411 | 754 / 404 | 19 / 7 | 3 / 2 | OK for x/0 and range (F8) |
| `bc1f` | 642 | 314 | 626 / 302 | 16 / 12 | 5 / 5 | OK (FCR31 bit 23) |
| `bc1t` | 437 | 228 | 437 / 228 | 0 / 0 | 0 / 0 | OK |
| `cvt.w.s` | 423 | 241 | 412 / 237 | 11 / 4 | 4 / 3 | **bug** (F2) |
| `mfc1` | 380 | 238 | 339 / 212 | 41 / 26 | 18 / 11 | OK (sign-extends) |
| `neg.s` | 336 | 192 | 310 / 184 | 26 / 8 | 13 / 3 | OK |
| `bc1fl` | 310 | 176 | 308 / 174 | 2 / 2 | 0 / 0 | OK (likely form nullifies the slot) |
| `c.le.s` | 189 | 113 | 187 / 112 | 2 / 1 | 0 / 0 | **suspect** (F7) |
| `bc1tl` | 167 | 106 | 166 / 105 | 1 / 1 | 1 / 1 | OK |
| `c.eq.s` | 114 | 87 | 107 / 83 | 7 / 4 | 3 / 2 | **suspect** (F7) |
| `sqrt.s` | 59 | 44 | 59 / 44 | 0 / 0 | 0 / 0 | **bug** (F1) |

Not used: `abs.s`, `rsqrt.s`, `adda.s` / `suba.s` / `mula.s` / `madd.s` / `msub.s` / `madda.s` / `msuba.s`,
`max.s` / `min.s`, `cfc1` / `ctc1` (so the FPU flags in FCR31, which PS2Recomp does not update, #164, are never read
by the game; only the condition bit matters).

### 3.5 COP0

`mfc0` 33, `ei` 28, `di` 20 sites, all SDK (libkernl, libkernl-tail, crt0, one `ei` in libmpeg), about half in
stubs. Not on the upstream bug list and not checked further (**unknown**). No `bc0*`, `eret`, `tlb*` in the ELF.

## 4. Differential test plan against PCSX2

### 4.1 What to test first (risk x frequency)

| Rank | Item | Why first | Method |
|---|---|---|---|
| 1 | `sqrt.s` (F1), `cvt.w.s` (F2) | confirmed wrong: 482 sites (471 in game code, in 269 game functions) | fix (port the upstream changes) and prove with A and C |
| 2 | vf0 in thread contexts (F3) | can zero whole matrix and reciprocal paths at once | first a cheap runtime check: log the thread id when one of the 15 functions runs (or print vf0 on `StartThread`); then fix and test with C |
| 3 | 128-bit alignment (F4) | silent data shifts if the game relies on the EE's truncation | runtime counter: log the PC and address of any `lq` / `sq` / `lqc2` / `sqc2` with `addr & 15` (first N, then a count) during the boot; test the fix with A |
| 4 | `vdiv`, `vsqrt`, `vrsqrt` (F5), `vftoi0` / `vftoi4` (F6) | 81 sites, edge values (zero divisor, overflow) can occur in camera / projection code | A, then C on the game-vu0 helpers |
| 5 | FPU compares, `vmaxx` (F7) | 1,601 sites, but only exponent-255 / exponent-0-with-mantissa inputs differ | A |
| 6 | Rounding mode (F8) | touches about 10,000 sites, one ulp each; matters for long-running physics and for matching PCSX2 bit for bit | A with both rounding modes; ruling needed (section 4.4) |
| 7 | sdk-vudev `vcallmsr`, `vlqi`, `vsqi`, MAC (F9-F11) | 4 sites, matter only if the game drives VU0 microprograms through that library | first find out whether they run (runtime log); fix if so |
| 8 | MMI set, `mult1` / `div1` / `madd`, shifts, `mtsab` / `qfsrv` | OK by reading; regression coverage, and needed once libmpeg or libc runs as recompiled code instead of HLE | A |

### 4.2 Methods

**A. Opcode kernel ELF (our own code, can live in git).** A bare-metal EE program, assembled and linked with the
project's Windows-native `ee-as` / `ee-ld` (2.9-ee-991111, `docs/toolchain.md` 4.2; no ps2sdk, no libc, no game
code). It holds a table of kernels; each kernel loads its inputs from a table (`lq`, `lqc2`, `lwc1`, `ctc2`), runs
one instruction under test (or a short fixed sequence, such as `vdiv` + `vwaitq` + `cfc2 vi22`), and stores every
output to a result buffer; at the end it writes a done word and spins. Inputs per opcode: ordinary values, 0, -0,
exponent-0 patterns with a mantissa, +-1, values just under and over 2^24 and 2^31, `0x7F800000` / `0x7FFFFFFF` /
`0xFFFFFFFF` (exponent-255 numbers), fractional values both signs for `cvt.w.s`, negative radicands, zero divisors,
unaligned base addresses for the 128-bit loads and stores (the store kernels write into a guard-filled buffer).
- PCSX2 side: boot the ELF directly (PCSX2 2.5.185, real BIOS, muted, secondary monitor, **LuciDin's yes per run**),
  wait for the done word and read the buffer over PINE (memory reads, as ER-018's `pine_probe.py` did); PCSX2.ini
  backed up and restored around the run.
- Recomp side: the same ELF through `ps2_analyzer` / `ps2_recomp` and a small runner built from the runtime that
  runs it to the done word and dumps the buffer. Built in its own tree (for example `C:\rs\recomp\optest`), never in
  `game_build`.
- Record PCSX2's EE and VU rounding and clamping settings with each result. The PCSX2 recompilers clamp
  differently from its interpreter, so run the reference with the interpreter as well where results differ
  (which is closer to hardware: **Unconfirmed**; ps2autotests' real-hardware `.expected` files, which upstream
  PRs cite, are a third reference where they cover the case).

**B. Game function break-and-compare.** PINE gives memory read / write, save states and status, but no register
access and no breakpoints (the PCSX2 debugger has them, through its GUI only). So "break at a game function and
compare registers" over PINE is not possible as ER-018 used it. Two workable forms:
- **C. Leaf-function harness** (the recommended one): a variant of A that loads the game's `.text` / `.data` at
  run time from LuciDin's own ELF (PCSX2's host filesystem, `host:`), so no game code enters git, links itself above
  the game image, and calls chosen leaf functions (no `jal`, `$gp` set to 0x00360B70) with generated inputs: the
  44 `sqrt.s` functions where they are leaves, the 28 game-vu0 helpers (`0x001C9B18`-`0x001CA0F0`), the 9 game
  `vdiv` functions. The recomp side calls the generated `func_XXXXXXXX` with the same inputs in a fresh
  `R5900Context` and memory image. Compare return registers, `$f0`, vf / ACC / Q and the output memory.
- **D. Whole-state snapshots** (later, coarse): once the recomp boots further, compare PINE memory dumps of known
  game variables at matched points (frame counter, fixed input). Finds divergence, not the opcode.
- **E. HLE vs original (host only, no PCSX2).** For the 30 libvu0 stubs, compare each stub with the recompiled
  original body on random inputs (a second config without that stub). Finds stub bugs such as #361; do it after the
  F1-F8 fixes, since the original bodies use the same opcodes.

### 4.3 What to compare

- GPRs as full 128-bit values (upper halves too: MMI and `lq` write them), HI / LO / HI1 / LO1 (stored with
  `mfhi` / `mflo` / `mfhi1` / `mflo1`), SA where `qfsrv` is tested.
- FPRs as raw bits (not as floats); FCR31 bit 23 (C). The FPU status flags are recorded but not compared at
  first (PS2Recomp does not model them, #164, and the game never reads FCR31).
- VU0: all written vf registers (raw bits), ACC (moved out with `vmaddx.xyzw vfN, vf0, vf0x`, which gives ACC + 0;
  **Unconfirmed** that it is exact for exponent-255 ACC values, so `vaddax` / `vmaddax` kernels also store a known
  ACC first), Q (`cfc2 vi22`), status (vi16), MAC (vi17), clip (vi18), I (vi21).
- Memory: the result buffer, plus guard words around every store target (catches the alignment and `vsqi` cases).

### 4.4 Tooling it needs

1. `tools/recomp/optest/`: kernel table (a small text format), a generator script that writes the `.s`, a linker
   script, a build script calling `ee-as` / `ee-ld` (MSYS2 programs; `cmd.exe` wrapper like `tools/recomp/*.bat`).
2. A PCSX2 runner (PowerShell): INI backup / PINE on / volume 0 / restore, launch on the secondary monitor, PINE
   reader that waits for the done word and writes a CSV, kill by PID. Base: ER-018's `C:\rs\captures\er018\`.
3. A recomp runner: a separate PS2Recomp build tree, a runner `main` that loads the test ELF, runs until the done
   word and dumps the buffer (or a small runtime hook).
4. `compare.py`: per kernel and input, bitwise diff, a pass / fail table per opcode, written into the doc.
5. The runtime counters of 4.1 (unaligned 128-bit accesses with PC; thread id at the vf0-reading functions).
6. A ruling from LuciDin on rounding (F8): the as-shipped port wants PS2 results, so either round toward zero around
   recompiled EE code (MXCSR RZ; cheap but global) or per-operation emulation (slow); the EX option could keep host
   rounding. The harness results should come first.

## 5. Follow-ups (ranked, for the dispatcher)

1. **Fix the confirmed bugs in the local patch** (F1 `sqrt.s` from ft, F2 `cvt.w.s` truncate and saturate, F3 pin
   vf0 in the `R5900Context` constructor so `StartThread` gets it too, F4 16-byte mask on all four 128-bit paths;
   upstream PRs #382 / #316 / #320 / #400 as the reference, written in our own words), regenerate the C++ (F1 is a
   translator change) and rebuild; add the unaligned-access counter run. Files: the PS2Recomp tree and
   `C:\rs\recomp\patches\`; F3 touches `EeScheduler.cpp` / `ps2_runtime.h`, which ER-029 is editing, so after
   ER-029 or in a separate clone. Done when the four fixes are in a patch, the game rebuilds, the boot is no worse,
   and the generated code shows `FPU_SQRT_S(ctx->f[ft])`. Opus, medium.
2. **Opcode test harness** (section 4.2 A and C, tooling 4.4 items 1-4): build it, run the rank 1-6 kernels on
   PCSX2 (LuciDin's yes) and on the recomp, write the pass / fail table. Opus, high (new subsystem).
3. **VU0 FDIV / VFTOI / compares** (F5, F6, F7), once the harness exists to prove them (upstream #317, #318, #321).
   Opus, medium.
4. **Rounding-mode ruling and fix** (F8): LuciDin's call after the harness shows how far results drift. Opus, medium.
5. **sdk-vudev VU0 paths** (F9-F11): find out whether the game runs `vcallmsr` / `vlqi` / `vsqi` (runtime log),
   fix if so (#262, #310, #292). Opus, medium; low priority.
6. **HLE libvu0 check** (method E), including `sceVu0MulMatrix` (#361). Opus, medium; after 1 and 3.

## 6. Not verified, Unconfirmed, Unofficial

- Nothing was executed: every status is from reading code. No PCSX2 or recomp run, no test.
- Which thread runs the 15 vf0-reading functions (F3), and whether the game issues unaligned 128-bit addresses (F4).
- Static reachability: a call graph from `main` over `jal`, tail jumps and code pointers, with HLE stubs cut, reaches
  only 630 of 3,609 functions, because it cannot follow indirect calls; so "probably never runs" for libmpeg's MMI
  code is **Unconfirmed**, and no site is called dead on that basis (except `func_001D46DC`, which nothing
  references in our split).
- Hardware and PCSX2 behaviour (divide-by-zero results, saturation, alignment, rounding) is taken from the upstream
  issue texts cited, not from PCSX2's source or a hardware test.
- `div1` divide-by-zero results (LO = 1 or -1 by the dividend's sign, HI = dividend) are PS2Recomp's as coded; not
  compared with PCSX2.
- `cvt.s.w` / `vitof` rounding above 2^24, and denormal operands taking part with their host value in `add.s` /
  `mul.s` / VU ops (stated in the macro header) are not assessed.
- The ACC read-out trick in 4.3 is Unconfirmed for exponent-255 values.
- Region labels `game-vu0`, `game-unk`, `sdk-vudev` are Unofficial (ER-004); the SCE names quoted from the PS2Recomp
  config (`sceVu0Normalize`, `_copyRefImage`, ...) are the analyzer's readings, Unconfirmed.
- Only the 100 most recently closed upstream items were listed; an older closed PR could have fixed something that
  this file calls open. Every status here was checked against the code at e563698, not against the issue state.
