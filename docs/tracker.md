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

## Who updates it

- **The dispatcher**, from start and completion messages: adds each new card to `tasks` when it is queued, sets
  statuses, and copies measured numbers (functions matched, totals from the census, test results) into the metrics.
- **Cards may update their own items**: their own task row, the milestones they deliver, and the metrics they measure
  (for example ER-003 sets the `.text` byte total, ER-004 the function totals). They name the change in their
  completion message.
- Keep it to facts from the repo. Anything that is a reading rather than a measurement is marked `Unconfirmed` in
  its notes.
- Edit with the tracker window or by hand (keep the format above). The file is committed with the work that changed
  it, by explicit path.
