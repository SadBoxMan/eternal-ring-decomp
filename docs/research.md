# Project Ringsmith research survey (ER-005)

Desk survey, Tue 17 Nov 2026 (12:10 to about 12:30 Eastern); second pass ER-007 adds §7 to §10. Nothing downloaded or cloned; no game data. Fetching was done
by sub-agents under the project's website rules (robots.txt / terms checked first per non-GitHub site). Every claim below
comes from the page named; items marked **Unconfirmed** were not read from a primary source. Counts (stars, issues) are
snapshots of 17 Nov 2026.

## 1. Sources table

Trust: **P** = primary / first-party, **S** = scanned or curated secondary, **A** = aggregator / unverified, **F** = single forum or wiki paraphrase.

### Eternal Ring

| Source | URL | Covers | Trust | Useful to Ringsmith? |
|---|---|---|---|---|
| Wikipedia article | https://en.wikipedia.org/wiki/Eternal_Ring | Release dates (JP 4 Mar 2000, NA 26 Oct 2000, EU 22 Dec 2000), publishers (FromSoftware JP; Agetec NA; Ubi Soft / Crave EU), PS4 emulation re-release (NA, 23 May 2017), gem / ring system summary | F (one article) | Background only. No version-difference notes. |
| Manual scan (archive.org) | https://archive.org/details/ps2_Eternal_Ring_USA | US manual, OCR text and 11.2 MB PDF; uploaded 2022-10-26. Page read; PDF **not** downloaded | S | **Yes**: official manual is the second source after the game (CLAUDE.md). Ask LuciDin before downloading. |
| archive.org other items | search only: `eternal-ring` (software), `eternal_ring_undub_patch`, `eternal-ring-ps2-hiresscans` (SLUS 20015 box scans), `ArmoredCore2EternalRingEvergraceAgetecPS2GraphicAssets20000609` (data), print ad, bootleg OST | Titles from search metadata only | Unverified | `eternal-ring` and the undub patch look like game data / patch: **do not fetch** (no game data rule). Possibly useful for later regional work. |
| CheatHappens | https://www.cheathappens.com/6618-PlayStation_2-Eternal_Ring_cheats | Action Replay codes labelled "EU" (167 codes, dated 22 Mar 2003) and "US" (3 codes, dated 30 Jan 2003). Two 8-hex-digit words per code; examples show item, spell, level and HP / mana writes (addresses like `4C8A7BBC`, `3CB86D59`) | A, low to medium | **Lead only**: reveals what data exists (player stats, inventory tables). Format of the words is not explained, region label is the site's claim, no disc ID. Verify against the disc before use. |
| almarsguides | https://almarsguides.com/retro/walkthroughs/PS2/Games/EternalRing (stub); linked CodeBreaker page "NTSC-U": https://almarsguides.com/retro/walkthroughs/ps2/games/eternalring/codebreaker/ | Stub page read (no codes). The NTSC-U CodeBreaker page was **not fetched**; robots.txt allows it | A (not read) | **Most likely US-region code list.** Worth one fetch in a later step. |
| GameRevolution cheats | https://www.gamerevolution.com/guides/30313-eternal-ring-ps2-cheats | Returned HTTP 403 to the fetcher | Unread | Unknown. |
| kokolika/ps2recomp_eternal_ring_ps2 | https://github.com/kokolika/ps2recomp_eternal_ring_ps2 | The only GitHub repo found for the game: a personal fork of PS2Recomp with a game config ("Trying to decompile the ps2 game"), 2 commits, 2 stars, GPL-3.0. Root file named `sles_500.51` suggests the **PAL** ELF (**Unconfirmed**). Not opened beyond the front page | A, hobby | Shows someone ran PS2Recomp on the PAL build; status unknown. Do not copy; check later for lessons (patch scripts, overrides). |

Sites skipped for anti-AI rules (list for LuciDin; he can supply saved HTML):

| Site | Reason |
|---|---|
| gamefaqs.gamespot.com | robots.txt disallows ClaudeBot and other AI crawlers, `ai-train=no` |
| mobygames.com | same signal block |
| pcsx2.net (wiki, compatibility list, forums) | same signal block |
| codeberg.org | robots.txt names ClaudeBot / anthropic-ai with `Disallow: /` (the RC1 mirrors there were skipped; GitHub OpenRAC repo used instead) |
| decomp.me (site) | robots.txt: `ai-train=no`, ClaudeBot disallowed; only robots.txt read |
| tcrf.net | never fetched by project rule; probably has a page (**Unconfirmed**). LuciDin: saved HTML if wanted |

Not reached (no source checked, **nothing found does not mean nothing exists**): romhacking.net / ROM-hack and translation pages
(robots allows; no game page located), gamehacking.org (robots allows, crawl-delay 30; no URL known), PCSX2 patch repo
https://github.com/PCSX2/pcsx2_patches (front page only; folder not opened), GameFAQs guides and FAQs, Japanese
disc serial, JP-versus-US difference notes, FromSoftware PS2 archive-format notes (King's Field / Armored Core), `ERDATA.BIN`
extraction tools. GitHub code search needs sign-in. **Open for a second pass.**

### PS2 recompilation tooling

| Source | URL | Covers | Trust | Useful? |
|---|---|---|---|---|
| PS2Recomp (ran-j) | https://github.com/ran-j/PS2Recomp | EE static recompiler to C++ plus runtime; see §3 | P (README, issues read) | **Yes**: the main existing option. |
| PS2Recomp IOP notes | https://github.com/ran-j/PS2Recomp/blob/main/ps2xIOP/README.md | R3000A interpreter running the game's own IRX modules, SIF RPC | P | Yes (IOP / SIF design). |
| recomp.fyi catalogue | https://recomp.fyi (named in the README; not read) | Third-party list of recomp projects | Unconfirmed | Look later. |

### PS2 decomp tooling

| Source | URL | Covers | Trust | Useful? |
|---|---|---|---|---|
| splat | https://github.com/ethteck/splat (wiki: `/wiki`, `/wiki/Quickstart-Elf`) | Binary splitter; PS2 supported but described as "limited" in the wiki; MIT; PyPI `splat64[mips]` | P | **Yes, ER-003.** |
| spimdisasm | https://github.com/Decompollaborate/spimdisasm | MIPS disassembler used by splat; README lists R5900 support | P | Yes. |
| rabbitizer | https://github.com/Decompollaborate/rabbitizer | Instruction decoder; README lists R5900 decoding; COP2 / VU0 not stated | P | Check MMI / COP2 coverage for ER-003. |
| Sly Cooper decomp | https://github.com/TheOnlyZac/sly1 | CC0-1.0; splat + ninja; SN ProDG implied by script name (version **Unconfirmed**); Linux / WSL only | P | Layout and build model. |
| SSX Tricky decomp | https://github.com/ssxdecomp/ssxtricky (the `ssxdvd` URL served this page) | ee-gcc 2.9-ee-991111-01 (also 2.9-ee-990721 earlier); flags in §4; 32-bit Linux compiler | P | **Best flag reference** for a GCC 2.9-ee game. |
| Ratchet & Clank decomp | https://github.com/OpenRAC/rac1-decomp | GPL-3.0; claims all 5116 functions of PAL v2.00 match, plus NTSC-J and NTSC-U; SN ProDG 3.01 and SCE PS2 SDK 24 compilers; Docker/Podman on Linux, Windows steps in `docs/build/building.md` (not read) | P (claim not independently checked) | Yes: Windows build doc, SN route. |
| PaRappa the Rapper 2 | https://github.com/bluisblu/parappa2 | Fork of parappadev/parappa2; ninja + asm-differ; needs i386 Ubuntu; compiler not named in read text | P | Minor. |
| decompme compilers | https://github.com/decompme/compilers | Compiler build recipes as Docker images on ghcr.io; PS2 entries **not confirmed** | P | Source of `ee-gcc2.9-991111-01.tar.xz` (SSX Tricky download script) |
| binutils (decompals) | https://github.com/decompals/binutils-mips-ps2-decompals | Binutils 2.40 fork with PS2 additions; SSX Tricky pins release v0.10 (Linux x86-64) | P | Windows build not confirmed. |
| SSX ee toolchain | https://github.com/SSXModding/ps2-ee-toolchain | GPL source for the ee-gcc parts | P (not read) | Possible Windows build source. |
| wibo | https://github.com/decompals/wibo | Runs 32-bit **Windows** CLI binaries on Linux / macOS; MIT | P | **No**: wrong direction for this machine. |
| ps2dev ps2toolchain | https://github.com/ps2dev/ps2toolchain | Unix build scripts (Docker file present); no Windows / MSYS2 note and no prebuilt binaries in the README; GCC version not read | P | Not a matching compiler (modern GCC). |
| decomp.me PS2 presets | not checked | See §4 | none | Open. |

## 2. Eternal Ring findings

- Facts to treat as background, not data: release dates and publishers as in the table; gameplay model (first-person action RPG;
  elemental gems and four ring families) comes from a single Wikipedia paraphrase and is **Unconfirmed** against the game.
- US disc details (SLUS_200.15, ERDATA.BIN etc.) come from our own disc read in `docs/handoff.md`; nothing in the sources contradicts them.
- **No public decompilation, symbol map or format documentation for Eternal Ring was found.** The only recomp attempt is the
  PAL-targeted hobby fork above. No mods, translations or hacks were found in the searches done (the undub patch on archive.org is
  a title only).
- Cheat codes exist (EU list long, US list short). They imply the existence of stat, inventory, ring and spell tables in RAM.
  Use them later as hints for Phase 2, only after checking against our dump. The address words are not documented on the
  page, so the format and meaning are **Unconfirmed**.
- Regional differences JP / US / EU: **nothing found**. Our target stays NTSC-U only.
- Debug leftovers: nothing found (tcrf.net not consulted; saved HTML would be the way).

## 3. PS2Recomp (and EE recompilation)

All from https://github.com/ran-j/PS2Recomp pages (README, LICENSE, ps2xIOP README, commit list p.1, issues p.1 of 3).

- **State:** "Experimental"; about 3.9k stars, 256 forks, 58 open issues, 173 open PRs. Newest commit read was dated 23 Oct 2026 but the list looks stale (an issue is dated 16 Nov 2026): **Unconfirmed**. No releases.
- **Licence:** GPL-3.0 (LICENSE read; "only" vs "or later" not stated). Anything linked or derived from it is GPL: decide before using it in our tree.
- **Build / platform:** C++20, CMake 3.20+, submodules (ELFIO, toml11, fmt). "Tested mainly with MSVC", with a Visual Studio style settings file, so it **should run natively on Windows** (not built by us; **Unconfirmed**). Some vector paths need SSE4 / AVX (SSE2 fallbacks exist).
- **Pipeline:** `ps2xAnalyzer` (ELF to TOML config) then `ps2xRecomp` (R5900 to C++) then `ps2xRuntime` (memory, syscalls, hardware stubs), plus `ps2xIOP` (R3000A interpreter for the real IRX modules). Ghidra-exported symbol tables are the preferred input for stripped games. Per-game override hook (`PS2_REGISTER_GAME_OVERRIDE`); stubs bound by name@address; optional `.recomp.json` metadata.
- **EE coverage:** README claims MMI and VU0 macro support. Commit titles show MMI2 mapping fixes, an explicit FPU accumulator, EE FPU float semantics for results and DIV.S / RSQRT.S, SSE2 paths. **Open bugs listed:** PROT3W, PSLLH / PSRLH / PSRAH shifts 16-31, PADDUH / PSUBUH, PABSW / PABSH, variable word shifts, BC0T / BC0TL branches, delay slot past function end, VU0 R-register ops (VRNEXT etc.), `sceVu0MulMatrix` operand order. So MMI / COP2 are present but known-wrong in places: every opcode needs our own tests.
- **VU1:** an interpreter with a cache exists; README says VU performance is "very bad"; open correctness issues (vf0 forwarding, rounding mode during XGKICK).
- **SCE / syscalls:** a syscall dispatcher with common IDs, SIF RPC handlers, sceVu0 macro library, stubs bindable per game. No complete SCE module coverage list was found. Open HLE bugs include rand / srand stubs, `sceCdStatus` always "reading", packet DMA tag size, boot hangs waiting on CDVD / LOADFILE servers.
- **GS:** a CPU rasteriser backend exists (rework and refactor commits); open issues on fill rule, sub-pixel offsets, COLCLAMP, bilinear filter. Other (GPU) backends **Unconfirmed**.
- **IOP / SIF:** runs original IRX modules with a virtual IOP kernel, no BIOS needed; generic HLE for memory card (MCSERV), LIBSD, DBCMAN; unknown modules fail to load. Relevant: our disc has `IOP/`, `IOPRP165.IMG` and `VOICE/`.
- **Games reported:** issue titles only (Medal of Honor Vanguard boots to guest code; ICO hangs at a room transition; Tekken 4 rand bug). **No game is confirmed playable.**
- **Other EE static recompilers:** none named in the pages read. Others may exist (**Unconfirmed**; not searched beyond one web query).

Assessment: the closest base for the recomp track, but immature and GPL. For Ringsmith it is best used as a reference and test bed in
Phase 0 / Recomp track 1, while keeping our as-shipped goal of exact behaviour (differential tests against PCSX2) in mind.

## 4. Decomp tooling and toolchain notes (for ER-002 / ER-003)

- **splat:** PS2 supported ("limited" per its wiki). Quickstart-Elf is written for PS2 ELFs: `platform: ps2`, `compiler: EEGCC`, segment types code / asm / textbin / data / rodata / gcc_except_table / sbss / bss. Relocatable ELFs are not supported (the main game ELF `SLUS_200.15` is presumably non-relocatable: **Unconfirmed**, ER-002 to check). It assumes Linux / WSL / Docker and `mips-linux-gnu-objcopy`.
- **No hypervisor on this machine (CLAUDE.md): WSL2 and Docker are out.** Every PS2 decomp project read assumes Linux, WSL2, Docker or Wine (SSX Tricky: 32-bit Linux binary compiler; sly1: Linux / WSL; wibo goes the wrong way). **A native-Windows path for the compiler was NOT found.** Candidates to investigate in ER-002: building the GPL ee-gcc sources (https://github.com/SSXModding/ps2-ee-toolchain, plus the decompals binutils) under MSYS2; checking the rac1-decomp Windows doc; whether PS2 SDK Windows binaries (`ee-gcc.exe`) or SN ProDG for PS2 Windows tools are obtainable legally (licence **Unconfirmed**; the ProDG repo returned a 504 and was not read). Also an MSYS2 approach as used for Twinblade.
- **Compilers by project:** ee-gcc 2.9-ee-991111-01 (SSX Tricky; also fetched as `ee-gcc2.9-991111-01.tar.xz` from the decompme/compilers release); SN ProDG 3.01 and SCE PS2 SDK 24 (Ratchet & Clank); SN ProDG implied (Sly Cooper). Metrowerks for PS2: no project read. Which family Eternal Ring uses is for ER-002 to determine from the ELF (a 2000 launch-window title: GCC 2.9-ee-990721 or -991111 is plausible, **Unconfirmed guess**).
- **Flags seen (SSX Tricky README, recovered from a demo makefile):** Release `-O2 -fno-edge-lcm -fomit-frame-pointer`; Test `-g -O2 -fno-edge-lcm`; Debug `-g -G0 -O0`; `-fno-exceptions` for C++. Treat as hypotheses for this game, not levers.
- **Disassembly:** spimdisasm lists R5900 support; rabbitizer lists R5900 decoding. Whether COP2 / VU0 macro opcodes disassemble correctly and how MMI is shown must be tested early on our ELF (ER-003).
- **decomp.me PS2 presets:** compiler IDs found in ER-007 via the GitHub repo (the table of §8 and the `values.yaml` of decompme/compilers: ee-gcc2.9-990721, 2.9-991111, 2.9-991111a, 2.9-991111b-r4, 2.9-991111-01 (+ `-dtls13010`), 2.95.2-273a / -274, 2.95.3-107 / 114 / 136, 2.96, 3.2-*, iop-gcc2.8.1, iop-gcc2.95.2-102, about 40 mwcps2-*). No SN ProDG IDs. Preset flag strings are database rows, not in the source: **not found**.
- Not covered: Twisted Metal Harbor City, Kingdom Hearts, Jak, Silent Hill 2/3, Gran Turismo, FromSoftware titles. The first search named Twisted Metal and a CrystalOSD project using ee-gcc 2.9-991111 (search snippet only, **Unconfirmed**).

## 5. Open leads (status after ER-007, Tue 17 Nov 2026)

1. almarsguides NTSC-U page: **CLOSED** (§7.1). Archive.org manual text: still waits on LuciDin's yes to download.
2. rac1-decomp Windows doc and ps2-ee-toolchain buildability: **CLOSED** (§8).
3. decomp.me PS2 compiler IDs: **CLOSED for IDs** (§8.4); the preset flag strings live in decomp.me's database and were not found.
4. kokolika PAL fork: **CLOSED** (§7.2).
5. PS2Recomp licence and last commit: **CLOSED** (§9). Wiki / ps2xAnalyzer readme / rest of issues: still open (low priority).
6. romhacking.net, gamehacking.org, PCSX2 patches folder: still open. TCRF and PCSX2 compatibility notes: LuciDin supplied no saved HTML, so still open.
7. JP serial and JP-versus-US differences: still open (needed only for the later "other versions" item).
8. FromSoftware PS2 file formats (King's Field IV, Armored Core 2 / 3) for `ERDATA.BIN`: still open.

## 6. Useful next

- ER-002 (Opus, high): identify compiler from the ELF; confirm SSX-style `ee-gcc 2.9-ee` vs SN vs Metrowerks; try an MSYS2 build of the GPL ee-gcc; if none runs natively, ask LuciDin for a ruling (Windows-native requirement vs Wine / Linux boot).
- ER-003: use splat `platform: ps2` with the SSX Tricky / sly1 layout as a template; test COP2 / MMI disassembly early.
- Recomp: build PS2Recomp natively with MSVC as a spike once the ELF is dumped (ER-001); do not rely on its VU / GS yet.
- Second-pass research card (Sonnet, medium): done as ER-007 (§7 to §10 below). Remaining open leads are in §5.

## 7. ER-007 findings: Eternal Ring leads

Second pass, Tue 17 Nov 2026 (about 12:20 to 12:50 Eastern). Fetching done by sub-agents, GitHub pages and APIs plus almarsguides (robots.txt and terms read first: robots disallows only members and search paths; terms have no anti-bot clause). Nothing downloaded or run. No game data.

### 7.1 almarsguides NTSC-U CodeBreaker page

Source: https://almarsguides.com/retro/walkthroughs/ps2/games/eternalring/codebreaker/ (site is a curated secondary source, trust A).

- Titled "Eternal Ring Codebreaker Codes (NTSC-U)". No game ID or CRC is given, and the page does not explain the code format.
- Two "must be on" enable lines (one for CodeBreaker v1-5, one for v6+). 15 codes sit on the page: level, HP, MP, experience (max, quick gain, quick level up) and seven stat codes (Fire, Earth, Wind, Water, Light, Dark, Str). Four more categories (Have Item, Have Mag Gem, Have Ring, Have Weapons) are on sub-pages **not fetched**.
- The addresses are in CodeBreaker form. The stat codes share a `1A`-prefixed pattern (for example Level 99 at `1AC6CED8`, Infinite HP at `1ACACED8`; the page shows values such as 99 and 999). One experience code is `2A`-prefixed (`2A2F3D05`).
- **Unconfirmed:** CodeBreaker words are encrypted in many code versions, so these addresses may not be raw RAM addresses. Do not use them as RAM offsets until checked against a RAM dump of our US disc (Phase 2). The CheatHappens "US" list in §1 is a separate 3-code set and was not compared.

### 7.2 kokolika/ps2recomp_eternal_ring_ps2 (PAL fork)

Read: repo page, `config.toml`, commit list (https://github.com/kokolika/ps2recomp_eternal_ring_ps2).

- 2 commits, both dated 22 Mar 2026 ("initiation", "Initial commit"). GPL-3.0. The README is the upstream PS2Recomp README with no Eternal Ring progress notes.
- The only input named is `sles_500.51` (PAL serial naming; **Unconfirmed** that it is the Eternal Ring PAL ELF, the file was not opened).
- `config.toml` (978 lines) says it was generated by the ElfAnalyzer. It lists a Ghidra map `map.csv`, 3 stubs (printf, malloc, free), 159 skipped entries (abort, exit and 157 `sub_XXXXXXXX` names), several hundred MMIO address-value pairs, and a performance list of about 150 functions with comments such as "uses SIMD" or "heavy loops". Patch flags: `patch_syscalls=false`, `patch_cop0=true`, `patch_cache=true`. There is no overrides section.
- The repo also holds `patch_iop_loops.py` and many `.bat` helpers (auto recompile loop, find missing functions, update config from missing). This suggests the author hit missing-function and IOP wait-loop problems; their content was not read, so that is a **reading, not a fact**.
- **Verdict:** hobby attempt that stopped after one day with no result recorded. Nothing in it shows the game booting. Lessons to look at later (not now): the IOP loop patch script and the missing-function loop.

## 8. Windows-native ee-gcc options (ER-002's key question)

Ranked by how likely each is to give a working 2.9x-era EE compiler on this machine (native Windows and MSYS2 only; no WSL2, Docker, Podman, hypervisor). "Known" = read on a page; "claimed" = stated by a page but not checked; "none" = no evidence found. We downloaded and ran nothing.

| Rank | Path | Where from | Licence | Known to work | Only claimed / unknown |
|---|---|---|---|---|---|
| 1 | **SN ProDG for PS2 Windows installs mirrored on GitHub** (AngheloAlf): `SN-Systems-ProDG_for_PS2_3.01` (GCC 2.95.3, SN build v1.36; driver `ee-gcc2953.exe`; also IOP 2.95.2), `sce_ps2_sdk_24` (GCC 2.95.2 SN build v2.74 and 2.95.3 v1.14; drivers `ee-gcc295.exe`, `ee-gcc2953.exe`), and a linked `..._2.0` repo (GCC 2.95.2 v2.73a) | https://github.com/AngheloAlf/SN-Systems-ProDG_for_PS2_3.01 and https://github.com/AngheloAlf/sce_ps2_sdk_24 | Proprietary SN Systems / Sony software redistributed with no licence statement; the repos give no permission. **Needs LuciDin's ruling** before anyone downloads | Real **Windows .exe** drivers (READMEs). rac1-decomp's Windows steps clone exactly these two repos, so the project depends on them | These are SN builds of 2.95.x, **not** 2.9-ee-991111-01. Whether they work on Win11 x64 without the Sony SDK is not shown (the 3.01 README says the installer wants a Sony SDK path and empty folders bypass the check). Last commit dates not read |
| 2 | **Reproduce ee-gcc 2.9-ee-991111 under MSYS2 from the GPL source** in https://github.com/SSXModding/ps2-ee-toolchain (single commit 14 Apr 2022, "Init (EE-GCC 2.9-ee-991111, IOP GCC 2.8.1)"; description "patched to build on GCC 11") | same repo; sources vendored in `ee/` and `iop/` | GPL-family: `ee/` carries `COPYING` / `COPYING.LIB` / `COPYING.NEWLIB`, no root LICENSE, Makefile header says GPL-2 or later (boilerplate only). Treat as GPL, version for the whole tree **Unconfirmed** | Builds on Linux with a 32-bit host. `build.sh` only uses `--host=i686-linux-gnu`, `CC='gcc -m32'`, `LDFLAGS='-static'`; no MinGW / Cygwin / Windows host anywhere in it | **Nothing shows a Windows host build.** The tree does contain upstream Windows host files (`gcc/config/i386/cygwin.h`, `mingw32.h`, `winsup/`, `configure.bat`), but they are unused. Needs `-m32` capable MSYS2 MinGW32 toolchain; an unpatched 1999 GCC on a modern MinGW will likely need patches. A try-build is a real task, not a lookup |
| 3 | **ps2dev prebuilt Windows tarball** `ps2dev-windows-latest.tar.gz` (v2.0.0, 17 May 2026, 272 MB) | https://github.com/ps2dev/ps2dev/releases | ps2dev MIT; the compilers inside are GPL | Asset exists (listed by the releases API) | Contents unread. The README / sub-repos say it is **GCC 15.2 / binutils 2.45.1** (ps2toolchain-ee). **Not a matching compiler** for a 2000 game; useful only for modern PS2 homebrew and for the Phase 5 port tooling |
| 4 | **decomp.me / decompme compilers packages** (`ee-gcc2.9-991111-01.tar.xz`, `ee-gcc2.95.2-274` inside `ps2_compilers.tar.xz`, `mwcps2-*`) | https://github.com/decompme/compilers (releases `compilers`) | Per-compiler; repo has no visible LICENSE | Dockerfiles only wget and unpack these archives into Alpine images, no Wine or wibo in those three files. The SSX Tricky project runs the 2.9-991111-01 package as a **32-bit Linux** binary | Whether the archives hold Linux ELFs or Windows .exe was **not determined** (archive not opened). If they are Linux ELFs they are useless here. Listed as a question for ER-002 if the download is approved |
| 5 | **Old Sony / SCE Windows toolchain** (`ee-gcc.exe` 2.9-ee, Cygwin-hosted) | none found | Proprietary | none | No page read names a download route. The ps2dev issues list ("not building in cygwin" 2023, "building on Windows (mingw)" 2021, "native 64-bit windows host build" PR 2019) concerns the modern ps2toolchain, titles only |
| 6 | **CodeWarrior PS2** (`mwcps2-*`: 2.3 to 3.0.x) | decompme/compilers lists about 40 `mwcps2` IDs | Proprietary (Metrowerks / Freescale) | Existed as a PS2 compiler (Wikipedia). Windows-hosted detail not read | Obtainable only via the decompme packages (same unknown as rank 4). Use only if ER-002 finds CodeWarrior code patterns |
| 7 | **Wine / wibo / Linux boot** | n/a | n/a | rac1-decomp uses a Wine container on Linux; sly1 uses `wibo-i686` | **Out** on this machine (CLAUDE.md). Not needed for the rank 1 .exe files: Windows runs 32-bit .exe itself (if they run, **Unconfirmed**) |

Other facts that bear on the ranking:

- rac1-decomp's `docs/build/building.md` offers a "Windows: Git Bash, Python 3.10+" path, clones the two AngheloAlf SN repos (rank 1), and runs `tools/setup_compilers.sh`. But its C front-end step (`python tools/gcc323.py --setup`) is described as using **Docker or Podman on Windows**, so that one step is not native (the game's own compilers are the SN .exe files; gcc323 is a separate piece). Keep the path short (legacy make limits).
- sly1 supports Linux or Windows via WSL only (`wine32` or `wibo-i686`, plus a ProDG setup script). ssxtricky says Linux with 32-bit support or WSL2, no native Windows path, no Docker. Both are Linux-hosted compilers; neither has a Windows-native route.
- decompals/binutils-mips-ps2-decompals v0.10 (released 4 Jun 2026) **has a Windows build** (`binutils-mips-ps2-decompals-windows-x86-64.zip`), so the assembler and linker side of the toolchain is available natively. Licence GPL-2.0 (and others, per the repo's COPYING files).
- ps2-ee-toolchain's `ee/gcc/README` says egcs 1.1; the "2.9-ee-991111" label is only the commit title, so the exact compiler version in the source is **Unconfirmed**.
- **No page read gave a working Windows-native build of the exact ee-gcc 2.9-ee-991111-01.** Rank 2 would be the first honest attempt at it; rank 1 is the only path with real Windows .exe compilers, but they are 2.95.x SN builds and carry a legal question.

**Recommendation for ER-002, in order:** (a) identify the compiler from the ELF first (strings, `GCC:` comment sections, code idioms), since that decides which of ranks 1, 2 and 4 matters; (b) if the ELF is SN ProDG 2.95.x, rank 1 is the only direct route: ask LuciDin for a ruling on using the AngheloAlf mirrors (unlicensed redistribution of proprietary tools; nothing is downloaded until he says yes); (c) if the ELF is Sony-style ee-gcc 2.9-ee-99xxxx, try rank 2 (MSYS2 MinGW32 build of the vendored GPL source), keeping the build log, and ask LuciDin whether to also open the decompme package to see if it is a Windows binary.

## 9. Licences and activity (items "not verified" in ER-005)

| Item | Licence (source) | Notes |
|---|---|---|
| PS2Recomp (ran-j) | GPL-3.0: LICENSE is the GPL v3 text; README says nothing more | "Only" vs "or later" **not stated**: the "or any later version" words appear only in the generic how-to-apply template at the end of LICENSE. Source-file headers not checked. Treat as **GPL-3.0, "only/or-later" unresolved**. Last commit on `main`: **23 Oct 2026** ("GIF: treat FLG=3 like IMAGE ...", e563698). No commit since by that API read, which makes the earlier "stale" worry plausible; the newest issue is 16 Nov 2026 |
| splat | MIT (LICENSE read) | |
| spimdisasm | MIT (LICENSE, 1.x branch) | |
| rabbitizer | MIT (LICENSE, 1.x branch) | |
| ps2dev/ps2dev, ps2toolchain-ee and sibling repos | MIT (GitHub sidebars); ps2toolchain itself BSD-2-Clause | The GCC and binutils they build are GPL-family; not read |
| decompals binutils-mips-ps2-decompals | GPL-2.0 plus three more (COPYING files) | version "only/or-later" not read |
| SSXModding/ps2-ee-toolchain | No root LICENSE; subdir COPYING files; Makefile header GPL-2.0-or-later | Unclear |
| decompme/compilers | No LICENSE visible | Compilers inside are proprietary or GPL per compiler |
| AngheloAlf SN ProDG 3.01 / sce_ps2_sdk_24 | No statement | Proprietary content (see §8 rank 1) |
| OpenRAC/rac1-decomp | GPL-3.0 (from ER-005) | |
| sly1 | CC0-1.0 | |

## 10. Sites skipped, and not verified

Skipped (anti-AI rules or project rule): gamefaqs, mobygames, pcsx2.net, codeberg.org, decomp.me (site), tcrf.net, and (new) **psdevwiki.com** (robots disallows ClaudeBot). No saved HTML from LuciDin for GameFAQs, PCSX2 compat, TCRF. ps2dev.github.io had no robots.txt (404) and was read; Wikipedia pages were read for SN Systems, ProDG, CodeWarrior and Linux for PlayStation 2.

Not verified: contents of `ps2dev-windows-latest.tar.gz`; whether the decompme archives are Linux or Windows binaries; whether the AngheloAlf SN installs run on Windows 11; whether ps2-ee-toolchain builds on MSYS2 at all; the exact gcc version inside ps2-ee-toolchain; decomp.me preset flag strings (database rows, not in the GitHub source); almarsguides sub-pages and what the CodeBreaker words map to; kokolika's `sles_500.51` is Eternal Ring PAL; rac1-decomp's `setup_compilers.sh` contents; the claim that ps2dev issues reflect working Windows builds (titles only); web search was not available to the fetch agents, so "nothing found" means GitHub and the pages named, not the whole web. Several claims rest on single pages (the Windows asset name on the releases API; "SN BUILD" versions on the AngheloAlf READMEs).
