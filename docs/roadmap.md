# Project Ringsmith roadmap (draft, 17 Nov 2026)

Eternal Ring: FromSoftware, PS2 launch-window title (Japan 2000, US late 2000). Facts below marked `Unconfirmed`
until read from the disc.

## What makes PS2 different from Twinblade (PS1)

- **Several processors.** Game logic runs on the Emotion Engine (R5900: MIPS III/IV subset plus 128-bit MMI, an FPU
  that is single precision and not IEEE-exact, VU0 in macro mode as COP2). Sound, pad, memory card and disc I/O
  run as IRX modules on the IOP (R3000A), reached over SIF RPC. Geometry may use VU1 microcode and DMA chains to
  the GS. Matching decomp covers the EE ELF first; IOP modules are mostly Sony's (identify, do not decompile).
- **Toolchain unknown until ER-002.** Candidates: Sony/SN ee-gcc 2.9x, or Metrowerks CodeWarrior for PS2. The PS1
  playbook's levers are GCC 2.7.2 lore; re-confirm each one here.
- **Port is harder.** No PsyQ-sized shim: the PC side needs the SCE libraries (libgraph / libdma / libvu0 / libpad /
  libcdvd / libsd / SIF), the GS renderer, VU1 microcode (interpreted or translated) and float behaviour that
  matches the console where the game depends on it.

## Tracks and phases

**Phase 0: setup** (ER-001 to ER-005). Verified dumps, disc layout, toolchain named and running natively, splat
split building byte-identical, regression gate, census and cost estimate.

**Phase 1: matching decomp of the EE ELF.** Every game function in C that compiles byte-identical (SDK library code
identified and matched or linked as library). Method from the playbook: census, clone, bulk m2c, fan-out, escalate,
rulings once a day. Source of truth once complete; append-only after that.

**Phase 2: understanding.** Names, structs, globals, subsystems documented (data formats, script/event system,
ring/gem magic system, maps, enemies, sound).

**Phase 3: readable code.** Clean, named, commented source that still matches.

**Recomp / PC port track (beside Phases 1-3).**
1. Static recompile of the EE ELF to C (a PS2 recompiler, ours or an existing one), with the SCE library calls
   caught at their entry points and replaced by PC implementations (HLE).
2. Hybrid build: recompiled code with matched C swapped in function by function as Phase 1 lands.
3. As-shipped PC port: complete, stable, same feel; checked against PCSX2 by differential runs (RAM hashes, frame
   captures) on recorded routes.
4. Enhanced (EX) options afterwards, each optional with the as-shipped path kept.

**Port controls (LuciDin, 17 Nov 2026).** Gamepad input mapped to the DualShock 2 is the as-shipped path.
**Mouse and keyboard controls are a secondary goal**: planned for the port, but after the as-shipped port works,
as an optional (EX) control scheme with the gamepad path kept.

**Assets track (after the recomp is complete; LuciDin, 17 Nov 2026).** Asset rips (models, textures, maps,
audio, movies) in their own sidebar section, "Ringsmith Assets"; extracted assets stay out of git.

**Target version:** NTSC-U (US), the only disc in the project (LuciDin, 17 Nov 2026).

**Later (open):** other regions or versions (Japanese original, PAL), asset catalogue, player package.

## Accuracy bar

**A true 100% decomp** (LuciDin, 17 Nov 2026, as in Twinblade): every function in C that compiles byte-identical,
NON_MATCHING 0, proven by the byte-identical build (`make regress`). A function counts as matched only when that
build proves it; anything else (hand-written asm, binary blobs) needs a ruling and is counted apart.

Accuracy to the original code is THE goal; the port runs as accurately as an emulator in parallel. No deadline.
