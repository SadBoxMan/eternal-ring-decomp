# Project Ringsmith dispatcher handoff

For every dispatcher session. Read this, `docs/handoff.md`, `docs/roadmap.md`, `docs/usage_log.md` and the memory index
(`C:\Users\lukea\.claude\projects\C--Users-lukea-Claude-Projects-Project-Ringsmith\memory\MEMORY.md`) at start.
Modelled on the Project Twinblade dispatcher (17 dispatchers, Oct-Nov 2026); its `docs/dispatcher_handoff.md` holds
the full history of how the rules below were learned.

## The job

Hand out work as task cards. Never do project work yourself: no code reading, no edits beyond the handoff note's
queue, the tracker, `docs/usage_log.md` and this file (commit those). One card per task: `start_session` with fresh
context and the full standard brief below. Right after a session starts: detach it (detach_session), file it in its
sidebar section, rename it with its task code if needed. When a session reports done, check what it changed (git
log, the diff, the tracker) before telling LuciDin it is done; then update the tracker (every completed card, without exception: LuciDin, 17 Nov 2026), the usage row and the
handoff note in one small commit, and offer the next card. Keep turns short.

## Rules and quirks (learned on Twinblade)

- **Proposals vs starting.** After another session's message or an automated event, `start_session` is refused;
  propose with initiation "own_initiative" and LuciDin clicks the card. A proposed card cannot be edited: dismiss it
  and propose again. Cards he starts come back as "The user started your proposal" reminders: detach and file them.
- **Model check.** A session cannot start above the dispatcher's own model and effort; run the dispatcher at the
  highest setting its cards need (Twinblade and Cordona ran theirs on Opus high for this reason). Each card stops at
  STEP 0 until the picker matches; tell LuciDin which setting to pick.
- **Numbering.** Dispatchers are titled "Ringsmith DISP-##" (two digits: DISP-01, DISP-02, ...; LuciDin, 17 Nov 2026). Hand over at about 50% context: the outgoing one writes a
  "STATE FOR A FRESH DISPATCHER" section here, starts the next DISP-##, confirms it is reachable by message, then updates the
  id in `docs/handoff.md` and the memory note. A fresh dispatcher at each weekly reset too.
- **Managerial rights.** Each card's STEP 0 asks LuciDin (interactive yes/no) whether the dispatcher approves its
  commits, merges and docs commit. On yes, approve only requests that pass the gates (byte-identical build,
  `make regress`, expected files only, accepted trick forms only, LF). Non-standard requests (new trick forms,
  design choices, scope changes, deletions, re-baselines) always go to LuciDin. A relayed yes is not his yes.
- **Rulings once a day.** At the start of LuciDin's day, one table of every pending ruling (function, form,
  recommendation, yes / no).
- **Writers never share files.** Two cards that edit the same files never run at once. Matching streams own whole
  source files; one port card at a time (shared build output); timing-sensitive runs one at a time.
- **Before LuciDin leaves the desk** (office days Wed / Thu 9 AM-2 PM, and before 2 AM), line up the next long run:
  cards cannot be started while he is on Remote Control. Do not turn Remote Control on for new sessions unless asked.
- **Backups** are the dispatcher's job (after every 3-5 completed tasks and at block end); sessions never run them.
  Run `python tools/backup.py` from the repo root (ER-025): a zip of our sources plus `repo.bundle` in
  `F:\Claude\Project Ringsmith\backups\`; it prints one line: check it says "bundle verify okay". Never run by work
  sessions. Log each one below.
- **Archive** work sessions once verified finished; never ones still working, waiting on LuciDin, or holding
  uncommitted work.
- **Windows paths in scripts.** Write them as Python raw strings (`r"C:\rs\..."`): plain strings turned `\r` / `\t`
  into control characters in `docs/handoff.md` twice on 17 Nov 2026. Check with `grep -P '[\x00-\x08\x0b-\x1f]'`.
- **PC port watch.** After every completed card, check the "Watch: when can the PC port be built?" section of
  `docs/handoff.md` and tracker milestone `ms10`; update its status line and tell LuciDin as soon as a first PC
  build becomes possible (LuciDin, 17 Nov 2026).
- **Usage records.** One row per session in `docs/usage_log.md` from each completion message; LuciDin uses the log
  to teach others about cost, so keep it complete.

## Standard card brief (copy it; fill the task)

"ER-NNN: <title>. Needs: <model>, <effort>, Ultracode <on/off>.
STEP 0: check your own model, effort and Ultracode (get_session self) and the plan usage (get_usage). If not what the
card needs, stop, say so and wait for LuciDin to switch in the picker and say go (if he says he is overriding, carry
on). If the weekly all-models bar is at the week's stop, stop and tell him. Ask LuciDin (AskUserQuestion, yes/no):
'Give the dispatcher managerial rights for this card (it approves commits, merges and the docs commit)?'. Then send
the dispatcher (<id>) a start message: code and title, model / effort / Ultracode, 5-hour and weekly readings, first
step. Report usage at start and end as a Field / Value table (Plan; Model / effort in use and needed; Ultracode;
5-hour % and reset; Weekly all models % and reset; Weekly Fable; wall time and work units; parallel sessions).
Read first: CLAUDE.md, docs/handoff.md, <task docs>.
Task: ...
Done looks like: ...
Rules: accuracy first, no corners cut without asking; nothing made up (mark Unconfirmed / Unofficial and list it);
no game data in git; before chasing a suspected crash or glitch ask LuciDin what he saw and what the game normally
does; commit only your own files (temporary index or explicit paths, LF), ask first unless managerial rights cover
it, never push; builds -j4; launches on the secondary monitor, muted; call cmd.exe, never bare cmd; below 95% ignore
the 5-hour bar, at 95% stop at a clean point and tell the dispatcher; do NOT propose cards: put follow-ups (task,
files, done-when, model / effort / Ultracode) in the completion message. Fan-out agents skip the model check (put that
line in every agent prompt) and get a token budget per batch.
Finish: docs, handoff note and tracker updated (`python tools/progress_tracker.py task ER-NNN --status Done --notes "..."`,
plus `metric` / `milestone` / `item` for anything measured or moved; see docs/tracker.md), usage table, completion message to the dispatcher, tell LuciDin to return to
it."

## Sidebar sections (LuciDin, 17 Nov 2026)

The dispatcher is pinned. After start_session and detach_session, file each card with move_sessions (section_id):
- "Ringsmith Decomp" `cg-c48eb8a6-ce65-440a-a3c5-ca5425ba91d4`: matching, census, toolchain, splits.
- "Ringsmith Recomp / PC Port" `cg-013fceec-fa15-4551-84be-da231715c2c6`: recompiler, HLE libraries, renderer, port.
- "Ringsmith Misc" `cg-2635b73d-871c-45d1-b4e6-9d5799f1a44d`: disc intake, research, docs, tracker.
- "Ringsmith Assets" `cg-d09c277b-8fcc-4b4e-b4f6-928cb158929c`: asset rips; they begin after the recomp is complete
  (LuciDin, 17 Nov 2026).

## STATE FOR A FRESH DISPATCHER (DISP-01 -> DISP-02, Thu 19 Nov 2026, 15:15 Eastern)

**Where the project is.** Phase 0 complete. Native toolchain (ee-gcc 2.9-ee-991111 + patched ee-binutils `-r1`, MSYS2,
`C:\rs\toolchain`), whole ELF byte-identical, `make regress` (~30 s; pre-commit hook), 10 game + 15 SDK source files,
every game file owns its `.lit4` / `.rodata` (5 also `.sdata` / `.data`). Matched: 153 / 3,609 functions (2,352 bytes).
Recomp: PS2Recomp e563698 + local patches (`C:\rs\recomp\patches\er018_movie_boot.patch`) builds a native exe that runs
the Agetec movie player; picture = scattered red / white tiles, sometimes the correct top half of the logo (LuciDin);
VBlank 5-9 Hz. Tracker `docs/progress_data.json` is current; usage log current; last backup at acf0284 (run
`python tools/backup.py` after 3-5 more cards).

**Running.** ER-029 (recomp VBlank pacing, Opus high, `local_f3810f8c-e5f4-4855-b904-7a92d3b6a55b`): WAITING for
LuciDin's ruling on its timing model (asked ~00:00 19 Nov); no code changed yet. It reports to the dispatcher id in
`docs/handoff.md` (DISP-01 told it the new id).

**Next cards (one `src/` writer at a time; queue rows in `docs/handoff.md` hold the full task text):**
- Decomp, ready: ER-035 (87 FP-compare functions, Opus medium), ER-034 (mul.s / div.s nop pattern, Opus high),
  ER-031 (cut `text_001343E8` near S3, Opus medium), ER-027 (more plain-C incl. one function with own data, Opus
  medium), ER-015 (matching tools, Opus medium) -> ER-016 (pilot, ~40 shapes, Ultracode for that card only).
  ER-023 (seam method, Opus medium; `tools/seams.py`, `docs/game_seams.md`). ER-032 waits on ER-031.
- Recomp: after ER-029: ER-030 (movie picture, Opus high; LuciDin's top-half observation is in its row), ER-033
  (FFmpeg from source, minimal LGPL; decided by LuciDin 19 Nov), ER-020 (opcode check, Opus medium, ready now, no
  shared files with ER-029).
- Waiting on LuciDin: ER-019 (PS2Recomp fork: GPL-3.0 decision), ER-009 (saved redump page), ER-021 (newer VS),
  ER-012 (VU assembler, only if VU microcode is rebuilt from source), ER-010 (Phase 2).

**Open decisions for LuciDin:** GPL-3.0 for the port (ER-019); a saved redump SLUS-20015 page (ER-009); Ringsmith's
weekly usage share (handoff Q2); first GitHub push (https://github.com/SadBoxMan/ringsmith: nothing pushed; author
email would become public, see the 18 Nov note).

**Rulings so far:** `docs/decomp_recipes.md` 3.1-3.4 (true 100%, matched = proven by regress; ELF layout pieces; SDK
asm as .s, VU0 / MMI inline asm, SDK in scope after game code; patched gas adopted; fall-off-the-end form accepted with
a PORT NOTE). Port: as shipped first; DualShock-family controllers first, Xbox later, mouse / keyboard secondary
(`docs/roadmap.md`). Assets track after the recomp. PC port watch: `docs/handoff.md` + tracker `ms10`.

**Quirks learned here.** (1) Commit through a temporary index (`GIT_INDEX_FILE=.git/disp-index`, `git read-tree HEAD`,
add, commit, then `git reset -q -- <paths>` on the shared index) whenever a card is working in the checkout; cards that
commit with their own temporary index leave the shared index stale (`git status` then shows staged reversals: fix with
`git reset -q -- <paths>`, never `git reset --hard`). (2) Windows paths in Python heredocs: use raw strings or files in
the scratchpad; plain strings turned `\r` / `\t` into control characters four times; check docs with
`grep -caP '[\x00-\x08\x0b-\x1f]'`. (3) The app caps starts per user message (about 5 working at once / 10 per
message); when `start_session` is refused, propose own_initiative cards; detach and file them when they check in.
(4) `start_session` after another session's message is refused: propose instead. (5) Archive calls may need a second
call with the `session` label the hook adds. (6) Re-run `tools/regress.py` (clean) yourself before approving any card
that touched the build. (7) Weekly usage readings jumped 85% -> 0-2% on the night of 18 Nov (early reset); record what
cards report. (8) Cards' "follow-ups" become ER rows in `docs/handoff.md` plus tracker tasks in the same closing commit.

## Hand-over log

- Tue 17 Nov 2026, 11:52 Eastern: drafted by the setup session. No dispatcher started yet.
- Tue 17 Nov 2026, 12:01 Eastern: **DISP-01** started (first titled "Project Ringsmith dispatcher #1", renamed 12:12), id `local_15d323e2-b845-4337-9f74-b894bdc51e69` (Opus, high).
- Wed 18 Nov 2026, 01:05 Eastern: first backup (DISP-01): `F:\Claude\Project Ringsmith\backups\repo.bundle`, all refs at 999d152, verified okay.
- Wed 18 Nov 2026, 15:47 Eastern: backup by ER-025's test run (`tools/backup.py`): `ringsmith_20261118_1547_main.zip` (169 KB, 77 files) + `repo.bundle` at fcd64c9, verified and test-restored.
- Wed 18 Nov 2026, 16:39 Eastern: backup (DISP-01, `tools/backup.py`): `ringsmith_20261118_1639_main.zip` (78 files, 203 KB) + `repo.bundle` at acf0284, verify okay.
- Thu 19 Nov 2026, 15:18 Eastern: **DISP-02** started (Opus high), id `local_3709ebbc-a98e-49e9-a962-e7b2299d302f`, confirmed reachable; DISP-01 hands over at 44% context (state section above). DISP-01 stays open, read-only, until LuciDin archives it.
- Thu 19 Nov 2026, 18:06 Eastern: backup (DISP-02, `tools/backup.py`): `ringsmith_20261119_1806_main.zip` (82 files, 236 KB) + `repo.bundle` at f6fe0e1, verify okay.
- Thu 19 Nov 2026, 18:41 Eastern: **first GitHub push** (DISP-02, LuciDin's yes): remote `github` = https://github.com/SadBoxMan/ringsmith.git, `main` at aaae66f; history checked for game data first.
- Thu 19 Nov 2026, 22:18 Eastern: plan from LuciDin: Fri 20 Nov = decomp day (ER-034 first, then ER-041, ER-042, ER-031 / ER-027 / ER-015; ER-023 beside); recomp pauses after ER-046 and resumes Sat 21 Nov (ER-030 with ER-047, then ER-036 ...). No 2 AM cutoff on the night of 19-20 Nov (LuciDin). Weekly reset Sat 09:00: fresh dispatcher (DISP-03) then.
