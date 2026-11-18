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
Usage:  .venv\\Scripts\\python.exe tools/seams.py [--md] [--span N] [--k N] [--out DIR]
  --md    also print the evidence R table
  --span  LOCAL_SPAN; --k  STEP_K; --out  output folder (the sensitivity runs of docs/game_seams.md 2 use
          --out build/seams/var_<span>_<k>)
The final grades in docs/game_seams.md 4 add stability over those runs to the tool's own grade.
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


def main() -> int:
    global LOCAL_SPAN, STEP_K, OUT_DIR
    ap = argparse.ArgumentParser()
    ap.add_argument("--md", action="store_true")
    ap.add_argument("--span", type=int, default=LOCAL_SPAN, help="LOCAL_SPAN (functions)")
    ap.add_argument("--k", type=int, default=STEP_K, help="STEP_K (functions on each side)")
    ap.add_argument("--out", default=OUT_DIR, help="output folder (default build/seams)")
    args = ap.parse_args()
    LOCAL_SPAN, STEP_K, OUT_DIR = args.span, args.k, args.out
    if not os.path.exists(BASE_ELF):
        print("seams: missing %s" % BASE_ELF)
        return 1
    funcs, dsyms, symsrc = load_symbols()
    elf = Elf(open(BASE_ELF, "rb").read())
    img = census.Image(elf)
    fstarts = {a for a, _, _ in funcs}
    game = [f for f in funcs if GAME_LO <= f[0] < GAME_HI]
    rows = [census.analyse(nm, a, sz, img, fstarts, "game") for a, sz, nm in game]
    n = len(rows)
    idx_of = {r["addr"]: i for i, r in enumerate(rows)}
    dstarts = [d[0] for d in dsyms]

    def label_of(a):
        i = bisect.bisect_right(dstarts, a) - 1
        if i >= 0 and a < dsyms[i][0] + max(dsyms[i][1], 1):
            return dsyms[i][0]
        return a

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
        rec = {"b": b, "bind_p": 0, "inv_p": 0, "bind_s": 0, "inv_s": 0, "nl": 0, "nr": 0, "call": 0}
        for s in PRIMARY + SECONDARY:
            Ls = set().union(*(flabs[i][s] for i in range(L0, b)))
            Rs = set().union(*(flabs[i][s] for i in range(b, R1)))
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
        rec["plausible"] = rec["bind_p"] == 0 and rec["inv_p"] == 0 and rec["call"] == 0
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
    print("file-name-like strings: %d" % len(names))
    for a_, s_, t in names[:40]:
        print("   %08X %s %r" % (a_, s_, t[:80]))
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
