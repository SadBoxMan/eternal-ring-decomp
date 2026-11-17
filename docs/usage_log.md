# Project Ringsmith usage log

One row per session, kept by the dispatcher from completion messages. LuciDin uses this record to teach others about
the time and cost of running projects like this through Claude, so keep every field.

Plan: Max (check tier at each weekly reset). Weekly bars reset Saturday morning (Eastern; read the time from get_usage).

| Date (Eastern) | Code | Session | Model / effort | Ultracode | Weekly start -> end | Fable start -> end | 5-hour start -> end | Wall time | Work units | Notes |
|---|---|---|---|---|---|---|---|---|---|---|
| Tue 17 Nov 2026, 12:08-12:13 | ER-005 | Research survey (`local_817d8d00`) | Sonnet 5.5 / medium | off | 47% -> 47% | 4% -> 4% | 3% -> 4% | ~6 min | 4 web-fetch sub-agents, ~330k tokens | Ran beside ER-001; commit 556120b; end time approximate (session's own clock read in Bash) |
| Tue 17 Nov 2026, 12:05-12:21 | ER-001 | Disc intake (`local_35986319`) | Sonnet 5.5 / medium | off (assumed) | 47% -> 47% | 4% -> 4% | 3% -> 5% | ~16 min (dump ~10 min) | 1 session, no sub-agents | Commits 4a6130c, 4e76424; 0 read errors; redump comparison not done (site unreachable) -> ER-009. Times as the session reported; DISP-01's clock read 12:19 on receipt |
