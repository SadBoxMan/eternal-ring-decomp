"""ER-017: candidate file (translation unit) seams inside the game code region of SLUS_200.15.

Read-only research tool. Inputs (generated, gitignored):
  disc/extract/SLUS_200.15            code and data bytes (the original ELF; byte-identical to the build)
  build/SLUS_200.15.syms.elf          function symbols with sizes and splat's data labels (`make
                                      build/SLUS_200.15.syms.elf`); if it is missing, the symbol list cached by the
                                      last run in build/seams/symbols.json is used
It reuses the linear address tracker of tools/census.py (`analyse`) to find each function's data references.

Evidence (docs/game_seams.md 2):
  O  (main) scan of every function boundary: initialized variables and string literals are emitted in source
     order, so inside one file a later function using a variable defined above an earlier function's strings gives
     an inversion (a label used only on the left above one used only on the right), and file-static data or a
     static function shared across a boundary binds it. A boundary with no binding, inversion or static call in
     .rodata / .sdata / .data is open; runs of open boundaries are windows (windows.csv, boundaries.csv).
  C  shared strings: one copy of an identical string per file, so a string used by several functions ties them
     into one file (clusters.csv).
  R  .rodata padding. Strings longer than 8 bytes go to .rodata with .align 3; inside one object the zero bytes
     after a string's NUL are at most 7. A label at a 16-byte boundary preceded by 8+ zero bytes needs a 16-byte
     alignment. Test compile (ee-gcc 2.9-ee-991111 -O2 -G8, scratch file, docs/game_seams.md 3): jump tables go to
     .rodata with .align 4 and object sections are only as aligned as their items, so such padding is explained
     inside one file when a jump table follows (kind RJ). Only string -> padding -> string (R1) is unexplained.
     Each padding point is mapped to the text window between the functions that use the .rodata on either side
     (median referencer per label, longest non-decreasing run in address order to drop tracker misreads).
  B  binding (negative evidence): things that tie the two sides of a window into one file.
       B-str  a .rodata item referenced from both sides (GCC emits one copy of an identical string per file; .rodata
              is never shared across files except for global const tables)
       B-dat  a .data / .sdata / .sbss / .bss label referenced from both sides by functions that all lie within
              LOCAL_SPAN functions of each other (file-static-looking data)
       B-call a call across the window to a function whose callers all lie within LOCAL_SPAN functions of it
              (static-looking function)
  S  step support in .data / .sdata / .sbss / .bss: within K functions on each side, the labels used only by the
     left side all lie below the labels used only by the right side (each object's data is contiguous, in link
     order). Inversions (a left-only label above a right-only label) count against the seam.

Outputs (build/seams/, gitignored: derived from game code):
  windows.csv     evidence O: windows of open boundaries with the tool's grade
  boundaries.csv  evidence O per function boundary
  seams.csv       evidence R: one row per padding point: kind, window, grade, evidence counts
  clusters.csv    minimum file extents from strings shared by several functions (one copy per file)
  rodata.csv      every .rodata label: padding before it, string or not, referencing game functions
  symbols.json    cached symbol list (functions and data labels: addresses, sizes, names only)
  cuts_<set>.csv  (--cuts, ER-022) per Strong seam and candidate start: one-sided labels per data section on each
                  side (whole neighbouring files and within STEP_K), bindings, inversions, the data cut range
Usage:  .venv\\Scripts\\python.exe tools/seams.py [--md] [--span N] [--k N] [--out DIR] [--cuts [--cut-set S]]
  --md    also print the evidence R table
  --span  LOCAL_SPAN; --k  STEP_K; --out  output folder (the sensitivity runs of docs/game_seams.md 2 use
          --out build/seams/var_<span>_<k>)
The final grades in docs/game_seams.md 4 add stability over those runs to the tool's own grade.
  --cuts  ER-022 data-cut check (docs/game_seams.md 8); --cut-set all (every Strong window), fixed (the 1-function
          windows) or cut (CUT_STARTS, the seams cut in slus_200.15.yaml), or list (--cut-starts, ER-023)
ER-023 (docs/game_seams.md 9): defaults changed to the control-flow-aware tracker with base attribution
(--tracker cfb), the far-label filter (--far 1) and .sbss / .bss as primary evidence (--bss primary); --legacy
restores ER-017 / ER-022's settings (their results reproduce exactly). Also: --region all (SDK too), --measure (hit
rate on the census seams), --validate-asm (references against spimdisasm's asm), --stability (final grades over the
5 variants, final.csv), --jt (jump-table alignment constraints, jt.csv), --icalls (indirect call edges; off),
--no-calls, --probe ADDR (referencers of data labels), --syms PATH (symbols ELF copy).
ER-053 (docs/game_seams.md 9.9): --cuts applies the far-label filter (--cuts-far, default 1), new cut sets final / solo
(the Strong windows of final.csv, all candidates), --cuts-no-common (COMMON-tail .bss labels out), starts tested in
address order, offending labels printed (inv-L>).
"""
from __future__ import annotations

import argparse
import bisect
import collections
import csv
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from elfutil import Elf  # noqa: E402
import census  # noqa: E402  (reuses its address tracker; importing it runs nothing)

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BASE_ELF = os.path.join(ROOT, "disc", "extract", "SLUS_200.15")
SYMS_ELF = os.path.join(ROOT, "build", "SLUS_200.15.syms.elf")
OUT_DIR = os.path.join(ROOT, "build", "seams")
SYM_CACHE = os.path.join(OUT_DIR, "symbols.json")

GAME_LO, GAME_HI = 0x001000C0, 0x001CE130      # game + game-vu0 + game-unk (docs/census.md 4.1)
GAME_MAIN_HI = 0x001C9B18                      # end of the game region proper
TEXT_HI = 0x001F0354                           # end of .text (ER-023 --region all)
DATA_SECS = (".data", ".sdata", ".sbss", ".bss")
LOCAL_SPAN = 120     # functions: referencers of a label (or callers of a function) within this span look file-local
STEP_K = 40          # functions on each side for the step test
MIN_PAD = 8          # zero bytes before a 16-aligned .rodata label that 8-byte alignment cannot explain


def load_symbols():
    if os.path.exists(SYMS_ELF):
        elf = Elf(open(SYMS_ELF, "rb").read())
        syms = elf.symbols()
        funcs = sorted({(s.value, s.size, s.name) for s in syms
                        if s.size and (s.type == 2 or s.name in census.DATA_BLOCKS)})
        dsyms = sorted({(s.value, s.size, s.name) for s in syms if s.type == 1 and s.name not in census.DATA_BLOCKS})
        os.makedirs(OUT_DIR, exist_ok=True)
        json.dump({"funcs": funcs, "dsyms": dsyms}, open(SYM_CACHE, "w"))
        return funcs, dsyms, "syms ELF"
    if os.path.exists(SYM_CACHE):
        j = json.load(open(SYM_CACHE))
        return [tuple(x) for x in j["funcs"]], [tuple(x) for x in j["dsyms"]], "cache"
    raise SystemExit("seams: need build/SLUS_200.15.syms.elf (make build/SLUS_200.15.syms.elf) or a cached list")


def is_string(bs: bytes) -> bool:
    """bytes up to the first NUL are printable text of 1+ characters"""
    k = bs.find(b"\0")
    if k <= 0:
        return False
    return all(32 <= c < 127 or c in (9, 10, 13) for c in bs[:k])


def lnds(vals):
    """indices of one longest non-decreasing subsequence"""
    tails, tail_idx, prev = [], [], [-1] * len(vals)
    for i, v in enumerate(vals):
        j = bisect.bisect_right(tails, v)
        if j == len(tails):
            tails.append(v)
            tail_idx.append(i)
        else:
            tails[j] = v
            tail_idx[j] = i
        prev[i] = tail_idx[j - 1] if j else -1
    out, i = [], tail_idx[-1] if tail_idx else -1
    while i >= 0:
        out.append(i)
        i = prev[i]
    return out[::-1]


# ER-022: the 22 Strong windows of docs/game_seams.md 4.1 (first and last candidate start of the new file)
STRONG = [
    (0x001343E8, 0x001343E8), (0x00134480, 0x00134488), (0x00134BB0, 0x00134DB0), (0x001359C0, 0x00135A50),
    (0x00140AC0, 0x00140C18), (0x001416A8, 0x001416B8), (0x00142920, 0x00142A68), (0x00144218, 0x00144308),
    (0x00145120, 0x00145120), (0x001464A8, 0x00146598), (0x00147018, 0x00147100), (0x0014A968, 0x0014A968),
    (0x0014B2C0, 0x0014B3A0), (0x0014BF40, 0x0014BF48), (0x0014C840, 0x0014C840), (0x0014D930, 0x0014D930),
    (0x0014E2E8, 0x0014E2E8), (0x0014F680, 0x0014F690), (0x0014FF38, 0x0014FF78), (0x00150878, 0x00150878),
    (0x001C8EE0, 0x001C8EE0), (0x001CA8B0, 0x001CA8D8),
]
CENSUS_CUTS = (0x001C9B18, 0x001CA0F0)          # existing game files (ER-013): always cuts
# ER-022: the seams cut in slus_200.15.yaml (Strong number, first function of the new file); docs/game_seams.md 8
CUT_STARTS = [(1, 0x001343E8), (9, 0x00145120), (12, 0x0014A968), (15, 0x0014C840), (16, 0x0014D930),
              (17, 0x0014E2E8), (20, 0x00150878)]


# ---- ER-023: control-flow-aware reference tracker -----------------------------------------------------------------
# census.analyse walks a function's words in address order and carries register state straight through branches,
# returns and calls. That can invent a reference (a register's value from the previous block in address order used
# as a base in a block reached from elsewhere) or attribute it to the wrong path. analyse_cf builds basic blocks
# (branch and jump targets, jump-table targets, after delay slots), propagates the hi/base register state forward
# with a meet that keeps only values equal on every incoming edge, clobbers the caller-saved registers after each
# call (EABI: everything but $s0-$s7, $gp, $sp, $fp), and records a reference only from the converged state.
# Jump tables: a .rodata label the function addresses whose words are all code addresses inside the function; its
# entries become successors of the function's `jr $reg` blocks.
CALLEE_SAVED = frozenset(range(16, 24)) | {28, 29, 30}
FAR_GAP = None       # --far: per-section address gap (bytes) that ends a boundary's local label cluster
FAR_DEFAULT = {".rodata": 0x1000, ".sdata": 0x400, ".data": 0x10000, ".sbss": 0x400, ".bss": 0x100000}
BASE_ATTR = False    # --tracker cfb: a load / store at base + offset counts as a reference to the base's symbol
                     # (spimdisasm's reading: the %hi / %lo pair names the object, the offset is a field or element)


def _transfer(st, w, ins, op, img, rec):
    """one instruction on register state st (dict reg -> value: ('h', hi) from lui or ('b', addr)); rec(addr) is
    called for each address formed from a known register. Mirrors census.analyse's register logic."""
    major = w >> 26
    if ins.isJType() or ins.isBranch():
        return
    dest_done = False
    if ins.isIType() and major != 0x01:
        imm = w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        rs, rt = (w >> 21) & 31, (w >> 16) & 31
        if op == "lui":
            val = imm << 16
            if census.IMAGE_LO <= val < census.IMAGE_HI + 0x10000:
                st[rt] = ("h", val)
            else:
                st.pop(rt, None)
            return
        a = None
        if rs == 28:
            a = census.GP + simm
        elif rs in st:
            k, v = st[rs]
            a = v + (imm if (op == "ori" and k == "h") else simm)
            if k == "b" and BASE_ATTR:
                a = v                   # base + offset: a field / element of the object at the base's symbol
        if a is not None:
            rec(a)
        if op in ("addiu", "daddiu", "ori") and a is not None:
            st[rt] = ("b", a)
            dest_done = True
        if ins.doesStore():
            dest_done = True
    if dest_done:
        return
    try:
        d = ins.getDestinationGpr()
    except Exception:
        d = None
    if op in ("daddu", "addu", "or") and ins.isRType():
        rs_, rt_, rd_ = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        src = rs_ if rt_ == 0 else (rt_ if rs_ == 0 else None)
        if rd_:
            if src is not None and src in st:
                st[rd_] = st[src]
            else:
                st.pop(rd_, None)
        return
    if d is not None and d >= 0:
        st.pop(d, None)


def _meet(a, b):
    if a is None:
        return dict(b)
    return {r: v for r, v in a.items() if b.get(r) == v}


def analyse_cf(name, addr, size, img, fstarts):
    """census.analyse's interface (calls, tails, coderefs, datarefs) with a control-flow-aware data tracker; the
    calls / tails / coderefs come from census.analyse (they do not depend on register state except coderefs, which
    are recomputed here too). Extra keys: 'jtables' (jump-table labels found), 'blocks', 'unreached' (blocks with no
    known predecessor, analysed from an empty state)."""
    base = census.analyse(name, addr, size, img, fstarts, "game")
    raw = img.elf.load_bytes(addr, size)
    n = size // 4
    words = [int.from_bytes(raw[i * 4:i * 4 + 4], "little") for i in range(n)]
    end = addr + size
    dec = []
    for i, w in enumerate(words):
        ins = census.rz.Instruction(w, addr + i * 4, census.rz.InstrCategory.R5900)
        dec.append((w, ins, ins.getOpcodeName()))
    # jump tables among the linear tracker's .rodata references (entries are code addresses in this function)
    jtabs, jt_targets = [], set()
    for d in sorted(base["datarefs"]):
        if img.sec_of(d) != ".rodata" or d % 4:
            continue
        ents, a = [], d
        while len(ents) < 1024:
            bs = img.elf.load_bytes(a, 4)
            if bs is None:
                break
            v = int.from_bytes(bs, "little")
            if not (addr <= v < end and v % 4 == 0):
                break
            ents.append(v)
            a += 4
        if len(ents) >= 2:
            jtabs.append(d)
            jt_targets.update((v - addr) // 4 for v in ents)
    leaders = {0} | jt_targets
    for i, (w, ins, op) in enumerate(dec):
        if ins.isBranch() or (ins.isJType() and op == "j"):
            if ins.isBranch():
                t = (addr + i * 4 + ins.getBranchOffset() - addr) // 4
            else:
                t = (ins.getInstrIndexAsVram() - addr) // 4
            if 0 <= t < n:
                leaders.add(t)
            if i + 2 < n:
                leaders.add(i + 2)
        elif ins.hasDelaySlot() and i + 2 < n:          # jal, jalr, jr
            leaders.add(i + 2)
    ls = sorted(x for x in leaders if 0 <= x < n)
    # blocks: [start, stop) where stop is the index after the control instruction (its delay slot is handled on the
    # edges) or the next leader
    blocks = {}
    for k, s in enumerate(ls):
        nxt = ls[k + 1] if k + 1 < len(ls) else n
        stop, ctl = nxt, None
        for i in range(s, nxt):
            if dec[i][1].hasDelaySlot():
                stop, ctl = i + 1, i
                break
        blocks[s] = (stop, ctl)
    jr_blocks = [s for s, (_, c) in blocks.items() if c is not None and dec[c][1].isJrNotRa()]

    def run_block(s, st, rec):
        """returns list of (successor start, state)"""
        stop, ctl = blocks[s]
        st = dict(st)
        for i in range(s, stop):
            if i == ctl:
                break
            _transfer(st, *dec[i], img, rec)
        if ctl is None:
            return [(stop, st)] if stop < n else []
        w, ins, op = dec[ctl]
        before = dict(st)
        if ctl + 1 < n:
            _transfer(st, *dec[ctl + 1], img, rec)
        after = st
        out = []
        fall = ctl + 2
        if ins.isBranch():
            t = (ins.getBranchOffset()) // 4 + ctl
            likely = ins.isBranchLikely()
            if 0 <= t < n:
                out.append((t, after))
            if not ins.isUnconditionalBranch() and fall < n:
                out.append((fall, before if likely else after))
        elif ins.isJType():
            if op == "jal":
                st2 = {r: v for r, v in after.items() if r in CALLEE_SAVED}
                if fall < n:
                    out.append((fall, st2))
            else:
                t = (ins.getInstrIndexAsVram() - addr) // 4
                if 0 <= t < n:
                    out.append((t, after))
        elif ins.doesLink():                             # jalr
            st2 = {r: v for r, v in after.items() if r in CALLEE_SAVED}
            if fall < n:
                out.append((fall, st2))
        elif ins.isJrNotRa():
            out.extend((t, after) for t in sorted(jt_targets))
        # jr $ra: no successor
        return out

    # predecessor-known blocks: fixpoint of a must analysis (None = not reached yet)
    state_in = {s: None for s in blocks}
    state_in[0] = {}
    work = [0]
    seen_pred = {0}
    noop = lambda a: None  # noqa: E731
    guard = 0
    while work and guard < 200000:
        guard += 1
        s = work.pop()
        for t, st in run_block(s, state_in[s], noop):
            if t not in blocks:
                continue
            seen_pred.add(t)
            new = _meet(state_in[t], st)
            if new != state_in[t]:
                state_in[t] = new
                work.append(t)
    unreached = [s for s in blocks if state_in[s] is None]
    datarefs, coderefs = set(), set()

    def rec(a):
        sec = img.sec_of(a)
        if sec == ".text":
            coderefs.add(a)
        elif sec:
            datarefs.add(a)
    for s in blocks:
        run_block(s, state_in[s] if state_in[s] is not None else {}, rec)
    out = dict(base)
    out["datarefs_linear"] = base["datarefs"]
    out["datarefs"] = datarefs
    out["coderefs"] = coderefs
    out["jtables"] = jtabs
    out["blocks"] = len(blocks)
    out["unreached"] = len(unreached)
    out["jr_blocks"] = len(jr_blocks)
    return out


def indirect_edges(rows, img, dsyms, fstart_set, label_of):
    """ER-023 'call clusters': function addresses taken in code (coderefs equal to a function start) and function
    pointers stored in data (.data / .sdata / .rodata words equal to a function start, outside jump tables). Returns
    {target index: set of referencing function indices}: code -> the function taking the address; data -> every
    function that references the label holding the pointer."""
    idx_of = {r["addr"]: i for i, r in enumerate(rows)}
    out = collections.defaultdict(set)
    jt = set()
    for r in rows:
        jt.update(r.get("jtables", ()))
    for i, r in enumerate(rows):
        for a in r["coderefs"]:
            j = idx_of.get(a)
            if j is not None and j != i:
                out[j].add(i)
    # pointers in data labels
    users = collections.defaultdict(set)
    for i, r in enumerate(rows):
        for d in r["datarefs"]:
            users[label_of(d)].add(i)
    for a, sz, _ in dsyms:
        sec = img.sec_of(a)
        if sec not in (".data", ".sdata", ".rodata") or a in jt or not users.get(a):
            continue
        bs = img.elf.load_bytes(a, min(max(sz, 4), 0x4000)) or b""
        for k in range(0, len(bs) - 3, 4):
            v = int.from_bytes(bs[k:k + 4], "little")
            if v in fstart_set:
                j = idx_of.get(v)
                if j is not None:
                    out[j].update(users[a] - {j})
    return out


# ER-023: the census library / region seams (docs/census.md 4.1, 4.3) as ground truth: start, ER-004 seam window,
# how the seam is known. 'fixed' = ER-004's evidence (strings, data, call graph) fixes the first function;
# 'padding' / 'call graph' = Confirmed seams; 'judgement' = ER-013 chose inside a window.
KNOWN_SEAMS = [
    (0x001C9B18, "game-vu0", "fixed"), (0x001CA0F0, "game-unk", "fixed"), (0x001CE130, "libgraph", "judgement"),
    (0x001CFD40, "libkernl", "padding"), (0x001D46E0, "libdma", "padding"), (0x001D5200, "sdk-vudev", "fixed"),
    (0x001D6280, "libpad", "judgement"), (0x001D7310, "libvu0", "fixed"), (0x001D8050, "libcdvd", "fixed"),
    (0x001DAA88, "libmpeg", "judgement"), (0x001E2128, "libipu", "judgement"), (0x001E2788, "libsdr", "fixed"),
    (0x001E2D00, "libmc", "fixed"), (0x001E3DE0, "libm", "fixed"), (0x001E6508, "libgcc", "judgement"),
    (0x001E9A50, "libc", "call graph"), (0x001F00E8, "libkernl-tail", "fixed"),
]


def measure(rows, scan, windows, win_of, idx_of, out_csv):
    """ER-023: how the boundary scan does on the census seams. For each known seam: is its boundary open, and the
    scan window holding it (width, grade, one-sided labels); hit within the ER-004 seam window. Per region: open
    boundaries and windows not holding a known seam (candidate object seams inside a library, or false positives:
    the SDK's object lists are not known, so this cannot be told apart here)."""
    win = {r[0]: r[4] for r in census.REGIONS}
    by_b = {r["b"]: r for r in scan}
    out = []
    for a, nm, how in KNOWN_SEAMS:
        b = idx_of.get(a)
        if b is None:
            continue
        wlo, whi = win.get(a, (a, a))
        cand = [idx_of[x["addr"]] for x in rows if wlo <= x["addr"] <= whi]
        hit_cut = b in win_of
        hit_win = any(c in win_of for c in cand)
        w_ = win_of.get(b) or next((win_of[c] for c in cand if c in win_of), None)
        r = by_b[b]
        out.append({"addr": a, "name": nm, "how": how, "hit_cut": hit_cut, "hit_win": hit_win,
                    "w_first": rows[w_["first"]]["addr"] if w_ else None,
                    "w_last": rows[w_["last"]]["addr"] if w_ else None,
                    "width": (w_["last"] - w_["first"] + 1) if w_ else 0, "grade": w_["grade"] if w_ else "closed",
                    "bind_p": r["bind_p"], "inv_p": r["inv_p"], "call": r["call"], "nl": r["nl"], "nr": r["nr"]})
    with open(out_csv, "w", newline="") as fh:
        wr = csv.writer(fh)
        wr.writerow(["seam", "region", "known_by", "open_at_cut", "open_in_er004_window", "scan_window",
                     "width", "grade", "bind_primary", "inv_primary", "static_calls", "labels_left", "labels_right"])
        for o in out:
            wr.writerow(["0x%08X" % o["addr"], o["name"], o["how"], int(o["hit_cut"]), int(o["hit_win"]),
                         "0x%08X-0x%08X" % (o["w_first"], o["w_last"]) if o["w_first"] else "", o["width"],
                         o["grade"], o["bind_p"], o["inv_p"], o["call"], o["nl"], o["nr"]])
    print()
    print("ER-023 measure: known seams (census 4.3): open at the cut %d / %d, open in the ER-004 window %d / %d" % (
        sum(o["hit_cut"] for o in out), len(out), sum(o["hit_win"] for o in out), len(out)))
    for o in out:
        print("  %08X %-13s %-10s cut=%s win=%s  %s w=%d %s  bind %d inv %d call %d  L/R %d/%d" % (
            o["addr"], o["name"], o["how"], "open" if o["hit_cut"] else "shut", "open" if o["hit_win"] else "shut",
            ("%08X-%08X" % (o["w_first"], o["w_last"])) if o["w_first"] else "-", o["width"], o["grade"],
            o["bind_p"], o["inv_p"], o["call"], o["nl"], o["nr"]))
    print("  game seams cut by ER-022 (Strong, not Confirmed):")
    for nn, a in CUT_STARTS:
        b = idx_of.get(a)
        w_ = win_of.get(b)
        r = by_b[b]
        print("    S%-2d %08X %s %s  bind %d inv %d call %d  L/R %d/%d" % (
            nn, a, "open" if w_ else "shut",
            ("%08X-%08X w=%d %s" % (rows[w_["first"]]["addr"], rows[w_["last"]]["addr"],
                                    w_["last"] - w_["first"] + 1, w_["grade"])) if w_ else "-",
            r["bind_p"], r["inv_p"], r["call"], r["nl"], r["nr"]))
    # per region: boundaries, open, windows (by grade) not holding a known seam
    known_b = {idx_of[a] for a, _, _ in KNOWN_SEAMS if a in idx_of}
    regs = sorted(census.REGIONS, key=lambda r: r[0])
    print("  per region: boundaries / open / windows without a known seam (Strong, Medium, Weak) / informative open "
          "(labels on both sides)")
    tot = collections.Counter()
    for k, r in enumerate(regs):
        lo = r[0]
        hi = regs[k + 1][0] if k + 1 < len(regs) else TEXT_HI
        bs = [x for x in scan if lo <= rows[x["b"]]["addr"] < hi and x["b"] not in known_b]
        if not bs:
            continue
        ws = [w_ for w_ in windows if lo <= rows[w_["first"]]["addr"] < hi
              and not any(b in known_b for b in range(w_["first"], w_["last"] + 1))]
        g = collections.Counter(w_["grade"] for w_ in ws)
        inf = sum(1 for x in bs if x["plausible"] and x["nl"] and x["nr"])
        kind = "sdk" if lo >= 0x001CE130 else "game"
        tot[kind + "_b"] += len(bs)
        tot[kind + "_open"] += sum(x["plausible"] for x in bs)
        tot[kind + "_inf"] += inf
        for gg in ("Strong", "Medium", "Weak"):
            tot[kind + "_" + gg] += g[gg]
        print("    %-13s %4d / %4d / %d, %d, %d / %d" % (r[1], len(bs), sum(x["plausible"] for x in bs), g["Strong"],
                                                      g["Medium"], g["Weak"], inf))
    for kind in ("game", "sdk"):
        if tot[kind + "_b"]:
            print("  %s: %d boundaries, %d open (%.1f%%), %d informative open; windows S/M/W %d/%d/%d" % (
                kind, tot[kind + "_b"], tot[kind + "_open"], 100.0 * tot[kind + "_open"] / tot[kind + "_b"],
                tot[kind + "_inf"], tot[kind + "_Strong"], tot[kind + "_Medium"], tot[kind + "_Weak"]))
    return out


def jt_constraints(rows, refs, labinfo, win_of, idx_of, img, out_csv):
    """ER-023 (ER-024's test, generalised): a jump table J makes its object's .rodata 16-aligned (gcc `.align 4`),
    so that object's .rodata starts at J itself or at a 16-aligned item start Y1 < J (the nearest one) with every
    item in [Y1, J) inside it. Either way: (a) S = J: the file of J's user starts after the users of the items
    before J; or (b) the users of [Y1, J) and J's user share one file. Decided cases:
      'together'  an item in [Y1, J) is used by J's own function -> (b): no seam in (first user of [Y1, J), fJ]
      'boundary'  a cut (census / ER-022) lies in (first user of [Y1, J), fJ] -> (a): seam in (last user, fJ]
      'either'    neither decided: seam in (last user, fJ] or no seam in (first user, fJ]
    Each row also counts the scan's open boundaries in the forced range."""
    cuts = {idx_of[a] for a in CENSUS_CUTS if a in idx_of} | {idx_of[a] for _, a in CUT_STARTS if a in idx_of}
    labs = [li["addr"] for li in labinfo]
    rlab = refs[".rodata"]
    out = []
    for k, li in enumerate(labinfo):
        J = li["addr"]
        if not li["is_jt"] or J % 16 or not rlab.get(J):
            continue
        fj = sorted(rlab[J])
        if len(fj) != 1:
            continue
        fj = fj[0]
        # nearest 16-aligned item start below J
        m = k - 1
        while m >= 0 and labs[m] % 16:
            m -= 1
        if m < 0:
            continue
        y1 = labs[m]
        users = set()
        for q in range(m, k):
            users |= set(rlab.get(labs[q], ()))
        if not users:
            continue
        fmin, fmax = min(users), max(users)
        if fmin > fj:
            continue                                  # not in link order with J (misread or shared table)
        if fj in users or fmax >= fj:
            kind, lo_b, hi_b = "together", fmin + 1, fj
        elif any(fmin < c <= fj for c in cuts):
            kind, lo_b, hi_b = "boundary", fmax + 1, fj
        else:
            kind, lo_b, hi_b = "either", fmax + 1, fj
        opens = [b for b in range(lo_b, hi_b + 1) if b in win_of]
        out.append({"jt": J, "fj": rows[fj]["addr"], "y1": y1, "n_items": k - m, "fmin": rows[fmin]["addr"],
                    "fmax": rows[fmax]["addr"], "kind": kind, "lo": rows[lo_b]["addr"] if lo_b < len(rows) else 0,
                    "hi": rows[hi_b]["addr"], "nb": hi_b - lo_b + 1, "open": len(opens),
                    "open_first": rows[opens[0]]["addr"] if opens else None,
                    "game": rows[fj]["addr"] < GAME_HI})
    with open(out_csv, "w", newline="") as fh:
        wr = csv.writer(fh)
        wr.writerow(["jump_table", "user", "y1", "items_y1_to_jt", "first_user_before", "last_user_before", "kind",
                     "range_lo", "range_hi", "boundaries_in_range", "open_in_range"])
        for o in out:
            wr.writerow(["0x%08X" % o["jt"], "0x%08X" % o["fj"], "0x%08X" % o["y1"], o["n_items"],
                         "0x%08X" % o["fmin"], "0x%08X" % o["fmax"], o["kind"], "0x%08X" % o["lo"],
                         "0x%08X" % o["hi"], o["nb"], o["open"]])
    print()
    c = collections.Counter((o["kind"], o["game"]) for o in out)
    print("ER-023 jump-table alignment: %d tables with a single user; game: together %d, boundary %d, either %d; "
          "sdk: together %d, boundary %d, either %d" % (len(out), c[("together", True)], c[("boundary", True)],
                                                         c[("either", True)], c[("together", False)],
                                                         c[("boundary", False)], c[("either", False)]))
    for o in out:
        if o["kind"] == "boundary" or (o["kind"] == "either" and o["game"]):
            print("  %-8s jt %08X (user %08X) items before from %08X [%08X..%08X]: seam in %08X..%08X (%d bnd, %d "
                  "open)%s" % (o["kind"], o["jt"], o["fj"], o["y1"], o["fmin"], o["fmax"], o["lo"], o["hi"], o["nb"],
                               o["open"], "" if o["kind"] == "boundary" else "  or one file from %08X" % o["fmin"]))
    return out


def validate_asm(rows, img, label_of, sym_addr):
    """ER-023: compare the tracker's data references with spimdisasm's (the %hi / %lo / %gp_rel operands in
    asm/nonmatchings/**/<func>.s, which the build assembles). Per function with an asm file: labels in the sections
    the scan uses. Returns counts (both, tracker only, asm only) and examples."""
    import glob
    import re
    pat = re.compile(r"(?:%(?:hi|lo|gp_rel)\(([A-Za-z_.$][\w.$]*)(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?\)"
                     r"|\(([A-Za-z_.$][\w.$]*)(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?\)\s*/\* gp_rel)")
    files = {os.path.splitext(os.path.basename(p))[0]: p
             for p in glob.glob(os.path.join(ROOT, "asm", "nonmatchings", "**", "*.s"), recursive=True)}
    secs = (".rodata",) + DATA_SECS
    tot = collections.Counter()
    ex_t, ex_a = [], []
    for r in rows:
        p = files.get(r["name"])
        if not p:
            continue
        tot["funcs"] += 1
        A = set()
        for m in pat.finditer(open(p, encoding="utf-8", errors="replace").read()):
            nm, off = (m.group(1), m.group(2)) if m.group(1) else (m.group(3), m.group(4))
            a = sym_addr.get(nm)
            if a is None:
                tot["unresolved"] += 1
                continue
            a += int(off, 0) if off else 0
            if img.sec_of(a) in secs:
                A.add(label_of(a))
        T = {label_of(d) for d in r["datarefs"] if img.sec_of(d) in secs}
        tot["both"] += len(A & T)
        tot["tracker_only"] += len(T - A)
        tot["asm_only"] += len(A - T)
        if len(ex_t) < 12:
            ex_t += [(r["addr"], x) for x in sorted(T - A)][:2]
        if len(ex_a) < 12:
            ex_a += [(r["addr"], x) for x in sorted(A - T)][:2]
    return tot, ex_t, ex_a


VARIANTS = ((60, 20), (240, 40), (120, 80), (40, 40), (120, 20))


def stability(args, rows, windows):
    """ER-023: docs/game_seams.md 2 final grades made reproducible. Each variant run (same tracker, region, icalls,
    syms) writes <out>/var_<span>_<k>/windows.csv; a window is stable in a variant when that run has an open window
    overlapping it. Final: Strong = tool Strong and stable in 4+ of 5; tool Strong 3/5 -> Medium; tool Medium stays
    Medium when stable 4+ and .sbss / .bss clean, else Weak; everything else Weak."""
    import subprocess
    opened = []
    for sp, k in VARIANTS:
        out = os.path.join(OUT_DIR, "var_%d_%d" % (sp, k))
        cmd = [sys.executable, os.path.abspath(__file__), "--span", str(sp), "--k", str(k), "--out", out,
               "--region", args.region, "--tracker", args.tracker]
        if args.icalls:
            cmd.append("--icalls")
        cmd += ["--far", str(args.far), "--bss", args.bss]
        if args.syms:
            cmd += ["--syms", args.syms]
        subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)
        ws = []
        with open(os.path.join(out, "windows.csv")) as fh:
            for rr in csv.DictReader(fh):
                ws.append((int(rr["first_candidate_start"], 16), int(rr["last_candidate_start"], 16)))
        opened.append(ws)
    with open(os.path.join(OUT_DIR, "final.csv"), "w", newline="") as fh:
        wr = csv.writer(fh)
        wr.writerow(["first_candidate_start", "last_candidate_start", "funcs", "tool_grade", "stable_of_5",
                     "bss_clean", "final_grade", "labels_left", "labels_right"])
        cnt = collections.Counter()
        for w_ in windows:
            a0, a1 = rows[w_["first"]]["addr"], rows[w_["last"]]["addr"]
            st = sum(1 for ws in opened if any(x0 <= a1 and x1 >= a0 for x0, x1 in ws))
            bb = w_["best"]
            clean = bb["bind_s"] == 0 and bb["inv_s"] == 0
            g = w_["grade"]
            if g == "Strong" and st >= 4:
                fg = "Strong"
            elif g == "Strong" and st == 3:
                fg = "Medium"
            elif g == "Medium" and st >= 4 and clean:
                fg = "Medium"
            else:
                fg = "Weak"
            w_["final"], w_["stable"] = fg, st
            cnt[fg] += 1
            wr.writerow(["0x%08X" % a0, "0x%08X" % a1, w_["last"] - w_["first"] + 1, g, st, int(clean), fg,
                         bb["nl"], bb["nr"]])
    print("final grades (stability over %d variants): %s" % (len(VARIANTS), dict(cnt)))


COMMON_LO = 0x01B7B900   # first .bss label of the COMMON tail (docs/game_seams.md 9.5; Unconfirmed that they are COMMON)


def far_keep(U, s):
    """ER-053: the labels of the address cluster around the median of the sorted list U (consecutive gaps up to
    FAR_GAP[s]); the same rule the boundary scan uses (ER-023), so --cuts and the scan agree. Returns (kept set, n
    dropped)."""
    if not FAR_GAP or not U:
        return set(U), 0
    m = len(U) // 2
    lo_k, hi_k = m, m
    g_ = FAR_GAP[s]
    while lo_k > 0 and U[lo_k] - U[lo_k - 1] <= g_:
        lo_k -= 1
    while hi_k + 1 < len(U) and U[hi_k + 1] - U[hi_k] <= g_:
        hi_k += 1
    return set(U[lo_k:hi_k + 1]), len(U) - (hi_k - lo_k + 1)


def data_cuts(rows, refs, span_ok, dsyms, out_csv, cut_set="all", extra=(), cut_far=True, final_csv=None,
              no_common=False):
    """ER-022: for each Strong window and each candidate start in it, put the text cuts at the census seams plus
    every Strong seam (the others at their window's first candidate) and check, per data section, that the labels
    used by the files on either side of the cut are in link order: every label of the file before lies below every
    label of the file after (max L < min R), and no file-local-looking label (referencers within LOCAL_SPAN) is used
    by both. Only labels in span_ok (LOCAL_SPAN) count, as in the scan. 'full' = the two whole files; 'local' = only
    the labels referenced within STEP_K functions of the cut. The data cut lies in (end of max L, min R]; the
    number of splat labels starting in that range says how far data alone pins it."""
    addrs = [r["addr"] for r in rows]
    idx = {a: i for i, a in enumerate(addrs)}
    dstart = sorted(d[0] for d in dsyms)
    dsize = {d[0]: d[1] for d in dsyms}
    secs = (".rodata", ".sdata", ".data", ".sbss", ".bss")

    def check(cut_idx, k):
        """cut_idx: sorted function indices that start a file; k: position of the cut under test"""
        c = cut_idx[k]
        lo = cut_idx[k - 1] if k else 0
        hi = cut_idx[k + 1] if k + 1 < len(cut_idx) else len(rows)
        res = {}
        for s in secs:
            for mode in ("full", "local"):
                a0, a1 = (lo, hi) if mode == "full" else (max(lo, c - STEP_K), min(hi, c + STEP_K))
                L, R, both = [], [], []
                for lab in span_ok[s]:
                    if no_common and s == ".bss" and lab >= COMMON_LO:
                        continue        # ER-053: the COMMON tail is global, its order is not tied to files
                    fs = refs[s][lab]
                    inl = any(a0 <= i < c for i in fs)
                    inr = any(c <= i < a1 for i in fs)
                    if inl and inr:
                        both.append(lab)
                    elif inl:
                        L.append(lab)
                    elif inr:
                        R.append(lab)
                L.sort()
                R.sort()
                nfar = 0
                if cut_far and FAR_GAP:
                    # ER-053: the scan's far-label filter: only the address cluster around the median of all the labels
                    # used by the two files counts (a label far from both files' data runs is a global of another file)
                    keep_, nfar = far_keep(sorted(L + R + both), s)
                    L = [x for x in L if x in keep_]
                    R = [x for x in R if x in keep_]
                    both = [x for x in both if x in keep_]
                inv = sum(bisect.bisect_left(R, x) for x in L)   # pairs (l, r) with l > r
                ok = not both and inv == 0
                rec = {"nL": len(L), "nR": len(R), "both": len(both), "inv": inv, "ok": ok, "far": nfar,
                       "maxL": L[-1] if L else None, "minR": R[0] if R else None, "ex_both": both[:3],
                       "ex_inv": [x for x in L if R and x > R[0]][-4:], "minR_": R[0] if R else None}
                if L and R and L[-1] < R[0]:
                    e = L[-1] + max(dsize.get(L[-1], 1), 1)
                    rec["cut_lo"] = e
                    rec["cut_hi"] = R[0]
                    rec["ncand"] = sum(1 for d in dstart if L[-1] < d <= R[0])
                res[(s, mode)] = rec
        return res

    # cut_set "all": every Strong seam at its window's first candidate; "fixed": only the 1-function windows plus
    # the census seams; "cut": the seams ER-022 cut (CUT_STARTS)
    if cut_set == "all":
        tested = list(enumerate(STRONG, 1))
        base = sorted({idx[a] for a in CENSUS_CUTS} | {idx[f] for f, _ in STRONG})
    elif cut_set == "fixed":
        tested = [(n_, w) for n_, w in enumerate(STRONG, 1) if w[0] == w[1]]
        base = sorted({idx[a] for a in CENSUS_CUTS} | {idx[w[0]] for _, w in tested})
    elif cut_set == "final":
        # ER-053: every Strong window of the final grades (<out>/final.csv from --stability), all candidate starts
        wins = []
        with open(final_csv) as fh:
            for rr in csv.DictReader(fh):
                if rr["final_grade"] == "Strong":
                    wins.append((int(rr["first_candidate_start"], 16), int(rr["last_candidate_start"], 16)))
        tested = [("W%08X" % w[0], w) for w in wins]
        base = sorted({idx[a] for a in CENSUS_CUTS} | {idx[a] for _, a in CUT_STARTS} | {idx[w[0]] for w in wins})
    elif cut_set == "solo":
        # ER-053: every Strong window of final.csv, each candidate tested alone on top of the 7 cut seams and the
        # census seams (the strictest view: the neighbouring files are the long uncut stretches; ER-023 / 9.7's view)
        wins = []
        with open(final_csv) as fh:
            for rr in csv.DictReader(fh):
                if rr["final_grade"] == "Strong":
                    wins.append((int(rr["first_candidate_start"], 16), int(rr["last_candidate_start"], 16)))
        tested = [("W%08X" % w[0], w) for w in wins]
        base = sorted({idx[a] for a in CENSUS_CUTS} | {idx[a] for _, a in CUT_STARTS})
    elif cut_set == "list":
        # ER-023: the given starts on top of the cut seams (each tested with the others in place)
        tested = [("X%08X" % a, (a, a)) for a in extra]
        base = sorted({idx[a] for a in CENSUS_CUTS} | {idx[a] for _, a in CUT_STARTS} | {idx[a] for a in extra})
    else:
        tested = [(n_, (a, a)) for n_, a in CUT_STARTS]
        base = sorted({idx[a] for a in CENSUS_CUTS} | {idx[a] for _, a in CUT_STARTS})
    tested = sorted(tested, key=lambda t: t[1][0])      # ER-053: link order = address order (--cut-starts may be unsorted)
    out = []
    for n_, (f, l_) in tested:
        cands = [i for i in range(idx[f], idx[l_] + 1)]
        for ci in cands:
            cut_idx = sorted((set(base) - {idx[f]}) | {ci})
            k = cut_idx.index(ci)
            res = check(cut_idx, k)
            prim_ok = all(res[(s, m)]["ok"] for s in (".rodata", ".sdata", ".data") for m in ("full", "local"))
            sec_ok = all(res[(s, "full")]["ok"] for s in (".sbss", ".bss"))
            out.append({"seam": n_ if isinstance(n_, str) else "S%d" % n_, "start": addrs[ci], "ncands": len(cands), "prim_ok": prim_ok,
                        "sec_ok": sec_ok, "res": res})
    with open(out_csv, "w", newline="") as fh:
        w = csv.writer(fh)
        hdr = ["seam", "candidate_start", "window_funcs", "primary_ok", "bss_ok"]
        for s in secs:
            hdr += ["%s_full_L/R/both/inv" % s, "%s_local_L/R/both/inv" % s, "%s_cut_lo" % s, "%s_cut_hi" % s,
                    "%s_labels_in_cut_range" % s]
        w.writerow(hdr)
        for o in out:
            row = [o["seam"], "0x%08X" % o["start"], o["ncands"], int(o["prim_ok"]), int(o["sec_ok"])]
            for s in secs:
                rf, rl = o["res"][(s, "full")], o["res"][(s, "local")]
                row += ["%d/%d/%d/%d" % (rf["nL"], rf["nR"], rf["both"], rf["inv"]),
                        "%d/%d/%d/%d" % (rl["nL"], rl["nR"], rl["both"], rl["inv"]),
                        "0x%08X" % rf["cut_lo"] if "cut_lo" in rf else "",
                        "0x%08X" % rf["cut_hi"] if "cut_hi" in rf else "", rf.get("ncand", "")]
            w.writerow(row)
    # link order: the data cut ranges of consecutive tested seams must not overlap backwards (per section)
    order_bad = []
    for s in (secs if cut_set != "solo" else ()):       # solo: each candidate has its own base, no common order
        prev = None
        for o in out:
            rf = o["res"][(s, "full")]
            if "cut_lo" not in rf:
                continue
            if prev is not None and rf["cut_hi"] < prev[1]:
                order_bad.append("%s %s<%s" % (s, o["seam"], prev[0]))
            prev = (o["seam"], rf["cut_lo"])
    print()
    print("ER-022 link order of the data cuts: %s" % (", ".join(order_bad) if order_bad else "in order"))
    print("ER-022 data cuts (full file / local K; L/R/both/inv per section; cut range = (end of max L, min R]):")
    for o in out:
        bits = []
        for s in secs:
            rf, rl = o["res"][(s, "full")], o["res"][(s, "local")]
            t = "%s %d/%d/%d/%d|%d/%d/%d/%d" % (s, rf["nL"], rf["nR"], rf["both"], rf["inv"],
                                                 rl["nL"], rl["nR"], rl["both"], rl["inv"])
            if "cut_lo" in rf:
                t += " [%08X..%08X n=%d]" % (rf["cut_lo"], rf["cut_hi"], rf["ncand"])
            elif "cut_lo" in rl:
                t += " [local %08X..%08X n=%d]" % (rl["cut_lo"], rl["cut_hi"], rl["ncand"])
            if rf["both"]:
                t += " both:" + ",".join("%08X" % x for x in rf["ex_both"])
            if rf["inv"]:
                # ER-053: the L labels above the lowest R label (the offending side of the inversions)
                t += " inv-L>%08X:" % rf["minR_"] + ",".join("%08X" % x for x in rf["ex_inv"])
            bits.append(t)
        print("%-4s %08X (%d) prim=%s bss=%s  %s" % (o["seam"], o["start"], o["ncands"],
                                                    "OK" if o["prim_ok"] else "NO", "ok" if o["sec_ok"] else "no",
                                                    "; ".join(bits)))


def main() -> int:
    global LOCAL_SPAN, STEP_K, OUT_DIR
    ap = argparse.ArgumentParser()
    ap.add_argument("--md", action="store_true")
    ap.add_argument("--span", type=int, default=LOCAL_SPAN, help="LOCAL_SPAN (functions)")
    ap.add_argument("--k", type=int, default=STEP_K, help="STEP_K (functions on each side)")
    ap.add_argument("--out", default=OUT_DIR, help="output folder (default build/seams)")
    ap.add_argument("--cuts", action="store_true", help="ER-022: data cut per section at the Strong windows")
    ap.add_argument("--cuts-out", default=None, help="CSV for --cuts (default <out>/cuts_<cut-set>.csv)")
    ap.add_argument("--cut-starts", nargs="*", default=(), help="ER-023: starts for --cut-set list (hex)")
    ap.add_argument("--cut-set", default="all", choices=("all", "fixed", "cut", "list", "final", "solo"),
                    help="text cuts assumed by --cuts: all Strong windows, the 1-function ones, or CUT_STARTS")
    ap.add_argument("--cuts-far", type=int, default=1, choices=(0, 1),
                    help="ER-053: apply the far-label filter in --cuts (default 1; needs --far; 0 = ER-022 / ER-023 view)")
    ap.add_argument("--cuts-no-common", action="store_true",
                    help="ER-053: --cuts leaves the .bss labels from 0x01B7B900 (the COMMON tail, 9.5) out")
    ap.add_argument("--region", default="game", choices=("game", "all"),
                    help="ER-023: functions scanned: the game region (default) or all of .text after crt0 (SDK too)")
    ap.add_argument("--tracker", default="cfb", choices=("linear", "cf", "cfb"),
                    help="ER-023: census.analyse's linear tracker, the control-flow-aware one (cf), or cf with "
                         "base + offset accesses counted as the base's symbol (cfb)")
    ap.add_argument("--icalls", action="store_true",
                    help="ER-023: add address-taken / data function-pointer edges to the static-call evidence")
    ap.add_argument("--far", type=float, default=1.0,
                    help="ER-023: drop labels far (in address) from a boundary's local data cluster; scale factor "
                         "for the per-section gaps FAR_DEFAULT (default 1; 0 = off)")
    ap.add_argument("--bss", default="primary", choices=("primary", "secondary"),
                    help="ER-023: .sbss / .bss as primary evidence (default: they follow link order, 9.5) or as the "
                         "second check of ER-017")
    ap.add_argument("--legacy", action="store_true",
                    help="ER-017 / ER-022 settings: --tracker linear --far 0 --bss secondary")
    ap.add_argument("--no-calls", action="store_true",
                    help="ER-023: leave the static-call evidence out of the scan (to measure what it costs)")
    ap.add_argument("--measure", action="store_true",
                    help="ER-023: hit rate against the census library seams (needs --region all)")
    ap.add_argument("--jt", action="store_true", help="ER-023: jump-table alignment constraints (ER-024 test)")
    ap.add_argument("--probe", nargs="*", default=(), help="ER-023: print the referencers of these data labels")
    ap.add_argument("--validate-asm", action="store_true",
                    help="ER-023: compare the tracker's references with spimdisasm's in asm/nonmatchings")
    ap.add_argument("--stability", action="store_true",
                    help="ER-023: rerun with the 5 variant settings of docs/game_seams.md 2 (same tracker / region / "
                         "icalls) and write the final grades to <out>/final.csv")
    ap.add_argument("--syms", default=None, help="symbols ELF (default build/SLUS_200.15.syms.elf)")
    args = ap.parse_args()
    if args.legacy:
        args.tracker, args.far, args.bss = "linear", 0.0, "secondary"
    LOCAL_SPAN, STEP_K, OUT_DIR = args.span, args.k, args.out
    global SYMS_ELF, SYM_CACHE
    if args.syms:
        SYMS_ELF = args.syms
    SYM_CACHE = os.path.join(OUT_DIR, "symbols.json")
    if not os.path.exists(BASE_ELF):
        print("seams: missing %s" % BASE_ELF)
        return 1
    funcs, dsyms, symsrc = load_symbols()
    elf = Elf(open(BASE_ELF, "rb").read())
    img = census.Image(elf)
    fstarts = {a for a, _, _ in funcs}
    lo_, hi_ = (GAME_LO, GAME_HI) if args.region == "game" else (GAME_LO, TEXT_HI)
    game = [f for f in funcs if lo_ <= f[0] < hi_]
    global BASE_ATTR, FAR_GAP
    BASE_ATTR = args.tracker == "cfb"
    if args.far:
        FAR_GAP = {k: int(v * args.far) for k, v in FAR_DEFAULT.items()}
    if args.tracker in ("cf", "cfb"):
        rows = [analyse_cf(nm, a, sz, img, fstarts) for a, sz, nm in game]
    else:
        rows = [census.analyse(nm, a, sz, img, fstarts, "game") for a, sz, nm in game]
    n = len(rows)
    idx_of = {r["addr"]: i for i, r in enumerate(rows)}
    dstarts = [d[0] for d in dsyms]

    def label_of(a):
        i = bisect.bisect_right(dstarts, a) - 1
        if i >= 0 and a < dsyms[i][0] + max(dsyms[i][1], 1):
            return dsyms[i][0]
        return a

    if BASE_ATTR:
        # cfb: an address inside no splat data label is a constant that happens to fall in the (huge) .bss range,
        # e.g. lui 0x0100 / addiu -1 = 0x00FFFFFF, or a reference splat never saw; spimdisasm labels every
        # address it reads as a reference (docs/game_seams.md 9)
        dset = set(dstarts)
        n_drop = 0
        for r in rows:
            keep = {d for d in r["datarefs"] if label_of(d) != d or d in dset}
            n_drop += len(r["datarefs"]) - len(keep)
            r["datarefs"] = keep
        print("cfb: dropped %d references inside no data label" % n_drop)

    # item -> referencing function indices, per section
    refs = {s: collections.defaultdict(set) for s in (".rodata",) + DATA_SECS}
    for i, r in enumerate(rows):
        for d in r["datarefs"]:
            s = img.sec_of(d)
            if s in refs:
                refs[s][label_of(d)].add(i)
    # call graph inside the game region
    callers = collections.defaultdict(set)
    for i, r in enumerate(rows):
        for t in r["calls"] | r["tails"]:
            j = idx_of.get(t)
            if j is not None and j != i:
                callers[j].add(i)
    n_icall = 0
    if args.icalls:
        for j, cs in indirect_edges(rows, img, dsyms, fstarts, label_of).items():
            n_icall += len(cs - callers[j])
            callers[j] |= cs
    if args.tracker in ("cf", "cfb"):
        nlin = sum(len(r["datarefs_linear"]) for r in rows)
        ncf = sum(len(r["datarefs"]) for r in rows)
        lost = sum(len(r["datarefs_linear"] - r["datarefs"]) for r in rows)
        new = sum(len(r["datarefs"] - r["datarefs_linear"]) for r in rows)
        print("cf tracker: data refs linear %d, cf %d (%d only linear, %d only cf); jump tables %d in %d functions; "
              "blocks %d, unreached %d" % (nlin, ncf, lost, new, sum(len(r["jtables"]) for r in rows),
                                            sum(1 for r in rows if r["jtables"]), sum(r["blocks"] for r in rows),
                                            sum(r["unreached"] for r in rows)))
    if args.icalls:
        print("indirect call edges added: %d" % n_icall)
    if args.validate_asm:
        sym_addr = {}
        if os.path.exists(SYMS_ELF):
            for s in Elf(open(SYMS_ELF, "rb").read()).symbols():
                if s.name:
                    sym_addr.setdefault(s.name, s.value)
        tot, ex_t, ex_a = validate_asm(rows, img, label_of, sym_addr)
        print("validate vs spimdisasm asm (%d functions with asm): labels both %d, tracker only %d, asm only %d, "
              "unresolved names %d" % (tot["funcs"], tot["both"], tot["tracker_only"], tot["asm_only"],
                                       tot["unresolved"]))
        print("  tracker-only examples: " + " ".join("%08X>%08X" % e for e in ex_t[:12]))
        print("  asm-only examples:     " + " ".join("%08X>%08X" % e for e in ex_a[:12]))
    for p in args.probe:
        pa = int(p, 16)
        lab = label_of(pa)
        for s in refs:
            if lab in refs[s]:
                us = sorted(refs[s][lab])
                print("probe %08X (label %08X, %s): %d users: %s" % (pa, lab, s, len(us),
                      " ".join("%08X" % rows[i]["addr"] for i in us)))
        if not any(lab in refs[s] for s in refs):
            print("probe %08X: no users" % pa)

    # ---- R: .rodata labels, padding and blocks
    ro = [s for s in img.secs if s.name == ".rodata"][0]
    ro_lo, ro_hi = ro.addr, ro.addr + ro.size
    labs = sorted({a for a, _, _ in dsyms if ro_lo <= a < ro_hi} | set(refs[".rodata"]))
    labinfo = []
    for k, a in enumerate(labs):
        prev = labs[k - 1] if k else None
        pad, prev_str = 0, False
        if prev is not None:
            bs = elf.load_bytes(prev, a - prev)
            z = len(bs)
            while z and bs[z - 1] == 0:
                z -= 1
            pad = max(0, (a - prev) - (z + 1)) if z else (a - prev)
            prev_str = is_string(bs)
        r_ = sorted(refs[".rodata"].get(a, ()))
        labinfo.append({"addr": a, "pad": pad, "prev_str": prev_str, "refs": r_,
                        "start": a % 16 == 0 and pad >= MIN_PAD})
    for k, li in enumerate(labinfo):
        li["is_str"] = labinfo[k + 1]["prev_str"] if k + 1 < len(labinfo) else False
        w0 = elf.load_bytes(li["addr"], 4)
        li["is_jt"] = w0 is not None and 0x00100000 <= int.from_bytes(w0, "little") < 0x001F0354
    # robust home of each referenced label: the median referencing function; the longest non-decreasing run of homes
    # in address order keeps the labels that follow link order (the rest are tracker misreads or shared tables)
    seq = [(li["addr"], li["refs"][len(li["refs"]) // 2]) for li in labinfo if li["refs"]]
    keep = lnds([h for _, h in seq])
    mono = [seq[k] for k in keep]
    mono_addrs = [a for a, _ in mono]
    n_out = len(seq) - len(mono)

    def binding(lo_i, hi_i):
        """evidence tying functions <= lo_i to functions >= hi_i (window = (lo_i, hi_i])"""
        out = collections.Counter()
        ex = collections.defaultdict(list)
        for s in refs:
            for lab, fs in refs[s].items():
                left = [i for i in fs if i <= lo_i]
                right = [i for i in fs if i >= hi_i]
                if not left or not right:
                    continue
                span = max(fs) - min(fs)
                if span > LOCAL_SPAN:
                    continue        # far-flung referencers: a global table or a tracker misread
                if s == ".rodata":
                    out["B-str"] += 1
                    ex["B-str"].append(lab)
                else:
                    out["B-dat"] += 1
                    ex["B-dat"].append(lab)
        for j, cs in callers.items():
            if max(cs | {j}) - min(cs | {j}) > LOCAL_SPAN:
                continue
            if (j <= lo_i and any(c >= hi_i for c in cs)) or (j >= hi_i and any(c <= lo_i for c in cs)):
                out["B-call"] += 1
                ex["B-call"].append(rows[j]["addr"])
        return out, ex

    def step(lo_i, hi_i):
        """per data section: (left-only labels, right-only labels, inversions, ok)"""
        res = {}
        L0, R1 = max(0, lo_i - STEP_K + 1), min(n - 1, hi_i + STEP_K - 1)
        for s in DATA_SECS:
            left, right = [], []
            for lab, fs in refs[s].items():
                inl = [i for i in fs if L0 <= i <= lo_i]
                inr = [i for i in fs if hi_i <= i <= R1]
                if not inl and not inr:
                    continue
                if max(fs) - min(fs) > LOCAL_SPAN:
                    continue        # widely shared global: says nothing about this seam
                if inl and not inr:
                    left.append(lab)
                elif inr and not inl:
                    right.append(lab)
            inv = sum(1 for a in left for b in right if a > b)
            res[s] = (len(left), len(right), inv, bool(left and right and inv == 0))
        return res

    # ---- scan of every function boundary (docs/game_seams.md 2, evidence O): initialized variables and string
    # literals are emitted in source order, so inside a file a later function that uses a variable defined above an
    # earlier function's strings gives an inversion (left-only label above a right-only label); across a seam that
    # cannot happen (link order), except through globals or tracker misreads. Primary sections hold no COMMON.
    PRIMARY, SECONDARY = (".rodata", ".sdata", ".data"), (".sbss", ".bss")
    if args.bss == "primary":
        # ER-023: the game's .sbss / .bss labels lie in link order before the SDK libraries' runs, and only 2 labels
        # sit in the COMMON tail after them (docs/game_seams.md 9.5), so both sections can count as primary evidence
        PRIMARY, SECONDARY = (".rodata", ".sdata", ".data", ".sbss", ".bss"), ()
    span_ok = {s: {lab for lab, fs in refs[s].items() if max(fs) - min(fs) <= LOCAL_SPAN} for s in refs}
    flabs = [collections.defaultdict(set) for _ in range(n)]
    for s in refs:
        for lab in span_ok[s]:
            for i in refs[s][lab]:
                flabs[i][s].add(lab)
    static_like = {j for j, cs in callers.items() if max(cs | {j}) - min(cs | {j}) <= LOCAL_SPAN}
    scan = []
    for b in range(1, n):
        L0, R1 = max(0, b - STEP_K), min(n, b + STEP_K)
        rec = {"b": b, "bind_p": 0, "inv_p": 0, "bind_s": 0, "inv_s": 0, "nl": 0, "nr": 0, "call": 0, "far": 0}
        for s in PRIMARY + SECONDARY:
            Ls = set().union(*(flabs[i][s] for i in range(L0, b)))
            Rs = set().union(*(flabs[i][s] for i in range(b, R1)))
            if FAR_GAP and (Ls or Rs):
                # ER-023: keep the labels of the address cluster around the median (consecutive gaps <= FAR_GAP):
                # a label far from both sides' data runs is a global defined in another file (an extern use)
                U = sorted(Ls | Rs)
                m = len(U) // 2
                lo_k, hi_k = m, m
                g_ = FAR_GAP[s]
                while lo_k > 0 and U[lo_k] - U[lo_k - 1] <= g_:
                    lo_k -= 1
                while hi_k + 1 < len(U) and U[hi_k + 1] - U[hi_k] <= g_:
                    hi_k += 1
                keep_ = set(U[lo_k:hi_k + 1])
                rec["far"] += len(U) - len(keep_)
                Ls &= keep_
                Rs &= keep_
            both = Ls & Rs
            lo_, ro_ = sorted(Ls - both), sorted(Rs - both)
            inv = sum(bisect.bisect_left(ro_, x) for x in lo_)
            key = "p" if s in PRIMARY else "s"
            rec["bind_" + key] += len(both)
            rec["inv_" + key] += inv
            rec["nl"] += len(lo_)
            rec["nr"] += len(ro_)
        for j in range(L0, R1):
            if j in static_like:
                cs = callers[j]
                if (j < b and any(b <= c < R1 for c in cs)) or (j >= b and any(L0 <= c < b for c in cs)):
                    rec["call"] += 1
        rec["plausible"] = rec["bind_p"] == 0 and rec["inv_p"] == 0 and (rec["call"] == 0 or args.no_calls)
        scan.append(rec)
    # windows: runs of consecutive plausible boundaries
    windows = []
    for rec in scan:
        if rec["plausible"]:
            if windows and windows[-1]["last"] == rec["b"] - 1:
                windows[-1]["last"] = rec["b"]
                windows[-1]["recs"].append(rec)
            else:
                windows.append({"first": rec["b"], "last": rec["b"], "recs": [rec]})
    for w_ in windows:
        best = min(w_["recs"], key=lambda r: (r["bind_s"] + r["inv_s"], -min(r["nl"], r["nr"])))
        w_["best"] = best
        width = w_["last"] - w_["first"] + 1
        rich = min(best["nl"], best["nr"]) >= 3
        clean2 = best["bind_s"] == 0 and best["inv_s"] == 0
        if width <= 3 and rich and clean2:
            w_["grade"] = "Strong"
        elif width <= 8 and rich:
            w_["grade"] = "Medium"
        else:
            w_["grade"] = "Weak"

    win_of = {}
    for w_ in windows:
        for b in range(w_["first"], w_["last"] + 1):
            win_of[b] = w_
    if args.measure:
        measure(rows, scan, windows, win_of, idx_of, os.path.join(OUT_DIR, "measure.csv"))
    if args.jt:
        jt_constraints(rows, refs, labinfo, win_of, idx_of, img, os.path.join(OUT_DIR, "jt.csv"))

    seams = []
    for li in labinfo:
        if not li["start"]:
            continue
        k = bisect.bisect_left(mono_addrs, li["addr"])
        if k == 0 or k == len(mono):
            continue
        lo_i, hi_i = mono[k - 1][1], mono[k][1]
        if not (GAME_LO <= rows[hi_i]["addr"] < GAME_HI):
            continue
        # R1 string, padding, string (only an object start explains it); RJ a jump table follows (ee-gcc puts jump
        # tables in .rodata with .align 4, so the padding is explained inside one file); R2 string before, other
        # item after; R3 the item before is not a string (its zero tail may be data, not padding)
        if li["is_jt"]:
            kind = "RJ"
        elif li["prev_str"] and li["is_str"]:
            kind = "R1"
        else:
            kind = "R2" if li["prev_str"] else "R3"
        conflict = lo_i >= hi_i
        if conflict:
            seams.append({"rodata": li["addr"], "pad": li["pad"], "kind": kind, "grade": "Conflict",
                          "after_func": rows[lo_i]["addr"], "first_func": rows[hi_i]["addr"]})
            continue
        bind, ex = binding(lo_i, hi_i)
        st = step(lo_i, hi_i)
        nsupport = sum(1 for v in st.values() if v[3])
        ninv = sum(v[2] for v in st.values())
        hard = bind["B-str"] + bind["B-dat"]
        if kind == "R1" and hard == 0 and bind["B-call"] == 0 and nsupport >= 1 and ninv == 0:
            grade = "Strong"
        elif kind == "R1" and hard == 0 and bind["B-call"] <= 1 and ninv <= 2:
            grade = "Medium"
        elif hard <= 1 and bind["B-call"] <= 2 and ninv <= 4:
            grade = "Weak"
        else:
            grade = "Contradicted"
        seams.append({
            "rodata": li["addr"], "pad": li["pad"], "kind": kind,
            "after_func": rows[lo_i]["addr"], "first_func": rows[hi_i]["addr"],
            "win_funcs": hi_i - lo_i, "win_lo": rows[lo_i + 1]["addr"], "win_hi": rows[hi_i]["addr"],
            "grade": grade, "bind": dict(bind), "ex": {k_: v[:4] for k_, v in ex.items()},
            "step": st, "nsupport": nsupport, "ninv": ninv,
        })

    # strings in the image that look like source file names
    names = []
    for s in img.secs:
        if s.name not in (".rodata", ".data", ".sdata"):
            continue
        bs = elf.load_bytes(s.addr, s.size) or b""
        cur_ = bytearray()
        for k, c in enumerate(bs):
            if 32 <= c < 127:
                cur_.append(c)
                continue
            if c == 0 and len(cur_) >= 3:
                t = cur_.decode("ascii")
                low = t.lower()
                if any(low.endswith(x) for x in (".c", ".cpp", ".cc", ".h", ".s", ".o", ".obj")) or "__file__" in low \
                        or "assert" in low or "line %d" in low or ".c:" in low or ".c " in low:
                    names.append((s.addr + k - len(cur_), s.name, t))
            cur_ = bytearray()

    # ---- string clusters: GCC 2.9 keeps one copy of an identical string per file (the constant hash table is per
    # compilation; strings of 8 bytes or less go to .sdata under -G8, longer ones to .rodata: checked with
    # ee-gcc 2.9-ee-991111 -O2 -G8 on a scratch file). A string label used by several functions ties them into one
    # file; the union of those intervals gives minimum file extents ("clusters").
    sd = [s for s in img.secs if s.name == ".sdata"][0]
    str_refs = {}
    for s, lab_refs in ((".rodata", refs[".rodata"]), (".sdata", refs[".sdata"])):
        for lab, fs in lab_refs.items():
            bs = elf.load_bytes(lab, 16)
            if bs is not None and is_string(bs) and len(fs) > 1:
                str_refs[(s, lab)] = sorted(fs)
    intervals = []
    far = 0
    for (s, lab), fs in str_refs.items():
        if fs[-1] - fs[0] > LOCAL_SPAN:
            far += 1          # a tracker misread or a string pooled further than LOCAL_SPAN: left out
            continue
        intervals.append((fs[0], fs[-1], s, lab))
    intervals.sort()
    clusters = []
    for a, b, s, lab in intervals:
        if clusters and a <= clusters[-1][1]:
            c = clusters[-1]
            c[1] = max(c[1], b)
            c[2] += 1
        else:
            clusters.append([a, b, 1])
    _ = sd

    os.makedirs(OUT_DIR, exist_ok=True)
    with open(os.path.join(OUT_DIR, "clusters.csv"), "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["first_func", "last_func", "funcs", "shared_strings", "region"])
        for a, b, k in clusters:
            w.writerow(["0x%08X" % rows[a]["addr"], "0x%08X" % rows[b]["addr"], b - a + 1, k,
                        "game" if rows[a]["addr"] < GAME_MAIN_HI else "game-vu0/unk"])

    def g(z, key, default=""):
        return z.get(key, default)

    with open(os.path.join(OUT_DIR, "seams.csv"), "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["rodata_block", "pad_before", "kind", "last_func_before", "first_func_after",
                    "window_funcs", "window_lo", "window_hi", "grade", "B_str", "B_dat", "B_call",
                    "step_support_secs", "step_inversions"] +
                   ["%s_L/R/inv" % s_ for s_ in DATA_SECS] + ["binding_examples"])
        for z in seams:
            bd = z.get("bind", {})
            w.writerow(["0x%08X" % z["rodata"], z["pad"], z["kind"], "0x%08X" % z["after_func"],
                        "0x%08X" % z["first_func"], g(z, "win_funcs"),
                        "0x%08X" % z["win_lo"] if "win_lo" in z else "",
                        "0x%08X" % z["win_hi"] if "win_hi" in z else "", z["grade"],
                        bd.get("B-str", 0), bd.get("B-dat", 0), bd.get("B-call", 0),
                        g(z, "nsupport"), g(z, "ninv")] +
                       ["%d/%d/%d" % z["step"][s_][:3] if "step" in z else "" for s_ in DATA_SECS] +
                       [" ".join("%s:%s" % (k, ",".join("%08X" % x for x in v)) for k, v in z.get("ex", {}).items())])
    with open(os.path.join(OUT_DIR, "rodata.csv"), "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["label", "pad_before", "prev_is_string", "is_string", "block_start", "in_link_order_run",
                    "game_refs"])
        monoset = set(mono_addrs)
        for li in labinfo:
            w.writerow(["0x%08X" % li["addr"], li["pad"], int(li["prev_str"]), int(li["is_str"]), int(li["start"]),
                        int(li["addr"] in monoset), " ".join("%08X" % rows[i]["addr"] for i in li["refs"])])

    starts = [li for li in labinfo if li["start"]]
    print("seams: symbols from %s; %d game functions; %d .rodata labels (%d referenced by game code, %d of them "
          "off the link-order run); %d block starts (pad >= %d to a 16-byte boundary)"
          % (symsrc, n, len(labinfo), len(seq), n_out, len(starts), MIN_PAD))
    print("candidate seams by grade: %s; by kind: %s" % (
        dict(collections.Counter(z["grade"] for z in seams)), dict(collections.Counter(z["kind"] for z in seams))))
    print("shared string labels: %d (%d with referencers further apart than %d functions, left out); clusters: %d "
          "covering %d functions; largest: %s" % (
              len(str_refs), far, LOCAL_SPAN, len(clusters), sum(b - a + 1 for a, b, _ in clusters),
              ", ".join("0x%08X-0x%08X (%d funcs, %d strings)" % (rows[a]["addr"], rows[b]["addr"], b - a + 1, k)
                        for a, b, k in sorted(clusters, key=lambda c: c[0] - c[1])[:5])))
    print("boundary scan: %d boundaries, %d plausible, %d windows: %s" % (
        len(scan), sum(r["plausible"] for r in scan), len(windows),
        dict(collections.Counter(w_["grade"] for w_ in windows))))
    with open(os.path.join(OUT_DIR, "windows.csv"), "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["first_candidate_start", "last_candidate_start", "funcs", "grade", "best_start", "labels_left",
                    "labels_right", "bss_bind", "bss_inv", "region"])
        for w_ in windows:
            bb = w_["best"]
            w.writerow(["0x%08X" % rows[w_["first"]]["addr"], "0x%08X" % rows[w_["last"]]["addr"],
                        w_["last"] - w_["first"] + 1, w_["grade"], "0x%08X" % rows[bb["b"]]["addr"], bb["nl"],
                        bb["nr"], bb["bind_s"], bb["inv_s"],
                        "game" if rows[w_["first"]]["addr"] < GAME_MAIN_HI else "game-vu0/unk"])
    with open(os.path.join(OUT_DIR, "boundaries.csv"), "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(["start", "plausible", "bind_primary", "inv_primary", "bind_bss", "inv_bss", "static_calls",
                    "labels_left", "labels_right"])
        for r in scan:
            w.writerow(["0x%08X" % rows[r["b"]]["addr"], int(r["plausible"]), r["bind_p"], r["inv_p"], r["bind_s"],
                        r["inv_s"], r["call"], r["nl"], r["nr"]])
    if args.stability:
        stability(args, rows, windows)
    print("file-name-like strings: %d" % len(names))
    for a_, s_, t in names[:40]:
        print("   %08X %s %r" % (a_, s_, t[:80]))
    if args.cuts:
        data_cuts(rows, refs, span_ok, dsyms, args.cuts_out or os.path.join(OUT_DIR, "cuts_%s.csv" % args.cut_set),
                  args.cut_set, [int(x, 16) for x in args.cut_starts], bool(args.cuts_far),
                  os.path.join(OUT_DIR, "final.csv"), args.cuts_no_common)
    if args.md:
        print()
        print("| # | First function of the new file lies in | Funcs | .rodata block | Pad | Kind | Grade | "
              "B-str / B-dat / B-call | Step secs / inv. |")
        print("|---|---|---|---|---|---|---|---|---|")
        k = 0
        for z in seams:
            if z["grade"] == "Conflict":
                continue
            k += 1
            win = "0x%08X-0x%08X" % (z["win_lo"], z["win_hi"])
            if z["win_lo"] == z["win_hi"]:
                win = "0x%08X (fixed)" % z["win_hi"]
            bd = z["bind"]
            print("| %d | %s | %d | 0x%08X | %d | %s | %s | %d / %d / %d | %d / %d |" % (
                k, win, z["win_funcs"], z["rodata"], z["pad"], z["kind"], z["grade"],
                bd.get("B-str", 0), bd.get("B-dat", 0), bd.get("B-call", 0), z["nsupport"], z["ninv"]))
        print()
        print("Conflicts (a 16-aligned .rodata item inside one function's data): " + ", ".join(
            "0x%08X (%s, %08X)" % (z["rodata"], z["kind"], z["first_func"]) for z in seams if z["grade"] == "Conflict"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
