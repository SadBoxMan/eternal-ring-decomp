# ER-029 local change to PS2Recomp's runtime: VBlank pacing (description only)

This file describes, in our own words, the second local change to PS2Recomp (ran-j, GPL-3.0, commit `e5636981`),
made by ER-029 on Thu 19 Nov 2026. The change itself (a diff of GPL code) stays outside this repository in
`C:\rs\recomp\patches\er029_vblank.patch` until LuciDin decides on the GPL-3.0 question and the fork (ER-019).
Findings, the model and the measurements: `docs/recomp_spike.md` section 9.

To apply on a fresh clone of e5636981: `git apply C:\rs\recomp\patches\er018_movie_boot.patch`, then
`git apply C:\rs\recomp\patches\er029_vblank.patch`, then build with `tools/recomp/build_game.bat` (spike doc
section 6). The ER-029 patch is a diff against the ER-018 state of the two files it touches.

## Change (`er029_vblank.patch`)

| File in PS2Recomp | Change | Why |
|---|---|---|
| `ps2xRuntime/include/runtime/ee_scheduler.h` | New private members: a host anchor (host instant of EE cycle 0), the earliest host deadline (atomic), a checkpoint poll counter; new helpers `hostDeadlineForCycle`, `syncCycleToHostTime`, `shiftHostAnchor` | Support for the host-time clock below |
| `ps2xRuntime/src/lib/Kernel/EeScheduler.cpp`, constants | VBlank period now the NTSC field rate 60000/1001 Hz: field n starts at EE cycle `n x 294,912,000 x 1001 / 60,000`, computed from the field number (no drift); the 60.00 Hz host and cycle periods are gone | The old 16,667 us period was 60.00 Hz, while the IOP side and the HLE CD clock already use 59.94 Hz |
| same, `reset` and `processDueDeadlines` | VBlankStart events carry their field number; each one schedules the next field and its VBlankEnd from cycle deadlines, with host deadlines derived from them | One time base for all events |
| same, `processDueDeadlines` | Calls `syncCycleToHostTime` first: the EE cycle counter is pulled up to host time x 294.912 MHz through `accountCycles` (so EE timers and the IOP advance too); a lag over 250 ms is skipped by moving the anchor and every stored host deadline | Generated code charges 32 cycles per loop back-edge and 8 per dispatch, not real costs, so the cycle gate held VBlank at 5 to 9 Hz while the movie threads only yielded |
| same, `checkpointDue` | Every 64 checkpoints, compare the host clock with the earliest host deadline and return to the dispatcher if it has passed | A host deadline is noticed even inside code that charges few cycles |
| same, `setAlarm`, `waitForEvent`, `updateNextDeadline` | Alarm and EE-timer host deadlines derived from cycles; `updateNextDeadline` also records the earliest host deadline | Same time base; feeds the checkpoint poll |

Kept as before: when charged cycles run ahead of host time, the guest still waits for the host deadline (it never
runs faster than the console); VBlankEnd 500 us after VBlankStart; the 64 us alarm tick.

## Not changed (seen, left for later cards)

The end of the first movie (the player's end check reads a libmpeg field through recompiled `func_001DBC40`, which
the HLE MPEG path most likely never writes: spike doc 9.5); per-block cycle costs in the recompiler (stage 2 of
LuciDin's ruling); the GS picture path (ER-030).
