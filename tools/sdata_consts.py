"""ER-101: census of the game's small $gp floats (.sdata / .sbss) that no code ever stores to.

Scans every instruction word of the original ELF's .text (all functions, placed C and asm alike, SDK included) for
accesses to .sdata / .sbss:

- $gp-relative: any load / store / address computation with base register $gp (28), address gp + simm16
  (gp = 0x00360B70, the yaml's gp_value; checked against the build's _gp symbol when it has one);
- absolute: `lui rX, hi` followed in the same function by a load / store / addiu with base rX whose
  (hi << 16) + simm16 falls in .sdata / .sbss (the last lui per register; conservative: a stale pairing can only add
  accesses, never hide one);
- data: every aligned word of the loaded data sections (.data, .vudata, .rodata, .lit4, .sdata) whose value points
  into .sdata / .sbss (an address kept in a table).

A symbol is a **const float** when an `lwc1` reads it, no store of any width covers any of its bytes, its address is
never computed (addiu / daddiu / ori from $gp or an absolute pair) and no data word points into it, and no
address-taken symbol lies before it in the same section within 0x40 bytes (a pointer to a neighbour could reach it
through an offset; the window is a guess, Unconfirmed). Symbols that fail only on that last test are listed apart.

`m2c_bulk.py --plain` uses the list (const_floats()) for its `extern const` variant (docs/matching_tools.md 6.10).
Reading (Unconfirmed): such values were `const` tuning constants in the original source (recipes 2.21). The list
proves only that no store exists in the shipped code, not that the original declared them const.

Usage: python -I tools/sdata_consts.py [--all] [--names D_X ...]     summary; --all lists every float symbol
Prints names and counts only (no values: game data stays out of the repo and the log).
"""
from __future__ import annotations

import argparse
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import eematch  # noqa: E402
from elfutil import Elf  # noqa: E402

GP = 0x00360B70
LOADS = {0x20: 1, 0x21: 2, 0x23: 4, 0x24: 1, 0x25: 2, 0x27: 4, 0x37: 8, 0x1E: 16, 0x31: 4, 0x35: 8, 0x36: 16,
         0x22: 4, 0x26: 4, 0x1A: 8, 0x1B: 8}                  # lb lh lw lbu lhu lwu ld lq lwc1 ldc1 lqc2 lwl lwr ldl ldr
STORES = {0x28: 1, 0x29: 2, 0x2B: 4, 0x3F: 8, 0x1F: 16, 0x39: 4, 0x3D: 8, 0x3E: 16, 0x2A: 4, 0x2E: 4, 0x2C: 8,
          0x2D: 8}                                            # sb sh sw sd sq swc1 sdc1 sqc2 swl swr sdl sdr
ADDRS = {0x09, 0x19, 0x0D, 0x08, 0x18}                        # addiu daddiu ori addi daddi
LWC1 = 0x31
NEAR = 0x40


@dataclass
class SymInfo:
    name: str
    addr: int
    size: int
    section: str
    lwc1: int = 0
    loads: int = 0
    stores: list[str] = field(default_factory=list)      # "func_X (gp sw)" ...
    addr_taken: list[str] = field(default_factory=list)


def s16(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


def census() -> tuple[dict[str, SymInfo], dict]:
    game = eematch.Game()
    syms_elf = Elf(eematch.SYMS_ELF.read_bytes())
    secs = {s.name: s for s in syms_elf.sections}
    ranges = [(n, secs[n].addr, secs[n].addr + secs[n].size) for n in (".sdata", ".sbss") if n in secs]
    gp = GP
    for s in syms_elf.symbols():
        if s.name == "_gp":
            gp = s.value

    def section_of(a: int) -> str | None:
        for n, lo, hi in ranges:
            if lo <= a < hi:
                return n
        return None

    # symbols of .sdata / .sbss (address-named objects of the build), sorted
    objs = sorted({s.value: s for s in syms_elf.symbols()
                   if s.name and s.type in (0, 1) and section_of(s.value) and not s.name.startswith(".")}.values(),
                  key=lambda s: s.value)
    infos: dict[int, SymInfo] = {}
    for i, s in enumerate(objs):
        nxt = objs[i + 1].value if i + 1 < len(objs) else s.value + 4
        size = s.size or min(nxt - s.value, 8)
        infos[s.value] = SymInfo(s.name, s.value, size, section_of(s.value))
    starts = sorted(infos)

    def owner(a: int) -> SymInfo | None:
        import bisect
        k = bisect.bisect_right(starts, a) - 1
        if k < 0:
            return None
        si = infos[starts[k]]
        return si if a < si.addr + max(si.size, 1) else None

    stats = {"gp words": 0, "abs words": 0, "data pointers": 0, "gp R-type uses": 0}
    text = secs[".text"]
    funcs = sorted((a, sz) for a, sz in game.size_of.items() if text.addr <= a < text.addr + text.size)

    def touch(a: int, op: int, kind: str, fname: str):
        if op in STORES:
            for b in range(a, a + STORES[op]):
                si = owner(b)
                if si and fname not in si.stores:
                    si.stores.append(fname)
        elif op in LOADS:
            si = owner(a)
            if si:
                si.loads += 1
                si.lwc1 += op == LWC1
        elif op in ADDRS:
            si = owner(a)
            if si:
                si.addr_taken.append(f"{fname} ({kind})")

    for fa, fsz in funcs:
        fname = game.name_at.get(fa, f"func_{fa:08X}")
        hi: dict[int, int] = {}
        for off in range(0, fsz, 4):
            w = game.word(fa + off)
            op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
            if op == 0 and 28 in (rs, rt) and (w & 0x3F) in (0x21, 0x2D, 0x20, 0x2C) and ((w >> 11) & 31) != 28:
                stats["gp R-type uses"] += 1           # addu / daddu with $gp: an address computed another way
            if op == 0x0F:                             # lui
                hi[rt] = imm << 16
                continue
            if op not in LOADS and op not in STORES and op not in ADDRS:
                continue
            if rs == 28:
                a = (gp + s16(imm)) & 0xFFFFFFFF
                if section_of(a):
                    stats["gp words"] += 1
                    touch(a, op, "gp", fname)
            elif rs in hi:
                a = (hi[rs] + s16(imm)) & 0xFFFFFFFF
                if section_of(a):
                    stats["abs words"] += 1
                    touch(a, op, "abs", fname)
            if op in LOADS or op in ADDRS:             # the destination register no longer holds the lui value
                if op not in (0x31, 0x35, 0x36) and rt in hi:
                    del hi[rt]
    for n in (".data", ".vudata", ".rodata", ".lit4", ".sdata"):
        s = secs.get(n)
        if not s:
            continue
        raw = game.bytes_at(s.addr, s.size) or b""
        for k in range(0, len(raw) - 3, 4):
            v = struct.unpack_from("<I", raw, k)[0]
            if section_of(v) and (si := owner(v)):
                si.addr_taken.append(f"data word at 0x{s.addr + k:08X}")
                stats["data pointers"] += 1
    stats["gp"] = f"0x{gp:08X}"
    return {si.name: si for si in infos.values()}, stats


def classify(infos: dict[str, SymInfo]) -> tuple[list[str], list[str], list[str]]:
    """(const floats, floats never stored but near an address-taken symbol, stored floats)"""
    taken = sorted(si.addr for si in infos.values() if si.addr_taken)
    const, near, stored = [], [], []
    for si in sorted(infos.values(), key=lambda s: s.addr):
        if not si.lwc1:
            continue
        if si.stores or si.addr_taken:
            stored.append(si.name)
        elif any(si.addr - NEAR <= t < si.addr for t in taken
                 if next(x for x in infos.values() if x.addr == t).section == si.section):
            near.append(si.name)
        else:
            const.append(si.name)
    return const, near, stored


_CACHE: set[str] | None = None


def const_floats() -> set[str]:
    """Names of the const floats (cached per process)."""
    global _CACHE
    if _CACHE is None:
        _CACHE = set(classify(census()[0])[0])
    return _CACHE


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--all", action="store_true", help="list every float symbol with its class")
    ap.add_argument("--names", nargs="*", default=[], help="show these symbols' accesses")
    args = ap.parse_args()
    infos, stats = census()
    const, near, stored = classify(infos)
    floats = [si for si in infos.values() if si.lwc1]
    print(f"sdata_consts: gp {stats['gp']}; {len(infos)} .sdata / .sbss symbols, {len(floats)} read by lwc1; "
          f"{stats['gp words']} $gp-relative and {stats['abs words']} absolute instruction accesses, "
          f"{stats['data pointers']} data words pointing in, {stats['gp R-type uses']} R-type $gp uses")
    for sec in (".sdata", ".sbss"):
        c = sum(infos[n].section == sec for n in const)
        print(f"  {sec}: {sum(si.section == sec for si in floats)} floats: {c} const, "
              f"{sum(infos[n].section == sec for n in near)} never stored but near an address-taken symbol, "
              f"{sum(infos[n].section == sec for n in stored)} stored or address-taken")
    if args.all:
        for n in const:
            print(f"  const  {n}  loads {infos[n].loads}")
        for n in near:
            print(f"  near   {n}  loads {infos[n].loads}")
        for n in stored:
            si = infos[n]
            print(f"  stored {n}  by {', '.join(si.stores[:4])}{' ...' if len(si.stores) > 4 else ''}"
                  f"{'  address: ' + ', '.join(si.addr_taken[:2]) if si.addr_taken else ''}")
    for n in args.names:
        si = infos.get(n)
        if not si:
            print(f"  {n}: not a .sdata / .sbss symbol of the build")
            continue
        kind = "const" if n in const else "near" if n in near else "stored" if n in stored else "not an lwc1 float"
        print(f"  {n} ({si.section}, {si.size} B): {kind}; loads {si.loads} (lwc1 {si.lwc1}); stores by "
              f"{', '.join(si.stores) or 'none'}; address taken: {', '.join(si.addr_taken) or 'no'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
