# Project Ringsmith handoff

**EVERY SESSION READS THIS PARAGRAPH.** Dispatcher: **"Ringsmith DISP-01"**, id `local_15d323e2-b845-4337-9f74-b894bdc51e69` (started Tue 17 Nov 2026, 12:01 Eastern).
(1) STEP 0 of every card: check your model, effort and Ultracode (get_session "self") and the plan usage (get_usage);
if they are not what the card names, stop, say what is needed and wait for LuciDin to switch and say go (if he says
he is overriding, carry on). (2) START MESSAGE: once STEP 0 passes, send the dispatcher one short message: task code
and title, model / effort / Ultracode, 5-hour and weekly readings, first step. (3) Weekly stop: **95% of the weekly
all-models bar (open: Ringsmith's share beside Twinblade, Cordona and Guild)**; below 95% ignore the 5-hour bar; at
95% (either bar) stop at a clean point, commit or save, tell the dispatcher, end the turn. (4) Before chasing a
suspected crash, hang or glitch, ask LuciDin what he saw and what the game normally does. (5) Do NOT propose cards.
(6) FINISH: update this note (thread and queue), report usage as a Field / Value table, send the dispatcher a
completion message (what changed, commits, what was not verified, follow-ups with model / effort), then tell LuciDin
to return to the dispatcher.

Written Tue 17 Nov 2026, 11:52 Eastern, by the setup session.

## State

- Local git repository initialised (branch `main`, nothing committed yet); `.gitignore` keeps disc images, ELFs,
  IRX modules, `disc/` and build output out of git. No disc dump in the folder yet.
- Sidebar sections created (ids in `docs/dispatcher_handoff.md`).
- Project rules drafted in `CLAUDE.md`, phases in `docs/roadmap.md`, dispatcher job in `docs/dispatcher_handoff.md`.
- Build (ER-003, 17 Nov 2026): `make regress` green, the whole `SLUS_200.15` rebuilds byte-identical from the split
  (`docs/build.md`); the pre-commit gate runs it for any commit touching the build.

## Decided (LuciDin, Tue 17 Nov 2026)

- Task codes: **ER-NNN** (session title prefix, brief, commits; next free: ER-015).
- Disc: **NTSC-U (US)** only; it is the matching target.
- Sidebar: dispatcher pinned in "Project Ringsmith"; cards in "Ringsmith Decomp", "Ringsmith Recomp / PC Port",
  "Ringsmith Misc", and "Ringsmith Assets" (asset rips, from when the recomp is complete; added 17 Nov 2026).
- Code: GitHub repository **https://github.com/SadBoxMan/ringsmith** (LuciDin, 17 Nov 2026). No remote added and
  nothing pushed yet: every push needs LuciDin's yes, given in that session, and a check that no game data is in the
  history.

## Open questions for LuciDin

1. ~~Tracker format~~ answered 17 Nov 2026, 12:05 Eastern: a tracker application (Decomp, Recomp and PC Port
   accuracy), laid out after Twinblade's progress tracker, with a shortcut in `Desktop\Tools`; card ER-006 after ER-001.
2. Weekly share of the usage bar for Ringsmith while Twinblade Phase 2 runs.

## The US disc (read Tue 17 Nov 2026, 12:00 Eastern; not dumped yet)

LuciDin's disc is in drive **D:** (TSSTcorp SN-208FB USB DVD writer). ER-001 starts from it: dumping is the first task.
SYSTEM.CNF: `BOOT2 = cdrom0:\SLUS_200.15;1`, `VER = 1.00`, `VMODE = NTSC`. The volume is 4,637,687,808 bytes (single-
layer DVD). Root: `SLUS_200.15` (2,485,924, the main EE ELF), `IOPRP165.IMG` (98,901, IOP module replacement image),
`IOP/` and `VOICE/` folders, `ERDATA.BIN` (432,832,512, likely the main data archive: Unconfirmed), six `.PSS` movies
(AGETEC, EROPEN, ERDEMO, TEXT0, TEXT1, STAFF), `DUMMY.OUT` (1 GiB, likely layout padding: Unconfirmed).

## Watch: when can the PC port be built? (LuciDin, 17 Nov 2026)

In Twinblade the PC port built not long after the decomp began, so the dispatcher checks after every completed
card whether a first PC build is now possible and says so to LuciDin. Tracker milestone `ms10`. What it needs:
(1) a recompiler that handles the ELF (ER-008 spike: PS2Recomp or our own); (2) SCE library entry points caught
and stubbed (HLE), using the census's SDK boundaries (ER-004); (3) a minimal runtime: memory map, syscalls,
a video path for the GS. It does not wait on Phase 1 matching; matched C is swapped in later (hybrid build).
Status: not yet possible (no recompiler result yet; ER-008 waits for LuciDin's click).

## Queue (the dispatcher turns these into cards; first ones need the answers above)

| Code | Task | Needs | Waits on |
|---|---|---|---|
| ER-001 (DONE 17 Nov 2026 except the redump comparison: needs LuciDin's saved redump page for SLUS-20015; ISO CRC32 BB98D81A, MD5 3F87FAD4395FE2AEA49D00CD3C921DAB, SHA-1 11DFDDCBD57F0424304CF821D627D8B7BC9A4F15; see `docs/disc_layout.md`) | Disc intake: dump the US disc in D: to an ISO (2048-byte sectors, whole volume; read errors logged, retried), hash it (CRC32 / MD5 / SHA-1) and check against redump's SLUS-20015 entry (LuciDin supplies the page if the site blocks fetching), extract the filesystem to `disc/` (gitignored), list every ELF / IRX / data archive with sizes and hashes, read SYSTEM.CNF, write `docs/disc_layout.md` | Sonnet, medium | nothing: disc is in D: |
| ER-002 (DONE Tue 17 Nov 2026; see `docs/toolchain.md`) Result: GCC (Cygnus r5900 "ee" port) + GNU as / ld from SCE's EE toolchain, Confirmed; no Metrowerks or SN marks. Build fits **ee-gcc 2.9-ee-991111** (Unconfirmed: 3 test functions identical in shape at `-O2 -G8`; `-O1` and `-G0` differ). EABI64, gp `0x00360B70`, SDK runtime libs 1.6.x (PsII tags 1600-1660), newlib. **Windows-native compiler works**: built from the GPL source under MSYS2 (host x86_64-pc-msys, bison 1.25 from the tree, LF clone), `C:\rs\toolchain\ee-gcc-2.9-991111\` (script in toolchain.md appendix A). Open: no assembler / linker yet (build Cygnus gas / ld under MSYS2, or decompals Windows binutils after LuciDin's yes) | Toolchain identification: read the main ELF (sections, `.comment`, `.mwcats`, symbol remnants, SDK library version strings), name the compiler (ee-gcc 2.9x / SN / Metrowerks) and SDK version, find a matching compiler build that runs natively on Windows, write `docs/toolchain.md` ER-005 found no Windows-native ee-gcc path (all PS2 decomp setups assume Linux / WSL / Docker / Wine, which are off here); SSX Tricky's ee-gcc 2.9-ee-991111-01 is a lead (`docs/research.md`). Try an MSYS2 build of the GPL ee-gcc sources; if that fails, ask LuciDin for a ruling before going further | Opus, high | ER-001 |
| ER-003 (DONE Tue 17 Nov 2026; see `docs/build.md`, `docs/toolchain.md` 6.1, `docs/decomp_recipes.md` 1.3 / 1.4) Result: **the whole ELF rebuilds byte-identical** (SHA-1 6087A8CA..., headers, `.reginfo`, DVP sections and section table included). splat 0.50.1 (`.venv`, `requirements.txt`), `slus_200.15.yaml`, `Makefile` (`make split / check / regress`), `tools/gen_ld.py` (SCE app.cmd layout), `make regress` about 30 s, pre-commit gate (`core.hooksPath tools/hooks`). 3,606 function bodies (4 crt0, 3,475 INCLUDE_ASM, 127 empty ones already C) plus 3 data-labelled blocks in `.text`; `.text` 984,916 bytes. Toolchain scripts and ER-011's compare tool in `tools/toolchain/`. Not verified: a fresh-machine setup run (only this machine); the pacman package list. Commits 542ed01 (build) and the ER-003 docs commit. LuciDin's rulings (elf_layout.s, DVP bytes, denormal `.word`s accepted; "matched" = proven by the byte-identical build, goal true 100%) in `docs/decomp_recipes.md` 3 | Byte-identical split: git init, splat yaml for the main ELF (and any overlays), Makefile, all functions as INCLUDE_ASM, SHA-1 check, regression gate (`make regress`) from day one; also moves `build_ee_gcc.sh` into the repo (e.g. `tools/toolchain/`); needs from `docs/toolchain.md` section 6; also moves `build_ee_binutils.sh` and `C:\rs\toolchain\tests\er011\er011_compare.py` into `tools/toolchain/`, and gives the Makefile the `-B` / `-mabi=eabi` assembler flags (`docs/decomp_recipes.md` 1.1) | Opus, high | nothing: toolchain byte-checked by ER-011 |
| ER-004 | Census: every function, shape hashes by size band, SDK library functions told apart from game code, clone families; cost estimate for Phase 1. Inputs (ER-003): `make split` output `asm/nonmatchings/text_001000C0/*.s` (3,478 files), `src/text_001000C0.c`, `build/SLUS_200.15.syms.elf` (function symbols with sizes), `docs/build.md` 4 (counts), `docs/toolchain.md` 6.1 (3 data-labelled blocks, crt0, alternate entries) | Opus, medium | nothing: ER-003 done |
| ER-005 | Research survey: existing Eternal Ring docs, mods, decomp or recomp work (check anti-AI rules per site first); PS2 static recompiler options (e.g. PS2Recomp) and what they cover | Sonnet, medium | none. DONE 17 Nov 2026 (see `docs/research.md`; open leads listed there) |
| ER-006 (DONE 17 Nov 2026) | Tracker application: track the status of the Decomp, Recomp and PC Port accuracy; layout after Twinblade's progress tracker (`Project Twinblade/tools/progress_tracker.py`, `docs/progress_data.json`); shortcut "Ringsmith Progress Tracker" in `Desktop\Tools\Ringsmith` (LuciDin, 17 Nov 2026). Done: `tools/progress_tracker.py` (tkinter), data `docs/progress_data.json`, launcher `Programs/Progress Tracker/Open Progress Tracker.bat`, guide `docs/tracker.md`; tracks Decomp / Recomp / PC Port / Assets, all metric totals "not measured yet" until ER-003 / ER-004. The dispatcher keeps `docs/progress_data.json` current from completion messages | Opus, medium | ER-001 |
| ER-007 | Second-pass research: the leads in `docs/research.md` section 5 (almarsguides NTSC-U page, rac1-decomp Windows build doc, ps2-ee-toolchain MSYS2 buildability, decomp.me PS2 presets, kokolika PAL fork); licences (-only / -or-later), PS2Recomp last commit; LuciDin's saved HTML for GameFAQs, PCSX2 compat, TCRF if he supplies it. Files: `docs/research.md` only | Sonnet, medium | **DONE Tue 17 Nov 2026**: `docs/research.md` §7 to §10 (leads closed, Windows-native ee-gcc ranking, licences). Open: ER-002 to identify the compiler first; ruling needed from LuciDin on the AngheloAlf SN ProDG mirrors |
| ER-008 | Recomp spike: build PS2Recomp (ran-j, GPL-3.0) natively with MSVC and run it on `SLUS_200.15`; record instruction coverage gaps, what it emits, what fails | Opus, medium | ER-001 |
| ER-009 | Redump check: compare the ISO's CRC32 BB98D81A / MD5 3F87FAD4395FE2AEA49D00CD3C921DAB / SHA-1 11DFDDCBD57F0424304CF821D627D8B7BC9A4F15 (`docs/disc_layout.md`) with redump's SLUS-20015 entry from LuciDin's saved page (redump.org was unreachable from ER-001); record match / mismatch in `docs/disc_layout.md` | Sonnet, low | LuciDin's saved redump page |
| ER-010 | CodeBreaker codes check (optional, Phase 2): fetch the 4 almarsguides NTSC-U sub-pages ER-007 did not read, decode the codes (they may be in encrypted form) and compare the addresses with RAM; record in `docs/research.md` | Sonnet, low | Phase 2 |
| ER-011 (DONE 17 Nov 2026) | EE binutils on Windows: build gas / ld from the same Cygnus tree (SSXModding/ps2-ee-toolchain b595ded) under MSYS2 and byte-compare the 3 ER-002 test functions. Done (commit a90158c): binutils 2.9-ee-991111 built first try, no prebuilt used; `ee-as`, `ee-ld`, `ee-objdump` etc. in `C:\rs\toolchain\ee-binutils-2.9-991111\`, run from PowerShell; script in appendix B of `docs/toolchain.md`. All 3 functions **byte-identical** (33 words; relocations masked, then exact after an `ee-ld` link); `docs/toolchain.md` 4.2 / 4.3, `docs/decomp_recipes.md` started. Not built: `ee-dvp-as` (VU assembler). Not verified: code with `.lit4`, `%hi`/`%lo`, branches, MMI / VU0 | Opus, medium | nothing |
| ER-012 | VU assembler (optional, low priority): build `ee-dvp-as` (target ee-dvp) with ER-011's script; done when it runs and reassembles one disassembled `.DVP.overlay` byte-identically. Files: `C:\rs\toolchain`, `docs/toolchain.md` 4.2 | Opus, medium | a decision to rebuild the VU microcode from source rather than keep it as binary |
| ER-013 | File split: cut `src/text_001000C0.c` into game files and SDK files at the boundaries ER-004 finds (c subsegments in `slus_200.15.yaml`, seams on function starts), and give data its own per-file splits where the census shows file boundaries. Done when `make regress` is green after the split | Opus, medium | ER-004 |
| ER-014 | First real C matches: a few small functions with branches, `%hi` / `%lo` and `.lit4` constants, to confirm `-O2 -G8` beyond empty functions; lessons into `docs/decomp_recipes.md` 2. Done when they are placed and `make regress` passes | Opus, medium | nothing (after ER-013 if both are queued, to avoid touching `src/` at once) |
