# Progress tracker

A small window showing where Project Ringsmith stands on each track: Decomp, Recomp, PC Port and Assets, plus the
ER-NNN task list and the Phase 0 milestones. It is laid out after Project Twinblade's tracker and uses the same
technology: Python with tkinter, no extra packages (ER-006, 17 Nov 2026).

## Opening it

- Desktop: `Desktop\Tools\Ringsmith\Ringsmith Progress Tracker.lnk`.
- The shortcut runs `Programs\Progress Tracker\Open Progress Tracker.bat` (minimised, so nothing flashes). That
  batch file starts `tools\progress_tracker.py` with `pythonw`, so no console window opens. It uses
  `.venv\Scripts\pythonw.exe` if the project ever gets a venv, otherwise the Python launcher (`pyw -3`, which finds
  the installed Python 3.12).
- From a shell: `pyw -3 tools\progress_tracker.py` (or `python tools\progress_tracker.py` to see errors).

## What it shows

| Tab | Content |
|---|---|
| Overview | One bar per metric, grouped by track; the Phase 0 milestones bar; the ER task count |
| Decomp / Recomp / PC Port / Assets | The track's summary, its metrics by phase, its phase items, its ER tasks and its Phase 0 milestones |
| Tasks (ER-NNN) | Every card: code, title, status, track, model / effort, what it waits on, notes |
| Phase 0 milestones | The Phase 0 list from `docs/roadmap.md`, with the cards that deliver each one |

A metric shows a percent only once its total is measured. Until then it reads "N units, total not measured yet"
with an empty bar and `--`. The totals come from the split (ER-003) and the census (ER-004). **Never enter an
estimate as a total.**

Editing: click a row and use the form below the list (status and notes; done and total for metrics), then Save.
Double-clicking a task, milestone or phase item steps its status to the next value. The window re-reads the file
every 2 seconds, so edits made by sessions show up without a restart. A bad status or track shows as `[?]` in red
and never crashes the window.

## Data: `docs/progress_data.json`

UTF-8, LF, 2-space indent (the program writes it this way). Top level:

| Key | Content |
|---|---|
| `updated` | ISO date of the last save (the program sets it) |
| `note` | Free text for whoever edits the file |
| `tracks` | List of tracks: `id` (`decomp` / `recomp` / `port` / `assets`), `name`, `summary`, `metrics`, `items` |
| `milestones` | Phase 0 milestones |
| `tasks` | ER-NNN cards |

Every metric, milestone and task belongs to a track: `decomp`, `recomp`, `port`, `assets` or `other` (on metrics and
phase items the track is the one they sit under; milestones and tasks carry a `track` field).

- **Metric** (`tracks[].metrics[]`): `id`, `phase`, `name`, `done` (whole number), `total` (whole number, or `null`
  for not measured yet), `unit`, `source` (where the numbers come from), `notes`.
- **Phase item** (`tracks[].items[]`): `id`, `phase`, `name`, `status` (`Not started` / `In progress` / `Complete`),
  `percent` (0 or 100 follow the status; only set something in between from a measured number), `notes`.
- **Milestone** (`milestones[]`): `id`, `phase`, `text`, `track`, `status` (as phase items), `by` (ER codes),
  `notes`.
- **Task** (`tasks[]`): `code` (`ER-NNN`), `title`, `status` (`Queued` / `In progress` / `Waiting` / `Done`),
  `track`, `model` (for example "Opus, high"), `waits_on`, `notes`.

## Who updates it (project rule, LuciDin, 17 Nov 2026; see `CLAUDE.md`)

**The tracker is updated after every session or card completes, without exception.**

- **The card updates its own entries before it finishes**: its task row (status `Done`, or `Waiting` with what it
  waits on, plus a short note with its commits), the milestones it delivers, the phase items it moves, and every
  number it changed or measured (for example ER-003 sets the `.text` byte total, ER-004 the function totals, a
  matching batch the functions-matched count). It commits `docs/progress_data.json` with its own work, by explicit
  path, and names the change in its completion message.
- **The dispatcher checks it in its closing commit**: the card's entries match the completion message; it adds newly
  queued cards to `tasks` and fixes anything the card missed.
- Keep it to facts from the repo. Anything that is a reading rather than a measurement is marked `Unconfirmed` in
  its notes. Never write an estimate as a total.

## Updating it from a session (no window)

The same program updates the file from the command line, so a session never has to hand-edit JSON (hand edits are
fine too; keep the format above). Run from the project folder; each call prints the saved entry. A bad id, status,
track or number is refused with a message and nothing is written.

```
python tools/progress_tracker.py show
python tools/progress_tracker.py task ER-003 --status Done --notes "Done 18 Nov 2026 (commits abc1234)"
python tools/progress_tracker.py task ER-011 --title "Short title" --track decomp --model "Opus, medium" --waits-on ER-004
python tools/progress_tracker.py metric m-dec-funcs --done 12 --total 3400
python tools/progress_tracker.py metric m-dec-bytes --total none
python tools/progress_tracker.py milestone ms5 --status Complete
python tools/progress_tracker.py item p1 --status "In progress"
```

- `show` lists every task, milestone, metric and phase item with its id.
- `task CODE` changes `--status` (Queued / In progress / Waiting / Done), `--title`, `--track`, `--model`,
  `--waits-on`, `--notes`; a code not yet in the file is added (needs `--title` and `--track`).
- `metric ID` sets `--done` and `--total` (`none` = not measured yet) and `--notes`.
- `milestone ID` and `item ID` set `--status` (Not started / In progress / Complete) and `--notes`.
- An open tracker window picks the change up within 2 seconds.
