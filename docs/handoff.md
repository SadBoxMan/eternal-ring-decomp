# Project Ringsmith handoff

**EVERY SESSION READS THIS PARAGRAPH.** Dispatcher: **"Project Ringsmith dispatcher #1"**, id `local_15d323e2-b845-4337-9f74-b894bdc51e69` (started Tue 17 Nov 2026, 12:01 Eastern).
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

## Decided (LuciDin, Tue 17 Nov 2026)

- Task codes: **ER-NNN** (session title prefix, brief, commits; next free: ER-007).
- Disc: **NTSC-U (US)** only; it is the matching target.
- Sidebar: dispatcher pinned in "Project Ringsmith"; cards in "Ringsmith Decomp", "Ringsmith Recomp / PC Port",
  "Ringsmith Misc".
- Code: **local git only for now**; GitHub decided once the split builds byte-identical.

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

## Queue (the dispatcher turns these into cards; first ones need the answers above)

| Code | Task | Needs | Waits on |
|---|---|---|---|
| ER-001 | Disc intake: dump the US disc in D: to an ISO (2048-byte sectors, whole volume; read errors logged, retried), hash it (CRC32 / MD5 / SHA-1) and check against redump's SLUS-20015 entry (LuciDin supplies the page if the site blocks fetching), extract the filesystem to `disc/` (gitignored), list every ELF / IRX / data archive with sizes and hashes, read SYSTEM.CNF, write `docs/disc_layout.md` | Sonnet, medium | nothing: disc is in D: |
| ER-002 | Toolchain identification: read the main ELF (sections, `.comment`, `.mwcats`, symbol remnants, SDK library version strings), name the compiler (ee-gcc 2.9x / SN / Metrowerks) and SDK version, find a matching compiler build that runs natively on Windows, write `docs/toolchain.md` | Opus, high | ER-001 |
| ER-003 | Byte-identical split: git init, splat yaml for the main ELF (and any overlays), Makefile, all functions as INCLUDE_ASM, SHA-1 check, regression gate (`make regress`) from day one | Opus, high | ER-002 |
| ER-004 | Census: every function, shape hashes by size band, SDK library functions told apart from game code, clone families; cost estimate for Phase 1 | Opus, medium | ER-003 |
| ER-005 | Research survey: existing Eternal Ring docs, mods, decomp or recomp work (check anti-AI rules per site first); PS2 static recompiler options (e.g. PS2Recomp) and what they cover | Sonnet, medium | none |
| ER-006 | Tracker application: track the status of the Decomp, Recomp and PC Port accuracy; layout after Twinblade's progress tracker (`Project Twinblade/tools/progress_tracker.py`, `docs/progress_data.json`); shortcut "Ringsmith Progress Tracker" in `Desktop\Tools\Ringsmith` (LuciDin, 17 Nov 2026) | Opus, medium | ER-001 |
