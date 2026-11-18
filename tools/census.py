"""ER-004: function census of the main EE ELF (SLUS_200.15).

Reads only generated, gitignored inputs (run `make split` and `make build/SLUS_200.15.syms.elf` first):
  build/SLUS_200.15.syms.elf              function symbols with sizes, data symbols, the code bytes
  src/**/*.c                              which functions are already C (the rest are INCLUDE_ASM)
  asm/nonmatchings/**/*.s                 spimdisasm's "handwritten" marks and `.word` lines
  asm/text_00100000.s                     crt0 (same marks)

For every function it records: size and size band, three shape keys, calls out / in, tail jumps, data and code
references, and the use of MMI, COP1, COP2 (VU0 macro), COP0, 128-bit loads / stores, $gp, jump tables, syscall.

Shape keys (sha1 of the instruction stream, first 12 hex digits):
  strict  instruction words with address parts masked: jal targets, j targets outside the function (inside: made
          relative), lui immediates in the image's address range, and the 16-bit offsets of instructions whose
          base is a tracked address register or $gp. Equal strict keys = the same code at other addresses (clone).
  regs    opcode and registers kept, every immediate / target masked.
  loose   opcode only (registers and immediates masked): near-identical shapes.

Writes everything to build/census/ (gitignored: derived from game code):
  functions.csv   one row per function
  families.csv    strict and loose families with 2+ members
  refs.txt        per function: referenced strings and data symbols (evidence for the SDK / game split)
  summary.json    the totals the doc quotes
The census is read-only: it never writes to src/, asm/ or the yaml.

Usage:  .venv\\Scripts\\python.exe tools/census.py [--md]
  --md  print the markdown tables used in docs/census.md
"""
from __future__ import annotations

import argparse
import collections
import csv
import hashlib
import json
import glob
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from elfutil import Elf  # noqa: E402

import rabbitizer as rz  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SYMS_ELF = os.path.join(ROOT, "build", "SLUS_200.15.syms.elf")
SRC_DIR = os.path.join(ROOT, "src")  # ER-013: one c file per region, src/ and src/sdk/
ASM_DIR = os.path.join(ROOT, "asm", "nonmatchings")
CRT0_ASM = os.path.join(ROOT, "asm", "text_00100000.s")
OUT_DIR = os.path.join(ROOT, "build", "census")

GP = 0x00360B70
IMAGE_LO, IMAGE_HI = 0x00100000, 0x01B7BAA0   # load image + bss (_end)
DATA_BLOCKS = ("func_001D73D0", "func_001D7658", "func_001DECA0")   # labelled as data by splat
# Regions of .text in link order (ER-004; evidence and grades in docs/census.md 4). Each entry: start address,
# region id, kind (crt0 / game / sdk), grade of the region's identity, the window in which its start seam lies
# (first and last candidate start; equal = the seam is fixed by evidence), one line of evidence.
REGIONS = [
    (0x00100000, "crt0", "crt0", "Confirmed", (0x00100000, 0x00100000),
     "SCE crt0 shape: syscalls 0x3C / 0x3D, FlushCache(0), ei, main, Exit"),
    (0x001000C0, "game", "game", "Confirmed", (0x001000C0, 0x001000C0),
     "main; game strings (memory card messages, cdrom0:\\%s;1); refs to game .data"),
    (0x001C9B18, "game-vu0", "game", "Unconfirmed", (0x001C9B18, 0x001C9B18),
     "28 VU0 macro leaf functions; no SDK string; linked before libgraph's .data"),
    (0x001CA0F0, "game-unk", "game", "Unconfirmed", (0x001CA0F0, 0x001CA0F0),
     "no strings, no .rodata; refs game .data (0x0034D1D0); threads via libkernl; game or middleware"),
    (0x001CE130, "libgraph", "sdk", "Confirmed", (0x001CE130, 0x001CE270),
     "sceGs* strings; .data next to PsIIlibgraph1620"),
    (0x001CFD40, "libkernl", "sdk", "Confirmed", (0x001CFAD8, 0x001CFD40),
     "syscall stub table, TTY:, rom0:UDNL; .data after PsIIlibkernl1620"),
    (0x001D46E0, "libdma", "sdk", "Confirmed", (0x001D46E0, 0x001D4718),
     "libdma: strings; .data at PsIIlibdma 1620; lone nop at 0x001D46DC ends the previous object"),
    (0x001D5200, "sdk-vudev", "sdk", "Unconfirmed", (0x001D5200, 0x001D5200),
     "VU / VIF control (cfc2/ctc2 $vi28), jump tables between libdma and libpad .rodata; no version string"),
    (0x001D6280, "libpad", "sdk", "Confirmed", (0x001D5868, 0x001D6280),
     "libpad: strings; .data after PsIIlibpad  1630"),
    (0x001D7310, "libvu0", "sdk", "Unconfirmed", (0x001D7310, 0x001D7310),
     "VU0 macro code (incl. the 3 data blocks), .data between libpad and libcdvd; no version string"),
    (0x001D8050, "libcdvd", "sdk", "Confirmed", (0x001D8050, 0x001D8050),
     "Libcdvd / sceCd strings; .data after PsIIlibcdvd 1650"),
    (0x001DAA88, "libmpeg", "sdk", "Confirmed", (0x001DAA88, 0x001DB2C8),
     "MPEG decoder strings; .data after PsIIlibmpeg 1620"),
    (0x001E2128, "libipu", "sdk", "Unconfirmed", (0x001E1D78, 0x001E2578),
     ".data after PsIIlibipu 1620 (0x001E2578); small IPU helpers called from libmpeg"),
    (0x001E2788, "libsdr", "sdk", "Unconfirmed", (0x001E2788, 0x001E2788),
     ".data after PsIIlibsdr 1620; called from the game's init"),
    (0x001E2D00, "libmc", "sdk", "Confirmed", (0x001E2D00, 0x001E2D00),
     "libmc: strings; .data after PsIIlibmc   1650"),
    (0x001E3DE0, "libm", "sdk", "Confirmed", (0x001E3DE0, 0x001E3DE0),
     "newlib libm wrapper names (acosf, asinf, atan2f, sqrtf); calls __errno"),
    (0x001E6508, "libgcc", "sdk", "Unconfirmed", (0x001E64D8, 0x001E6560),
     "global dtor-list walker; fp-bit unpack / op / pack call pattern (soft double)"),
    (0x001E9A50, "libc", "sdk", "Confirmed", (0x001E9A10, 0x001E9A50),
     "newlib: vfprintf strings, (null), strtod Infinity, _impure_ptr, reentrant syscall wrappers"),
    (0x001F00E8, "libkernl-tail", "sdk", "Unconfirmed", (0x001F00E8, 0x001F00E8),
     "D-cache maintenance (cache / mfc0 TagLo loops), called from libpad; libkernl objects pulled late"),
]


def region_of(addr: int):
    cur = REGIONS[0]
    for reg in REGIONS:
        if addr >= reg[0]:
            cur = reg
    return cur


BANDS = [(2, "empty (jr ra; nop)"), (8, "3-8"), (16, "9-16"), (32, "17-32"), (64, "33-64"), (128, "65-128"),
         (256, "129-256"), (400, "257-400"), (10 ** 9, "401+")]


def band_of(n: int, empty: bool) -> str:
    if empty:
        return BANDS[0][1]
    for hi, name in BANDS[1:]:
        if n <= hi:
            return name
    return BANDS[-1][1]


def h12(items) -> str:
    m = hashlib.sha1()
    for x in items:
        m.update(str(x).encode())
        m.update(b",")
    return m.hexdigest()[:12]


def load_marks():
    """Per function: handwritten instruction count, `.word` count, invalid-instruction count, from the asm."""
    marks = {}
    files = sorted(glob.glob(os.path.join(ASM_DIR, "**", "*.s"), recursive=True))
    for path in files + [CRT0_ASM]:
        cur = None
        with open(path, encoding="utf-8", errors="replace") as fh:
            for line in fh:
                m = re.match(r"\s*(?:glabel|dlabel)\s+(func_[0-9A-F]{8})", line)
                if m:
                    cur = m.group(1)
                    marks.setdefault(cur, [0, 0, 0, False])
                    continue
                if cur is None:
                    continue
                if "handwritten instruction" in line:
                    marks[cur][0] += 1
                if re.search(r"\*/\s*\.word\b", line):
                    marks[cur][1] += 1
                if "invalid instruction" in line:
                    marks[cur][2] += 1
    # "/* Handwritten function */" precedes the nonmatching line of the next function
    for path in files + [CRT0_ASM]:
        with open(path, encoding="utf-8", errors="replace") as fh:
            pending = False
            for line in fh:
                if "/* Handwritten function */" in line:
                    pending = True
                    continue
                m = re.match(r"\s*(?:glabel|dlabel)\s+(func_[0-9A-F]{8})", line)
                if m and pending:
                    marks.setdefault(m.group(1), [0, 0, 0, False])[3] = True
                    pending = False
    return marks


def load_c_status():
    text = "\n".join(open(p, encoding="utf-8").read()
                     for p in sorted(glob.glob(os.path.join(SRC_DIR, "**", "*.c"), recursive=True)))
    asm = set(re.findall(r"INCLUDE_ASM\([^,]+,\s*(func_[0-9A-F]{8})\)", text))
    c = set(re.findall(r"^\s*\w[\w\s\*]*\b(func_[0-9A-F]{8})\s*\([^;{)]*\)\s*\{", text, re.M))
    return asm, c


class Image:
    def __init__(self, elf: Elf):
        self.elf = elf
        self.secs = [s for s in elf.sections if s.addr and s.flags & 2]   # SHF_ALLOC

    def sec_of(self, addr: int) -> str:
        for s in self.secs:
            if s.addr <= addr < s.addr + max(s.size, 1):
                return s.name
        return ""

    def cstring(self, addr: int, limit: int = 160) -> str | None:
        b = self.elf.load_bytes(addr, 1)
        if b is None:
            return None
        out = bytearray()
        a = addr
        while len(out) < limit:
            c = self.elf.load_bytes(a, 1)
            if c is None:
                return None
            c = c[0]
            if c == 0:
                break
            if not (32 <= c < 127 or c in (9, 10, 13)):
                return None
            out.append(c)
            a += 1
        if len(out) < 3:
            return None
        return out.decode("ascii")


def analyse(name, addr, size, img: Image, fstarts, kind):
    raw = img.elf.load_bytes(addr, size)
    words = struct.unpack("<%dI" % (size // 4), raw)
    end = addr + size
    hi = {}        # reg -> address-ish value from lui
    base = {}      # reg -> full address
    strict, regs, loose = [], [], []
    calls, tails, coderefs, datarefs = set(), set(), set(), set()
    gp_uses = 0
    flags = collections.Counter()
    ops = collections.Counter()
    invalid = 0
    syscall_nums = []
    last_v1_imm = None
    for i, w in enumerate(words):
        pc = addr + i * 4
        ins = rz.Instruction(w, pc, rz.InstrCategory.R5900)
        op = ins.getOpcodeName()
        ops[op] += 1
        t = ins.instrIdType.name
        valid = ins.isValid() and t not in ("R5900_INVALID", "CPU_INVALID")
        if not valid:
            invalid += 1
        if t.startswith("R5900_MMI") or op in ("pmfhl", "pmthl"):
            flags["mmi"] += 1
        if "COP1" in t or op in ("lwc1", "swc1", "ldc1", "sdc1"):
            flags["cop1"] += 1
        if "COP2" in t or op in ("lqc2", "sqc2", "qmfc2", "qmtc2", "cfc2", "ctc2"):
            flags["cop2"] += 1
        if "COP0" in t or op in ("ei", "di", "eret", "sync", "sync.l", "sync.p", "cache"):
            flags["cop0"] += 1
        if op in ("lq", "sq"):
            flags["quad"] += 1
        if op in ("mult1", "div1", "divu1", "mfhi1", "mflo1", "mthi1", "mtlo1", "madd1", "maddu1", "multu1"):
            flags["pipe1"] += 1
        if op == "syscall":
            flags["syscall"] += 1
            syscall_nums.append(last_v1_imm)
        major = w >> 26
        mw = w
        dest = None
        if ins.isJType():
            target = ins.getInstrIndexAsVram()
            if op == "jal":
                calls.add(target)
                mw = w & 0xFC000000
            else:
                if addr <= target < end:
                    mw = (w & 0xFC000000) | ((target - addr) >> 2)
                else:
                    tails.add(target)
                    mw = w & 0xFC000000
        elif ins.isIType() and major not in (0x01,) and not ins.isBranch():
            imm = w & 0xFFFF
            simm = imm - 0x10000 if imm & 0x8000 else imm
            rs = (w >> 21) & 31
            rt = (w >> 16) & 31
            if op == "lui":
                val = imm << 16
                if IMAGE_LO <= val < IMAGE_HI + 0x10000:
                    mw = w & 0xFFFF0000
                    hi[rt] = val
                    base.pop(rt, None)
                else:
                    hi.pop(rt, None)
                    base.pop(rt, None)
                dest = None
            else:
                a = None
                if rs == 28:
                    a = GP + simm
                    gp_uses += 1
                elif rs in hi:
                    a = hi[rs] + (imm if op == "ori" else simm)
                elif rs in base:
                    a = base[rs] + simm
                if a is not None:
                    mw = w & 0xFFFF0000
                    sec = img.sec_of(a)
                    if sec in (".text",):
                        coderefs.add(a)
                    elif sec:
                        datarefs.add(a)
                if op in ("addiu", "daddiu", "ori") and a is not None:
                    base[rt] = a
                    hi.pop(rt, None)
                    dest = -1
                elif op == "addiu" and rs == 0 and rt == 3:
                    last_v1_imm = imm
                if ins.doesStore():
                    dest = -1
        elif ins.isBranch():
            off = ins.getBranchOffset()
            tgt = pc + off
            if not (addr <= tgt < end):
                tails.add(tgt)
        if ins.isJrNotRa():
            flags["jr_reg"] += 1
        if op in ("jalr",):
            flags["jalr"] += 1
        if (w >> 21) & 31 == 28 and ins.isRType():
            gp_uses += 1
        # register invalidation for registers written by anything not handled above
        if dest is None:
            try:
                d = ins.getDestinationGpr()
            except Exception:
                d = None
            if d is not None and d >= 0 and op != "lui":
                hi.pop(d, None)
                base.pop(d, None)
            if op in ("daddu", "addu", "or") and ins.isRType():
                rs_, rt_, rd_ = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
                src = rs_ if rt_ == 0 else (rt_ if rs_ == 0 else None)
                if src is not None and rd_:
                    if src in hi:
                        hi[rd_] = hi[src]
                    if src in base:
                        base[rd_] = base[src]
        strict.append(mw)
        if ins.isJType():
            regs.append(w & 0xFC000000)
        elif ins.isIType():
            regs.append(w & 0xFFFF0000)
        else:
            regs.append(w)
        loose.append(ins.uniqueId.name if valid else "INVALID")
    n = len(words)
    empty = n == 2 and words[0] == 0x03E00008 and words[1] == 0
    return {
        "name": name, "addr": addr, "size": size, "n": n, "kind": kind,
        "band": band_of(n, empty), "empty": empty,
        "strict": h12(strict), "regs": h12(regs), "loose": h12(loose),
        "calls": calls, "tails": tails, "coderefs": coderefs, "datarefs": datarefs,
        "gp": gp_uses, "flags": flags, "invalid": invalid, "syscalls": syscall_nums, "ops": ops,
    }


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--md", action="store_true")
    args = ap.parse_args()
    for p in (SYMS_ELF, SRC_DIR, ASM_DIR):
        if not os.path.exists(p):
            print("census: missing %s (run make split and make build/SLUS_200.15.syms.elf)" % p)
            return 1
    elf = Elf(open(SYMS_ELF, "rb").read())
    img = Image(elf)
    syms = elf.symbols()
    funcs = sorted({(s.value, s.size, s.name) for s in syms
                    if s.size and (s.type == 2 or s.name in DATA_BLOCKS)})
    alt = sorted(s.name for s in syms if s.name.startswith("func_") and s.size == 0 and s.shndx == 0xFFF1)
    dsyms = sorted((s.value, s.size, s.name) for s in syms if s.type == 1 and s.name not in DATA_BLOCKS)
    asm_set, c_set = load_c_status()
    marks = load_marks()
    fstarts = {a for a, _, _ in funcs}
    rows = []
    for a, sz, nm in funcs:
        if a < 0x001000C0:
            kind = "crt0"
        elif nm in DATA_BLOCKS:
            kind = "data-block"
        elif nm in c_set:
            kind = "C"
        elif nm in asm_set:
            kind = "asm"
        else:
            kind = "?"
        r = analyse(nm, a, sz, img, fstarts, kind)
        reg = region_of(a)
        r["region"], r["rkind"] = reg[1], reg[2]
        mk = marks.get(nm, [0, 0, 0, False])
        r["hw_instr"], r["words"], r["invalid_marked"], r["hw_func"] = mk
        rows.append(r)
    by_addr = {r["addr"]: r for r in rows}
    callers = collections.defaultdict(set)
    for r in rows:
        for t in r["calls"] | r["tails"]:
            if t in by_addr and t != r["addr"]:
                callers[t].add(r["addr"])
        for t in r["coderefs"]:
            if t in by_addr:
                r.setdefault("ptrs", set()).add(t)
    ptr_taken = collections.Counter()
    for r in rows:
        for t in r.get("ptrs", ()):
            ptr_taken[t] += 1
    # data symbol lookup
    dstarts = [d[0] for d in dsyms]
    import bisect

    def dsym(a):
        i = bisect.bisect_right(dstarts, a) - 1
        if i >= 0:
            v, s, n = dsyms[i]
            if a < v + max(s, 1):
                return n if a == v else "%s+0x%X" % (n, a - v)
        return "0x%08X" % a

    os.makedirs(OUT_DIR, exist_ok=True)
    # strict / loose families
    fam_strict = collections.defaultdict(list)
    fam_loose = collections.defaultdict(list)
    fam_regs = collections.defaultdict(list)
    for r in rows:
        if r["kind"] in ("crt0", "data-block"):
            continue
        fam_strict[r["strict"]].append(r)
        fam_regs[r["regs"]].append(r)
        fam_loose[r["loose"]].append(r)
    for r in rows:
        r["callers"] = len(callers.get(r["addr"], ()))
        r["ptr_taken"] = ptr_taken.get(r["addr"], 0)
        r["fam_strict"] = len(fam_strict.get(r["strict"], [])) or 1
        r["fam_loose"] = len(fam_loose.get(r["loose"], [])) or 1
    assign_categories(rows, fam_strict)
    # functions.csv
    with open(os.path.join(OUT_DIR, "functions.csv"), "w", newline="", encoding="utf-8") as fh:
        w = csv.writer(fh, lineterminator="\n")
        w.writerow(["name", "addr", "size", "instr", "band", "kind", "region", "region_kind", "category", "strict", "regs", "loose", "fam_strict",
                    "fam_loose", "calls_out", "tail_jumps", "callers", "ptr_taken", "jal_targets", "mmi", "cop1",
                    "cop2", "cop0", "quad", "pipe1", "gp", "jr_reg", "jalr", "syscall", "syscall_v1",
                    "hw_func", "hw_instr", "dot_words", "invalid_decode", "data_refs", "code_refs"])
        for r in rows:
            f = r["flags"]
            w.writerow([r["name"], "0x%08X" % r["addr"], r["size"], r["n"], r["band"], r["kind"], r["region"],
                        r["rkind"], r["cat"], r["strict"],
                        r["regs"], r["loose"], r["fam_strict"], r["fam_loose"], len(r["calls"]), len(r["tails"]),
                        r["callers"], r["ptr_taken"],
                        " ".join("%08X" % c for c in sorted(r["calls"])),
                        f["mmi"], f["cop1"], f["cop2"], f["cop0"], f["quad"], f["pipe1"], r["gp"], f["jr_reg"],
                        f["jalr"], f["syscall"], " ".join(str(x) for x in r["syscalls"]), int(r["hw_func"]),
                        r["hw_instr"], r["words"], r["invalid"], len(r["datarefs"]), len(r["coderefs"])])
    with open(os.path.join(OUT_DIR, "refs.txt"), "w", encoding="utf-8") as fh:
        for r in rows:
            strs = []
            others = []
            for a in sorted(r["datarefs"]):
                s = img.cstring(a)
                if s is not None:
                    strs.append("%08X %s %r" % (a, img.sec_of(a), s))
                else:
                    others.append("%s(%s)" % (dsym(a), img.sec_of(a)))
            fh.write("%s %s n=%d calls=%s\n" % (r["name"], r["kind"], r["n"],
                                                 ",".join("%08X" % c for c in sorted(r["calls"]))))
            for s in strs:
                fh.write("  S %s\n" % s)
            if others:
                fh.write("  D %s\n" % " ".join(others))
    with open(os.path.join(OUT_DIR, "families.csv"), "w", newline="", encoding="utf-8") as fh:
        w = csv.writer(fh, lineterminator="\n")
        w.writerow(["key_type", "key", "members", "instr", "representative", "matched_member", "names"])
        for kt, fam in (("strict", fam_strict), ("loose", fam_loose)):
            for k, mem in sorted(fam.items(), key=lambda kv: (-len(kv[1]), kv[1][0]["addr"])):
                if len(mem) < 2:
                    continue
                matched = [m for m in mem if m["kind"] == "C"]
                rep = matched[0] if matched else min(mem, key=lambda m: m["addr"])
                w.writerow([kt, k, len(mem), mem[0]["n"], rep["name"], matched[0]["name"] if matched else "",
                            " ".join(m["name"] for m in mem)])
    summary = summarise(rows, fam_strict, fam_regs, fam_loose, alt)
    with open(os.path.join(OUT_DIR, "summary.json"), "w", encoding="utf-8", newline="\n") as fh:
        json.dump(summary, fh, indent=2)
        fh.write("\n")
    print("census: %d units (%d functions + %d data blocks), %d alternate entries; output in %s"
          % (len(rows), sum(1 for r in rows if r["kind"] != "data-block"),
             sum(1 for r in rows if r["kind"] == "data-block"), len(alt), OUT_DIR))
    if args.md:
        print_md(summary)
    return 0


CATEGORIES = ["matched", "crt0", "handwritten", "clone-of-matched", "clone-copy", "vu0-mmi", "small", "medium",
              "large", "huge"]


def assign_categories(rows, fam_strict):
    """One work category per unit, first rule that applies (docs/census.md 6):
    matched (already C) > crt0 > handwritten (spimdisasm mark) > clone-of-matched (strict shape equals a C
    function's) > clone-copy (strict family member other than its representative, the lowest address) >
    vu0-mmi (any COP2 / VU0 macro or MMI instruction, or a data-labelled block) > size: small <= 32 instr.,
    medium 33-128, large 129-400, huge 401+."""
    for k, mem in fam_strict.items():
        has_c = any(m["kind"] == "C" for m in mem)
        asm_mem = sorted((m for m in mem if m["kind"] != "C"), key=lambda m: m["addr"])
        for i, m in enumerate(asm_mem):
            m["clone"] = "of-matched" if has_c else ("copy" if i > 0 else "")
    for r in rows:
        f = r["flags"]
        if r["kind"] == "C":
            c = "matched"
        elif r["kind"] == "crt0":
            c = "crt0"
        elif r["hw_func"]:
            c = "handwritten"
        elif r.get("clone") == "of-matched":
            c = "clone-of-matched"
        elif r.get("clone") == "copy":
            c = "clone-copy"
        elif f["cop2"] or f["mmi"] or r["kind"] == "data-block":
            c = "vu0-mmi"
        elif r["n"] <= 32:
            c = "small"
        elif r["n"] <= 128:
            c = "medium"
        elif r["n"] <= 400:
            c = "large"
        else:
            c = "huge"
        r["cat"] = c


def summarise(rows, fam_strict, fam_regs, fam_loose, alt):
    s = {"units": len(rows), "alt_entries": alt}
    s["kinds"] = dict(collections.Counter(r["kind"] for r in rows))
    bands = collections.OrderedDict((b, {"funcs": 0, "instr": 0, "asm": 0, "C": 0}) for _, b in BANDS)
    for r in rows:
        if r["kind"] == "data-block":
            continue
        b = bands[r["band"]]
        b["funcs"] += 1
        b["instr"] += r["n"]
        if r["kind"] == "C":
            b["C"] += 1
        elif r["kind"] in ("asm", "crt0"):
            b["asm"] += 1
    s["bands"] = bands
    feat = collections.Counter()
    for r in rows:
        f = r["flags"]
        for k in ("mmi", "cop1", "cop2", "cop0", "quad", "pipe1", "jr_reg", "jalr", "syscall"):
            if f[k]:
                feat[k] += 1
        if r["gp"]:
            feat["gp"] += 1
        if r["hw_func"]:
            feat["hw_func"] += 1
        if r["hw_instr"]:
            feat["hw_instr_any"] += 1
        if r["words"]:
            feat["dot_words"] += 1
        if r["invalid"]:
            feat["invalid_decode"] += 1
    s["features"] = dict(feat)
    s["dot_words_total"] = sum(r["words"] for r in rows)
    s["invalid_decode_total"] = sum(r["invalid"] for r in rows)
    s["hw_instr_total"] = sum(r["hw_instr"] for r in rows)
    for nm, fam in (("strict", fam_strict), ("regs", fam_regs), ("loose", fam_loose)):
        sizes = [len(m) for m in fam.values()]
        s["shapes_" + nm] = {
            "distinct": len(fam),
            "singletons": sum(1 for x in sizes if x == 1),
            "families_2plus": sum(1 for x in sizes if x >= 2),
            "funcs_in_families": sum(x for x in sizes if x >= 2),
            "copies_beyond_first": sum(x - 1 for x in sizes if x >= 2),
        }
    # regions
    regs = collections.OrderedDict()
    for reg in REGIONS:
        regs[reg[1]] = {"start": "0x%08X" % reg[0], "kind": reg[2], "grade": reg[3],
                        "seam": ["0x%08X" % reg[4][0], "0x%08X" % reg[4][1]], "evidence": reg[5],
                        "funcs": 0, "instr": 0, "C": 0, "hw": 0, "vu0_mmi": 0, "cop1": 0, "end": None}
    for r in rows:
        g = regs[r["region"]]
        g["funcs"] += 1
        g["instr"] += r["n"]
        g["C"] += r["kind"] == "C"
        g["hw"] += bool(r["hw_func"])
        g["vu0_mmi"] += bool(r["flags"]["cop2"] or r["flags"]["mmi"])
        g["cop1"] += bool(r["flags"]["cop1"])
        g["end"] = "0x%08X" % (r["addr"] + r["size"])
    s["regions"] = regs
    s["by_region_kind"] = {k: {"funcs": sum(1 for r in rows if r["rkind"] == k),
                               "instr": sum(r["n"] for r in rows if r["rkind"] == k)}
                           for k in ("crt0", "game", "sdk")}
    s["sdk_confirmed_funcs"] = sum(1 for r in rows if r["rkind"] == "sdk"
                                   and region_of(r["addr"])[3] == "Confirmed")
    # work categories, split game / sdk, with distinct strict shapes and instructions
    cats = collections.OrderedDict()
    for c in CATEGORIES:
        cats[c] = {}
        for k in ("game", "sdk", "crt0"):
            mem = [r for r in rows if r["cat"] == c and r["rkind"] == k]
            if mem:
                cats[c][k] = {"funcs": len(mem), "instr": sum(r["n"] for r in mem),
                              "shapes": len({r["strict"] for r in mem})}
    s["categories"] = cats
    # strict families: within game, within sdk, or across
    cross = collections.Counter()
    for k, mem in fam_strict.items():
        if len(mem) < 2:
            continue
        kinds = {m["rkind"] for m in mem}
        cross["both" if len(kinds) > 1 else kinds.pop()] += 1
    s["strict_family_spread"] = dict(cross)
    return s


def print_md(s):
    print("\n| Band (instr.) | Functions | Instructions | Already C | Asm |")
    print("|---|---|---|---|---|")
    for b, v in s["bands"].items():
        print("| %s | %d | %d | %d | %d |" % (b, v["funcs"], v["instr"], v["C"], v["asm"]))
    print("\nfeatures:", json.dumps(s["features"]))
    for nm in ("strict", "regs", "loose"):
        print(nm, json.dumps(s["shapes_" + nm]))
    print()
    print("| Region | Kind | Start | End | Seam window | Grade | Functions | Instr. | Handwritten | VU0/MMI | COP1 |")
    print("|---|---|---|---|---|---|---|---|---|---|---|")
    for nm, g in s["regions"].items():
        seam = g["seam"][0] if g["seam"][0] == g["seam"][1] else "%s-%s" % tuple(g["seam"])
        print("| %s | %s | %s | %s | %s | %s | %d | %d | %d | %d | %d |" % (
            nm, g["kind"], g["start"], g["end"], seam, g["grade"], g["funcs"], g["instr"], g["hw"],
            g["vu0_mmi"], g["cop1"]))
    print()
    print("by kind:", json.dumps(s["by_region_kind"]), "sdk confirmed:", s["sdk_confirmed_funcs"])
    print()
    print("| Category | Game funcs | Game shapes | Game instr. | SDK funcs | SDK shapes | SDK instr. |")
    print("|---|---|---|---|---|---|---|")
    for c, v in s["categories"].items():
        g = v.get("game", {}) or v.get("crt0", {})
        k = v.get("sdk", {})
        print("| %s | %s | %s | %s | %s | %s | %s |" % (c, g.get("funcs", 0), g.get("shapes", 0), g.get("instr", 0),
                                                    k.get("funcs", 0), k.get("shapes", 0), k.get("instr", 0)))
    print()
    print("strict family spread:", json.dumps(s["strict_family_spread"]))


if __name__ == "__main__":
    sys.exit(main())
