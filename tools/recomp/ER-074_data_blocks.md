# ER-074 change to the recomp pipeline: code blocks the split labels as data (description only)

This file describes, in our own words, the change ER-074 made on Sun 22 Nov 2026 so that the game runs past NEWGAME
in the recompiled build. Unlike ER-018 to ER-058 it needs **no change to PS2Recomp's runtime**: the fix is in our
own two pipeline scripts (in git). One run convenience for PS2Recomp (GPL-3.0, commit `e5636981`) is kept as a diff
outside this repository in `C:\rs\recomp\patches\er074_local_run_audio.patch`.

Findings and evidence: `docs/recomp_spike.md` section 13.

## Cause

After NEWGAME the game thread stopped at `[guest-branch:missing-target] kind=DirectCall op=JAL source=0x123cac
target=0x1d73d0`. Our split (spimdisasm, `docs/toolchain.md` 6.1) writes three functions as `dlabel` data blocks,
because one word in each decodes as an invalid instruction or is a data constant:

| Block | Size | Why it is labelled as data | Analyzer name (Unconfirmed) | Callers |
|---|---|---|---|---|
| `func_001D73D0` | 56 | `0x4A2503BD`: `vsqrt Q, vf5x` with a non-zero `fsf` field (unused by `vsqrt`) | `sceVu0Normalize` | 20+ game sites (0x00123CAC among them) and two libvu0 functions |
| `func_001D7658` | 120 | `0x4BA703BD`: `vsqrt Q, vf7w`, same stray field | `_sceVu0ecossin` | `sceVu0RotMatrixZ / X / Y` (0x001D76D0 / 0x001D7778 / 0x001D7820) only |
| `func_001DECA0` | 88 | a 16-byte constant `0x00FF00FF` x 4 inside the code (loaded by `lq`, and executed as `dsra32 $zero, $ra, 3`, i.e. harmless, before the `jr ra`) | `_copyRefImage` | libmpeg `func_001DDD80` (tail `j`) |

They are `STT_OBJECT` symbols, and `tools/recomp/syms_to_csv.py` took only `STT_FUNC` symbols, so PS2Recomp's
function map had no entry for them. 0x001D73D0 therefore had no body at all. 0x001DECA0 already had one, because the
analyzer lists `_copyRefImage` as an entry point and `ps2_recomp` made a stand-alone `entry_1deca0_0x1decf8` for it;
its instructions are the same as the new function's (checked).

## Changes

| Where | Change | Why |
|---|---|---|
| `tools/recomp/syms_to_csv.py` (ours) | Also writes `STT_OBJECT` symbols named `func_*` with a size, in an executable section (3 today); checks that no two entries overlap; writes every entry with its address name `func_XXXXXXXX` | The three blocks get function boundaries. Address names: at ER-008 every split name was an address name; since then the decomp has named 13 functions (`sqrtf`, `dpadd`, ...). PS2Recomp only avoids C++ keywords, so a generated function called `sqrtf` could clash with the C runtime, and each rename would rename generated files. With address names the CSV differs from ER-008's by exactly the three new rows |
| `tools/recomp/make_configs.py` (ours) | New `DROP_STUBS` list: removes `sceVu0Normalize@0x001D73D0` from the analyzer's `stubs` list, so the shipped code is recompiled | As shipped first. The runtime's HLE `sceVu0Normalize` uses the length of all four components, scales w too, and returns zero below 1e-6. The shipped code (our reading of the encodings: `lqc2 vf4,(a1)`, `vmul.xyz vf5,vf4,vf4`, `vaddy.x`, `vaddz.x`, `vsqrt Q,vf5x`, `vaddq.x vf5,vf0,Q`, `vdiv Q,vf0w,vf5x`, `vsub vf6,vf0,vf0`, `vmulq.xyz vf6,vf4,Q`, `sqc2 vf6,(a0)`) normalises xyz by the xyz length and writes w = 0. Any vector with w = 1 would come out wrong under the HLE |
| `tools/recomp/build_game.bat` (ours) | Brought in line with the working copy in `C:\rs\recomp`: `-DPS2X_ENABLE_AGRESSIVE_LOGS=OFF` | The repo copy still had the default (ON), which writes a trace of about 1 GB a minute (spike doc 4.3). Every build since ER-018 used the working copy |
| `ps2xRuntime/src/lib/ps2_runtime.cpp` (local, run convenience) | `InitAudioDevice()` only when `PS2X_AUDIO=1`; muted stays the default | LuciDin asked for sound in his driving sessions (Sun 22 Nov 2026). Before this the call was commented out (ER-008 local tweak) |

`_sceVu0ecossin` stays bound to the runtime's HLE handler, which is **wrong for this game** (it reads an angle from
`$f12` and writes cos / sin to `*$a0`; the shipped helper takes its input in `vf6` and returns in VU registers). It
is harmless today because all three callers are HLE'd themselves (`sceVu0RotMatrixZ / X / Y`), so the helper is
never reached; it is listed for the libvu0 HLE audit (follow-up in `docs/handoff.md`).

## To reproduce on a fresh clone of e5636981

1. Apply the runtime patches as in `ER-058_gs_dma_patch.md` (ER-018, ER-029, ER-043, ER-046, ER-058, and the
   run conveniences `er058_local_run_nopad.patch` and `er074_local_run_audio.patch`; the last is a diff against
   the ER-058 state of `ps2_runtime.cpp`, checked: it reverse-applies on `game_build`).
2. `python -I tools\recomp\syms_to_csv.py build\SLUS_200.15.syms.elf C:\rs\recomp\work_er074\split_functions.csv`
   (prints `3609 functions, of which 3 data-labelled code blocks`), then `make_configs.py` with that CSV,
   `ps2_recomp` on `cfg_split.toml`, and copy the output as in spike doc section 6 (delete the old
   `entry_1deca0_0x1decf8.cpp` from `runner`).
3. Against the ER-043 output: three new files (`func_001D73D0_0x1d73d0.cpp` recompiled, `func_001D7658_0x1d7658.cpp`
   an HLE call, `func_001DECA0_0x1deca0.cpp` recompiled), one removed (`entry_1deca0_0x1decf8.cpp`), and four
   changed: `func_001DDD80_0x1ddd80.cpp` (two tail calls renamed), `register_functions.cpp` (table entries for the
   three), `ps2_recompiled_functions.h`, `ps2_recompiled_stubs.h`. Recompiler: 3,609 functions, 3,359 recompiled,
   250 stubs, 0 decode failures, 0 unhandled instructions.
4. Build with `tools/recomp/build_game.bat`.

## Run switches (local)

| Variable | Effect |
|---|---|
| `PS2X_NO_PAD_INPUT=1` | ignore the host pad and keyboard (ER-058) |
| `PS2X_AUDIO=1` | open the audio device (ER-074); in run 3 nothing was heard (spike doc 13.3) |

Run script: `C:\rs\recomp\scripts\er074_run.ps1 -Tag <t> -Seconds <s> [-Pad] [-Sound]`;
`er074_attach.ps1` takes over a running game to extend a run (captures, then a kill by PID at a set time).
