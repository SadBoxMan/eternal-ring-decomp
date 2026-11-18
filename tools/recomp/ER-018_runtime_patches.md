# ER-018 local changes to PS2Recomp's runtime (description only)

This file describes, in our own words, the local changes ER-018 made to PS2Recomp (ran-j, GPL-3.0, commit
`e5636981`) for Eternal Ring's first movie. The changes themselves (a diff of GPL code) are kept outside this
repository in `C:\rs\recomp\patches\` until LuciDin decides on the GPL-3.0 question and the fork (ER-019). Findings
and evidence: `docs/recomp_spike.md` section 8.

To apply on a fresh clone of e5636981: `git apply C:\rs\recomp\patches\er018_movie_boot.patch` in the clone, then
build with `tools/recomp/build_game.bat` as in section 6 of the spike doc.

## Fixes (`er018_movie_boot.patch`)

| File in PS2Recomp | Change | Why |
|---|---|---|
| `ps2xRuntime/src/lib/Kernel/EeScheduler.cpp`, `EeScheduler::createThread` | Accept thread priority 0 (range 0 to 127 instead of 1 to 127) | The game creates its movie decoder and idle threads at priority 0; the real BIOS accepts it (PCSX2: thread ids 3 and 2); the runtime already runs the main thread at 0 |
| `ps2xRuntime/src/lib/Kernel/Stubs/MPEG.cpp`, `sceMpegGetPicture` | Before the picture step, call each callback the game registered as type 4 (sceMpegCbBackground) with `sceMpegAddCallback`, once per call, synchronously on the calling thread (the runtime's `invokeCurrentSequence`); the old body is kept unchanged as `mpegGetPictureBody`, and its wait-resume paths re-enter the body only | Real libmpeg calls the Background callback while it decodes; Eternal Ring sends movie audio to the IOP from it, and its player starts the display only once that audio is preset |
| `ps2xRuntime/src/lib/Kernel/Stubs/GS.cpp`, `sceGsSetDefDispEnv` | Fill the whole display env the way the shipped libgraph code (SLUS_200.15 0x001CE2E8) does: PMODE 0x66, SMODE2 from the GS params, DISPFB, DISPLAY with the NTSC / PAL origin and MAGH, BGCOLOR 0; read dx / dy from $t0 / $t1 | The old stub left PMODE 0, so `sceGsPutDispEnv` switched both display circuits off and no frame was ever presented (magenta window) |

## Run conveniences (`er008_er018_local_run_tweaks.patch`, not fixes)

- `ps2_runtime.cpp`: window placed on the secondary monitor and no audio device opened (ER-008).
- `include/ps2_debug_panel.h`: the runtime debugger panel starts hidden (F1 shows it), so screenshots show the
  picture (ER-018).

## Not changed (seen, left for later cards)

Guest VBlank pacing in `EeScheduler` (about 5 to 9 VBlanks a second during the movie), the invisible movie picture
(GS image upload / sprite path), `sceGsResetGraph`'s PMODE / SMODE2 writes to A+D 0x41 / 0x42, `ChangeThreadPriority`
refusing priority 0, the libmpeg Nodata callback, `sceMpegGetPictureRAW8`.
