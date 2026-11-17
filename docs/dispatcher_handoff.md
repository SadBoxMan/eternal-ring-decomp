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
log, the diff, the tracker) before telling LuciDin it is done; then update the tracker, the usage row and the handoff
note in one small commit, and offer the next card. Keep turns short.

## Rules and quirks (learned on Twinblade)

- **Proposals vs starting.** After another session's message or an automated event, `start_session` is refused;
  propose with initiation "own_initiative" and LuciDin clicks the card. A proposed card cannot be edited: dismiss it
  and propose again. Cards he starts come back as "The user started your proposal" reminders: detach and file them.
- **Model check.** A session cannot start above the dispatcher's own model and effort; run the dispatcher at the
  highest setting its cards need (Twinblade and Cordona ran theirs on Opus high for this reason). Each card stops at
  STEP 0 until the picker matches; tell LuciDin which setting to pick.
- **Numbering.** Dispatchers are "#N" in their titles. Hand over at about 50% context: the outgoing one writes a
  "STATE FOR A FRESH DISPATCHER" section here, starts "#N+1", confirms it is reachable by message, then updates the
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
- **Archive** work sessions once verified finished; never ones still working, waiting on LuciDin, or holding
  uncommitted work.
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
Finish: docs and handoff note updated, usage table, completion message to the dispatcher, tell LuciDin to return to
it."

## Sidebar sections (LuciDin, 17 Nov 2026)

The dispatcher is pinned. After start_session and detach_session, file each card with move_sessions (section_id):
- "Ringsmith Decomp" `cg-c48eb8a6-ce65-440a-a3c5-ca5425ba91d4`: matching, census, toolchain, splits.
- "Ringsmith Recomp / PC Port" `cg-013fceec-fa15-4551-84be-da231715c2c6`: recompiler, HLE libraries, renderer, port.
- "Ringsmith Misc" `cg-2635b73d-871c-45d1-b4e6-9d5799f1a44d`: disc intake, research, docs, tracker.

## Hand-over log

- Tue 17 Nov 2026, 11:52 Eastern: drafted by the setup session. No dispatcher started yet.
- Tue 17 Nov 2026, 12:01 Eastern: **dispatcher #1** started, id `local_15d323e2-b845-4337-9f74-b894bdc51e69` (Opus, high).
