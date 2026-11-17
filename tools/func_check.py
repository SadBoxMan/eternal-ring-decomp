#!/usr/bin/env python3
"""ER-003: per-function byte check of a build against the original SLUS_200.15.

Reads the function symbols (STT_FUNC with a size) of the unstripped build (build/SLUS_200.15.syms.elf) and
compares each function's bytes with the original ELF at the same address. Also checks that every symbol
named func_XXXXXXXX / D_XXXXXXXX sits at the address its name says, so a shift shows up at the first
moved symbol even where the bytes happen to agree. Exact compare, nothing masked: a placed C function
counts only if it is byte-identical in the linked image.

Usage: python -I tools/func_check.py <built.syms.elf> <original ELF> [--list N]
Exit 0 when every function matches and no symbol moved; 1 otherwise.
"""
import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from elfutil import Elf  # noqa: E402

ADDR_NAME = re.compile(r"^(?:func|D)_([0-9A-F]{8})$")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("built")
    ap.add_argument("original")
    ap.add_argument("--list", type=int, default=20, help="how many failures to print (default 20)")
    args = ap.parse_args()

    built = Elf(Path(args.built).read_bytes())
    orig = Elf(Path(args.original).read_bytes())
    syms = built.symbols()
    if not syms:
        print(f"{args.built}: no symbol table (link the unstripped ELF)", file=sys.stderr)
        return 1

    funcs = sorted({(s.value, s.size, s.name) for s in syms if s.type == 2 and s.size > 0})
    bad_bytes, moved = [], []
    total_bytes = 0
    for addr, size, name in funcs:
        total_bytes += size
        a, b = built.load_bytes(addr, size), orig.load_bytes(addr, size)
        if a is None or b is None or a != b:
            first = next((i for i in range(min(len(a or b""), len(b or b""))) if a[i] != b[i]), 0)
            bad_bytes.append((name, addr, size, first))
    for s in syms:
        m = ADDR_NAME.match(s.name)
        if m and s.shndx != 0 and int(m[1], 16) != s.value:
            moved.append((s.name, s.value))

    ok = not bad_bytes and not moved
    print(f"func_check: {len(funcs)} functions, {total_bytes:#x} bytes: "
          f"{len(funcs) - len(bad_bytes)} match, {len(bad_bytes)} differ; {len(moved)} address-named symbols moved")
    for name, addr, size, first in bad_bytes[:args.list]:
        print(f"  DIFF  {name} at {addr:#010x} size {size:#x}: first difference at +{first:#x}")
    for name, value in moved[:args.list]:
        print(f"  MOVED {name} is at {value:#010x}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
