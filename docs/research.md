# Project Ringsmith research survey (ER-005)

Desk survey, Tue 17 Nov 2026 (12:10 to about 12:30 Eastern). Nothing downloaded or cloned; no game data. Fetching was done
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
- **decomp.me PS2 presets:** not checked (site forbids AI crawlers; GitHub file read failed with 503). Open lead.
- Not covered: Twisted Metal Harbor City, Kingdom Hearts, Jak, Silent Hill 2/3, Gran Turismo, FromSoftware titles. The first search named Twisted Metal and a CrystalOSD project using ee-gcc 2.9-991111 (search snippet only, **Unconfirmed**).

## 5. Open leads

1. Fetch the almarsguides NTSC-U CodeBreaker page and (with LuciDin's yes) the archive.org manual text.
2. Read `docs/build/building.md` in OpenRAC/rac1-decomp for the Windows build route; read SSX ps2-ee-toolchain for MSYS2 buildability.
3. decomp.me PS2 compiler IDs (via a saved page from LuciDin or the GitHub source when reachable).
4. The PAL fork (kokolika) front page and config, to see what the PAL ELF run looked like.
5. PS2Recomp wiki / ps2xAnalyzer readme and the unread parts of its issue list; check licence implications.
6. romhacking.net, gamehacking.org, PCSX2 patches repo folder, TCRF (saved HTML), PCSX2 compatibility notes (saved HTML).
7. JP serial and JP-versus-US differences (needed only for the later "other versions" item).
8. FromSoftware PS2 file formats (King's Field IV, Armored Core 2 / 3) as a lead for `ERDATA.BIN` in Phase 2.

## 6. Useful next

- ER-002 (Opus, high): identify compiler from the ELF; confirm SSX-style `ee-gcc 2.9-ee` vs SN vs Metrowerks; try an MSYS2 build of the GPL ee-gcc; if none runs natively, ask LuciDin for a ruling (Windows-native requirement vs Wine / Linux boot).
- ER-003: use splat `platform: ps2` with the SSX Tricky / sly1 layout as a template; test COP2 / MMI disassembly early.
- Recomp: build PS2Recomp natively with MSVC as a spike once the ELF is dumped (ER-001); do not rely on its VU / GS yet.
- Second-pass research card (Sonnet, medium): the open leads above.
