# ER-099 local change to PS2Recomp: import a PCSX2 memory card (description only)

This file describes, in our own words, the local change ER-099 made to PS2Recomp (ran-j, GPL-3.0, commit
`e5636981`) on Sun 22 Nov 2026. It changes one runtime file and is kept as a diff of GPL code outside this repository
in `C:\rs\recomp\patches\er099_mc_import.patch` until LuciDin decides on the GPL-3.0 question and the fork (ER-019).
Our own cross-check reader is `tools/recomp/ps2mc.py` (in git). No config or generated file changes.

Findings and evidence: `docs/recomp_spike.md` section 14.

## What a memory card is to the runtime (before and after)

Eternal Ring reaches its cards only through libmc (`sceMc*`). The analyzer binds the game's 18 libmc functions to the
runtime's HLE handlers (`ps2xRuntime/src/lib/Kernel/Stubs/MemoryCard.cpp`); the IOP side's card requests
(`PS2IopHostAdapter::memoryCard`) call the same handlers. The game's MCMAN / MCSERV modules are loaded into the IOP
interpreter but no card traffic goes through them. The handlers treat each card as **a plain host folder**: port 0 is
`<elf folder>\mc0`, port 1 `<elf folder>\mc1` (for our runs `C:\rs\recomp\boot\mc0` / `mc1`, both empty), save
folders and files are host folders and files, and GetDir reports host file times read as local time. There is no card
image, no FAT and no ECC in the runtime.

The import keeps that model. With the switch set, a port's folder is `<elf folder>\mc<port>_import`, refilled from the
PCSX2 card at every launch; without it, nothing changes (as shipped).

## Usage

| Variable | Effect |
|---|---|
| `PS2X_MC0_IMPORT=<card>` | Card 1 (port 0) is a copy of `<card>` |
| `PS2X_MC1_IMPORT=<card>` | Card 2 (port 1) is a copy of `<card>` |

`<card>` is either a PCSX2 card image (`Mcd001.ps2`: 8 MB, normally with the 16-byte ECC spare area after every
512-byte page, 8,650,752 bytes; the plain 8,388,608-byte form and other sizes whose superblock fits are read too) or a
PCSX2 folder card (a folder holding `_pcsx2_superblock`). Point it at a **copy** of the card outside git (ER-099 used
`C:\rs\captures\cards\`); the runtime opens it for reading only in any case.

Run script: `C:\rs\recomp\scripts\er099_run.ps1 -Mc0 <card> -Mc1 <card>` (otherwise as `er074_run.ps1`: `-Pad`,
`-Sound`, window on the secondary monitor, console hidden, killed by PID). The import prints `[MC-import]` lines to
stderr: source and target, folders / files / bytes, the ECC result for an image, and the top-level save folders.

Saves the game makes go to the copy and are lost at the next launch (LuciDin's choice, 22 Nov 2026: "fresh copy each
launch"). Nothing is ever written back to a PCSX2 card.

## The change (`er099_mc_import.patch`, `Stubs/MemoryCard.cpp` only)

| Part | What it does |
|---|---|
| `getMcRootPath(port)` | Asks the new `er099::importedRoot(port)` first; only if the port has no import does the original code run |
| `importedRoot` | Once per port (`std::call_once`): reads the variable; if set, empties `<elf folder>\mc<port>_import` and fills it from the card. Refuses (the port keeps its as-shipped folder, with an `ERROR` line) when the card does not exist, a folder has no `_pcsx2_superblock`, or the card and the import folder overlap. A card that fails while being read leaves the port with an empty card and an `ERROR` line |
| Card image reader | Superblock (magic, page size, pages per cluster, clusters, allocation offset, root cluster, indirect FAT list); ECC or plain layout chosen by the file size; FAT chains through the indirect list (end `0xFFFFFFFF`, allocated bit 31, loop and free-cluster checks); 512-byte directory entries (a folder's entry count is its own entry's length in the parent; the root's is in its `.` entry); skips `.`, `..` and entries without the exists bit. Checks mcman's 3-byte ECC on every 128-byte chunk it reads and reports the count of bad chunks (it does not correct them) |
| Folder card copy | Copies the folder tree, skipping PCSX2's `_pcsx2_*` files |
| Names | Only names that can be one Windows path component unchanged are written (no `/ \ : * ? " < > |`, control characters, `.` / `..`, trailing dot or space, at most 32 bytes); others are skipped and logged, so a damaged card cannot write outside the import folder |
| Time stamps | Each folder's and file's host time is set so that GetDir (local-time reading) reports the card's stored "modified" stamp unchanged, as mcman reports what is on the card. Image: the entry's stamp. Folder card: the `timeModified` value of PCSX2's `_pcsx2_index` (the stored stamp written as if UTC; its keys are NUL-padded 32-byte names); with no index entry, the host time read in Japan time (UTC+9), the relation seen on LuciDin's card (Unconfirmed for other cards) |

## What it does not do (the runtime's folder model, unchanged)

- GetDir reports fixed attributes (read, write, file / folder, closed, exists) and the modified stamp as the created
  stamp too; the card's own mode bits (e.g. `0x8497` files, `0x8427` folders on LuciDin's cards) and created stamps
  are not carried. Entries are listed in name order, not the card's order.
- GetInfo always reports a formatted card with 8,192 free clusters, whatever the copy holds.
- File I/O on `mc0:` paths through the runtime's file system (`sceOpen("mc0:...")`) still resolves to the as-shipped
  `mc0` folder; Eternal Ring does not use it (all its card access is libmc).

## To reproduce on a fresh clone of e5636981

1. Apply the earlier patches as in `ER-074_data_blocks.md`, then `er099_mc_import.patch` (a diff against the
   unpatched `MemoryCard.cpp`, which no earlier patch touches; CRLF kept; SHA-1 95efca17...). Checked: it applies
   to a copy of the e5636981 file and gives a file identical to `game_build`'s, and it reverse-applies on
   `game_build`.
2. Configs and generated code as for ER-074 (no change).
3. Build with `tools/recomp/build_game.bat`.
4. Cross-check an import: `python -I tools/recomp/ps2mc.py <card> --sha1` and
   `python -I tools/recomp/ps2mc.py --plain C:\rs\recomp\boot\mc<port>_import --sha1` list the same files with the
   same sizes and SHA-1s.
