"""ER-020: count the EE-specific opcodes Eternal Ring's main ELF uses, per opcode and per region.

Reads the code bytes of every function symbol in build/SLUS_200.15.syms.elf (byte-identical to the original, from
ER-003), decodes them with rabbitizer (R5900 mode) and counts, per opcode in the families that a recompiler is most
likely to get wrong:

  mmi    major opcode 0x1C (MMI, MMI0-3: parallel ops, madd / mult1 / div1 / plzcw / pmfhl ...)
  ee     R5900 additions outside MMI: lq / sq, mfsa / mtsa / mtsab / mtsah, three-operand mult / multu
  cop2   major 0x12 (VU0 macro mode and qmfc2 / qmtc2 / cfc2 / ctc2 / bc2*) plus lqc2 / sqc2
  cop1   major 0x11 (FPU) plus lwc1 / swc1
  cop0   major 0x10 (only the branches bc0f / bc0t / bc0fl / bc0tl and eret / ei / di / sync are of interest)
  shiftv sllv / srlv / srav and dsllv / dsrlv / dsrav (variable shifts; on the open-bug list as "variable word shifts")

For each opcode: sites, functions, sites and functions in game vs SDK regions (regions from tools/census.py, ER-004).
Also reports branches whose delay slot lies past the function's end (another open upstream bug class).

With --cfg (a PS2Recomp config TOML, read only), sites inside functions bound to HLE stubs (`stubs = [...]`) are
counted apart: their bodies are not run as recompiled code, so only the stub's own C++ matters there.
Also counts VU0 macro reads of vf0.w (the constant 1.0), which depend on vf0 being pinned to (0, 0, 0, 1).

Output: a markdown table on stdout (--md) and build/recomp/opcode_sites.csv (addresses only, gitignored).
No game code is written anywhere: only opcode names, counts and addresses.

    .venv/Scripts/python.exe tools/recomp/opcode_scan.py --md --cfg C:/rs/recomp/work/cfg_split.toml

Note: rabbitizer 1.16.2 names the EE `sqrt.s` (COP1 fmt S, funct 4) only "c1"; it is renamed here.
"""
import argparse
import collections
import csv
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from elfutil import Elf  # noqa: E402
from census import REGIONS, DATA_BLOCKS, region_of  # noqa: E402

import rabbitizer as rz  # noqa: E402

SYMS_ELF = os.path.join(ROOT, "build", "SLUS_200.15.syms.elf")
OUT_DIR = os.path.join(ROOT, "build", "recomp")

EE_EXTRA = {"lq", "sq", "mfsa", "mtsa", "mtsab", "mtsah"}
SHIFTV = {"sllv", "srlv", "srav", "dsllv", "dsrlv", "dsrav"}


def family(w: int, op: str, ins) -> str | None:
    major = w >> 26
    if major == 0x1C:
        return "mmi"
    if major == 0x12 or op in ("lqc2", "sqc2"):
        return "cop2"
    if major == 0x11 or op in ("lwc1", "swc1"):
        return "cop1"
    if major == 0x10:
        return "cop0"
    if op in EE_EXTRA:
        return "ee"
    if op in ("mult", "multu") and (w >> 11) & 31:   # R5900 three-operand form writes rd too
        return "ee"
    if op in SHIFTV:
        return "shiftv"
    return None


def load_stubs(path: str) -> dict:
    """Addresses bound to HLE stubs in a PS2Recomp config: {addr: name}."""
    txt = open(path, encoding="utf-8").read()
    m = re.search(r"^stubs\s*=\s*\[(.*?)^\]", txt, re.S | re.M)
    if not m:
        return {}
    return {int(a, 16): n for n, a in re.findall(r'"([^"@]+)@0x([0-9A-Fa-f]+)"', m.group(1))}


def reads_vf0w(op: str, w: int) -> bool:
    """True when a VU0 macro instruction reads the w lane of vf0 (the constant 1.0)."""
    ft, fs, dest = (w >> 16) & 31, (w >> 11) & 31, (w >> 21) & 15
    fsf, ftf = (w >> 21) & 3, (w >> 23) & 3
    if op in ("vdiv", "vrsqrt"):
        return (fs == 0 and fsf == 3) or (ft == 0 and ftf == 3)
    if op == "vsqrt":
        return ft == 0 and ftf == 3
    if op == "sqc2":
        return ft == 0
    if not op.startswith("v") or op in ("vnop", "vwaitq", "vcallms", "vcallmsr", "vlqi", "vsqi"):
        return False
    base = op[1:]
    if base[-1] in "xyzw" and base[:-1] in ("add", "sub", "mul", "madd", "msub", "max", "mini",
                                            "adda", "suba", "mula", "madda", "msuba"):
        return (ft == 0 and base[-1] == "w") or (fs == 0 and dest & 1 == 1)
    if base in ("add", "sub", "mul", "madd", "msub", "max", "mini", "adda", "suba", "mula", "madda", "msuba"):
        return (fs == 0 or ft == 0) and dest & 1 == 1
    return fs == 0 and dest & 1 == 1      # q / i forms, moves and conversions read only fs


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--md", action="store_true", help="print the markdown tables")
    ap.add_argument("--elf", default=SYMS_ELF)
    ap.add_argument("--cfg", help="PS2Recomp config TOML: count sites in HLE-stubbed functions apart")
    args = ap.parse_args()
    stubs = load_stubs(args.cfg) if args.cfg else {}
    if not os.path.exists(args.elf):
        print("opcode_scan: missing %s (make build/SLUS_200.15.syms.elf)" % args.elf)
        return 1
    elf = Elf(open(args.elf, "rb").read())
    syms = elf.symbols()
    funcs = sorted({(s.value, s.size, s.name) for s in syms if s.size and (s.type == 2 or s.name in DATA_BLOCKS)})

    sites = collections.defaultdict(list)          # (family, op) -> [(addr, func, region, kind)]
    invalid = []                                   # (addr, func, word)
    slot_past_end = []                             # (addr, func, op)
    for a, sz, nm in funcs:
        raw = elf.load_bytes(a, sz)
        words = struct.unpack("<%dI" % (sz // 4), raw)
        reg = region_of(a)
        for i, w in enumerate(words):
            pc = a + i * 4
            ins = rz.Instruction(w, pc, rz.InstrCategory.R5900)
            op = ins.getOpcodeName()
            t = ins.instrIdType.name
            if not ins.isValid() or t in ("R5900_INVALID", "CPU_INVALID"):
                invalid.append((pc, nm, w))
                continue
            if (ins.isBranch() or ins.isJump()) and i == len(words) - 1:
                slot_past_end.append((pc, nm, op))
            fam = family(w, op, ins)
            if fam is None:
                continue
            if op in ("mult", "multu") and fam == "ee":
                op += " (3-op)"
            if op == "c1" and (w >> 21) & 31 == 16 and w & 63 == 4:
                op = "sqrt.s"
            sites[(fam, op)].append((pc, nm, reg[1], reg[2], a in stubs, fam == "cop2" and reads_vf0w(op, w)))

    os.makedirs(OUT_DIR, exist_ok=True)
    with open(os.path.join(OUT_DIR, "opcode_sites.csv"), "w", newline="", encoding="utf-8") as f:
        wr = csv.writer(f, lineterminator="\n")
        wr.writerow(["family", "opcode", "addr", "function", "region", "region_kind", "hle_stub", "reads_vf0w"])
        for (fam, op), lst in sorted(sites.items()):
            for pc, nm, rg, rk, stb, v0 in lst:
                wr.writerow([fam, op, "0x%08X" % pc, nm, rg, rk, int(stb), int(v0)])
        for pc, nm, w in invalid:
            wr.writerow(["undecodable", "0x%08X" % w, "0x%08X" % pc, nm, region_of(pc)[1], region_of(pc)[2]])
        for pc, nm, op in slot_past_end:
            wr.writerow(["slot-past-end", op, "0x%08X" % pc, nm, region_of(pc)[1], region_of(pc)[2]])

    if args.md:
        order = ["mmi", "ee", "shiftv", "cop2", "cop1", "cop0"]
        for fam in order:
            rows = [(op, lst) for (f, op), lst in sites.items() if f == fam]
            if not rows:
                continue
            rows.sort(key=lambda r: (-len(r[1]), r[0]))
            tot = sum(len(lst) for _, lst in rows)
            print("\n### %s: %d opcodes, %d sites, %d functions\n" % (
                fam, len(rows), tot, len({x[1] for _, lst in rows for x in lst})))
            print("| Opcode | Sites | Functions | Game sites / funcs | SDK sites / funcs | In HLE stubs (sites / funcs) "
                  "| Regions (sites) |")
            print("|---|---|---|---|---|---|---|")
            for op, lst in rows:
                g = [x for x in lst if x[3] == "game"]
                sd = [x for x in lst if x[3] != "game"]
                hs = [x for x in lst if x[4]]
                regs = collections.Counter(x[2] for x in lst)
                rs = ", ".join("%s %d" % (k, v) for k, v in sorted(regs.items(), key=lambda kv: -kv[1]))
                print("| `%s` | %d | %d | %d / %d | %d / %d | %s | %s |" % (
                    op, len(lst), len({x[1] for x in lst}), len(g), len({x[1] for x in g}),
                    len(sd), len({x[1] for x in sd}),
                    ("%d / %d" % (len(hs), len({x[1] for x in hs}))) if args.cfg else "-", rs))
        v0 = [x for lst in sites.values() for x in lst if x[5]]
        v0r = [x for x in v0 if not x[4]]
        print("\nVU0 reads of vf0.w: %d sites in %d functions (game %d / %d); outside HLE stubs %d sites in %d functions" % (
            len(v0), len({x[1] for x in v0}), sum(1 for x in v0 if x[3] == "game"),
            len({x[1] for x in v0 if x[3] == "game"}), len(v0r), len({x[1] for x in v0r})))
        print("\nUndecodable words: %d in %s" % (len(invalid), sorted({x[1] for x in invalid})))
        print("Branch / jump as the last word of a function (delay slot past the end): %d %s" % (
            len(slot_past_end), [("0x%08X" % p, n, o) for p, n, o in slot_past_end][:20]))
        print("Functions scanned: %d" % len(funcs))
    return 0


if __name__ == "__main__":
    sys.exit(main())
