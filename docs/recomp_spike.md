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
| Prebuilt binaries | none used. All dependencies are fetched as source by CMake `FetchContent`: ELFIO Release_3.12, toml11 v4.4.0, fmt 12.1.0, libdwarf v2.2.0, rabbitizer 1.14.3, nlohmann/json v3.11.3, raylib 5.5, imgui v1.92.7-docking, rlImGui Raylib_5_5 (Studio, not built: SDL 2.32.10, ImGuiColorTextEdit, imgui_club, ImGuiFileDialog). `.gitmodules` does not exist despite the README's `--recurse-submodules` |

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
| Workaround for the tests | Debug build (`out\dbg`) compiles. Run: **199 tests passed, 0 failed**, then the run aborts on a debug assertion, `m_executorThread == std::this_thread::get_id()` in `ps2xRuntime/src/lib/Kernel/EeScheduler.cpp:1552`. Not chased. Tests after that point did not run |

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
BC0T/BC0TL and VU0 R-register ops. Whether this game uses any of them was not counted.

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

**Not chased** (project rule: ask LuciDin first). Question for LuciDin: on a real PS2, what does the game show
first after the PS2 logo (the Agetec movie, a FromSoftware logo, a warning screen)? That tells whether step 6
is the expected first movie and the stall is the MPEG / IPU path.

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
- Whether Eternal Ring uses any of the MMI / VU0 opcodes with open upstream bugs.
- The analyzer's 502 SDK names, apart from the libgraph start.
- Why the heuristic run stubbed 155 functions, not 250.
