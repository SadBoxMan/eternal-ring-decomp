# ER-058 local changes to PS2Recomp: the logo and title screens (description only)

This file describes, in our own words, the fifth local change to PS2Recomp (ran-j, GPL-3.0, commit `e5636981`),
made by ER-058 (with ER-047) on Sun 22 Nov 2026. It changes five runtime files and is kept as a diff of GPL code
outside this repository in `C:\rs\recomp\patches\er058_gs_dma.patch` until LuciDin decides on the GPL-3.0 question
and the fork (ER-019). A separate run convenience is in `C:\rs\recomp\patches\er058_local_run_nopad.patch`. No config
or generated file changes.

Findings and evidence: `docs/recomp_spike.md` section 12.

## To reproduce on a fresh clone of e5636981

1. `git apply` `er018_movie_boot.patch`, `er029_vblank.patch`, `er043_mpeg_isend.patch`,
   `er046_mpeg_seqend.patch`, then `er058_gs_dma.patch` (a diff against the ER-046 state of `Stubs/GS.cpp`,
   `ps2_memory.cpp`, `gs/gs_cpu_backend.cpp`, `gs/gs_frontend.cpp` and `include/runtime/gs/gs_frontend.h`, CRLF
   kept; SHA-1 61b6d8de...). Optionally `er058_local_run_nopad.patch` (`ps2_pad.cpp`; SHA-1 cf343911...). Checked:
   both apply to copies of the ER-046 files and give files identical to `game_build`'s, and both reverse-apply on
   `game_build`. The script that makes the fixes from the ER-046 files is `C:\rs\recomp\scripts\er058_fixes.py`.
2. Configs and generated code as for ER-043 (`tools/recomp/make_configs.py`, spike doc section 6).
3. Build with `tools/recomp/build_game.bat`.

## The five fixes (`er058_gs_dma.patch`)

| # | File, function | Change | Why |
|---|---|---|---|
| 1 | `Stubs/GS.cpp`, HLE `sceGsExecLoadImage` / `sceGsExecStoreImage` | The destination / source block address is libgraph's `dbp` / `sbp` as given (`& 0x3FFF`), no longer `* 2048 / 256` | The shipped `sceGsSetDefLoadImage` (SLUS_200.15 `0x001CEF98`) puts `dbp` into BITBLTBUF.DBP unchanged (block units of 64 words). Upstream multiplied by 8, so every LoadImage landed at `(dbp * 8) & 0x3FFF`: the logo CLUT meant for 0x3B40 sat at 0x1A00, the title art at 0x1200, and boot textures inside the frame buffers (the "static tiles" seen since ER-018) |
| 2 | `ps2_memory.cpp`, DMA start, source-chain branch | When a chain-mode transfer starts with QWC != 0, first send the QWC qwords at MADR, set QWC to 0 and advance MADR; then stop if the tag ID held in CHCR is REFE (0) or END (7), or it had IRQ with TIE; otherwise continue with the tags at TADR | Eternal Ring sends its own GIF packets this way: `func_00104478` and `func_00104548` write QWC and MADR (scratchpad, `0x70002000 \| 0x80000000`) and start D2 with CHCR `0x105` (chain mode, tag field 0). Upstream ignored QWC / MADR and walked whatever stale chain TADR pointed to, so not one logo or title primitive reached the GS (run 2: no vertex kick at all) |
| 3 | `Stubs/GS.cpp`, HLE `sceGsSetDefDBuff`, `sceGsSwapDBuff`, `sceGsPutDrawEnv`, `sceGsSetDefDrawEnv`, `sceGsSetDefClear` | Rewritten to do what the shipped libgraph code does (`0x001CE8D8`, `0x001CEB88`, `0x001CE7F0`, `0x001CE608`, `0x001CFC28`, with `sceGszbufaddr` `0x001CFB60`), through new helpers in a local `er058` namespace | Upstream's versions made up their own values. SetDefDBuff read ztest / zpsm / clear from the stack (EABI passes them in `$t0`-`$t2`: the game passes 3, 0x30, 1), put both buffers at FBP 0, set PMODE to both circuits with DISPLAY MAGH 0, and never built the clear sprite. SwapDBuff applied the draw env as 8 register writes and ignored the giftag's loop count (so no clear). PutDrawEnv read 8 pairs starting at the giftag itself (the tag became a register write, TEST was dropped). SetDefDrawEnv asked zbufaddr with the wrong argument registers. SetDefClear did nothing. Now: draw0 / disp[1] at the second buffer (FBP 0x46 for 640x224), disp via the HLE `sceGsSetDefDispEnv` (ER-018), and the giftag's 14 A+D pairs (draw env plus clear) are sent by GIF DMA from the struct, as `sceGsPutDrawEnv` does |
| 4 | `gs/gs_cpu_backend.cpp`, `PresentFromLocalMemory` | Interlaced with SMODE2.FFMD = 1, read `(DH + 1) / 2` buffer lines and show each twice (new `lineDoubleRows`) | libgraph's `sceGsSetDefDispEnv` sets DH = 2h - 1 for an h-line buffer in this mode (h = 224 here), so the GS reads 224 consecutive lines per field. Upstream read DH + 1 = 448 lines, so each 640x224 buffer was shown with the next one (FBP + 0x46) below it: the logo and title appeared twice. Line doubling is a presentation choice for the progressive host window (**Unofficial**) |
| 5 | `gs/gs_frontend.*` (new `GS::resetBackendState`), `Stubs/GS.cpp`, HLE `sceGsResetGraph` | In mode 0, before its register writes, reset the raster backend's internal state (CLUT buffer and CBP0 / CBP1, texture cache, transfer state); registers and local memory are kept | The shipped `sceGsResetGraph` (`0x001CE130`) writes GS CSR = 0x200 (RESET) in mode 0, which the runtime ignored. The movie player calls it at every start. Between the title and the attract loop's second FromSoftware logo an Agetec movie plays; the title loads its CLUT at CBP 0x3B40 with CLD 1 (CBP0 unchanged), and the logo uses CLD 4 (load only if CBP != CBP0) at the same CBP. Without a reset CBP0 still equalled 0x3B40 from the first logo, the reload was skipped and the logo was drawn through the title's CLUT (black on grey). PCSX2 shows it white on black. That a GS reset clears CBP0 on the console is **Unconfirmed** (read from PCSX2's picture and the shipped code's CSR write) |

`sceGsPutDrawEnv` keeps one deviation, noted in the code: for a scratchpad giftag the original passes
`(addr & 0x0FFFFFFF) | 0x80000000` as MADR, which the runtime does not recognise as scratchpad; the HLE passes the
`0x7xxxxxxx | 0x80000000` form instead. Eternal Ring's dbuff structs and draw envs are in main RAM, so this path is
not used here.

## Run convenience (`er058_local_run_nopad.patch`, not a fix)

`ps2_pad.cpp`: when the environment variable `PS2X_NO_PAD_INPUT` is set (and not `0`), the pad reports no button
pressed and centred sticks, ignoring the host controller and keyboard. ER-058's run script sets it, so runs are not
steered by whoever is at the desk (LuciDin's request, 22 Nov 2026). Normal launches are unaffected.

## Not changed (seen, left for later cards)

- Movies: with fix 4 the movie pictures are not visible at all (before it, their bottom half showed, an artefact of
  the 448-line read). The movie draws its half pictures into frame buffers that the corrected presentation does not
  show; ER-030 to trace (spike doc 12.5).
- The rest of a GS reset (registers, drawing state) is still not modelled; only the backend state of fix 5.
- HLE `sceGsResetGraph` still sends PMODE / SMODE2 as A+D writes to 0x41 / 0x42 (SCISSOR_2 / ALPHA_1), as ER-018
  noted; the display env from `sceGsPutDispEnv` overrides them here.
- HLE `sceGszbufaddr` still reads w / h from `$a2` / `$a3` and the gparam from a byte-packed scratchpad copy; it is
  only called by HLE functions this game does not use (`sceGsSetDefDBuffDc`, `sceGsSetDefDrawEnv2`), which were
  written around that.
- `sceGsPutDispEnv` (HLE) writes both display circuits; harmless with libgraph's PMODE 0x66 (circuit 2 only).
