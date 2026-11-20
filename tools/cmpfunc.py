#!/usr/bin/env python3
"""ER-015: compile C with the build's ee-gcc settings and compare functions with the original SLUS_200.15.

  python -I tools/cmpfunc.py scratch/func_0014D918.c                 every function in the file
  python -I tools/cmpfunc.py src/text_0014C840.c func_0014D918        one function of a src/ file (real make rule)
  python -I tools/cmpfunc.py my.c my_name --addr 0x0014D918           a function not named after its address
  options: --full (every word, with relocation notes), --context N (words around a difference, default 3),
           --cflags "-O1" (extra gcc flags, scratch compile only: never in the build without a ruling)

The report shows ours | game side by side (R5900 disassembly by rabbitizer), marks each differing word with !!,
and notes every relocation: the game's jal / j target, %hi / %lo address, $gp address and the .lit4 value, each
checked against the symbol's address (func_ / D_ names, or names from build/SLUS_200.15.syms.elf). Targets that
are section-relative (.rodata jump tables, strings) cannot be checked by this tool and are listed. Exit 0 when every
compared function matches, 1 when one differs, 2 on a compile error.

Not proof (ruling docs/decomp_recipes.md 3.1): place the function (tools/place.py) and run `make regress`.
Usage notes: docs/matching_tools.md.
"""
import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import eematch  # noqa: E402


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("c_file")
    ap.add_argument("funcs", nargs="*", help="function names (default: every func_XXXXXXXX of a scratch file)")
    ap.add_argument("--addr", help="game address for a single function not named func_XXXXXXXX")
    ap.add_argument("--full", action="store_true")
    ap.add_argument("--context", type=int, default=3)
    ap.add_argument("--cflags", default="", help="extra gcc flags (scratch compile only)")
    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)

    c_path = Path(args.c_file)
    if not c_path.exists():
        print(f"cmpfunc: no such file: {c_path}", file=sys.stderr)
        return 2
    in_src = c_path.resolve().is_relative_to(eematch.ROOT / "src")
    if in_src and not args.funcs:
        print("cmpfunc: name the function(s) to compare in a src/ file", file=sys.stderr)
        return 2
    try:
        obj = eematch.Obj(eematch.compile_c(c_path, args.cflags.split() or None))
    except SystemExit as e:
        print(e, file=sys.stderr)
        return 2
    game = eematch.Game()
    if not game.have_syms:
        print(f"cmpfunc: note: {eematch.SYMS_ELF.relative_to(eematch.ROOT)} missing: game sizes and named symbols "
              "unknown (make build/SLUS_200.15.syms.elf)")
    names = args.funcs or [n for n, _o, _s in obj.functions() if eematch.ADDR_NAME.match(n)]
    if not names:
        print("cmpfunc: no func_XXXXXXXX function in the object; name one and pass --addr", file=sys.stderr)
        return 2
    if args.addr and len(names) != 1:
        print("cmpfunc: --addr needs exactly one function name", file=sys.stderr)
        return 2
    all_ok = True
    for n in names:
        res = eematch.compare_function(obj, game, n, int(args.addr, 16) if args.addr else None)
        all_ok &= res.match
        print(eematch.report(res, args.context, args.full))
    return 0 if all_ok else 1


if __name__ == "__main__":
    sys.exit(main())
