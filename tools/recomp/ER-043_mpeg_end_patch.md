# ER-043 local change to PS2Recomp: the end of the first movie (description only)

This file describes, in our own words, the third local change to PS2Recomp (ran-j, GPL-3.0, commit `e5636981`),
made by ER-043 on Thu 19 Nov 2026. It has two parts:

1. **Two libmpeg bindings in the recompiler config**, added by our own `tools/recomp/make_configs.py` (in git).
2. **One HLE change in `Stubs/MPEG.cpp`**, kept as a diff of GPL code outside this repository in
   `C:\rs\recomp\patches\er043_mpeg_isend.patch` until LuciDin decides on the GPL-3.0 question and the fork (ER-019).

Findings and evidence: `docs/recomp_spike.md` section 10.

## To reproduce on a fresh clone of e5636981

1. `git apply` `er018_movie_boot.patch`, then `er029_vblank.patch`, then `er043_mpeg_isend.patch` (a diff against
   the ER-018 state of `MPEG.cpp`; checked: it applies there to a file identical to `game_build`'s and
   reverse-applies on `game_build`).
2. Make the configs with `tools/recomp/make_configs.py` (it now adds the two bindings below to the `stubs` list),
   run `ps2_recomp` on `cfg_split.toml` and copy the output as in spike doc section 6. Against the ER-008 output
   only three generated files change: `func_001DBC30_0x1dbc30.cpp`, `func_001DBC40_0x1dbc40.cpp` (each now a
   one-line call of the HLE handler) and `ps2_recompiled_stubs.h` (two declarations). `register_functions.cpp`
   is unchanged.
3. Build with `tools/recomp/build_game.bat`.

## Changes

| Where | Change | Why |
|---|---|---|
| `tools/recomp/make_configs.py` (ours) | Adds `sceMpegIsEnd@0x001DBC30` and `sceMpegIsRefBuffEmpty@0x001DBC40` to the recompiler's `stubs` list when the analyzer has not named them | The analyzer's symbol database does not recognise these two four-instruction functions, so they ran as recompiled guest code reading libmpeg's private state (`*(mp+0x40)`, words 0 and 1), which the HLE MPEG path never writes: the decoder thread's end check could never pass. The runtime already had HLE handlers for both; the names are our reading of the code (**Unconfirmed**) |
| `ps2xRuntime/src/lib/Kernel/Stubs/MPEG.cpp`, `sceMpegIsEnd` | The "producer EOF" gate also passes when no `sceCdSt` stream has ever been started (`cdStreamGeneration` 0, globally and for this playback) and the demuxer has seen the stream's program end code (`streamEnded`) | The HLE learns about end of input only from the `sceCdSt*` stubs. Eternal Ring never calls them (no reference from game code or data); its player reads the movie with `sceRead` 64 KiB at a time and counts the bytes itself, so the gate never opened. Games that stream with `sceCdSt` keep the old rule unchanged. The other two conditions (no decoded picture left, last picture's presentation time passed) are unchanged |

## Not changed (seen, left for later cards)

- After the last picture the HLE's `sceMpegIsEnd` waits one picture interval (upstream's presentation rule), so the
  decoder thread makes one more `sceMpegGetPicture` call, which returns no picture, and queues one slot with an old
  picture for one display period. Whether the real decoder does the same: not checked (**Unconfirmed**).
- `sceMpegDelete` (`0x001DBAD8`, a plain `return 1` in the game) stays recompiled: the HLE `sceMpegCreate` already
  resets the HLE state for the next movie.
- The FromSoftware logo is not visible between the two movies (black screen); the picture paths are ER-030's area.
