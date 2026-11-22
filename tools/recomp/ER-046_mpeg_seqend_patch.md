# ER-046 local change to PS2Recomp: the end of the opening FMV (description only)

This file describes, in our own words, the fourth local change to PS2Recomp (ran-j, GPL-3.0, commit `e5636981`),
made by ER-046 on Sun 22 Nov 2026. It is one HLE change in `Stubs/MPEG.cpp`, kept as a diff of GPL code outside this
repository in `C:\rs\recomp\patches\er046_mpeg_seqend.patch` until LuciDin decides on the GPL-3.0 question and the
fork (ER-019). No config or generated file changes.

Findings and evidence: `docs/recomp_spike.md` section 11.

## To reproduce on a fresh clone of e5636981

1. `git apply` `er018_movie_boot.patch`, `er029_vblank.patch`, `er043_mpeg_isend.patch`, then
   `er046_mpeg_seqend.patch` (a diff against the ER-043 state of `MPEG.cpp`, CRLF line endings kept; checked: it
   applies there to a file identical to `game_build`'s and reverse-applies on `game_build`).
2. Configs and generated code as for ER-043 (`tools/recomp/make_configs.py`, spike doc section 6).
3. Build with `tools/recomp/build_game.bat`.

## Why

The game's movie player (`func_001C5188`) counts the bytes it still has to demux, starting from the file size
(`sceLseek`), and leaves its read / demux loop as soon as **fewer than 5** remain. On the PS2 the end of input then
comes from the game itself: it writes a sequence end code `00 00 01 B7` into its own video bitstream buffer
(`func_001C8A30`), which the hardware decoder reads. The HLE decodes from the demuxed PSS and never reads that buffer;
since ER-043 it took the PSS program end code `00 00 01 B9` (the last 4 bytes of each file) as the end of input.

`EROPEN.PSS` is 60,620,804 bytes = 925 x 64 KiB + **4**. The player reads 64 KiB at a time, so the last full read
leaves exactly the 4-byte program end code unread, the remaining count drops to 4 and the loop ends: the HLE never
sees the end, the decoder thread waits in the HLE `sceMpegGetPicture` for ever, and the boot never leaves the movie.
`AGETEC.PSS` ends 16,388 bytes past a 64 KiB boundary, so its end code arrived with real data (which is why ER-043's
rule worked there).

## Changes (`ps2xRuntime/src/lib/Kernel/Stubs/MPEG.cpp`)

| Where | Change |
|---|---|
| `MpegPlaybackState` | New `videoEndedAtSequenceEnd` flag and a 3-byte `videoEsTail` |
| new `videoDataEndsWithSequenceEnd` | True when the last start code in the video data fed so far (the 3-byte tail plus the new payload, so a code split across two PES payloads is still found) is a sequence end code `00 00 01 B7` with only zero bytes after it |
| `feedElementaryStream` | Sets the flag from every video payload; any later video data clears it again (a stream with a second sequence keeps playing) |
| `flushDecoderIfEnded` | Also drains the decoder when the flag is set, so the last pictures come out |
| `sceMpegGetPicture` (wait) | Does not wait for more pictures when the flag is set and no `sceCdSt` stream was ever started |
| `sceMpegIsEnd` | For games with no `sceCdSt` stream (ER-043's case), a video stream that ended with its sequence end code also counts as end of input |
| `sceMpegReset` | Keeps the flag, as it keeps `streamEnded` |
| `sceMpegDemuxPss`, `sceMpegDemuxPssRing` | Wake a waiting `sceMpegGetPicture` when a demux call sets the flag |

Games that stream with `sceCdSt` keep upstream's rule (a sequence end code is not authoritative there). Audio packets
after the video's end code are still demuxed and passed to the game (the flag does not set `streamEnded`).

## Evidence that the rule fits this game

All six PSS files have exactly one video sequence (one `00 00 01 B3`, one `00 00 01 B7` in the 0xE0 elementary
stream), and the `B7` is the last 4 bytes of the last video packet; after it come only audio and padding (AGETEC
17,451 bytes, EROPEN 255,101). Read by a parser over the files in `C:\rs\recomp\boot\` (not kept).

## Not changed (seen, left for later cards)

- The title screen stage is reached but nothing is visible on it (black where PCSX2 shows the white title screen):
  picture path, ER-030 / ER-047 area.
- That real libmpeg ends on the stream's own `B7` the same way: **Unconfirmed** (the game's own `B7` normally
  arrives right after it, so the real decoder would stop at the same picture either way).
