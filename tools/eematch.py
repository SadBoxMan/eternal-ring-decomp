"""ER-015: shared code for the EE matching tools (tools/cmpfunc.py, tools/place.py, tools/clones.py).

- build settings read from the Makefile itself (make --eval), so the tools always compile like the build does
- compile one C file to an object: a file under src/ through `make build/src/<file>.o` (the real rule), any other
  file (a scratch file) with the same gcc / ee-as command lines into build/scratch/
- read a relocatable EE object: sections, symbols, REL relocations (implicit addends)
- compare one function of an object with the original ELF's bytes: exact where no relocation applies; where one
  does, the instruction's fixed part is compared and the game's target is resolved and checked against the symbol
  (R_MIPS_26 jal / j, R_MIPS_HI16 + LO16 pairs, R_MIPS_GPREL16, R_MIPS_LITERAL with the .lit4 word's value)

A masked compare is a working aid, never proof (ruling docs/decomp_recipes.md 3.1): the proof is the function placed
in src/ and `make regress` green. Usage notes: docs/matching_tools.md.
"""
from __future__ import annotations

import os
import re
import struct
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from elfutil import Elf  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
MSYS = Path(r"C:\msys64\usr\bin")
BASEELF = ROOT / "disc" / "extract" / "SLUS_200.15"
SYMS_ELF = ROOT / "build" / "SLUS_200.15.syms.elf"
SCRATCH = ROOT / "build" / "scratch"
GP = 0x00360B70          # _gp of the game (docs/decomp_recipes.md 1.2)
ADDR_NAME = re.compile(r"^(?:func|D)_([0-9A-F]{8})$")

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_GPREL16, R_MIPS_LITERAL = 2, 4, 5, 6, 7, 8
RELOC_NAMES = {2: "R_MIPS_32", 4: "R_MIPS_26", 5: "R_MIPS_HI16", 6: "R_MIPS_LO16", 7: "R_MIPS_GPREL16",
               8: "R_MIPS_LITERAL"}


def s16(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


# --- build settings ------------------------------------------------------------------------------------------

def clean_env() -> dict[str, str]:
    """Same environment as tools/regress.py: MSYS2 usr/bin first, Git for Windows' msys tools removed."""
    env = dict(os.environ)
    git_dirs = (r"git\usr\bin", r"git\mingw64\bin", r"git\bin")
    parts = [p for p in env.get("PATH", "").split(os.pathsep)
             if p and not p.lower().replace("/", "\\").rstrip("\\").endswith(git_dirs)]
    env["PATH"] = os.pathsep.join([str(MSYS)] + parts)
    for k in ("MSYSTEM", "MSYS", "CHERE_INVOKING", "SHELL", "MAKEFLAGS", "MFLAGS", "MAKELEVEL"):
        env.pop(k, None)
    return env


_SETTINGS: dict[str, str] | None = None


def settings() -> dict[str, str]:
    """CC, AS, GCC_DIR, CFLAGS, CPPFLAGS, ASFLAGS exactly as the Makefile expands them."""
    global _SETTINGS
    if _SETTINGS is None:
        names = ("CC", "AS", "GCC_DIR", "CFLAGS", "CPPFLAGS", "ASFLAGS")
        rule = "ermatch_vars: ; @echo " + "'" + "|".join(f"$({n})" for n in names) + "'"
        r = subprocess.run([str(MSYS / "make.exe"), "-s", "--no-print-directory", f"--eval={rule}", "ermatch_vars"],
                           cwd=ROOT, env=clean_env(), capture_output=True, text=True, stdin=subprocess.DEVNULL)
        if r.returncode != 0:
            raise SystemExit(f"eematch: cannot read the Makefile settings:\n{r.stderr}")
        vals = r.stdout.strip().splitlines()[-1].split("|")
        _SETTINGS = dict(zip(names, vals))
    return _SETTINGS


def compile_c(c_path: Path, extra_cflags: list[str] | None = None) -> Path:
    """Compile a C file to an object and return its path. Raises SystemExit with gcc's / ee-as's message on error."""
    c_path = c_path.resolve()
    try:
        rel = c_path.relative_to(ROOT).as_posix()
    except ValueError:
        rel = None
    if rel and rel.startswith("src/") and not extra_cflags:
        obj = ROOT / "build" / (rel[:-2] + ".o")
        r = subprocess.run([str(MSYS / "make.exe"), "-s", f"build/{rel[:-2]}.o"], cwd=ROOT, env=clean_env(),
                           capture_output=True, text=True, stdin=subprocess.DEVNULL)
        if r.returncode != 0:
            raise SystemExit(f"compile failed (make build/{rel[:-2]}.o):\n{r.stdout}{r.stderr}")
        return obj
    st = settings()
    SCRATCH.mkdir(parents=True, exist_ok=True)
    stem = c_path.stem
    s_path, o_path = SCRATCH / f"{stem}.s", SCRATCH / f"{stem}.o"
    cc = [st["CC"], f"-B{st['GCC_DIR']}/", *st["CFLAGS"].split(), *(extra_cflags or []),
          *[f.replace("-I", "-I" + str(ROOT) + "/", 1) if f.startswith("-I") else f for f in st["CPPFLAGS"].split()],
          "-S", str(c_path), "-o", str(s_path)]
    r = subprocess.run(cc, cwd=ROOT, capture_output=True, text=True, stdin=subprocess.DEVNULL)
    if r.returncode != 0:
        raise SystemExit(f"gcc failed on {c_path.name}:\n{r.stdout}{r.stderr}")
    as_cmd = [st["AS"], *st["ASFLAGS"].split(), "-o", str(o_path), str(s_path)]
    r = subprocess.run(as_cmd, cwd=ROOT, capture_output=True, text=True, stdin=subprocess.DEVNULL)
    if r.returncode != 0:
        raise SystemExit(f"ee-as failed on {s_path.name}:\n{r.stdout}{r.stderr}")
    return o_path


# --- the relocatable object -----------------------------------------------------------------------------------

@dataclass
class Reloc:
    offset: int
    type: int
    sym: str          # symbol name, or the section name for a section symbol (".lit4", ".rodata", ...)
    is_section: bool


class Obj:
    def __init__(self, path: Path):
        self.path = path
        self.elf = Elf(path.read_bytes())
        self.syms = self.elf.symbols()
        self.secidx = {s.name: i for i, s in enumerate(self.elf.sections)}
        self.gp0 = 0
        if ".reginfo" in self.secidx:
            sec = self.elf.sections[self.secidx[".reginfo"]]
            self.gp0 = struct.unpack_from("<I", self.elf.data, sec.offset + 20)[0]

    def section_bytes(self, name: str) -> bytes:
        sec = self.elf.sections[self.secidx[name]]
        return self.elf.data[sec.offset:sec.offset + sec.size]

    def relocs(self, target: str = ".text") -> dict[int, Reloc]:
        out: dict[int, Reloc] = {}
        name = ".rel" + target
        if name not in self.secidx:
            return out
        sec = self.elf.sections[self.secidx[name]]
        for k in range(sec.size // 8):
            off, info = struct.unpack_from("<II", self.elf.data, sec.offset + k * 8)
            sym = self.syms[info >> 8]
            if sym.type == 3:  # STT_SECTION
                out[off] = Reloc(off, info & 0xFF, self.elf.sections[sym.shndx].name, True)
            else:
                out[off] = Reloc(off, info & 0xFF, sym.name, False)
        return out

    def functions(self) -> list[tuple[str, int, int]]:
        """(name, offset, size) of each function in .text; size from st_size, else up to the next symbol, with
        trailing zero words (8-byte entry alignment) dropped."""
        ti = self.secidx[".text"]
        text = self.section_bytes(".text")
        starts = sorted({s.value for s in self.syms if s.shndx == ti and s.type in (0, 2) and s.name
                         and not s.name.startswith("$")})
        out = []
        for s in self.syms:
            if s.shndx != ti or not s.name or s.name.startswith(("$", ".")) or s.type not in (0, 2):
                continue
            if s.type == 0 and not ADDR_NAME.match(s.name) and s.bind == 0:
                continue  # local labels
            size = s.size
            if not size:
                nxt = [x for x in starts if x > s.value]
                size = (nxt[0] if nxt else len(text)) - s.value
                while size >= 4 and text[s.value + size - 4:s.value + size] == b"\0\0\0\0":
                    size -= 4
            out.append((s.name, s.value, size))
        return sorted(set(out), key=lambda t: t[1])


# --- the original and the symbol map --------------------------------------------------------------------------

class Game:
    """The original ELF plus names / sizes from the unstripped build (build/SLUS_200.15.syms.elf)."""

    def __init__(self):
        if not BASEELF.exists():
            raise SystemExit(f"original missing: {BASEELF} (see docs/build.md 2 / Worktrees)")
        self.orig = Elf(BASEELF.read_bytes())
        self.addr_of: dict[str, int] = {}
        self.size_of: dict[int, int] = {}
        self.name_at: dict[int, str] = {}
        if SYMS_ELF.exists():
            for s in Elf(SYMS_ELF.read_bytes()).symbols():
                if s.shndx == 0 or not s.name or s.type == 4 or s.type == 3:
                    continue
                self.addr_of.setdefault(s.name, s.value)
                if s.type == 2 and s.size:
                    self.size_of[s.value] = s.size
                if s.type in (1, 2) or ADDR_NAME.match(s.name):
                    self.name_at.setdefault(s.value, s.name)
        self.have_syms = bool(self.addr_of)

    def addr(self, name: str) -> int | None:
        m = ADDR_NAME.match(name)
        if m:
            return int(m[1], 16)
        return self.addr_of.get(name)

    def bytes_at(self, addr: int, size: int) -> bytes | None:
        return self.orig.load_bytes(addr, size)

    def word(self, addr: int) -> int:
        return struct.unpack("<I", self.bytes_at(addr, 4))[0]

    def label(self, addr: int) -> str:
        return self.name_at.get(addr, f"{addr:#010x}")


# --- disassembly ----------------------------------------------------------------------------------------------

def disasm(word: int, vram: int) -> str:
    try:
        import rabbitizer
    except ImportError:
        return f".word 0x{word:08X}"
    ins = rabbitizer.Instruction(word, vram=vram, category=rabbitizer.InstrCategory.R5900)
    return ins.disassemble() if ins.isValid() else f".word 0x{word:08X}"


# --- the compare ----------------------------------------------------------------------------------------------

@dataclass
class WordResult:
    off: int
    ours: int | None
    game: int | None
    ok: bool
    note: str = ""


@dataclass
class FuncResult:
    name: str
    addr: int
    our_size: int
    game_size: int | None
    words: list[WordResult] = field(default_factory=list)
    unchecked: list[str] = field(default_factory=list)   # relocations whose target could not be checked

    @property
    def match(self) -> bool:
        return self.our_size == (self.game_size or self.our_size) and all(w.ok for w in self.words)

    @property
    def first_diff(self) -> WordResult | None:
        return next((w for w in self.words if not w.ok), None)


def compare_function(obj: Obj, game: Game, name: str, addr: int | None = None) -> FuncResult:
    funcs = {n: (o, s) for n, o, s in obj.functions()}
    if name not in funcs:
        raise SystemExit(f"{name}: not a function of {obj.path.name} (have: {', '.join(funcs) or 'none'})")
    off, size = funcs[name]
    if addr is None:
        addr = game.addr(name)
        if addr is None:
            raise SystemExit(f"{name}: no address (not func_XXXXXXXX and not in {SYMS_ELF.name}); pass --addr")
    game_size = game.size_of.get(addr)
    text = obj.section_bytes(".text")
    relocs = obj.relocs(".text")
    lit4 = obj.section_bytes(".lit4") if ".lit4" in obj.secidx else b""
    res = FuncResult(name, addr, size, game_size)
    n = max(size, game_size or 0)
    gbytes = game.bytes_at(addr, n) or b""
    pending_hi: dict[str, list[int]] = {}
    # every HI16 of the function by symbol, in address order: a LO16 whose lui sits in another branch (a shared %hi)
    # has no HI16 pending before it, so it is paired with the last HI16 of its symbol seen, else the first anywhere
    all_hi: dict[str, list[int]] = {}
    for o, r in sorted(relocs.items()):
        if off <= o < off + size and r.type == R_MIPS_HI16:
            all_hi.setdefault(r.sym, []).append(o - off)
    seen_hi: dict[str, int] = {}
    for w in range(0, n, 4):
        a = struct.unpack_from("<I", text, off + w)[0] if w < size else None
        b = struct.unpack_from("<I", gbytes, w)[0] if w + 4 <= len(gbytes) else None
        if a is None or b is None:
            res.words.append(WordResult(w, a, b, False, "past the end of " + ("ours" if a is None else "the game's")))
            continue
        rel = relocs.get(off + w)
        if rel is None:
            res.words.append(WordResult(w, a, b, a == b))
            continue
        kind = RELOC_NAMES.get(rel.type, f"type {rel.type}")
        if rel.type == R_MIPS_26:
            target = ((addr + w) & 0xF0000000) | ((b & 0x03FFFFFF) << 2)
            fixed_ok = (a >> 26) == (b >> 26)
            if rel.is_section:
                # a jump inside our own object (section-relative): compare the in-object target
                want = (((a & 0x03FFFFFF) << 2) - off) + addr
                ok = fixed_ok and target == want
                note = f"{kind} {rel.sym}: game target {game.label(target)}"
            else:
                want = game.addr(rel.sym)
                ok = fixed_ok and want is not None and target == want
                note = f"{kind} {rel.sym}: game target {game.label(target)}" + \
                       ("" if want is not None else " (symbol address unknown)")
                if want is None:
                    res.unchecked.append(f"+{w:#x} {kind} {rel.sym}")
                    ok = fixed_ok
            res.words.append(WordResult(w, a, b, ok, note))
        elif rel.type == R_MIPS_HI16:
            pending_hi.setdefault(rel.sym, []).append(w)
            seen_hi[rel.sym] = w
            ok = (a >> 16) == (b >> 16)
            res.words.append(WordResult(w, a, b, ok, f"{kind} {rel.sym} (checked with its LO16)"))
        elif rel.type == R_MIPS_LO16:
            fixed_ok = (a >> 16) == (b >> 16)
            his = pending_hi.pop(rel.sym, [])
            shared = False
            if not his:
                hw = seen_hi.get(rel.sym, (all_hi.get(rel.sym) or [None])[0])
                if hw is None:
                    res.words.append(WordResult(w, a, b, fixed_ok and a == b, f"{kind} {rel.sym}: no HI16 in the function"))
                    continue
                shared = True
            else:
                hw = his[-1]
            our_hi = struct.unpack_from("<I", text, off + hw)[0] & 0xFFFF
            game_hi = struct.unpack_from("<I", gbytes, hw)[0] & 0xFFFF
            addend = (our_hi << 16) + s16(a & 0xFFFF)
            gaddr = ((game_hi << 16) + s16(b & 0xFFFF)) & 0xFFFFFFFF
            ok, note = _check_target(game, rel, addend, gaddr, kind, obj, res, w)
            if shared:
                note += f" [shares the HI16 at +{hw:#x}]"
            res.words.append(WordResult(w, a, b, fixed_ok and ok, note))
            # every HI16 of this pair must give the same game address
            for h in his:
                gh = struct.unpack_from("<I", gbytes, h)[0] & 0xFFFF
                if gh != game_hi:
                    res.words.append(WordResult(h, None, None, False, f"HI16 at +{h:#x} differs from its pair"))
        elif rel.type in (R_MIPS_GPREL16, R_MIPS_LITERAL):
            fixed_ok = (a >> 16) == (b >> 16)
            gaddr = (GP + s16(b & 0xFFFF)) & 0xFFFFFFFF
            if rel.is_section and rel.sym == ".lit4":
                lit_off = s16(a & 0xFFFF) + obj.gp0
                ours_v = lit4[lit_off:lit_off + 4] if 0 <= lit_off <= len(lit4) - 4 else None
                game_v = game.bytes_at(gaddr, 4)
                ok = ours_v is not None and ours_v == game_v
                note = (f"{kind} .lit4+{lit_off:#x}: ours {_f32(ours_v)}, game {gaddr:#010x} = {_f32(game_v)}"
                        " (value checked; the literal's place is judged by make regress)")
                res.words.append(WordResult(w, a, b, fixed_ok and ok, note))
            else:
                ok, note = _check_target(game, rel, s16(a & 0xFFFF) + (obj.gp0 if rel.is_section else 0),
                                         gaddr, kind, obj, res, w)
                res.words.append(WordResult(w, a, b, fixed_ok and ok, note))
        else:
            res.words.append(WordResult(w, a, b, False, f"unhandled relocation {kind} {rel.sym}"))
    return res


def _f32(b: bytes | None) -> str:
    if not b or len(b) < 4:
        return "?"
    return f"{struct.unpack('<f', b)[0]!r} (0x{struct.unpack('<I', b)[0]:08X})"


def _check_target(game: Game, rel: Reloc, addend: int, gaddr: int, kind: str, obj: Obj, res: FuncResult,
                  w: int) -> tuple[bool, str]:
    if rel.is_section:
        # an address-named symbol of our own object at that place (INCLUDE_SDATA / INCLUDE_DATA / INCLUDE_RODATA)
        si = obj.secidx.get(rel.sym)
        named = [s.name for s in obj.syms if s.shndx == si and s.value == addend and ADDR_NAME.match(s.name)]
        if named:
            want = game.addr(named[0])
            if want == gaddr:
                return True, f"{kind} {rel.sym}+{addend:#x} = {named[0]}: ok"
            return False, f"{kind} {rel.sym}+{addend:#x} = {named[0]}, game uses {game.label(gaddr)}"
        res.unchecked.append(f"+{w:#x} {kind} {rel.sym}+{addend:#x} -> game {gaddr:#010x}")
        return True, f"{kind} {rel.sym}+{addend:#x}: game {game.label(gaddr)} (section-relative: not checkable here)"
    base = game.addr(rel.sym)
    if base is None:
        res.unchecked.append(f"+{w:#x} {kind} {rel.sym}+{addend:#x} -> game {gaddr:#010x}")
        return True, f"{kind} {rel.sym}+{addend:#x}: game {game.label(gaddr)} (symbol address unknown)"
    want = (base + addend) & 0xFFFFFFFF
    sym = rel.sym + (f"+{addend:#x}" if addend else "")
    if want == gaddr:
        return True, f"{kind} {sym} = {want:#010x}: ok"
    return False, f"{kind} {sym} = {want:#010x}, game uses {game.label(gaddr)}"


def report(res: FuncResult, context: int = 3, full: bool = False) -> str:
    """Text report: status line, then the words around each difference (or every word with --full)."""
    gs = f"{res.game_size:#x}" if res.game_size is not None else "unknown"
    head = f"{res.name} at {res.addr:#010x}: ours {res.our_size:#x} bytes, game {gs}"
    if res.match:
        lines = [f"MATCH  {head}"]
    else:
        fd = res.first_diff
        where = f"first difference at +{fd.off:#x}" if fd else "size differs"
        ndiff = sum(not w.ok for w in res.words)
        lines = [f"DIFF   {head}: {where}, {ndiff} word(s) differ"]
    show = set()
    for i, w in enumerate(res.words):
        if full or not w.ok:
            show.update(range(max(0, i - context), min(len(res.words), i + context + 1)))
        if w.note and (full or not w.ok):
            show.add(i)
    last = -2
    for i in sorted(show):
        w = res.words[i]
        if i != last + 1 and last >= 0:
            lines.append("       ...")
        last = i
        mark = "  " if w.ok else "!!"
        ours = f"{w.ours:08x} {disasm(w.ours, res.addr + w.off):<34}" if w.ours is not None else " " * 43
        game = f"{w.game:08x} {disasm(w.game, res.addr + w.off)}" if w.game is not None else ""
        lines.append(f"  {mark} +{w.off:04x}  {ours} | {game}")
        if w.note:
            lines.append(f"             {w.note}")
    if res.unchecked:
        lines.append("  not checkable here (judged by make regress): " + "; ".join(res.unchecked))
    return "\n".join(lines)
