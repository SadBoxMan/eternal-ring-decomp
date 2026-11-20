#!/usr/bin/env python3
"""ER-015: clone tool on the census's strict shape keys (tools/census.py).

  python -I tools/clones.py func_00150810            list its strict family (same code at other addresses)
  python -I tools/clones.py func_00150810 --try      take its placed C, retarget it to each member, compile, compare
  python -I tools/clones.py func_00150810 --try --place     also place every member that matched (tools/place.py c)
  python -I tools/clones.py --matched                every family that holds a placed C function and members still asm

Strict key (census): the instruction words with address parts masked (jal / j targets, lui of image addresses, the
offsets of $gp and tracked address registers). Equal keys = the same code, maybe calling other functions and using
other globals. `--try` compiles the placed function once, reads its relocations, and for each member reads the game's
words at the same offsets: a jal / j target, a %hi / %lo address or a $gp address names the member's symbol (a real
symbol of the build if there is one at that address, else func_ / D_ + address). Those names replace the original's
in the C text (whole words; extern lines too), the function is renamed, and the result is compiled and compared with
tools/cmpfunc.py's compare. .lit4 literals are compared by value: a member whose literal differs is reported (edit its
C by hand). Results go to build/scratch_in/clones/<member>.c.

A masked MATCH here is a working aid: placed members need `make regress` (ruling docs/decomp_recipes.md 3.1).
Needs build/SLUS_200.15.syms.elf (any `make regress` leaves it). Usage notes: docs/matching_tools.md.
"""
import argparse
import collections
import csv
import re
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import eematch  # noqa: E402
import place  # noqa: E402

ROOT = eematch.ROOT
CENSUS = ROOT / "build" / "census" / "functions.csv"
OUT = ROOT / "build" / "scratch_in" / "clones"


def load_census(refresh: bool = True) -> list[dict]:
    if refresh or not CENSUS.exists():
        r = subprocess.run([sys.executable, "-I", str(ROOT / "tools" / "census.py")], cwd=ROOT,
                           capture_output=True, text=True)
        if r.returncode != 0:
            raise SystemExit(f"clones: census failed:\n{r.stdout}{r.stderr}")
    with CENSUS.open(encoding="utf-8") as fh:
        return list(csv.DictReader(fh))


def family(rows: list[dict], name: str) -> list[dict]:
    me = next((r for r in rows if r["name"] == name), None)
    if me is None:
        raise SystemExit(f"clones: {name} is not in the census")
    return [r for r in rows if r["strict"] == me["strict"]]


def sym_name(game: eematch.Game, addr: int, code: bool) -> tuple[str, bool]:
    """Name for an address: a symbol of the build there, else func_/D_ + address. Second value: is a real symbol."""
    n = game.name_at.get(addr)
    if n:
        return n, True
    return (f"func_{addr:08X}" if code else f"D_{addr:08X}"), False


def retarget(obj: eematch.Obj, game: eematch.Game, name: str, member: int) -> tuple[dict, list[str]]:
    """Symbol renames that turn `name`'s code into the member's, from the game words at the member."""
    funcs = {n: (o, s) for n, o, s in obj.functions()}
    off, size = funcs[name]
    text = obj.section_bytes(".text")
    lit4 = obj.section_bytes(".lit4") if ".lit4" in obj.secidx else b""
    relocs = obj.relocs(".text")
    gb = game.bytes_at(member, size)
    renames: dict[str, str] = {}
    notes: list[str] = []
    pending_hi: dict[str, list[int]] = collections.defaultdict(list)

    def add(old: str, new: str, real: bool, where: int):
        if renames.get(old, new) != new:
            notes.append(f"+{where:#x}: {old} would be both {renames[old]} and {new}: conflict")
            return
        if old != new:
            renames[old] = new
            if not real:
                notes.append(f"+{where:#x}: {new} is not a symbol of the build (link would fail)")

    for w in range(0, size, 4):
        rel = relocs.get(off + w)
        if rel is None:
            continue
        a = struct.unpack_from("<I", text, off + w)[0]
        b = struct.unpack_from("<I", gb, w)[0]
        if rel.type == eematch.R_MIPS_26 and not rel.is_section:
            t = ((member + w) & 0xF0000000) | ((b & 0x03FFFFFF) << 2)
            add(rel.sym, *sym_name(game, t, True), w)
        elif rel.type == eematch.R_MIPS_HI16:
            pending_hi[rel.sym].append(w)
        elif rel.type == eematch.R_MIPS_LO16 and not rel.is_section:
            his = pending_hi.pop(rel.sym, [])
            if not his:
                continue
            our_hi = struct.unpack_from("<I", text, off + his[-1])[0] & 0xFFFF
            g_hi = struct.unpack_from("<I", gb, his[-1])[0] & 0xFFFF
            addend = (our_hi << 16) + eematch.s16(a & 0xFFFF)   # unrelocated: the addend on the symbol
            gaddr = ((g_hi << 16) + eematch.s16(b & 0xFFFF)) & 0xFFFFFFFF
            add(rel.sym, *sym_name(game, gaddr - addend, False), w)
        elif rel.type in (eematch.R_MIPS_GPREL16, eematch.R_MIPS_LITERAL):
            gaddr = (eematch.GP + eematch.s16(b & 0xFFFF)) & 0xFFFFFFFF
            if rel.is_section and rel.sym == ".lit4":
                lo = eematch.s16(a & 0xFFFF) + obj.gp0
                if lit4[lo:lo + 4] != game.bytes_at(gaddr, 4):
                    notes.append(f"+{w:#x}: float literal differs: ours {eematch._f32(lit4[lo:lo + 4])}, member "
                                 f"{eematch._f32(game.bytes_at(gaddr, 4))} (edit the C)")
            elif not rel.is_section:
                add(rel.sym, *sym_name(game, gaddr - eematch.s16(a & 0xFFFF), False), w)
            else:
                notes.append(f"+{w:#x}: {rel.sym}-relative $gp access: not retargeted")
        elif rel.is_section:
            notes.append(f"+{w:#x}: {eematch.RELOC_NAMES.get(rel.type, rel.type)} {rel.sym}-relative: not retargeted "
                         "(jump table / string: needs the member's own data)")
    return renames, notes


def apply_renames(text: str, renames: dict[str, str]) -> str:
    if not renames:
        return text
    pat = re.compile(r"\b(" + "|".join(map(re.escape, sorted(renames, key=len, reverse=True))) + r")\b")
    return pat.sub(lambda m: renames[m[1]], text)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("name", nargs="?")
    ap.add_argument("--try", dest="try_", action="store_true")
    ap.add_argument("--place", action="store_true", help="with --try: place each member that matched")
    ap.add_argument("--matched", action="store_true", help="list families with a placed member and asm members")
    ap.add_argument("--no-refresh", action="store_true", help="use the existing build/census/functions.csv")
    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)
    rows = load_census(not args.no_refresh)

    if args.matched:
        fams = collections.defaultdict(list)
        for r in rows:
            fams[r["strict"]].append(r)
        for k, mem in sorted(fams.items(), key=lambda kv: -len(kv[1])):
            c = [r["name"] for r in mem if r["category"] == "matched"]
            if c and len(c) < len(mem):
                print(f"{k}: {len(mem)} members, {len(c)} C ({', '.join(c[:3])}), size {mem[0]['size']}")
        return 0
    if not args.name:
        ap.error("name a function (or --matched)")

    fam = family(rows, args.name)
    print(f"{args.name}: strict key {fam[0]['strict']}, {len(fam)} member(s)")
    for r in fam:
        loc = place.locate(r["name"])
        where = f"{loc[0]:3} {loc[1].relative_to(ROOT).as_posix()}" if loc else "not in src/ (crt0?)"
        print(f"  {r['name']}  size {int(r['size']):#6x}  {r['category']:<16} {where}")
    if not args.try_:
        return 0

    loc = place.locate(args.name)
    if not loc or loc[0] != "c":
        print(f"clones: {args.name} is not C in src/: --try needs a placed function")
        return 1
    OUT.mkdir(parents=True, exist_ok=True)
    base_c = OUT / f"_{args.name}.c"
    place.cmd_extract(args.name, base_c)
    base_text = base_c.read_text(encoding="utf-8")
    obj = eematch.Obj(eematch.compile_c(base_c))
    game = eematch.Game()
    if not game.have_syms:
        print("clones: build/SLUS_200.15.syms.elf missing (make build/SLUS_200.15.syms.elf)")
        return 1
    matched = []
    for r in fam:
        m = r["name"]
        if m == args.name:
            continue
        mloc = place.locate(m)
        if mloc and mloc[0] == "c":
            print(f"  {m}: already C")
            continue
        renames, notes = retarget(obj, game, args.name, int(r["addr"], 16))
        renames[args.name] = m
        out = OUT / f"{m}.c"
        out.write_bytes(apply_renames(base_text, renames).encode("utf-8"))
        try:
            res = eematch.compare_function(eematch.Obj(eematch.compile_c(out)), game, m)
        except SystemExit as e:
            print(f"  {m}: compile failed: {str(e).splitlines()[-1] if str(e) else ''}")
            continue
        status = "MATCH" if res.match else f"DIFF at +{res.first_diff.off:#x}" if res.first_diff else "DIFF (size)"
        ren = ", ".join(f"{k}->{v}" for k, v in renames.items() if k != args.name) or "no renames"
        print(f"  {m}: {status} ({ren}) -> {out.relative_to(ROOT).as_posix()}")
        for n in notes:
            print(f"      {n}")
        if res.match:
            matched.append((m, out))
    print(f"clones: {len(matched)} member(s) match with retargeted C")
    if args.place:
        for m, out in matched:
            place.cmd_c(m, out, False)
        if matched:
            print("clones: placed; now run make regress")
    return 0


if __name__ == "__main__":
    sys.exit(main())
