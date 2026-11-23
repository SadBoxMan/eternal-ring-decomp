# Project Ringsmith dispatcher handoff

For every dispatcher session (since ER-110 this file is in `Project Ringsmith\Decomp\docs\`; see "Repository layout").
Read this, `docs/handoff.md`, `docs/roadmap.md`, `docs/usage_log.md` and the memory index
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
- **Pin.** Every new dispatcher pins itself in the sidebar at start, without asking (LuciDin, Mon 23 Nov 2026: "Always pin the DISP").
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
  **Drive E: from Sun 22 Nov 2026 (LuciDin; F: was the drive before).** Run from the repo root (ER-025):
  `python tools/backup.py` (default E:\Claude\Project Ringsmith\backups since ER-056, 3095f89):
  a zip of our sources plus `repo.bundle`; it prints one line: check it says "bundle verify okay". Never run by work
  sessions. Log each one below. Since ER-110 run it in `Decomp` (`git -C` does not apply: `cd Decomp` first, or
  `python Decomp\tools\backup.py`); it also writes a `_recomp` zip and `recomp.bundle` of the sibling Recomp
  repository on the same line (check "recomp.bundle verify okay" too).
- **Archive** work sessions once verified finished; never ones still working, waiting on LuciDin, or holding
  uncommitted work.
- **Windows paths in scripts.** Write them as Python raw strings (`r"C:\rs\..."`): plain strings turned `\r` / `\t`
  into control characters in `docs/handoff.md` twice on 17 Nov 2026. Check with `grep -P '[\x00-\x08\x0b-\x1f]'`.
- **PC port watch.** After every completed card, check the "Watch: when can the PC port be built?" section of
  `docs/handoff.md` and tracker milestone `ms10`; update its status line and tell LuciDin as soon as a first PC
  build becomes possible (LuciDin, 17 Nov 2026).
- **Usage records.** One row per session in `docs/usage_log.md` from each completion message; LuciDin uses the log
  to teach others about cost, so keep it complete.

## Repository layout (ER-110, Sun 22 Nov 2026)

| Folder | Repository | Remote `origin` | Holds |
|---|---|---|---|
| `C:\Users\lukea\Claude\Projects\Project Ringsmith\` | none (parent) | - | `CLAUDE.md` (not in git; imports `Decomp\CLAUDE.md` so sessions started here get the rules), the app's `.claude\` |
| `...\Project Ringsmith\Decomp` | decomp, full history | https://github.com/SadBoxMan/eternal-ring-decomp | `src/`, `include/`, `tools/`, yaml, Makefile, hooks, `.venv`, gitignored `disc/`, `asm/`, `build/`; **every project doc** (handoff, this file, tracker data, usage log, roadmap, `CLAUDE.md`) |
| `...\Project Ringsmith\Recomp` | port, GPL-3.0 | https://github.com/SadBoxMan/ringsmith | `tools/recomp/`, `docs/recomp_spike.md`, `docs/recomp_opcodes.md`, `patches/` (local PS2Recomp patches), `README.md`, `LICENSE`, a short `CLAUDE.md` |
| `C:\rs\recomp` | none (PS2Recomp clone + build tree) | - | MSVC build, `game_build`, generated C++, boot files (game data), working copies of the patches |
| `C:\rs\wt\dispverify` | worktree of Decomp | - | the dispatcher's verify worktree (junctions to `Decomp\disc`, `Decomp\.venv`) |

- **Cards start with `cwd` = `...\Decomp` (decomp, docs, tracker) or `...\Recomp` (port)**: app worktrees need a git
  folder, and the parent is not one. A port card that also updates the handoff note or tracker commits those in
  Decomp (two commits, one per repository). Port scripts read the decomp's outputs from the sibling `..\Decomp`
  (`Recomp\README.md`, "What it needs"), so a port card still needs Decomp's `build/SLUS_200.15.syms.elf`.
- The dispatcher itself stays in the parent folder and works with `git -C Decomp ...` / `git -C Recomp ...`.
- **Memory:** the Claude memory folder is keyed on the session's folder: the parent keeps
  `C--Users-lukea-Claude-Projects-Project-Ringsmith`; sessions started in Decomp or Recomp get their own (empty)
  folders. The dispatcher decides what to copy or point to there.
- Pushes: each repository's `main` to its own `origin`, each with LuciDin's yes in that session and the history check;
  never force. The `claude/*` branches stay local (they are in `repo.bundle`).

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
it, never push; builds -j4; launches on the secondary monitor, muted; port runs keep the exe's console window hidden (else on the secondary monitor; LuciDin, 22 Nov 2026); call cmd.exe, never bare cmd; below 95% ignore
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

## STATE FOR A FRESH DISPATCHER (DISP-03 -> DISP-04, Mon 23 Nov 2026, 13:40 Eastern)

**Where the project is.** Decomp: **1,135 / 3,609 functions matched (115,268 bytes)**, every one proven by `make regress`
(ruling 3.1). Rulings now 3.1-3.18: since DISP-02, 3.14 (address-taken local that only blocks a sibcall), 3.15 (aligned(16)
4-float vector type), 3.16 (`volatile` only on hardware-register reads / writes; first ruled no, then accepted narrowly),
3.17 (no `do { } while (0)` sibcall blocker), 3.18 (unused local whose stores the game makes). LuciDin: the goal stays a true
100% decomp as in Twinblade; every use of the narrow rulings (3.13-3.18) is listed in recipes 3 for a later review.
`tools/m2c_bulk.py` (ER-084, ER-114): tail-`j` slot moves, byte bases, own-extern handling, offset-order stores, 3.14 variant,
GOFAST names; its m2c context was silently broken 22 Nov 17:04 to 23 Nov 03:28 (ER-092's `G390` typedef), now fixed and warned.
Recipes section 2 runs 2.1-2.34 (renumber on merge). Port target **60 FPS** (roadmap).
**Repos (ER-110, 22 Nov):** `Project Ringsmith\Decomp` (eternal-ring-decomp; all docs) and `Project Ringsmith\Recomp` (ringsmith,
GPL-3.0, 13 local patches in `patches/`); the parent folder is not a git repo (its CLAUDE.md imports Decomp's). Pushed 23 Nov 12:17
(Decomp de596dc, Recomp 98b7f3c); since then **not pushed**: Decomp from de596dc, Recomp 3135af4 (ER-036). A push needs a new yes.
**Recomp / port:** logos, all movies (ER-030), title, NEWGAME cutscene, gameplay from a PCSX2 save (ER-099) with 3D models
(ER-102 / ER-103); opcode fixes (ER-036: attack animations now right); 12-15 FPS (ER-104: VU1 interpreter 33 ms + GS CPU
renderer 24 ms of 65 ms per frame); no sound. Not playable yet (`ms10`).

**Running.** ER-132 + ER-152 + ER-153 (`text_00150878` hard ones 2, Opus high, `local_e21489cd-0d73-4fc8-a571-e1a7acc99a92`,
worktree `C:\rs\wt\er132`). ER-106 (alias model, `local_aff7362b-afcb-4f46-9adc-fe31fce01644`) idle since 22 Nov 17:30 with 2
uncommitted changes in `C:\rs\wt\er106` (17 GB) and no report: ask LuciDin to look at it (probably a question waiting).

**Ready next (one writer per file):**
- Port (one recomp card at a time): **ER-037** opcode harness vs PCSX2 (accuracy; DISP-03's pick, LuciDin chose accuracy and
  stability over speed), ER-136 FMV freeze (needs LuciDin pressing Start), ER-135 shake, ER-105 sound, ER-127 / 128 / 129,
  ER-044 (regenerate with the generator built from game_build: `C:\rs\recomp\work_er036\build_recomp_er036.bat`), speed
  ER-139 (VU1 recompile: design at Opus xhigh, which a high dispatcher cannot start; LuciDin starts it himself or a brief is pasted),
  ER-140 to ER-146, ER-155 (after speed), ER-108 / 109 memory card, ER-126 capture tool (after playable). Not coded: a PCSX2
  reference recorder (PINE: per-frame input, RAM regions, frame captures; DISP-03 suggested it in a Query answer, no decision).
- Decomp: `text_00150878` after ER-132: ER-133, ER-134; `text_001000C0`: ER-131 (Opus xhigh: func_00120EF0, func_00124BE8 and
  five parked), ER-093 (func_00119FE0, own session); ER-154's nine files: ER-158 (high), ER-159 (medium), ER-107 (waits on ER-106);
  five small files: only ER-118 (waits on ER-106) and ER-122 left; tools (one `m2c_bulk.py` writer): ER-138 + ER-147 + ER-151 +
  ER-160, ER-117 (search tools); small: ER-156, ER-157 (string scan); ER-032, ER-052, ER-031 wait.
- **Thursday / Friday (Fable / Ultracode):** ER-122 batch (func_00147F78, func_00145FE8, func_0014D3F8, func_00146598,
  func_00145D08 (a static helper would need an invented name: ask LuciDin), func_0014E450), then ER-131 / ER-133 if Opus resists.

**Open for LuciDin:** GPL-3.0-only or -or-later (Recomp); a licence for the decomp repo; ER-058's patch published (ER-110's
judgement); Ringsmith's weekly share; the PCSX2 recorder card; pinning the dispatcher (DISP-03 asked, no answer).

**Quirks learned by DISP-03.** (16) Since ER-110 the app sees the parent folder as a non-git project: `start_session` makes no
worktree; every decomp brief says WORKTREE FIRST: `git -C ...\Decomp worktree add -b <branch> C:\rs\wt\<code> main` (EnterWorktree is
refused), junctions, regress there; run folders under `C:\rs\wt\<code>_runs` (they get large: 1-10 GB each; delete after merging, on
LuciDin's yes). (17) Start sessions with `initiation` "user_asked" and `cwd` set to `...\Decomp` or `...\Recomp` only on LuciDin's own
words in that turn; after a session message the start is refused: propose an own_initiative card (no `cwd`; the brief names the
folder to work in). The app caps 5 working at once. (18) The dispatcher's own folder flips between the parent and `Decomp`: use
absolute paths in every command. (19) Control characters: Bash heredocs turned `C:\rs` into CR + `s` three more times (also in a
card's docs commit, ER-123); write fix scripts with the Write tool, build backslashes with `chr(92)`, and scan
`grep -caP '[\x00-\x08\x0b-\x1f]' docs/*.md docs/*.json` before every docs commit. (20) `archive_session` needs a second call with
the hook's `session` label; LuciDin often archives from the sidebar first. (21) A card with managerial rights may commit without
asking first (ER-036): check the commit after. (22) Most card branches fast-forward; otherwise `git merge --no-ff` in the main
checkout (the hook runs regress). (23) A card's numbers in its completion message are deltas on main at its last merge; check
`m-dec-funcs` / `m-dec-bytes` in `docs/progress_data.json` after each merge. Next free code: **ER-161**.
DISP-01's and DISP-02's quirks 1-15 still hold.

## STATE FOR A FRESH DISPATCHER (DISP-02 -> DISP-03, Sun 22 Nov 2026, 15:10 Eastern)

**Where the project is.** Decomp: **787 / 3,609 functions matched (57,200 bytes)**, every one proven by `make regress`
(whole-ELF SHA-1 + all 3,606 functions; ruling 3.1). Toolchain now: ee-gcc `-r1` (fdiv nops, 3.7) with CFLAGS
`-O2 -G8 -fno-strict-aliasing -fno-edge-lcm` (3.6, 3.11), ee-binutils `-r4` (3.4 c.cond/bc1, 3.5 mtc1, 3.8 mfc1, 3.12
delay-slot swap). Code-shape rulings: 3.4 (no-return, also func_001776D8), 3.9 (+ extension: explicit / empty case labels),
3.10 (double literal replaces a standalone .rodata constant), 3.13 (an unused local that only sizes the frame, narrowly).
Split: ER-031 cut five files out of `text_001343E8` plus S22 / 0x001CB528; `tools/split.py` aligns 8- and 16-byte rodata
and accepts anonymous-constant takeovers. Tools: `m2c_bulk.py` (`--plain`, job-object gcc timeout, pass-through retry,
`--rerun-diffs`, `--tag`), `place.py`, `cmpfunc.py`, `clones.py`, `census.py`, `seams.py` (`docs/matching_tools.md`).
Recipes section 2 runs 2.1-2.22 (numbers collide when parallel cards merge: renumber on merge, keep both).
**Recomp / port:** PS2Recomp + local patches (`C:\rs\recomp\patches\`: er018, er029, er043, er046, er058; ER-074 changed only
our scripts) boots both movies, the FromSoftware logo and title screen (as PCSX2), the attract loop, and runs past NEWGAME
into the opening cutscene (15 min, no fault). Movie pictures invisible (ER-030), 3D models missing (ER-102), single-digit FPS
(ER-104), no sound (ER-105). Tracker `ms10` notes current.

**Running.** ER-099 (PCSX2 memory card import, Opus high, `local_739cb03e-323f-42ee-8472-6ea9a3fff63d`, started 14:59 in the
main checkout + `C:\rs\recomp`): it reports to the dispatcher id in `docs/handoff.md` (DISP-02 tells it the new id).

**How waves run (LuciDin, 21-22 Nov).** Decomp in waves: each `src/` writer owns whole files and works in its own worktree
(`docs/build.md` 6: junctions to the main checkout's `disc/` and `.venv`, read-only); **one port card in every wave**
(recomp cards one at a time: shared build tree). Merge each branch yourself: fast-forward when it already has main, else
`git merge --no-ff`; resolve docs (keep both sides; metrics = main's numbers + the card's delta, never the card's total);
then a clean `python tools/regress.py`. If a card is building in the main checkout, merge with `RINGSMITH_SKIP_REGRESS=1`
and prove main in `C:\rs\wt\dispverify` (quirk 9). After a toolchain / CFLAGS / split.py change lands, tell every running
card to merge main. Never remove a worktree's junctions while its session is running (DISP-02 did once, ER-069).
Managerial rights: each card asks LuciDin at STEP 0; toolchain changes and new matching shapes always go to him.

**Ready next (file ownership in brackets; one writer per file):**
- `text_001000C0.c`: ER-089 (frame-sizing locals, 3.13), ER-091, ER-092, ER-093 (large), ER-094 (func_00126F00). Opus high.
- `text_00150878.c`: ER-095 (3.4 applied), ER-096 (hard ones), ER-098 (187 DIFFs rerun). Also ER-051's S21 / 0x001C4F68 cuts
  (yaml + this file: not beside the others). Opus medium / high.
- Five small files (`text_00145120`, `0014A968`, `0014C840`, `0014D930`, `0014E2E8`): ER-100. Opus high / medium.
- ER-031's five files: ER-107 after ER-106. Toolchain research (no `src/`): ER-090 (loop register promotion), ER-106
  (alias model). Tools: ER-084, ER-097 (m2c_bulk), ER-101 (const floats census). ER-088 folds into bulk cards.
- Port, one at a time, after ER-099: ER-102 (3D models), ER-103 (libvu0 HLE audit; reading can run beside), ER-030 (movie
  picture), ER-036 (opcode bugs), ER-033 (FFmpeg from source), ER-104, ER-105, ER-059, ER-075, ER-048, ER-044, ER-045.
- Thursday / Friday only (LuciDin): Fable and Ultracode. Candidates: ER-016 pilot fan-out, ER-093 if it resists Opus.
- Assets (after the recomp is complete): ER-057, music first (LuciDin).

**Open decisions for LuciDin:** GPL-3.0 for the port (ER-019); saved redump page (ER-009); Ringsmith's weekly share;
optional: pull the public-source SDK parts (newlib / libgcc) ahead of ruling 3.3's "SDK after game" order (DISP-02 suggested,
not decided). **GitHub:** first push Thu 19 Nov 18:41 (main at aaae66f); 169 commits on main since, not pushed: every push
needs his yes in that session plus the history check.

**Quirks learned by DISP-02.** (10) Never write Windows paths through `printf` or plain Python strings: `\r`, `\b` became
control characters three more times; use raw strings in a scratch .py file, then `grep -caP '[\x00-\x08\x0b-\x1f]'`.
(11) `grep -c` returns exit 1 on zero matches: do not chain a commit after it with `&&`. (12) Archive calls need a second call
with the hook's `session` label; several sessions may already be archived by LuciDin from the sidebar. (13) Cards often report
"code from the dispatcher" rows: assign the next free ER code on merge (next free: ER-108). (14) `tools/backup.py` defaults to
E: (ER-056). (15) PCSX2 is at `C:\Users\lukea\Games\Emulated\Playstation 2`; port runs keep the console hidden (handoff item 7).
DISP-01's quirks 1-9 above still hold. (16, DISP-03, 22 Nov) Since ER-110 the app still treats the parent `Project Ringsmith` as a non-git project, so `start_session` with `use_worktree: true` gives **no worktree**: the card shares the `Decomp` main checkout. Put in every decomp brief: right after STEP 0, `EnterWorktree`, else `git -C ...\Decomp worktree add -b <branch> C:\rs\wt\<code> main` and work only there (junctions per `docs/build.md` 6). The app also caps starts at 5 working at once per user message: the rest go up as cards.

## OLDER STATE (DISP-01 -> DISP-02, Thu 19 Nov 2026, 15:15 Eastern)

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
cards report. (8) Cards' "follow-ups" become ER rows in `docs/handoff.md` plus tracker tasks in the same closing commit. (9) Merging a card's branch while another card builds in the main checkout: a cherry-pick or merge that touches `src/` runs the pre-commit hook, which wipes and rebuilds the shared `build/` with that card's uncommitted changes (happened 22 Nov 02:21 with ER-034). Merge such branches with `RINGSMITH_SKIP_REGRESS=1` (say so in the message) and prove `main` in the dispatcher's own worktree `C:\rs\wt\dispverify` (detached; junctions to `disc` and `.venv`; `git -C C:\rs\wt\dispverify checkout --detach main`, then `.venv\Scripts\python.exe -I tools/regress.py` there; since ER-110 its junctions point at `Decomp\disc` and `Decomp\.venv`).

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
- Sat 21 Nov 2026, 20:47 Eastern: LuciDin gave the dispatcher managerial rights for all cards today (21 Nov). Wave 1 started: ER-049, ER-050 (worktree), ER-023; ER-034 after ER-049. Weekly bar reset (7%).
- Sun 22 Nov 2026, 01:36 Eastern: backup (DISP-02) to **E:** on LuciDin's word (F: not connected): `python tools/backup.py "E:\Claude\Project Ringsmith\backups"` -> `ringsmith_20261122_0136_main.zip` (90 files, 308 KB) + `repo.bundle` at 9a135fb, verify okay (folder created for it). ER-052 ruled: wait for matching. Wave 2 started 01:37: ER-034 (main checkout), ER-054 (worktree; owns text_00145120 / 0014A968 / 0014C840 / 0014D930 / 0014E2E8), ER-053 (tools/seams.py only).
- Sun 22 Nov 2026, 01:53 Eastern: backup by ER-056's test run (plain `python tools/backup.py`, now defaulting to E:): `ringsmith_20261122_0153_main.zip` (90 files, 309 KB) + `repo.bundle` at d95736a, verify okay.
- Sun 22 Nov 2026, 03:09 Eastern: wave 3 proposed, file ownership: ER-060 part 1 = src/text_00150878.c; ER-063 part 1 = src/text_001000C0.c; ER-031 + ER-051 = yaml, src/text_001343E8.c, src/text_001CA0F0.c (S21 / 0x001C4F68 wait for ER-060); ER-041 = symbol_addrs.txt, src/sdk/libgcc.c (+ newlib file of func_001E42E0); ER-062 keeps the five ER-054 files; ER-058 (recomp) in C:\rs\recomp. Every writer in its own worktree; merges verified in C:\rs\wt\dispverify.
- Sun 22 Nov 2026, 03:40 Eastern: LuciDin: **Fable and Ultracode are available on Thursdays and Fridays only** (Eastern day). Not needed so far; candidates if they become needed: Ultracode for ER-016 (pilot fan-out) or a many-file bulk pass, Fable for functions still parked after an Opus high / xhigh round (e.g. func_00142788). Queue such cards for the next Thursday / Friday.
- Sun 22 Nov 2026, 04:55 Eastern: PCSX2 location from LuciDin (via ER-058): `C:\Users\lukea\Games\Emulated\Playstation 2`; recorded in `docs/handoff.md` item 7. Cards still ask him before opening it.
- Sun 22 Nov 2026, 05:26 Eastern: backup (DISP-02, plain `python tools/backup.py` to E:): `ringsmith_20261122_0526_main.zip` (101 files, 383 KB) + `repo.bundle` at 06c9445, verify okay (after ER-053, ER-054, ER-034, ER-062, ER-046, ER-041, ER-063, ER-031 + ER-051, ER-058, ER-072).
- Sun 22 Nov 2026, 13:20 Eastern: wave 4 started (LuciDin: 'continue some more decomp waves'), file ownership: ER-060 part 1 = src/text_00150878.c (running since 03:15, stalled 05:34-13:14 on a looping cc1, recovering); ER-060 part 2 = src/text_001000C0.c; ER-069 = build_ee_gcc.sh, ee-gcc patches, Makefile GCC_DIR, toolchain.md; ER-064 + ER-066 = tools/m2c_bulk.py, src/text_00145120.c, text_0014A968.c, text_0014C840.c; ER-077 + ER-068 = tools/split.py, tools/census.py, src/text_001343E8.c, text_0013A410.c, text_0013B748.c, text_0013DBC0.c, text_0014D930.c, text_0014E2E8.c. Proposed: ER-070 (build_ee_binutils.sh, ee-as patches, Makefile BIN_DIR), ER-065 (place.py, cmpfunc.py, eematch.py, recipes numbering). DISP-02 at 49% context: hand over to DISP-03 once this wave lands.
- Sun 22 Nov 2026, 14:10 Eastern: LuciDin: **every wave includes one port card** (one recomp card at a time, as before). Wave 5 started: ER-074 (port, NEWGAME, C:\rs\recomp); ER-086 + ER-081 + ER-073 = src/text_001000C0.c; ER-080 = src/text_00150878.c; ER-083 + ER-085 (+ ER-087's part) = src/text_00145120.c, text_0014A968.c, text_0014C840.c, text_0014D930.c, text_0014E2E8.c. Still running from wave 4: ER-069 (second turn, may place in text_0013A410 / 0013B748 / 0013DBC0; I removed its junctions by mistake at ~14:00 and told it), ER-070 (assembler r4; Makefile BIN_DIR). Held for after ER-069: ER-078 and ER-087's text_0013DBC0 part.
- Sun 22 Nov 2026, 15:03 Eastern: backup (DISP-02, plain `python tools/backup.py` to E:): `ringsmith_20261122_1503_main.zip` (103 files, 464 KB) + `repo.bundle` at f00bdc1, verify okay (after waves 4 and 5: ER-060 x2, ER-064, ER-065, ER-069, ER-070, ER-077, ER-086 round, ER-080, ER-083 round, ER-074, ER-078 round). Matched 787 / 3,609.
- Sun 22 Nov 2026, 15:13 Eastern: **DISP-03** started (Opus high, 15:11), id `local_9d112acd-e664-4a1c-9a42-3bfb18b6adaa`, confirmed reachable; DISP-02 hands over past 50% context (state section above). DISP-02 takes no new work and can be archived once DISP-03 has checked the ids.
- Sun 22 Nov 2026, 15:20 Eastern: wave 6 proposed by DISP-03 (cards; LuciDin starts them): ER-089 + ER-094 + ER-091 + ER-092 = src/text_001000C0.c; ER-095 + ER-096 + ER-098 = src/text_00150878.c; ER-100 = text_00145120 / 0014A968 / 0014C840 / 0014D930 / 0014E2E8; next: ER-106 (no src/), m2c_bulk tools card (ER-084 + ER-097 + ER-101 + ER-055), port ER-102 + ER-103 after ER-099.
- Sun 22 Nov 2026, 15:55 Eastern: ER-110 done: repo split into `Decomp` (eternal-ring-decomp) and `Recomp` (ringsmith, GPL-3.0), both pushed 15:49 on LuciDin's yes. DISP-03's folder is now `Decomp`. Memory: the Claude memory folder is keyed on each session's folder; cards rely on the repo docs, the project memory stays with the dispatcher (no copies, to avoid drift). Wave 6 cards withdrawn before the split; re-proposed with the new paths.
- Sun 22 Nov 2026, 16:00 Eastern: wave 6 started on LuciDin's 'Start the wave': ER-089 + ER-094 + ER-091 + ER-092 = src/text_001000C0.c; ER-095 + ER-096 + ER-098 = src/text_00150878.c; ER-100 = the five ER-054 files; ER-106 (no src/); ER-084 + ER-097 + ER-101 + ER-055 = tools/m2c_bulk.py. The app made no worktrees (quirk 16): DISP-03 stopped the five at STEP 0 and sent each the move; worktrees under C:\rs\wt\er0NN / er1NN. Port card ER-102 + ER-103 proposed as a card (start cap), working in Recomp + C:\rs\recomp.
- Sun 22 Nov 2026, 17:14 Eastern: backup (DISP-03, plain `python tools/backup.py` to E:, first run with the Recomp bundle): `ringsmith_20261122_1714_main.zip` (91 files, 427 KB) + `repo.bundle` at bcdf013, verify okay; `ringsmith_20261122_1714_recomp.zip` (30 files, 114 KB) + `recomp.bundle` at cffeaf4, verify okay (after ER-099, ER-110, ER-084 round, ER-089 round). Matched 790 / 3,609.
- Sun 22 Nov 2026, 18:20 Eastern: backup (DISP-03, plain `python tools/backup.py` to E:): `ringsmith_20261122_1820_main.zip` (91 files, 455 KB) + `repo.bundle` at 87ce830, verify okay; `ringsmith_20261122_1820_recomp.zip` (32 files, 123 KB) + `recomp.bundle` at c7a9b7d, verify okay (after ER-095 round, ER-100, ER-102 + ER-103, ER-112 + ER-119, ER-115 round). Matched 916 / 3,609. Also: eternal-ring-decomp pushed 17:50 at a375f83 (record 937cbc0) on LuciDin's yes; ringsmith c7a9b7d not pushed.
- Mon 23 Nov 2026, 03:35 Eastern: LuciDin: the next decomp cards wait till morning. Ready then (one card per file group, each starting with its ER-137 share): ER-113 (`text_00150878` rerun with the fixed m2c_bulk), ER-125 (`text_001000C0` rerun), ER-120 (five ER-054 files rerun). Running overnight: ER-104 (recomp speed; needs LuciDin for runs). ER-106 idle since 22 Nov 17:30 with uncommitted work, not reported. ringsmith has 3 unpushed commits (c7a9b7d, 3a32cd4, 4507eed); eternal-ring-decomp has docs commits since 937cbc0. Port target 60 FPS (roadmap).
- Mon 23 Nov 2026, 04:04 Eastern: ER-104 paused for the night (LuciDin), nothing committed yet: gameplay 83 ms per frame as shipped, 65 ms with three behaviour-identical speed-ups and /Ob2 (default on, LuciDin's yes); of the 65 ms VU1 interpreter 33 ms, GS CPU backend ~24 ms, EE + scheduler + IOP ~3 ms (target 16.7 ms). Resumes in its own session; notes ER-104_notes.md in the side-session notes folder.
- Mon 23 Nov 2026, ~11:20 Eastern: LuciDin's yes (DISP-03 session): push both repos to GitHub after the current cards complete (ER-104, ER-113 + ER-137, ER-125 + ER-137, ER-120), each after the game-data history check; main only, never force.
- Mon 23 Nov 2026, 13:26 Eastern: backup (DISP-03, plain `python tools/backup.py` to E:): `ringsmith_20261123_1326_main.zip` (91 files, 510 KB) + `repo.bundle` at 06fa493, verify okay; `ringsmith_20261123_1326_recomp.zip` (41 files, 156 KB) + `recomp.bundle` at 3135af4, verify okay (after ER-030, ER-114 round 2, ER-104, ER-113, ER-125, ER-120, ER-036, ER-148, ER-149 + ER-071). Matched 1,059 / 3,609. Rulings since the last backup: 3.16 (volatile, hardware registers only), 3.17 (no do-while(0)), 3.18 (unused local whose stores the game makes).
- Mon 23 Nov 2026, 13:42 Eastern: **DISP-04** started (Opus high, 13:38, in `Decomp`), id `local_14744be3-5a0c-4c46-a20f-0595608745e8`, confirmed reachable; DISP-03 hands over at 55% context (state section above). DISP-03 takes no new work and can be archived once LuciDin likes.
- Mon 23 Nov 2026, 13:55 Eastern: DISP-04 checked the hand-over (id in the three places) and pinned itself (LuciDin's standing rule). ER-132 + ER-152 + ER-153 merged (fast-forward to 10f84ec, clean regress PASS): 1,140 / 3,609 (116,096 bytes); follow-ups ER-161 to ER-163, ER-117 widened. First combined use of rulings 3.13 + 3.14 (func_001636A0, `s32 buf[4]; (void)&buf;`) flagged to LuciDin.
- Mon 23 Nov 2026, 14:10 Eastern: wave 7 started on LuciDin's 'Start all 4' (DISP-04), file ownership: ER-093 + ER-163 = src/text_001000C0.c (`local_a4f46d82`, C:\rs\wt\er093); ER-158 + ER-159 = ER-154's nine files (matching_tools 6.16) (`local_2b800f83`, C:\rs\wt\er158); ER-138 + ER-147 + ER-151 + ER-160 = tools/m2c_bulk.py, docs/matching_tools.md 6 (`local_bee767d9`, C:\rs\wt\er138; Opus medium); port ER-037 = Recomp tools/recomp/, docs/recomp_opcodes.md + C:\rs\recomp\work_er037 (`local_bc0d3697`). Cleanup on LuciDin's word: ER-132, ER-148, ER-149 + ER-071, ER-154 archived (DISP-03 already archived); worktrees er132 / er148 / er149 / er154 / .claude\worktrees\er089 and the run / scratch folders er095 / er120 / er125 / er132 / er148 / er149 / er154 (about 3 GB) removed; branches kept. ER-106 (C:\rs\wt\er106, 17 GB, 2 uncommitted doc edits) left alone.
