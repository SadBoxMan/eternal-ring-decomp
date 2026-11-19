# Recomp spike: PS2Recomp on SLUS_200.15 (ER-008)

Written Wed 18 Nov 2026 by ER-008 (Opus 5.5, medium). Work ran Tue 17 Nov 2026, 16:01 Eastern, to Wed 18 Nov,
about 00:45 Eastern (wall clock; the session sat waiting for an answer for several hours of that).
Question: can the existing PS2Recomp static recompiler handle Eternal Ring (US, `SLUS_200.15`) natively on
Windows (no WSL, Docker or hypervisor)? **Short answer: yes, further than expected.** It recompiles every
function with no unhandled instruction, the output compiles and links cleanly with MSVC, and the result boots
into the first movie before it stalls.

No game data and no generated code is in git: the recompiled C++, the configs (they point at game files), the
logs and the disc copy all stay under `C:\rs\recomp\`. Only our helper scripts are in `tools/recomp/`.

## 1. Version and licence

| Item | Value |
|---|---|
| Source | https://github.com/ran-j/PS2Recomp, `main` |
| Commit | `e5636981a1c9de368f6b71e819c3dc188dfedbf4` (Fri 23 Oct 2026, "GIF: treat FLG=3 like IMAGE everywhere a GIFtag is parsed (#384)"); still the `HEAD` on 17 Nov 2026 (`git ls-remote`) |
| Licence | GPL-3.0 (see `docs/research.md` §9; "only" vs "or later" still unresolved) |
| Clone | `C:\rs\recomp\PS2Recomp` (clean, built there); `C:\rs\recomp\game_build` (local clone of it, holds the game output and two local edits, section 4.4) |
| Prebuilt binaries | **Correction (ER-018, 18 Nov 2026):** the game build (`ps2EntryRunner`) does use one prebuilt: its CMake `ExternalProject` downloads FFmpeg n7.1 as LGPL Windows DLLs and import libraries (`System233/ffmpeg-msvc-prebuilt`, tag `n7.1-241205`, into `out\game\ThirdParty\ffmpeg-prefix`); the runtime uses it to decode movies (`PS2X_ENABLE_FFMPEG`, ON by default on Windows). The tool build below fetched only source. Original line: none used. All dependencies are fetched as source by CMake `FetchContent`: ELFIO Release_3.12, toml11 v4.4.0, fmt 12.1.0, libdwarf v2.2.0, rabbitizer 1.14.3, nlohmann/json v3.11.3, raylib 5.5, imgui v1.92.7-docking, rlImGui Raylib_5_5 (Studio, not built: SDL 2.32.10, ImGuiColorTextEdit, imgui_club, ImGuiFileDialog). `.gitmodules` does not exist despite the README's `--recurse-submodules` |

## 2. Build on native Windows

Nothing had to be installed. Already on the machine: Visual Studio Community 2022 17.11.35327 with MSVC
14.41.34120, and its bundled CMake 3.29.5-msvc4 and Ninja (`Common7\IDE\CommonExtensions\Microsoft\CMake`). Git
for Windows. (Not on `PATH`: cmake, ninja, cl; the batch files call `vcvars64.bat` and add the bundled tools.)

Script: `tools/recomp/build_ps2recomp.bat` (Release, Ninja, `-j 4`, `PS2X_BUILD_STUDIO=OFF`).

| Step | Result |
|---|---|
| Configure + build Release (281 steps, a few minutes including the dependency fetch; not timed exactly) | `ps2_recomp.exe`, `ps2_analyzer.exe`, `ps2EntryRunner.exe` built, no warnings counted in the log |
| Problem 1 | My first run called the batch file by a relative name through `cmd.exe /c` from Git Bash: "not recognized". Fix: call it by its full path |
| Problem 2: `ps2x_tests.exe` (unit tests) | **MSVC internal compiler error C1001** at `ps2xTest/src/ps2_runtime_kernel_tests.cpp(139)` (a lambda using `_mm_extract_epi64`). Happens in Release (link-time code generation) and in RelWithDebInfo (in `cl` itself). Not fixed in source |
| Workaround for the tests | A Debug build compiles (my script meant to put it in `out\dbg`, but a bad path substitution reconfigured `out\rwdi` as Debug, so the exe is `out\rwdi\ps2xTest\ps2x_tests.exe`). Run: **199 tests passed, 0 failed**, then the run **aborted**: `assert(m_executorThread == std::this_thread::get_id())` in `EeScheduler::assertExecutor()` (`ps2xRuntime/src/lib/Kernel/EeScheduler.cpp:1552`) failed, and the debug CRT then called `abort()`. The last test that passed is "sceDmaReset re-enables DMAC DMAE" (`ps2xTest/src/ps2_memory_tests.cpp:1983`), so the test that aborted is most likely the next one, "sceDmaSend preserves guest-configured VIF1 TTE" (line 2015; **Unconfirmed**, it printed no line of its own before the abort). My reading: that test reaches scheduler code from a thread that is not the EE executor thread. The assert only exists in Debug builds (`assert` is compiled out with `NDEBUG`), so a Release run would go straight past it. Not chased. Tests after it did not run |
| Side effect (my mistake) | The debug CRT showed a modal "Debug Error! ... abort() has been called" dialog on LuciDin's screen. Future test runs must turn off CRT dialogs (a Release / RelWithDebInfo build, or `_set_abort_behavior(0, _WRITE_ABORT_MSG \| _CALL_REPORTFAULT)` plus `_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE)` in a wrapper, or `SetErrorMode`), and any hung test process is killed by PID |

A newer MSVC (VS 17.12 or later) may not hit the C1001 (**Unconfirmed**, not tried: it would mean updating
Visual Studio, LuciDin's call).

## 3. Results on SLUS_200.15

Input: a read-only copy of `disc/extract/SLUS_200.15` (SHA-1 6087A8CA..., same as `docs/toolchain.md`). The ELF
is stripped, so PS2Recomp's paths are its own JAL-scan heuristic or a "Ghidra map" CSV. Instead of Ghidra I fed
it **our split's function list** (`tools/recomp/syms_to_csv.py` reads the FUNC symbols of
`build/SLUS_200.15.syms.elf` from ER-003 and writes `name,start,end,size`). Configs: `tools/recomp/make_configs.py`.

### 3.1 Analyzer (`ps2_analyzer`, embedded SCE symbol database)

| Measure | Value |
|---|---|
| Run time | under 1 s |
| Functions found (JAL scan) | **2,991** (our split: **3,606**) |
| SCE SDK symbol matches from its embedded database | 502 |
| `stubs` written (SDK functions with a runtime handler, bound `name@address`) | 250; by family: sceCd 35, sceSif 28, scePad 24, sceMc 18, sceGs 15, sceDma 13, sceMpeg 12, sceIpu 4, sceTty 4, sceSd 3, newlib/libc and others the rest |
| `entry_points` (SDK-like functions without a runtime handler) | 252 (for example `kprintf`, `VSync`, `sbrk`, `sceDevVu0*`, SIF RPC internals) |
| Patches, jump tables detected | 0, 0 |

The database names are its reading, not ours (**Unconfirmed** per name). One cross-check agrees with ER-004:
`sceGsResetGraph@0x001CE130` is exactly where `docs/census.md` starts libgraph.

### 3.2 Recompiler (`ps2_recomp`)

| Measure | Heuristic boundaries | Our split's boundaries |
|---|---|---|
| Functions | 2,991 | **3,606** (all of them) |
| Recompiled / stubbed (HLE) | 2,836 / 155 | **3,359 / 247** |
| Decode failures | 0 | **0** |
| **Unhandled instructions** | **0** | **0** |
| Errors / warnings | 0 / 324 | 0 / 304 |
| Warnings are | "unresolved JR/JALR", handled by promoting fallback entry points (36,948) | same (33,236 fallback entries in 304 functions) |
| Extra resumable entry points | 32,096 | 30,162 |
| `TODO_NAMED` (skipped) wrappers | 0 | 0 |
| `// Unimplemented` COP0 / FCR / VU control register reads or writes in the output | 0 | 0 |
| `SYSCALL` sites routed to `runtime->handleSyscall` | 136 | 140 |
| Output | 3,000 files, 54.6 MB, 1,219,771 lines of C++ | 3,610 files, 54.6 MB, 1,220,633 lines of C++ |
| Run time | about 2 s (4 output workers) | about 2 s |

Why the heuristic run stubs only 155 of the 250 configured stubs is not checked (**Unconfirmed**; probably its
function starts do not line up with all the bound addresses).

So the instruction set this game uses (ER-002 counted 2,222 VU0 macro ops, 330 MMI ops, 408 `lq`/`sq`, COP1
throughout) is all accepted: no instruction is rejected or stubbed. **Accepted is not the same as correct**:
`docs/research.md` §3 lists open upstream bugs in PROT3W, PSLLH/PSRLH/PSRAH, PADDUH/PSUBUH, PABSW/PABSH,
BC0T/BC0TL and VU0 R-register ops. Whether this game uses any of them was not counted. *(ER-020 counted it: see
`docs/recomp_opcodes.md`.)*

What the C++ looks like: one statement per guest instruction, with the address and disassembly as a comment,
for example `vmulax.xyzw` becomes `PS2_VMUL` on `__m128` with `_mm_blendv_ps` masking. Guest floats follow PS2
rules in `ps2_runtime_macros.h` (no Inf / NaN / denormal, saturate to 0x7FFFFFFF, x/0), checked upstream against
ps2autotests. Gaps seen in passing: denormal operands are left to the host (stated in the header);
`PS2_VDIV` and `PS2_VMULQ` are plain SSE without the PS2 range handling; the PS2's round-toward-zero is not
mentioned (**Unconfirmed** whether it is modelled); and the Release build flags of the runtime use `/fp:fast`
and `/arch:AVX2`, which can change float results (**Unconfirmed** in practice; the spike game build was
RelWithDebInfo, without them). For an as-shipped port these are the first things to test.

### 3.3 Does the C++ compile?

**Yes, cleanly.** The 3,610 generated files go in `ps2xRuntime/src/runner` and `include` of the
`game_build` clone (that is the upstream way); `tools/recomp/build_game.bat` builds `ps2EntryRunner` in
RelWithDebInfo (no link-time code generation, to avoid the C1001 above) with `-j 4`, reusing the raylib / imgui
sources already fetched. Result: 231 steps (unity build, batches of 32), **0 errors, 0 warnings, 237 s**,
`ps2EntryRunner.exe` 36 MB. A Release (LTCG) build of the game was not tried.

### 3.4 Boot try (LuciDin's yes, 17/18 Nov 2026)

One 60-second run, window moved to the secondary monitor and audio never initialised (two local lines in the
`game_build` clone: `SetWindowPosition(2020, 100)`, `InitAudioDevice()` commented out). Disc files copied to
`C:\rs\recomp\boot` (all but `DUMMY.OUT`; the runtime writes `mc0`, `mc1`, `imgui.ini` and a log next to the
ELF, so it must not run in `disc/extract`). Killed by PID after 60 s; it was still running (63 s CPU, 215 MB).

What happened, from the runtime's own logs:

1. ELF loaded (one segment 0x100000-0x1B7BAA0), entry 0x100008, heap set up at 0x1B7BAA0.
2. `sceSifInitRpc`, then **9 IRX modules loaded and started in PS2Recomp's IOP interpreter** from
   `cdrom0:\IOP\`: SIO2MAN, PADMAN, MCMAN, MCSERV, LIBSD, MODMIDI, MODHSYN, SDRDRV ("sdr driver version 1.4.0"),
   FSIOPSND. 45 SIF DMA transfers to the IOP followed (sound data, **Unconfirmed**).
3. `sceCdSearchFile("\ERDATA.BIN;1")` found it (LSN 0x100000, size 0x19CC8000).
4. Memory cards probed on both ports (`GetDir '/BISLUS-20015??'`), empty cards reported.
5. GS set up through GIF packets (display buffers, registers 0x41/0x42, 0x50-0x53 image transfers).
6. An MPEG (PSS) stream started: 4 `MPEG:DemuxPssRing` calls (64 KiB each) fed 32 video elementary-stream
   packets, the first starting with a sequence header (`000001B3`). The host decoder produced frames (its own
   count reached 3; the demux counter says `decoded=9`), then the ring stopped with "BACKPRESSURE decoded=9
   avail=327680" repeated: decoded pictures were not taken off the queue, so the stream stopped advancing (my
   reading of the log, **Unconfirmed**). Which `.PSS` file it was is not in the log (the first one on the disc in boot order is presumably
   AGETEC.PSS: **Unconfirmed**).
7. From then on the EE sat in a loop: PC 0x1C529C, one thread, about 1.8 million passes through the same 8
   functions (`func_001C5188` ... `func_001C8980`, which are in ER-004's game region, plus `func_001D0010` in
   libkernl). In 60 s only **141 of the 3,606 functions ran**. No picture was checked (no screenshot taken).

**Second run (Wed 18 Nov 2026, 00:53-00:54 Eastern, LuciDin's "run it again" through the dispatcher).** Same
setup, rebuilt with `PS2X_ENABLE_AGRESSIVE_LOGS=OFF` (no trace file was written; the console log now ends after the
SIF module loads, so it no longer shows the MPEG lines). Child-process error dialogs were off (`SetErrorMode`).
Window screenshots every 10 s (`C:\rs\recomp\shots\boot2_10s.png` ... `boot2_60s.png`, outside git). **From
10 s to 60 s the screen is one solid magenta frame** (the PS2 picture area, black bars at the sides) with the
runtime's own "Runtime Debugger" panel over it showing `PC 0x001C529C RA 0x001C529C SP 0x01EFFA90 GP 0x00360B70`.
That is the same stall as run 1. LuciDin confirmed from his monitor: "It's just this". So no movie frame and no
game picture ever reaches the screen. What the magenta means (for example the runtime's fill colour for a frame
it has not been given) is **Unconfirmed**. The window opened maximised on the secondary monitor, not at the
position the local patch asked for (not looked into).

**Not chased** (project rule: ask LuciDin first). Question for LuciDin: on a real PS2, what does the game show
first after the PS2 logo (the Agetec movie, a FromSoftware logo, a warning screen)? That tells whether step 6
is the expected first movie and the stall is the MPEG / IPU path. *Answered and chased by ER-018: see section 8.*

### 3.5 What the runtime still lacks (for this game), as far as the spike saw

| Area | State |
|---|---|
| SCE library HLE | 543 named stub handlers and 121 syscall handlers in `ps2_call_list.h`; the analyzer bound 250 of the game's SDK functions to them, 252 more have no handler. Families present: CD, SIF / RPC, pad, memory card, GS, DMA, MPEG, IPU, TTY, libsd, file I/O, VU0, font, Deci2 |
| MPEG / IPU (movies) | **The first blocker seen**: frames decode, then the PSS ring stalls on backpressure (3.4 step 6) |
| GS | CPU rasteriser backend (`gs_cpu_backend.cpp`); upstream open issues on fill rule, sub-pixel offsets, COLCLAMP, bilinear (`docs/research.md` §3). Not exercised beyond setup |
| VU1 | interpreter (`src/lib/vu/ps2_vu1_*.cpp`) and a VIF1 interpreter; README: "performance is very bad". The game's 4 VU microprograms (`.DVP.overlay`, ER-002) not exercised |
| IOP / SIF | works for this game's 9 modules at boot (real IRX code in the interpreter); `IOPRP165.IMG` not loaded or needed so far |
| Sound | SPU2 / libsd path not tried (run was muted) |
| Pad, memory card | pad not tried; memory card probe answers |

## 4. Gaps and risks

1. **Correctness, not coverage, is the open question.** No instruction is refused, but MMI and VU0 have known
   upstream bugs (3.2), and the float model has holes (denormal operands, VDIV / VMULQ, rounding mode,
   `/fp:fast` in Release). For our "as shipped first" rule, every opcode family the game uses needs tests
   against hardware or PCSX2 results.
2. **HLE replaces SDK code.** With the analyzer's config, 247 SDK functions are not run as recompiled code but
   swapped for runtime handlers. That is the fast route to a boot; the as-shipped route would instead recompile
   the SDK code too and emulate the hardware it touches. Both are possible with the same tool (just drop entries
   from `stubs`).
3. **Build quirks**: MSVC 14.41 C1001 on one test file; the debug-build assertion in the scheduler; the
   aggressive runtime logs (`PS2X_ENABLE_AGRESSIVE_LOGS` ON by default) wrote a **1.09 GB** trace in 60 s (the file
   was deleted; turn the option off for real runs).
4. **GPL-3.0.** A port linked with `ps2xRuntime` is GPL-3.0. Our decomp code is ours; a port repository that
   includes PS2Recomp has to be GPL. LuciDin to decide before any port code is published.
5. **Project health**: last commit 23 Oct 2026, 173 open PRs (`docs/research.md`). If upstream stalls, we carry
   our own fixes anyway.
6. **Function boundaries**: our split's 3,606 functions give a full, exact boundary set without Ghidra, and
   the census (ER-004) gives the SDK regions; both already line up with PS2Recomp's input format.

## 5. Recommendation

**Build on PS2Recomp; keep a fork for our fixes; do not write our own recompiler now.** (A judgement:
**Unconfirmed** until the boot is taken further.)

Reasons:

- It already accepts 100% of the game's instructions and functions, compiles cleanly on this machine with the
  tools already installed, and boots through IOP module loading, CD file lookup, memory card probe and GS setup
  in a first, untuned try. Writing our own recompiler to reach that point would take weeks, and would still
  need the same runtime (IOP interpreter, GS, VU1, HLE), which is most of PS2Recomp's value.
- Our split gives it exact function boundaries, and our decomp's matched C can later replace generated functions
  one by one (the hybrid build of the roadmap), since generated functions are per-address files.
- A fork (on LuciDin's GitHub, nothing pushed without his yes) is needed anyway: for our per-game override
  module, the as-shipped options (no `/fp:fast`, recompiled SDK instead of HLE where it matters), and fixes we
  find. Upstream fixes can be merged in; ours can be offered back.
- Reasons it could change: if the MPEG / GS / VU1 paths turn out to need deep rework for this game, or if the
  GPL is unacceptable for the port, the recompiler core (about 12k lines in `ps2xRecomp/src/lib`) is small
  enough to replace later while keeping the same output layout.

## 6. Reproducing it

```
rem 1. clone (source only; deps come from FetchContent)
git clone https://github.com/ran-j/PS2Recomp C:\rs\recomp\PS2Recomp
rem 2. build tools (MSVC 2022 + bundled CMake/Ninja, -j 4)
cmd.exe /c C:\rs\recomp\build_ps2recomp.bat          (copy of tools\recomp\build_ps2recomp.bat)
rem 3. function map from our split (needs make build/SLUS_200.15.syms.elf)
python -I tools\recomp\syms_to_csv.py build\SLUS_200.15.syms.elf C:\rs\recomp\work\split_functions.csv
rem 4. analyzer on a read-only copy of the ELF, then the two configs
ps2_analyzer.exe C:\rs\recomp\work\SLUS_200.15 C:\rs\recomp\work\analyzer_stripped.toml
python -I tools\recomp\make_configs.py C:\rs\recomp\work\analyzer_stripped.toml C:\rs\recomp\work C:/rs/recomp/work/split_functions.csv
rem 5. recompile
ps2_recomp.exe C:/rs/recomp/work/cfg_split.toml
rem 6. copy out_split\*.cpp to game_build\ps2xRuntime\src\runner, *.h to ...\include, then
cmd.exe /c C:\rs\recomp\build_game.bat               (copy of tools\recomp\build_game.bat)
rem 7. run (only with LuciDin's yes): ps2EntryRunner.exe C:\rs\recomp\boot\SLUS_200.15
```

## 7. Not verified

- Whether the generated code is *correct* for any function (no differential test was run).
- Which PSS was playing at the stall, and whether the screen showed anything (no screenshot).
- A Release / LTCG build of the game; the unit tests past the debug assertion.
- Whether a newer MSVC avoids the C1001.
- Whether Eternal Ring uses any of the MMI / VU0 opcodes with open upstream bugs. *Answered by ER-020 (19 Nov 2026):
  `docs/recomp_opcodes.md` (yes: `sqrt.s` and `cvt.w.s` are wrong at every site; VU0 FDIV / VFTOI edge cases; vf0
  not pinned in thread contexts; 128-bit alignment; the MMI the game uses is right by reading).*
- The analyzer's 502 SDK names, apart from the libgraph start.
- Why the heuristic run stubbed 155 functions, not 250.

## 8. ER-018: the first movie (Wed 18 Nov 2026)

Written by ER-018 (Opus 5.5, high), work 15:51 to about 16:40 Eastern. Question: why does the recompiled build stall
in the first movie (section 3.4), and can it get past it? **Short answer: the stall had three causes, all in
PS2Recomp's runtime, all fixed locally; the movie now plays to its end-of-movie wait, but (a) far too slowly and
(b) without a visible picture.** So the boot is **not** past the first movie yet; the two remaining causes are
written down in 8.5 for the next card.

Nothing here is in git except this text and `tools/recomp/ER-018_runtime_patches.md` (a description in our own
words). The patches, logs, screenshots and the PCSX2 captures stay in `C:\rs\recomp\` and `C:\rs\captures\er018\`.

### 8.1 Reference: what the real game does (PCSX2 2.5.185, real BIOS, LuciDin's yes)

One muted run of the US ISO, fast boot, PCSX2 window on the secondary monitor (LuciDin's saved layout). Instead of
driving the debugger GUI, the game's own variables were read over PCSX2's PINE socket (read-only, every 0.25 s;
`C:\rs\captures\er018\pine_probe.py`, log `pine_log.csv`). PINE and volume 0 were set in `PCSX2.ini` for the run
only; the file was backed up and restored byte-identical afterwards (checked with `diff`). Addresses and what they
hold come from the game code read in 8.2.

| Reading | Agetec movie (from about 10.6 s after launch) | Second run of the same player (from about 23.5 s) |
|---|---|---|
| CreateThread result for the decoder thread (`0x35C3C8`, priority 0) | **3** (valid id) | 5 |
| CreateThread result for the idle thread (`0x35C3CC`, priority 0) | **2** (valid id) | 4 |
| Picture size (`sceMpeg` at `0x34D068`) | 640 x 448 | 640 x 448 |
| Pictures decoded (`0x34D070`) | rises steadily to 206 (0xCE) at 17.4 s, so about 30 pictures a second for about 7 s | rising at the same rate when the run stopped |
| Display queue (`0x34D014`, 5 slots) | 4 to 5 all the time | 4 to 5 |
| VBlank handler "playing" flag / counter (`0x35C3E4` / `0x35C3E8`) | 1 / rises by about 60 a second | same |
| Screenshot | Agetec logo (red and white on black) | the opening FMV (lake and pier), so this is `EROPEN.PSS` (Unconfirmed by a file log) |

Between the two runs (17.9 s to 23.5 s) the player is idle (flag 0, thread ids unchanged), so the FromSoftware logo
is **not** drawn by this movie player (where it comes from: not checked). The first stream's packet layout matches
`AGETEC.PSS` (first private-stream-1 audio packet at file offset 0x1700E, which fits the callback counts in the
recomp log), but no log names the file: **Unconfirmed**.

### 8.2 How the game's movie player works (read from our split's asm)

The player is game code in `text_00150878` and has the same shape as the SDK's PSS sample player (my reading:
**Unconfirmed**; all role names below are **Unofficial**):

| Address | Role |
|---|---|
| `func_001C53D8` | setup: display env (`func_001C5F40`: `sceGsResetGraph(0,1,2,1)`, `sceGsSetDefDispEnv`, `sceGsPutDispEnv`), videoDec create (`func_001C87E0`: `sceMpegCreate` and six `sceMpegAddCallback`s, types 0 to 5), audio buffer (`func_001C57C0`: 0xC000 bytes on the EE, a 0x6000-byte IOP buffer, preset size 0x6000), two `sceMpegAddStrCallback`s (video `func_001C6FE0`, type 0; audio `func_001C7108`, type 2), two threads at **priority 0** (idle `func_001C5798`, decoder `func_001C8B48`), CD stream open, VBlank-start handler `func_001C6E30` (INTC 2) and GIF-DMA-end handler `func_001C6F50` (DMAC 2) |
| `func_001C5188` | main loop: CD stream to ring, `RotateThreadReadyQueue(0)` (`func_001C53D0`, syscall 0x2B; the stall PC 0x001C529C is its return address), `sceMpegDemuxPssRing`; display start gate at 0x001C52E8: audio preset (`func_001C5A48`: sent-to-IOP count `+0x50` >= `+0x48`) **and** display queue full (`func_001C8660`), then `func_001C6F90(1)` sets the playing flag; afterwards waits for the decoder to finish (0x001C5340-0x001C5388) |
| `func_001C8B48` / `func_001C8BB8` | decoder thread: wait for a free display slot (`func_001C8700`, yielding), `sceMpegGetPicture(mpeg, slot, 0x460)`, mark the slot full, yield; until `sceMpegIsEnd` |
| `func_001C6E30` | VBlank handler: if playing and `sceGsSyncPath(1,0)` reports idle, takes the oldest full slot and, by GS field (CSR bit 13), sends one of its two pre-built half-picture DMA packets (`func_001C6E00`), switching DISPFB with `func_001C64F8` (`sceGsPutDispEnv`) |
| `func_001C6160` | builds each slot's GIF packet: 40 x 28 IMAGE transfers of 16 x 16 RGBA32 macroblocks (column by column, 1 KiB each) to a texture at TBP 4480, then sprites into the frame buffer |
| `func_001C8CA8`, `func_001C8D30` | libmpeg **Nodata** (type 1) and **Background** (type 4) callbacks; **both call `func_001C5B38`, the only code that sends movie audio to the IOP** (SIF DMA) and raises the preset count |

### 8.3 The three faults in PS2Recomp (e563698) and the fixes

1. **Priority-0 threads were refused.** `EeScheduler::createThread` accepted priorities 1 to 127 only and returned
   KE_ILLEGAL_PRIORITY (-403) for 0, although the runtime itself runs the main thread at 0. The game creates its
   decoder and idle threads at 0, so neither existed (`activeThreads=1` in run 1) and nobody called
   `sceMpegGetPicture`; the demux filled its picture queue and then refused input ("BACKPRESSURE"). The real BIOS
   accepts 0 (8.1: ids 3 and 2). **Fix:** accept 0 to 127. After it: `activeThreads=3` (as on PCSX2) and the decoder
   ran, but stopped after 5 pictures (the 5 display slots were never emptied).
2. **libmpeg callbacks registered with `sceMpegAddCallback` were stored but never called.** The display never
   started because its gate waits for movie audio to be preset on the IOP, and the game sends that audio only from
   its Nodata and Background callbacks, which real libmpeg calls from inside `sceMpegGetPicture`. In the verbose
   trace, `func_001C5A48` ran 362,445 times and never passed; `func_001C5B38` ran 0 times. **Fix:** the HLE
   `sceMpegGetPicture` now calls every registered Background (type 4) callback once per call, on the calling
   thread, before the picture step (Nodata is not called: it refills the guest IPU path, which the HLE does not
   use). After it: audio was sent (`func_001C5B38` 7 calls in a short verbose run), the display started
   (`func_001C6F90`), the VBlank handler sent pictures and the GIF handler freed slots; the main thread reached the
   end-of-movie wait (PC 0x001C5368). The screen was still magenta.
3. **PMODE stayed 0, so no frame was ever presented** (the magenta is the host window's own fill colour when the GS
   backend returns no frame: `GenImageColor(..., MAGENTA)` in `ps2_runtime.cpp`). The HLE `sceGsSetDefDispEnv` wrote
   only DISPFB and DISPLAY into the game's env struct and left PMODE, SMODE2 and BGCOLOR as the game's zeroed
   memory, and the HLE `sceGsPutDispEnv` copies the struct into the registers, so both display circuits were off.
   **Fix:** the HLE `sceGsSetDefDispEnv` now computes what the shipped libgraph code at 0x001CE2E8 computes: PMODE
   0x66, SMODE2 from the GS params (non-interlaced 2; interlaced 3 in field mode, 1 in frame mode), DISPFB from psm
   and width, DISPLAY with the NTSC (DX 0x27C; DY +0x32 interlaced, +0x19 not) or PAL (0x290; +0x48 / +0x24) origin,
   MAGH = (2559 + w) / w - 1 and DH doubled in interlaced field mode, BGCOLOR 0; dx / dy are read from $t0 / $t1
   (EABI) instead of the runtime's register-or-stack guess. After it: PMODE 0x66, SMODE2 3, DISPLAY
   0x1BF9FF0183227C (DX 0x27C, DY 0x32, MAGH 3, DW 2559, DH 447), a frame is presented each time, and the magenta
   is gone.

### 8.4 Boot runs (muted, secondary monitor, killed by PID; LuciDin's yes)

| Run | Build | Result |
|---|---|---|
| 3 (60 s) | fix 1 | still magenta; a second thread (the idle thread) is now seen running |
| 4 (verbose, 24 s) | fix 1 | `activeThreads=3`; 5 pictures served, then the decoder waits for a display slot; the VBlank handler returns at once |
| 5 (60 s) | fixes 1, 2 | magenta; at 60 s the main thread is at the end-of-movie wait 0x001C5368 |
| 6 (verbose, 24 s) | fixes 1, 2 | display path runs; DISPFB1 switches to FBP 0x46 |
| 7, 8 (24 s, 30 s) | + a temporary latch log | PMODE 0 and SMODE2 0 at every presentation |
| 9, 10 (60 s, 40 s) | fixes 1-3 (+ debugger panel hidden at start) | black screen with some static garbage tiles that move with the field; no logo (screenshots every 2 s for 40 s) |
| 11 (verbose, 40 s) | fixes 1-3 | per picture: one 1,218,752-byte GIF packet, then textured sprites (TBP 4480, FST) into FBP 0 / 0x46 |
| 12 (150 s, temporary PC log) | fixes 1-3 | main thread at the end-of-movie wait from about 65 s to 150 s, the decoder still feeding the display; guest VBlank count only 928 after 150 s |
| 13 (30 s) | final build (fixes 1-3, no diagnostics) | same picture as run 10 |

### 8.5 What still blocks the first movie (for the next card)

1. **Guest VBlank runs at about 5 to 9 per second, not 60.** *(Fixed by ER-029: section 9.)* `EeScheduler` raises VBlank only when both 16.7 ms of
   host time **and** about 4.9 million EE cycles have passed. The movie threads spend their time in
   `RotateThreadReadyQueue` loops, which add few guest cycles per host second, so VBlank falls behind (run 12:
   vsync 469 at 75 s, 928 at 150 s; about 500 presentation latches in 60 s in run 9). The movie advances one
   picture per two VBlanks, so it plays 7 to 12 times too slowly and had not finished after 150 s. Likely
   direction: charge realistic cycle costs for syscalls and thread switches, or let VBlank follow host time when
   every thread is only yielding (to be compared with how PCSX2 counts them). Not tried.
2. **The movie picture is not visible.** The decoded pictures are real (FFmpeg, 640 x 448), they are written in the
   macroblock column order the game's packets expect (checked against `func_001C6160`), the packets reach the GS
   and frames are presented, yet the screen shows black plus static tiles. Not yet known whether the IMAGE
   transfers land at TBP 4480 in the CPU GS backend, or whether the sprite sampling is wrong. Next step: dump GS
   memory at TBP 4480 after one upload and compare it with the HLE frame.

Other HLE gaps seen on the way (not needed for this movie, not changed): the HLE `sceGsResetGraph` sends PMODE and
SMODE2 as A+D writes to 0x41 / 0x42, which the GS front end treats as SCISSOR_2 / ALPHA_1, so they never reach the
privileged registers (only its 0x59-0x5F writes do); `ChangeThreadPriority` still refuses priority 0;
`RotateThreadReadyQueue(0)` is read as "the caller's priority" (same result here, since the caller runs at 0);
`sceMpegGetPictureRAW8` is a TODO; the libmpeg Nodata callback is never called.

### 8.6 Files (outside git)

| File | What |
|---|---|
| `C:\rs\recomp\patches\er018_movie_boot.patch` | the three fixes as a diff against e563698 (`EeScheduler.cpp`, `Stubs/MPEG.cpp`, `Stubs/GS.cpp`); SHA-1 929e44d0... |
| `C:\rs\recomp\patches\er008_er018_local_run_tweaks.patch` | run conveniences, not fixes: ER-008's window position and no audio device, ER-018's debugger panel hidden at start (F1 shows it) |
| `C:\rs\recomp\game_build` | the patched clone; both builds (`out\game` normal, `out\game_logs` verbose) are built from it |
| `C:\rs\recomp\scripts\boot_run3.ps1` ... `boot_run13.ps1`, `C:\rs\recomp\build_game_logs.bat` | run and verbose-build scripts |
| `C:\rs\recomp\boot3_*.txt` ... `boot13_*.txt`, `C:\rs\recomp\shots\boot3_*.png` ... `boot13_*.png` | logs and screenshots (the verbose traces, 0.4-0.7 GB each, were deleted) |
| `C:\rs\captures\er018\` | PCSX2 PINE probe, run script, log and screenshots, `PCSX2.ini.bak` |

A description of the patches in our own words is in `tools/recomp/ER-018_runtime_patches.md`. Nothing is pushed
anywhere (the PS2Recomp fork waits on the GPL-3.0 decision, ER-019).

### 8.7 Not verified / Unconfirmed

- That the first stream is `AGETEC.PSS` and the second `EROPEN.PSS` (layout and screenshot match; no file log).
- The SDK-sample reading of the player and every Unofficial name in 8.2.
- That calling Background once per `sceMpegGetPicture` matches how often real libmpeg calls it (it is called while
  waiting on the IPU, so likely several times per picture); enough here, audio rate not measured.
- Movie audio: never heard (all runs muted; the IOP-side playback was not checked).
- The VBlank-pacing explanation in 8.5 item 1 is my reading of the scheduler code and the counts; no profile taken.
- Whether fix 3 is right for every game: it follows this game's libgraph 1.6x copy; other SDK versions may differ.
- The unit tests (`ps2x_tests`) were not run against the patched runtime.

## 9. ER-029: VBlank pacing (Thu 19 Nov 2026)

Written by ER-029 (Opus 5.5, high). Question: why does guest VBlank run at 5 to 9 Hz (8.5 item 1), and how should
the runtime pace it? **Short answer: the runtime's cycle counter is not a clock (it counts loop turns, not
instructions), and VBlank waited for it. With LuciDin's ruling, guest time now follows host time; VBlank runs at
59.94 Hz (measured) and the Agetec movie plays its 210 pictures in about 7 s, as on PCSX2. The boot still does not
leave the movie: the player never sees the end of the stream (9.5).** The change is a second local patch,
`C:\rs\recomp\patches\er029_vblank.patch`, on top of ER-018's (outside git; described in
`tools/recomp/ER-029_vblank_patch.md`).

### 9.1 Cause (read from the code)

- `EeScheduler` raises VBlankStart only when **both** its host deadline (16,667 us after the last one) **and** its
  cycle deadline (about 4.9 M EE cycles after the last one) have passed (`processDueDeadlines`).
- The EE cycle counter only moves at checkpoints: generated code charges a fixed **32 cycles per backward branch
  inside a function** (`eeCheckpointDue()`, emitted by `ps2xRecomp`'s `ControlFlowEmitter::emitInternalTarget`) and
  **8 cycles per transfer through the dispatcher** (`dispatchGuestBranch`, and each turn of `EeScheduler::run`).
  Straight-line code, direct calls and syscalls charge nothing, whatever their length. So the counter measures
  loop turns, not time.
- Eternal Ring's movie threads spend their time in `RotateThreadReadyQueue(0)` loops (8.2): each turn is a
  syscall, a thread switch and a dispatcher pass, which costs the host far more than the 8 to 40 cycles it is
  charged. The cycle deadline therefore arrives only 5 to 9 times per host second, and VBlank with it.
- The EE side also used the wrong rate: 16,667 us is 60.00 Hz, while the IOP side (`kVblankPeriodCycles` in
  `iop_emulator_const.h`) and the HLE CD stream clock (`Stubs/CD.cpp`) both use the NTSC field rate 60000/1001 Hz
  (59.94), so EE and IOP VBlanks drifted apart by 0.1%.

### 9.2 The model (LuciDin's ruling, 19 Nov 2026: "Both, staged")

Two options were put to LuciDin: (a) make guest time follow host time in the runtime; (b) make the recompiler
charge each block's real instruction count and keep the cycle gate. He chose both, staged: (a) now in ER-029,
(b) later as its own card, with host time kept as a floor.

What ER-029 does (all in `Kernel/EeScheduler.cpp` and `include/runtime/ee_scheduler.h`):

1. **Guest time = host time at the console clock.** The scheduler keeps a host anchor (the host instant of EE cycle
   0). Whenever events are processed, the EE cycle counter is pulled up to `(now - anchor) x 294,912,000`
   (`syncCycleToHostTime`). Cycle advances go through the existing `accountCycles`, so the **EE timers and the IOP
   also advance at the console rate** (the IOP runs `eeCycles / 8`).
2. **Host deadlines are derived from cycle deadlines** (`hostDeadlineForCycle` = anchor + cycle / 294.912 MHz), for
   VBlankStart, VBlankEnd, alarms and the idle wait on EE timers, so all events share one time base.
3. **VBlank at the NTSC field rate.** Field n starts at EE cycle `floor(n x 294,912,000 x 1001 / 60,000)`
   (4,920,115.2 cycles per field), computed from the field number each time, so no rounding accumulates; VBlankStart
   events carry their field number. VBlankEnd stays 500 us after VBlankStart (unchanged).
4. **Host time is polled at checkpoints.** Every 64 checkpoints, `checkpointDue` compares the host clock with the
   earliest host deadline and returns to the dispatcher if it has passed, so a deadline is noticed even inside
   generated code that charges few cycles.
5. **Unchanged:** if charged cycles run ahead of host time (fast loops), the guest still waits at the cycle deadline
   until the host deadline, so it never runs faster than the console.
6. **Long host stalls** (debugger, window drag, a long HLE call): the guest clock catches up by at most 250 ms
   (15 fields; **Unofficial** choice) and skips the rest of the stall, so a pause is not followed by a storm of
   VBlanks. Shorter stalls are caught up in full, as on a console whose game misses VBlanks while busy.

What the yield loops count as: nothing special. They charge what generated code charges; time passes by the host
clock while they spin, as it does on the console while its threads yield.

What matches the console and what does not:

| Behaviour | After ER-029 |
|---|---|
| VBlank count over time, VBlank-driven clocks, logic run once per VBlank, field parity (CSR bit 13) | at the console rate, 59.94 Hz (measured, 9.3) |
| EE timers, IOP progress (IOP VBlank waits, sound driver timing) | at the console rate in host time (by construction; not measured) |
| How many turns a busy-wait or polling loop makes per VBlank (and any RNG advanced in such loops) | **not** console-matched: depends on host speed. It did not match before either; matching needs real cycle costs from the recompiler (follow-up, 9.6) |
| A game frame that takes the host longer than a field | VBlanks still come on time and the game sees a missed VBlank, as an overloaded console would |

### 9.3 Measurement (run 1, Thu 19 Nov 2026, about 15:37 Eastern; 90 s, muted, killed by PID)

Build with a temporary diagnostic (removed before the final build) that printed, every 30 VBlanks, the VBlank count,
host seconds since the anchor, EE seconds (cycles / 294.912 MHz), and three game variables from 8.1: the playing flag
`0x35C3E4`, the handler's VBlank counter `0x35C3E8`, and `sceMpeg` frameCount `0x34D070`.

| Reading | Recomp after ER-029 | PCSX2 2.5.185 (ER-018's PINE log, 8.1) |
|---|---|---|
| VBlank rate | 5,310 VBlanks from 1.502 s to 90.090 s = **59.940 Hz**; EE time equals host time to within 1 ms throughout | handler counter about 60 a second |
| Movie start (playing flag 1) | about 1.33 s after launch (counter 10 at 1.502 s) | 10.89 s after launch (with BIOS and fast boot) |
| Pictures decoded | 9 at 1.5 s, 112 at 5.0 s, 197 at 8.0 s, **210** from 8.5 s on: about 7.0 s of decoding, one picture per two VBlanks | rises to 206 at 17.43 s, then the counter is reset; playback took 421 VBlanks (counter 0x1A5) = about 7.0 s, so about 210 pictures |
| After the last picture | playing flag stays 1, counter keeps rising, frameCount stays 210 until the run was killed at 90 s | videoDec state (`0x34D110`) goes 2 then 3, display queue empties, playing flag 0 at 17.94 s, then the FromSoftware logo |

Before ER-029 (8.4 run 12): 928 VBlanks in 150 s (6.2 Hz) and the movie unfinished after 150 s.

Run 2 (about 18:36 Eastern, 30 s): the final build, no diagnostics, runs the same way. The screen is black with
static tiles, as in ER-018. In run 1's 6 s screenshot a fragment of the **red Agetec logo** shows at the bottom edge
of the frame (`C:\rs\recomp\shots\er029_r1_06s.png`), which is a lead for ER-030 (a picture landing at the wrong
place). Run 2's window opened at x = 806, which may be the primary monitor (run 1: x = 2012); the window code was
not changed.

### 9.4 Files (outside git)

| File | What |
|---|---|
| `C:\rs\recomp\patches\er029_vblank.patch` | the change, as a diff against the ER-018 state of `ee_scheduler.h` and `EeScheduler.cpp` (237 lines; SHA-1 0bd1aee6...). Checked: applies on the ER-018 state to identical files and reverse-applies on `game_build` |
| `C:\rs\recomp\work\er029_base\` | the two files as they were after ER-018 (the patch base) |
| `C:\rs\recomp\scripts\er029_run1.ps1`, `er029_run2.ps1` | run scripts; logs `C:\rs\recomp\er029_r1_*.txt`, `er029_r2_*.txt`; screenshots `C:\rs\recomp\shots\er029_r1_*.png`, `er029_r2_*.png`; build logs `C:\rs\recomp\er029_build*.txt` |

### 9.5 What still blocks the first movie (not traced: LuciDin, "write it down only")

The main thread sits in the player's end-of-movie loop at 0x001C5368 (8.2): it loops until either
`func_001C8B00(videoDec)` is true (the CD ring holds no data, `func_001C89D8` / `func_001C8080`, **and**
`func_001DBC40(mpeg)` is non-zero) or `func_001C8980(videoDec)` (videoDec `+0xA8`, the state PCSX2 shows going to 3)
equals 3. A cheap read found that **`func_001DBC40` runs as recompiled guest code, not through the HLE
`sceMpegIsEnd`**: it returns the word at `*(mpeg + 0x40) + 4`, a field inside libmpeg's own state. The HLE MPEG path
replaced `sceMpegGetPicture` and friends, so most likely nothing ever writes that field (my reading,
**Unconfirmed**; that `func_001DBC40` is `sceMpegIsEnd` is also **Unconfirmed**, from its shape and its use). What
sets videoDec `+0xA8` to 2 and 3 was not looked up. Next step: find the writers of videoDec `+0xA8` and of the libmpeg
end field, and either bind `func_001DBC40` to the HLE `sceMpegIsEnd` or have the HLE write the field.

### 9.6 Follow-ups

1. The end of the first movie (9.5).
2. Stage 2 of LuciDin's ruling: per-block cycle costs from the recompiler (instruction count per block, charged at
   block exits), with host time kept as the floor. Touches `ps2xRecomp` (`control_flow_emitter.cpp`) and needs a
   full regenerate.
3. Unit tests (`ps2x_tests`) against the patched runtime: not run by ER-018 or ER-029 (no test in `ps2xTest` names
   the VBlank period, but scheduler tests may rely on the old cadence).

### 9.7 Not verified / Unconfirmed

- The PCSX2 side was not run again: the comparison uses ER-018's PINE log (8.1). That PCSX2 also counted 210
  pictures is inferred from its 421 VBlanks of playback (frameCount was last sampled at 206).
- That EE timers and the IOP now run at the console rate (by construction; not measured). The IOP now runs about
  36.9 M cycles per host second, far more than before; no slowdown was seen in the movie, but sound was never heard
  (all runs muted).
- VBlankEnd 500 us after VBlankStart (EE) against the IOP's period / 16 (about 1.04 ms): both unchanged; the real
  NTSC VBlank length was not checked.
- The alarm tick stays 64 us; one NTSC line is about 63.56 us (from the NTSC line rate; **Unconfirmed** for the
  PS2's SetAlarm unit). Whether Eternal Ring uses SetAlarm was not checked.
- The non-interlaced NTSC rate and PAL are not handled (the game runs interlaced NTSC: SMODE2 3, 8.3); some
  emulators use a slightly different rate for non-interlaced NTSC (**Unconfirmed**).
- The 250 ms catch-up cap and the 64-checkpoint poll interval are **Unofficial** choices.
