# Project Ringsmith

Decompilation, recompilation and eventual PC port of **Eternal Ring** (FromSoftware, PlayStation 2, 2000).
Owner: LuciDin. Started Tue 17 Nov 2026. DRAFT: items marked **(open)** wait on LuciDin's answer.

The global rules in `~/.claude/CLAUDE.md` apply in full (cards, archive question, shift gears, "Query:", ask about
expected behaviour first, Eastern times, the 5-hour bar). This file adds the project's own rules.

**Two repositories since ER-110 (Sun 22 Nov 2026):** this one, `Project Ringsmith\Decomp` (decomp and every project
doc; GitHub eternal-ring-decomp), and `Project Ringsmith\Recomp` (the port, GPL-3.0; GitHub ringsmith). Paths in
these rules are relative to `Decomp`. Layout: `docs/dispatcher_handoff.md` "Repository layout".

## Read at session start

1. `docs/handoff.md`: the top paragraph (dispatcher id, start and completion messages), open threads, the queue.
2. `docs/roadmap.md`: phases and tracks.
3. If you are the dispatcher: `docs/dispatcher_handoff.md`.
4. For matching work: `docs/decomp_recipes.md` (created with the first match; add each compiler lesson the moment it
   is confirmed). The `ps1-decomp-playbook` skill holds the method (census, clone, bulk, fan-out, escalate); its
   compiler levers are PS1 / GCC 2.7.2 and must be re-confirmed on this game's toolchain before they are trusted.

## How work flows (dispatcher project, global rule 5)

- Task cards come from the pinned session **"Ringsmith DISP-##"** (DISP-01, DISP-02, ...) (id in `docs/handoff.md`). It only
  hands out work and keeps the tracker, usage log and handoff notes; it does no project work.
- Every card has a task code **ER-NNN** at the start of its session title, brief and commits.
- Work sessions never propose New-session cards. Follow-ups (task, files, done-when, model / effort / Ultracode) go
  in the completion message to the dispatcher and in the handoff queue; then tell LuciDin to return to the dispatcher.
- Start message and completion message to the dispatcher, as described at the top of `docs/handoff.md`.
- **Tracker after every card.** The tracker (`docs/progress_data.json`, shown by the Ringsmith Progress Tracker app) is
  updated after each session or card completes: the card updates its own item and any numbers it changed, and the
  dispatcher checks it in its closing commit (LuciDin, 17 Nov 2026).

## Hard rules

- **Accuracy first, no corners cut** unless absolutely necessary, and then ask. Reports say what was not verified.
- **Nothing made up.** Names, values, timings and asset data come from the game (code, data, text) first, the
  official manual second, guides third. Anything guessed is marked `Unconfirmed` (a reading) or `Unofficial` (an
  invented label) and listed to LuciDin every time. Address names (`func_00123456`, `D_00456789`) are safe defaults.
- **No game data in git or in anything published**: no disc images, ELFs, IRX modules, extracted assets, captures,
  audio. The repo holds our code, configs, tools and docs only. Pushing anything anywhere needs LuciDin's yes, given
  in that session.
- **Matching tricks need a ruling.** No `asm()`, `volatile` keep-alives or dead code that only steers the compiler
  without LuciDin's ruling; park the function with a one-line cause instead. Rulings are recorded in
  `docs/decomp_recipes.md`.
- **Port: as shipped first.** Every fix or enhancement is optional (AS = As-Shipped, EX = Enhanced); the original
  code path is always kept.
- **Commits:** one shared git index if sessions share this checkout. Commit only your own files (temporary index or
  explicit paths); check `git status` and `git diff --cached --stat` first; LF line endings for C, headers, yaml,
  json. Ask before committing unless the card's managerial rights cover it.
- **Websites:** check each site for anti-AI measures (robots.txt, terms) before fetching; never fetch tcrf.net; ask
  LuciDin for saved HTML instead.

## Machine and Windows quirks (carried from Twinblade)

- No hypervisor (switched off after a BSOD): native Windows and MSYS2 only, never WSL2 or Docker.
- Always call `cmd.exe`, never bare `cmd` (it pops a Windows "open with" dialog). Kill processes by PID.
- i7-14700K, 28 threads, 64 GB: builds use `-j4` and fan-outs `--par 4`, so parallel streams stay near 28 threads.
- Worktrees at short paths without spaces (this folder has a space; toolchains often break on it).
- Game and emulator launches run on the **secondary monitor** (144 Hz) and **muted**; ask LuciDin first if a run
  needs sound. Tell him before anything that pops a window over his work.
