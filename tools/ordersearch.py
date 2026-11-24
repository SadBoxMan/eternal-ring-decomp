#!/usr/bin/env python3
"""ER-117 / ER-176: register-blind scoring and statement-order search for near-miss functions (one tool, nine modes).

  python -I tools/ordersearch.py score    a.c [b.c ...] [--func F] [--diff] [--regs] [--drift]
  python -I tools/ordersearch.py perm     a.c [--lines A-B | --text F] [--before X,Y ...] [--samples N] [--all]
  python -I tools/ordersearch.py climb    a.c [--lines A-B ... | --text F ...] [--before X,Y ...] [--restarts N]
  python -I tools/ordersearch.py switch   a.c [--lines A-B] [--key REGEX] [--search climb|all] [--restarts N]
  python -I tools/ordersearch.py variants t.c [--search auto|all|climb] [--restarts N] [order blocks]
  python -I tools/ordersearch.py decl     a.c [--lines A-B] [--samples N]
  python -I tools/ordersearch.py group    a.c --prefixes 'col.,uv.,v[,q['
  python -I tools/ordersearch.py sub      a.c OLD NEW [OLD NEW ...] [-o out.c]
  python -I tools/ordersearch.py dump     a.c [--passes sched|all] [--verbose 6]

score   compile and compare (tools/eematch.py, as tools/cmpfunc.py does), then the register-blind count: our and the
        game's instructions with registers, branch targets and callee names masked, relocated immediates masked (the
        game's from the function's splat asm file), aligned by difflib; "equal E/G" = E of the game's G instructions
        line up. --diff prints the aligned differing runs (game offsets), --regs keeps registers, --drift prints where
        the offset between ours and the game's changes. Several files: one line each (in parallel).
perm    every order of one block of lines (--lines, or the lines between a `/*ORDER*/` line and an `/*END*/` line),
        or --samples N random orders (the first is as written) when there are too many; --all counts every MATCH.
climb   steepest ascent over single-line moves, block by block (each --lines / marker block in turn), until no move
        of any block improves the score.
switch  one statement order shared by every `case` of a switch in the block: lines whose --key (default: the left
        side of `x = ...;`) is found once in every case are reordered together; other lines keep their place.
variants a template: each `@{a@|b@|c@}` group picks one alternative (`@name{..@}` groups share one choice), together
        with the template's order blocks; every combination (--search all) or a climb over them.
        climb / switch / variants climbs: --restarts N adds N climbs from random starts (seeded by --seed).
decl   random orders of the function's local declarations (default block: after the opening `{` of --func up to
        the first blank line).
group   sort each run of lines that start with one of --prefixes by prefix, for every order of the prefixes.
sub     apply string replacements (variable reuse, types, spellings) to a copy and score it.
dump    gcc's scheduler dumps (-dS -dR, or every pass with -da) and -fsched-verbose, with the build's flags.

Scores (lower is better, compared as tuples): `words` (default) = (size differs, differing words in --range,
differing words); `noreg` = (size differs, game instructions not equal register-blind, differing words). Ties keep the
earlier candidate (as written first), so a run is deterministic for a given --seed and --par. The input file is never
changed: candidates are compiled from --out (default `<input dir>/<stem>_os/`), the best order is written to
`best.c`, the first MATCH to `match.c`, every improvement to `log.txt`.

A masked compare is a working aid, never proof (ruling docs/decomp_recipes.md 3.1): place the function and run
`make regress`. Usage notes: docs/matching_tools.md 8.
"""
from __future__ import annotations

import argparse
import concurrent.futures as cf
import difflib
import itertools
import math
import os
import random
import re
import sys
import threading
import time
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import eematch  # noqa: E402

MARK_START = "/*ORDER*/"
MARK_END = "/*END*/"
BIG = 10 ** 6


# --- register-blind listing -----------------------------------------------------------------------------------

def asm_reloc_offsets(func: str) -> set[int] | None:
    """Offsets (from the function start) of the game's relocated words, read from the function's splat asm file
    (`%hi(` / `%lo(` / `gp_rel` / `li.s` lines). None when no asm file is found (a placed function)."""
    hits = sorted((eematch.ROOT / "asm" / "nonmatchings").glob(f"*/{func}.s"))
    if not hits:
        return None
    offs, base = set(), None
    for line in hits[0].read_text(errors="replace").splitlines():
        m = re.match(r"\s*/\* \w+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(.*)", line)
        if not m:
            continue
        a = int(m.group(1), 16)
        if base is None:
            base = a
        if re.search(r"%(hi|lo)\(|gp_rel|li\.s", m.group(2)):
            offs.add(a - base)
    return offs


def _norm(text: str, regs: bool) -> str:
    s = re.sub(r"\s+", " ", text.strip())
    s = re.sub(r"\. \+ 4 \+ \(.*?\)", "L", s)
    s = re.sub(r"func_\w+", "F", s)
    s = re.sub(r"(lui \$\w+), 0x\w+", r"\1, HI", s)
    if not regs:
        s = re.sub(r"\$\w+", "R", s)
    return s


def _maskimm(s: str) -> str:
    s = re.sub(r"-?0x[0-9A-Fa-f]+(\(\$?\w+\))", r"LO\1", s)
    return re.sub(r", -?0x[0-9A-Fa-f]+$", ", LO", s)


RELOC_WORDS = ("LO16", "HI16", "GPREL", "LITERAL")


def blind_lists(res: eematch.FuncResult, game_relocs: set[int] | None, regs: bool = False):
    """(ours, game): lists of (offset, normalised text). Our relocated immediates are masked from our relocations;
    the game's from its asm file, or, with no asm file, at the offsets where ours has a relocation (approximate)."""
    ours, game = [], []
    for w in res.words:
        if w.ours is None and w.game is None:      # an extra "HI16 differs from its pair" entry
            continue
        reloc = any(k in w.note for k in RELOC_WORDS)
        if w.ours is not None:
            x = _norm(eematch.disasm(w.ours, res.addr + w.off), regs)
            ours.append((w.off, _maskimm(x) if reloc else x))
        if w.game is not None:
            y = _norm(eematch.disasm(w.game, res.addr + w.off), regs)
            g_reloc = (w.off in game_relocs) if game_relocs is not None else reloc
            game.append((w.off, _maskimm(y) if g_reloc else y))
    return ours, game


def blind_equal(ours, game) -> tuple[int, difflib.SequenceMatcher]:
    sm = difflib.SequenceMatcher(None, [t for _, t in ours], [t for _, t in game], autojunk=False)
    return sum(b.size for b in sm.get_matching_blocks()), sm


def headline(res: eematch.FuncResult) -> str:
    return eematch.report(res, 0, False).splitlines()[0]


def diff_text(ours, game, sm) -> list[str]:
    out = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            continue
        oa = f"+{ours[i1][0]:04x}" if i1 < len(ours) else "end"
        ga = f"+{game[j1][0]:04x}" if j1 < len(game) else "end"
        out.append(f"---- {tag} ours {oa}.. game {ga}..")
        for k in range(max(i2 - i1, j2 - j1)):
            a = ours[i1 + k][1] if i1 + k < i2 else ""
            b = game[j1 + k][1] if j1 + k < j2 else ""
            out.append(f"   {a:<44} | {b}")
    return out


def drift_text(ours, game, sm) -> list[str]:
    out, last = [], None
    for b in sm.get_matching_blocks():
        if b.size == 0:
            continue
        d = b.a - b.b
        if d != last:
            out.append(f"ours +{ours[b.a][0]:04x} game +{game[b.b][0]:04x} len {b.size} delta {d:+d} words")
            last = d
    return out


# --- compile and score ----------------------------------------------------------------------------------------

@dataclass
class Score:
    key: tuple
    match: bool
    head: str
    words: int = 0
    equal: int = 0
    game_len: int = 0


class Scorer:
    def __init__(self, func: str | None, addr: int | None, out_dir: Path, inc_dir: Path, metric: str = "words",
                 rng: tuple[int, int] | None = None, keep: bool = False):
        eematch.settings()                          # read the Makefile once, before any thread
        self.game = eematch.Game()
        self.func, self.addr = func, addr
        self.metric, self.rng, self.keep = metric, rng, keep
        self.cdir = out_dir / "c"
        self.cdir.mkdir(parents=True, exist_ok=True)
        self.inc = ["-I" + inc_dir.resolve().as_posix()]
        self.cache: dict[str, Score] = {}
        self.lock = threading.Lock()
        self.count = itertools.count()
        self.compiles = 0
        self._relocs: set[int] | None | bool = False

    def game_relocs(self) -> set[int] | None:
        if self._relocs is False:
            self._relocs = asm_reloc_offsets(self.func)
        return self._relocs

    def compile_obj(self, text: str) -> tuple[eematch.Obj | None, str, list[Path]]:
        n = next(self.count)
        c = self.cdir / f"os{os.getpid()}_{n}.c"
        c.write_text(text, newline="\n")
        junk = [c, eematch.SCRATCH / f"{c.stem}.s", eematch.SCRATCH / f"{c.stem}.o"]
        try:
            return eematch.Obj(eematch.compile_c(c, self.inc)), "", junk
        except SystemExit as e:
            return None, str(e), junk

    def _clean(self, junk: list[Path]) -> None:
        if self.keep:
            return
        for p in junk:
            try:
                p.unlink()
            except OSError:
                pass

    def result(self, text: str) -> tuple[eematch.FuncResult | None, str]:
        """Compile text and compare self.func; (FuncResult, '') or (None, error message)."""
        obj, err, junk = self.compile_obj(text)
        try:
            if obj is None:
                return None, err
            if self.func is None:
                names = [nm for nm, _o, _s in obj.functions() if eematch.ADDR_NAME.match(nm)]
                if len(names) != 1:
                    return None, f"name the function with --func (the file defines: {', '.join(names) or 'none'})"
                self.func = names[0]
            try:
                return eematch.compare_function(obj, self.game, self.func, self.addr), ""
            except SystemExit as e:
                return None, str(e)
        finally:
            self._clean(junk)

    def score(self, text: str) -> Score:
        with self.lock:
            if text in self.cache:
                return self.cache[text]
        res, err = self.result(text)
        if res is None:
            msg = err.strip().splitlines()[-1] if err.strip() else "?"
            sc = Score((BIG, BIG, BIG), False, "COMPILE ERROR: " + msg)
        else:
            words = sum(not w.ok for w in res.words)
            size_pen = int(res.game_size is not None and res.our_size != res.game_size)
            if self.rng:
                local = sum(1 for w in res.words if not w.ok and self.rng[0] <= w.off < self.rng[1])
            else:
                local = words
            eq, glen = 0, 0
            if self.metric == "noreg":
                ours, game = blind_lists(res, self.game_relocs())
                eq, _sm = blind_equal(ours, game)
                glen = len(game)
                key = (size_pen, glen - eq, words)
            else:
                key = (size_pen, local, words)
            sc = Score((0, 0, 0) if res.match else key, res.match, headline(res), words, eq, glen)
        with self.lock:
            self.cache[text] = sc
            self.compiles += 1
        return sc


# --- blocks of lines ------------------------------------------------------------------------------------------

def parse_range(s: str) -> tuple[int, int]:
    a, b = s.split("-")
    return int(a), int(b)


def text_block(lines: list[str], path: str) -> tuple[int, int]:
    """The block whose lines are exactly the non-blank lines of file `path` (compared without surrounding
    whitespace), found once, consecutive, in `lines` (ER-100's permg.py named its block this way)."""
    want = [l.strip() for l in Path(path).read_text(errors="replace").splitlines() if l.strip()]
    if not want:
        raise SystemExit(f"ordersearch: --text {path}: no lines")
    have = [l.strip() for l in lines]
    hits = [i for i in range(len(have) - len(want) + 1) if have[i:i + len(want)] == want]
    if len(hits) != 1:
        raise SystemExit(f"ordersearch: --text {path}: its {len(want)} line(s) found {len(hits)} times "
                         "(need exactly once, consecutive)")
    return hits[0] + 1, hits[0] + len(want)


def find_blocks(lines: list[str], specs: list[str] | None, texts: list[str] | None = None) -> list[tuple[int, int]]:
    """Blocks as (first, last) 1-based inclusive line numbers: from --lines and --text, else every /*ORDER*/ ...
    /*END*/ pair (the lines strictly between the two marker lines)."""
    if specs or texts:
        blocks = [parse_range(s) for s in (specs or [])] + [text_block(lines, t) for t in (texts or [])]
        blocks.sort()
        for (a1, b1), (a2, b2) in zip(blocks, blocks[1:]):
            if a2 <= b1:
                raise SystemExit(f"ordersearch: blocks {a1}-{b1} and {a2}-{b2} overlap")
    else:
        blocks, start = [], None
        for i, l in enumerate(lines, 1):
            if l.strip() == MARK_START:
                start = i + 1
            elif l.strip() == MARK_END and start is not None:
                blocks.append((start, i - 1))
                start = None
    for a, b in blocks:
        if not (1 <= a <= b <= len(lines)):
            raise SystemExit(f"ordersearch: bad block {a}-{b} (file has {len(lines)} lines)")
    return blocks


def parse_before(specs: list[str] | None) -> list[tuple[int, int]]:
    return [tuple(int(x) for x in s.split(",")) for s in (specs or [])]


def ok_order(order: list[int], cons: list[tuple[int, int]]) -> bool:
    pos = {ln: i for i, ln in enumerate(order)}
    return not any(x in pos and y in pos and pos[x] > pos[y] for x, y in cons)


def assemble(lines: list[str], orders: dict[tuple[int, int], list[int]]) -> str:
    """The file with each block's lines replaced by the given order of its line numbers."""
    out, i = [], 1
    starts = {a: (a, b) for a, b in orders}
    while i <= len(lines):
        if i in starts:
            a, b = starts[i]
            out += [lines[k - 1] for k in orders[(a, b)]]
            i = b + 1
        else:
            out.append(lines[i - 1])
            i += 1
    return "\n".join(out) + "\n"


def moves(order: list) -> list[list]:
    """Every order reachable by moving one element to another place (no duplicates)."""
    n, out = len(order), []
    for i in range(n):
        for j in range(n):
            if i != j and j != i + 1:
                c = order[:]
                x = c.pop(i)
                c.insert(j if j < i else j - 1, x)
                out.append(c)
    return out


# --- the search driver ----------------------------------------------------------------------------------------

class Run:
    def __init__(self, args, path: Path):
        self.args, self.path = args, path
        self.text = path.read_text(errors="replace")
        self.lines = self.text.split("\n")
        if self.lines and self.lines[-1] == "":
            self.lines.pop()
        out = Path(args.out) if args.out else path.parent / f"{path.stem}_os"
        out.mkdir(parents=True, exist_ok=True)
        self.out = out
        rng = None
        if getattr(args, "range", None):
            lo, hi = args.range.split(":")
            rng = (int(lo, 16), int(hi, 16))
        self.sc = Scorer(args.func, int(args.addr, 16) if args.addr else None, out, path.parent, args.metric, rng,
                         args.keep)
        self.log = open(out / "log.txt", "a", encoding="utf-8", newline="\n")
        self.t0 = time.time()
        self.best: tuple[tuple, str, str] | None = None     # (key, text, label)
        self.matches = 0
        self.match_written = False

    def say(self, s: str) -> None:
        print(s, flush=True)
        self.log.write(s + "\n")
        self.log.flush()

    def start(self, mode: str, text: str | None = None) -> Score:
        """Score the starting text (default: the input as written) and write the log header."""
        if text is None:
            text = self.text if self.text.endswith("\n") else self.text + "\n"
        base = self.sc.score(text)
        if base.key[0] == BIG:
            raise SystemExit(f"ordersearch: the input does not compile or compare: {base.head}")
        self.say(f"== {time.strftime('%Y-%m-%d %H:%M:%S')} {mode} {self.path} func {self.sc.func} metric "
                 f"{self.args.metric} seed {self.args.seed} par {self.args.par}")
        self.say(f"as written: {list(base.key)}  {base.head}")
        return base

    def offer(self, key: tuple, text: str, label: str, match: bool) -> None:
        """Record a scored candidate (called in candidate order, so ties keep the earlier one)."""
        if match:
            self.matches += 1
            if not self.match_written:
                (self.out / "match.c").write_text(text, newline="\n")
                self.match_written = True
                self.say(f"MATCH {label}")
            elif getattr(self.args, "all", False):
                self.say(f"MATCH {label}")
        if self.best is None or key < self.best[0]:
            self.best = (key, text, label)
            (self.out / "best.c").write_text(text, newline="\n")
            self.say(f"best {list(key)} {label}")

    def evaluate(self, cands: list[tuple[str, str]], stop_on_match: bool = True) -> list[Score]:
        """Score (label, text) candidates with --par threads; results in candidate order."""
        res: list[Score] = []
        with cf.ThreadPoolExecutor(self.args.par) as ex:
            for chunk_start in range(0, len(cands), 64 * self.args.par):
                chunk = cands[chunk_start:chunk_start + 64 * self.args.par]
                for (label, text), s in zip(chunk, ex.map(lambda c: self.sc.score(c[1]), chunk)):
                    res.append(s)
                    self.offer(s.key, text, label, s.match)
                if stop_on_match and self.matches and not self.args.all:
                    break
        return res

    def finish(self) -> int:
        k, _t, label = self.best
        self.say(f"final {list(k)} {label}  compiles {self.sc.compiles}, {time.time() - self.t0:.0f} s"
                 + (f", {self.matches} MATCH order(s) found" if self.matches else "")
                 + f"; best.c in {self.out}")
        self.log.close()
        return 0 if self.matches else 1


# --- modes ----------------------------------------------------------------------------------------------------

def mode_score(args) -> int:
    files = [Path(f) for f in args.files]
    sc = Scorer(args.func, int(args.addr, 16) if args.addr else None,
                Path(args.out) if args.out else eematch.ROOT / "build" / "ordersearch", files[0].parent,
                "noreg", None, args.keep)
    single = len(files) == 1

    def one(f: Path):
        s = Scorer.__new__(Scorer)            # share the game and settings, own function name per file
        s.__dict__.update(sc.__dict__)
        s.inc = ["-I" + f.parent.resolve().as_posix()]
        res, err = s.result(f.read_text(errors="replace"))
        if res is None:
            return f, None, err, None
        ours, game = blind_lists(res, asm_reloc_offsets(s.func), args.regs)
        eq, sm = blind_equal(ours, game)
        return f, res, (ours, game, sm), eq

    rc = 0
    with cf.ThreadPoolExecutor(args.par) as ex:
        for f, res, extra, eq in ex.map(one, files):
            if res is None:
                print(f"{f.name}: COMPILE ERROR\n{extra}" if single else f"{f.name}: ERROR {extra.strip()[:200]}")
                rc = max(rc, 2)
                continue
            ours, game, sm = extra
            tag = "" if single else f"{f.name}: "
            blind = "register-blind" if not args.regs else "with registers"
            relnote = "" if asm_reloc_offsets(res.name) is not None else " (no asm file: game immediates masked " \
                                                                          "where ours has a relocation)"
            print(f"{tag}{headline(res)} | equal {eq}/{len(game)} (ours {len(ours)}) {blind}{relnote}")
            if not res.match:
                rc = max(rc, 1)
            if args.diff:
                print("\n".join(diff_text(ours, game, sm)))
            if args.drift:
                print("\n".join(drift_text(ours, game, sm)))
    return rc


def mode_perm(args, decl: bool = False) -> int:
    r = Run(args, Path(args.file))
    if decl and not args.lines:
        blocks = [decl_block(r.lines, args.func)]
    else:
        blocks = find_blocks(r.lines, args.lines, getattr(args, "text", None))
    if len(blocks) != 1:
        raise SystemExit(f"ordersearch: {'decl' if decl else 'perm'} takes exactly one block "
                         f"(found {len(blocks)}); use climb for several")
    blk = blocks[0]
    cons = parse_before(args.before)
    r.start("decl" if decl else "perm")
    items = list(range(blk[0], blk[1] + 1))
    total = math.factorial(len(items))
    samples = args.samples if args.samples else (200 if decl else 0)
    if not samples and total > args.max:
        raise SystemExit(f"ordersearch: {total} orders of {len(items)} lines (over --max {args.max}): "
                         "pass --samples N for random orders, or use climb")
    if samples:
        rnd = random.Random(args.seed)
        orders, seen = [items], {tuple(items)}
        tries = 0
        while len(orders) < min(samples, total) and tries < samples * 50:
            tries += 1
            o = items[:]
            rnd.shuffle(o)
            if tuple(o) not in seen and ok_order(o, cons):
                seen.add(tuple(o))
                orders.append(o)
    else:
        orders = [list(p) for p in itertools.permutations(items) if ok_order(list(p), cons)]
    r.say(f"block {blk[0]}-{blk[1]}: {len(items)} lines, {len(orders)} order(s) to try"
          + (f" (random, seed {args.seed})" if samples else ""))
    cands = [(f"order {o}", assemble(r.lines, {blk: o})) for o in orders]
    r.evaluate(cands)
    return r.finish()


def decl_block(lines: list[str], func: str | None) -> tuple[int, int]:
    pat = re.compile(r"^\S.*\b(" + (re.escape(func) if func else r"func_[0-9A-F]{8}") + r")\s*\(.*\)\s*\{\s*$")
    heads = [i for i, l in enumerate(lines, 1) if pat.match(l)]
    if len(heads) != 1:
        raise SystemExit("ordersearch: decl: cannot find one function definition line ending in '{' "
                         f"({len(heads)} found); pass --func or --lines")
    a = heads[0] + 1
    b = a
    while b <= len(lines) and lines[b - 1].strip():
        b += 1
    if b == a:
        raise SystemExit("ordersearch: decl: no declaration lines after the opening brace")
    return a, b - 1


def random_order(items: list, cons: list[tuple[int, int]], rnd: random.Random, tries: int = 2000) -> list:
    """A random order of items that keeps every --before constraint (rejection sampling; falls back to a random
    linear extension built one free item at a time, so it always succeeds when the constraints allow any order)."""
    for _ in range(tries):
        o = items[:]
        rnd.shuffle(o)
        if ok_order(o, cons):
            return o
    left, out = items[:], []
    while left:
        free = [x for x in left if not any(y in left and y != x and (y, x) in cons for y in left)]
        if not free:
            raise SystemExit("ordersearch: the --before constraints form a cycle")
        x = rnd.choice(free)
        out.append(x)
        left.remove(x)
    return out


def restart_loop(r: Run, args, climb_from, random_start) -> None:
    """Run climb_from(start_state, start_key, label) from the as-written start, then from --restarts random starts
    (random_start(rnd) -> state, text); stop at the first MATCH. Each restart is announced in the log."""
    climb_from(None, None, "as written")
    rnd = random.Random(args.seed)
    for i in range(1, args.restarts + 1):
        if r.matches:
            break
        state, text = random_start(rnd)
        s = r.sc.score(text)
        r.say(f"restart {i}/{args.restarts}: start {list(s.key)}")
        r.offer(s.key, text, f"restart {i} start", s.match)
        if s.match:
            break
        climb_from(state, s.key, f"restart {i}")


def mode_climb(args) -> int:
    r = Run(args, Path(args.file))
    blocks = find_blocks(r.lines, args.lines, args.text)
    if not blocks:
        raise SystemExit("ordersearch: climb: no block (--lines A-B, --text FILE or /*ORDER*/ ... /*END*/)")
    cons = parse_before(args.before)
    base = r.start("climb")
    written = {b: list(range(b[0], b[1] + 1)) for b in blocks}
    r.offer(base.key, assemble(r.lines, written), "as written", base.match)

    def climb_from(start, key, tag):
        orders = dict(start) if start else dict(written)
        cur = key if key is not None else base.key
        climb_blocks(r, args, blocks, orders, cons, cur, tag)

    def random_start(rnd):
        o = {b: random_order(written[b], cons, rnd) for b in blocks}
        return o, assemble(r.lines, o)

    if not base.match:
        restart_loop(r, args, climb_from, random_start)
    return r.finish()


def climb_blocks(r: Run, args, blocks, orders, cons, cur, tag: str) -> tuple:
    """Steepest ascent over single-line moves, block by block, passes until none improves (ER-117's climb)."""
    rounds = 0
    pre = "" if tag == "as written" else f"{tag}: "
    while not r.matches and rounds < args.rounds:
        rounds += 1
        improved = False
        for blk in blocks:
            while True:
                cands = []
                for o in moves(orders[blk]):
                    if ok_order(o, cons):
                        trial = dict(orders)
                        trial[blk] = o
                        cands.append((o, assemble(r.lines, trial)))
                if not cands:
                    break
                with cf.ThreadPoolExecutor(args.par) as ex:
                    scores = list(ex.map(lambda c: r.sc.score(c[1]), cands))
                k = min(range(len(cands)), key=lambda q: scores[q].key)
                if scores[k].key < cur:
                    orders[blk] = cands[k][0]
                    cur = scores[k].key
                    improved = True
                    r.offer(cur, cands[k][1], f"{pre}block {blk[0]}-{blk[1]} order {cands[k][0]}",
                            scores[k].match)
                    if scores[k].match:
                        break
                else:
                    break
            if r.matches:
                break
        if not improved:
            break
    if tag != "as written":
        r.say(f"{tag}: stopped at {list(cur)}")
    return cur


CASE_RE = re.compile(r"^\s*(case\b.*|default\s*):\s*$")


def mode_switch(args) -> int:
    r = Run(args, Path(args.file))
    blocks = find_blocks(r.lines, args.lines)
    if len(blocks) != 1:
        raise SystemExit("ordersearch: switch takes one block holding the case labels (--lines or markers)")
    a, b = blocks[0]
    key_re = re.compile(args.key)
    cases: list[list[int]] = []              # line numbers of each case body
    for i in range(a, b + 1):
        if CASE_RE.match(r.lines[i - 1]):
            cases.append([])
        elif cases:
            cases[-1].append(i)
    if len(cases) < 2:
        raise SystemExit(f"ordersearch: switch: {len(cases)} case label(s) in lines {a}-{b}")
    keyed: list[dict[str, int]] = []
    for body in cases:
        d, dup = {}, set()
        for ln in body:
            m = key_re.match(r.lines[ln - 1])
            if m:
                k = m.group(1)
                if k in d:
                    dup.add(k)
                d[k] = ln
        for k in dup:
            d.pop(k)
        keyed.append(d)
    common = [k for k in keyed[0] if all(k in d for d in keyed[1:])]
    if len(common) < 2:
        raise SystemExit(f"ordersearch: switch: {len(common)} key(s) common to every case (--key {args.key!r})")
    base = r.start("switch")
    r.say(f"{len(cases)} cases, shared keys in the first case's order: {common}")
    slots = [sorted(d[k] for k in common) for d in keyed]     # where each case's shared lines sit

    def build(order: list[str]) -> str:
        new = r.lines[:]
        for ci, sl in enumerate(slots):
            for ln, k in zip(sl, order):
                new[ln - 1] = r.lines[keyed[ci][k] - 1]
        return "\n".join(new) + "\n"

    cur_o = common[:]
    r.offer(base.key, build(cur_o), f"order {cur_o}", base.match)
    if args.search == "all":
        total = math.factorial(len(common))
        if total > args.max:
            raise SystemExit(f"ordersearch: {total} orders (over --max {args.max}); use --search climb")
        r.evaluate([(f"order {list(p)}", build(list(p))) for p in itertools.permutations(common)])
    elif not base.match:
        def climb_from(start, key, tag):
            steepest(r, args, start or cur_o, key if key is not None else base.key,
                     lambda o: [(m, build(m), f"order {m}") for m in moves(o)], tag)

        def random_start(rnd):
            o = common[:]
            rnd.shuffle(o)
            return o, build(o)

        restart_loop(r, args, climb_from, random_start)
    return r.finish()


def steepest(r: Run, args, state, cur: tuple, neigh, tag: str) -> tuple:
    """Steepest ascent: neigh(state) -> [(state, text, label)]; move to the best neighbour while it beats cur
    (ties keep the earlier neighbour). Returns the final key."""
    pre = "" if tag == "as written" else f"{tag}: "
    while not r.matches:
        cands = neigh(state)
        if not cands:
            break
        with cf.ThreadPoolExecutor(args.par) as ex:
            scores = list(ex.map(lambda c: r.sc.score(c[1]), cands))
        k = min(range(len(cands)), key=lambda q: scores[q].key)
        if scores[k].key >= cur:
            break
        state, cur = cands[k][0], scores[k].key
        r.offer(cur, cands[k][1], pre + cands[k][2], scores[k].match)
    if tag != "as written":
        r.say(f"{tag}: stopped at {list(cur)}")
    return cur


VAR_RE = re.compile(r"@(\w*)\{(.*?)@\}")


@dataclass
class VarSlot:
    line: int            # 0-based line index in the template
    start: int
    end: int
    var: int             # index of the variable (named slots share one)
    alts: list[str]      # this slot's own alternatives (a shared variable picks the same index in every slot)


def parse_variants(lines: list[str]):
    """Find every `@{a@|b@|c@}` / `@name{a@|b@}` group (one line each). Returns (slots, variables) where variables
    is a list of (name, first slot's alternatives); every slot of one name shares one variable, i.e. one choice
    index (each slot keeps its own texts and must list the same number of alternatives). Alternative 0 is the
    as-written one."""
    slots: list[VarSlot] = []
    vars_: list[tuple[str, list[str]]] = []
    by_name: dict[str, int] = {}
    for i, line in enumerate(lines):
        for m in VAR_RE.finditer(line):
            name, alts = m.group(1), m.group(2).split("@|")
            if len(alts) < 2:
                raise SystemExit(f"ordersearch: variants: line {i + 1}: a group needs two or more alternatives "
                                 f"(separated by '@|'): {m.group(0)!r}")
            if name and name in by_name:
                v = by_name[name]
                if len(vars_[v][1]) != len(alts):
                    raise SystemExit(f"ordersearch: variants: line {i + 1}: group '{name}' has {len(alts)} "
                                     f"alternatives here, {len(vars_[v][1])} at its first use")
            else:
                v = len(vars_)
                vars_.append((name or f"L{i + 1}#{sum(1 for s in slots if s.line == i) + 1}", alts))
                if name:
                    by_name[name] = v
            slots.append(VarSlot(i, m.start(), m.end(), v, alts))
        rest = VAR_RE.sub("", line)
        if re.search(r"@\w*\{|@\}|@\|", rest):
            raise SystemExit(f"ordersearch: variants: line {i + 1}: an unclosed or stray '@{{' / '@|' / '@}}' "
                             "(a group must open and close on one line)")
    return slots, vars_


def mode_variants(args) -> int:
    path = Path(args.file)
    r = Run(args, path)
    slots, vars_ = parse_variants(r.lines)
    if not vars_:
        raise SystemExit("ordersearch: variants: no '@{a@|b@}' group in the file")
    # the order blocks are found on the template's lines (markers or --lines / --text); a group may sit inside one
    marker_lines = [l for l in r.lines if l.strip() in (MARK_START, MARK_END)]
    blocks = find_blocks(r.lines, args.lines, args.text) if (args.lines or args.text or marker_lines) else []
    cons = parse_before(args.before)
    slots_by_line: dict[int, list[VarSlot]] = {}
    for s in slots:
        slots_by_line.setdefault(s.line, []).append(s)

    def render(choice: tuple, orders: dict) -> str:
        lines = r.lines[:]
        for i, sl in slots_by_line.items():
            line, out, pos = lines[i], [], 0
            for s in sl:
                out += [line[pos:s.start], s.alts[choice[s.var]]]
                pos = s.end
            lines[i] = "".join(out) + line[pos:]
        return assemble(lines, orders) if orders else "\n".join(lines) + "\n"

    def label(choice: tuple, orders: dict) -> str:
        parts = [f"{vars_[v][0]}={c}:{vars_[v][1][c]!r}" for v, c in enumerate(choice) if c]
        parts += [f"block {a}-{b} order {o}" for (a, b), o in orders.items() if o != list(range(a, b + 1))]
        return "; ".join(parts) or "as written"

    written_c = tuple(0 for _ in vars_)
    written_o = {b: list(range(b[0], b[1] + 1)) for b in blocks}
    n_choice = math.prod(len(a) for _n, a in vars_)
    block_orders = {b: None for b in blocks}
    n_orders = 1
    for b in blocks:
        n_orders *= math.factorial(b[1] - b[0] + 1)
    total = n_choice * n_orders
    search = args.search
    if search == "auto":
        search = "all" if total <= args.max else "climb"
    base = r.start("variants", render(written_c, written_o))
    r.say(f"{len(vars_)} variable(s) in {len(slots)} group(s): "
          + ", ".join(f"{n}({len(a)})" for n, a in vars_)
          + (f"; block(s) {', '.join(f'{a}-{b}' for a, b in blocks)}" if blocks else "")
          + f"; {total} combination(s); search {search}")
    r.offer(base.key, render(written_c, written_o), "as written", base.match)
    if base.match:
        return r.finish()

    if search == "all":
        if total > args.max:
            raise SystemExit(f"ordersearch: {total} combinations (over --max {args.max}); use --search climb")
        for b in blocks:
            block_orders[b] = [list(p) for p in itertools.permutations(written_o[b]) if ok_order(list(p), cons)]
        choices = list(itertools.product(*[range(len(a)) for _n, a in vars_]))
        order_sets = [dict(zip(blocks, os_)) for os_ in itertools.product(*[block_orders[b] for b in blocks])] \
            if blocks else [{}]
        cands = []
        for c in choices:
            for o in order_sets:
                cands.append((label(c, o), render(c, o)))
        r.evaluate(cands)
        return r.finish()

    def neigh(state):
        c, o = state
        out = []
        for v, (_n, alts) in enumerate(vars_):
            for k in range(len(alts)):
                if k != c[v]:
                    c2 = c[:v] + (k,) + c[v + 1:]
                    out.append(((c2, o), render(c2, o), label(c2, o)))
        for b in blocks:
            for m in moves(o[b]):
                if ok_order(m, cons):
                    o2 = dict(o)
                    o2[b] = m
                    out.append(((c, o2), render(c, o2), label(c, o2)))
        return out

    def climb_from(start, key, tag):
        steepest(r, args, start or (written_c, written_o), key if key is not None else base.key, neigh, tag)

    def random_start(rnd):
        c = tuple(rnd.randrange(len(a)) for _n, a in vars_)
        o = {b: random_order(written_o[b], cons, rnd) for b in blocks}
        return (c, o), render(c, o)

    restart_loop(r, args, climb_from, random_start)
    return r.finish()


def mode_group(args) -> int:
    r = Run(args, Path(args.file))
    prefixes = [p for p in args.prefixes.split(",") if p]
    if len(prefixes) < 2:
        raise SystemExit("ordersearch: group needs at least two --prefixes")

    def gkey(line: str, order: list[str]):
        s = line.strip()
        for p in order:
            if s.startswith(p):
                return order.index(p)
        return None

    def build(order: list[str]) -> str:
        out, i, L = [], 0, r.lines
        while i < len(L):
            if gkey(L[i], prefixes) is not None:
                j = i
                while j < len(L) and gkey(L[j], prefixes) is not None:
                    j += 1
                out += sorted(L[i:j], key=lambda l: gkey(l, order))
                i = j
            else:
                out.append(L[i])
                i += 1
        return "\n".join(out) + "\n"

    r.start("group")
    perms = [list(p) for p in itertools.permutations(prefixes)]
    if len(perms) > args.max:
        raise SystemExit(f"ordersearch: {len(perms)} prefix orders (over --max {args.max})")
    r.say(f"{len(prefixes)} prefixes, {len(perms)} group orders")
    r.evaluate([(f"groups {p}", build(p)) for p in perms])
    return r.finish()


def mode_dump(args) -> int:
    """gcc RTL dumps with the Makefile's CC / CFLAGS / CPPFLAGS: a copy of the file is compiled in --out, so the
    dumps (named <file>.c.<pass>) land there; the input's folder is on the include path."""
    import shutil
    import subprocess
    src = Path(args.file).resolve()
    out = (Path(args.out) if args.out else src.parent / f"{src.stem}_dump").resolve()
    out.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(src, out / src.name)
    st = eematch.settings()
    flags = ["-da"] if args.passes == "all" else ["-dS", "-dR"]
    cc = [st["CC"], f"-B{st['GCC_DIR']}/", *eematch.scratch_cflags(src), *(args.cflags.split() if args.cflags else []),
          *[f.replace("-I", "-I" + eematch.ROOT.as_posix() + "/", 1) if f.startswith("-I") else f
            for f in st["CPPFLAGS"].split()],
          "-I" + src.parent.as_posix(), "-S", *flags, f"-fsched-verbose-{args.verbose}", src.name,
          "-o", src.stem + ".s"]
    r = subprocess.run(cc, cwd=out, capture_output=True, text=True, stdin=subprocess.DEVNULL)
    if r.returncode != 0:
        print(f"gcc failed:\n{r.stdout}{r.stderr}", file=sys.stderr)
        return 2
    made = sorted(p.name for p in out.iterdir() if p.name.startswith(src.name + ".") or p.name == src.stem + ".s")
    print(f"{out}: " + ", ".join(made))
    return 0


def mode_sub(args) -> int:
    path = Path(args.file)
    s = path.read_text(errors="replace")
    pairs = args.pairs
    if len(pairs) % 2:
        raise SystemExit("ordersearch: sub takes OLD NEW pairs")
    for old, new in zip(pairs[::2], pairs[1::2]):
        old = old.encode().decode("unicode_escape")
        new = new.encode().decode("unicode_escape")
        if old not in s:
            raise SystemExit(f"ordersearch: sub: not found: {old[:60]!r}")
        s = s.replace(old, new)
    outp = Path(args.o) if args.o else path.with_name(path.stem + ".sub.c")
    outp.write_text(s, newline="\n")
    print(f"wrote {outp}")
    args.files = [str(outp)]
    return mode_score(args)


# --- command line ---------------------------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0], epilog="\n".join(__doc__.splitlines()[1:]),
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="mode", required=True)

    def common(p, search=True):
        p.add_argument("--func", help="function to compare (default: the one func_XXXXXXXX the file defines)")
        p.add_argument("--addr", help="game address, for a function not named func_XXXXXXXX")
        p.add_argument("--par", type=int, default=4, help="parallel compiles (default 4)")
        p.add_argument("--out", help="work folder (default <input dir>/<stem>_os; score: build/ordersearch)")
        p.add_argument("--keep", action="store_true", help="keep candidate .c / .s / .o files")
        if search:
            p.add_argument("--metric", choices=("words", "noreg"), default="words",
                           help="words: differing words (default); noreg: register-blind unequal instructions")
            p.add_argument("--range", help="LO:HI game offsets (hex): count only differing words there (words metric)")
            p.add_argument("--seed", type=int, default=1, help="seed for random orders (default 1)")
            p.add_argument("--max", type=int, default=40320, help="most orders to enumerate (default 40320 = 8!)")
            p.add_argument("--all", action="store_true", help="do not stop at the first MATCH (count them all)")

    p = sub.add_parser("score", help="compile, compare, register-blind count / diff (one or many files)")
    p.add_argument("files", nargs="+")
    p.add_argument("--diff", action="store_true", help="print the aligned differing runs")
    p.add_argument("--regs", action="store_true", help="keep register names (not register-blind)")
    p.add_argument("--drift", action="store_true", help="print where the ours / game offset changes")
    common(p, search=False)

    text_help = ("FILE holding the block's exact lines (blank lines ignored, whitespace-trimmed), found once in the "
                 "input")
    restart_help = "after the climb, N more climbs from random starts (from --seed); default 0"

    p = sub.add_parser("perm", help="every (or N random) order(s) of one block of lines")
    p.add_argument("file")
    p.add_argument("--lines", action="append", help="A-B: 1-based inclusive line numbers (default: markers)")
    p.add_argument("--text", action="append", help=text_help)
    p.add_argument("--before", action="append", help="X,Y: line X must stay before line Y (file line numbers)")
    p.add_argument("--samples", type=int, default=0, help="N random orders instead of all (first: as written)")
    common(p)

    p = sub.add_parser("climb", help="steepest ascent over line moves, block by block")
    p.add_argument("file")
    p.add_argument("--lines", action="append", help="A-B, repeatable: one block each (default: markers)")
    p.add_argument("--text", action="append", help=text_help + "; repeatable")
    p.add_argument("--before", action="append", help="X,Y: line X must stay before line Y")
    p.add_argument("--rounds", type=int, default=20, help="most passes over all blocks (default 20)")
    p.add_argument("--restarts", type=int, default=0, help=restart_help)
    common(p)

    p = sub.add_parser("switch", help="one statement order shared by every case of a switch")
    p.add_argument("file")
    p.add_argument("--lines", action="append", help="A-B: the block holding the case labels (default: markers)")
    p.add_argument("--key", default=r"^\s*([^=;{}]+?)\s*=[^=]",
                   help="regex whose group 1 names a line (default: left side of an assignment)")
    p.add_argument("--search", choices=("climb", "all"), default="climb")
    p.add_argument("--restarts", type=int, default=0, help=restart_help + " (climb only)")
    common(p)

    p = sub.add_parser("variants", help="a template with alternatives (@{a@|b@}), plus optional order blocks",
                       description="Every group `@{a@|b@|c@}` on a line is replaced by one of its alternatives; "
                       "groups written `@name{...@}` share one choice wherever the name appears (variable reuse: "
                       "the declaration and every use). An alternative may be empty (`@{@|s32 t;@}`), so a "
                       "group can add or drop a line's text. Order blocks (/*ORDER*/ ... /*END*/, --lines, --text) "
                       "are searched together with the choices. --search all tries every combination; climb "
                       "changes one group or moves one line at a time; auto (default) is all when the combinations "
                       "fit in --max. The first alternative of each group is the as-written start.")
    p.add_argument("file")
    p.add_argument("--lines", action="append", help="A-B, repeatable: order blocks (default: markers, if any)")
    p.add_argument("--text", action="append", help=text_help + "; repeatable")
    p.add_argument("--before", action="append", help="X,Y: line X must stay before line Y (order blocks)")
    p.add_argument("--search", choices=("auto", "all", "climb"), default="auto")
    p.add_argument("--restarts", type=int, default=0, help=restart_help + " (climb only)")
    common(p)

    p = sub.add_parser("decl", help="random orders of the local declarations")
    p.add_argument("file")
    p.add_argument("--lines", action="append", help="A-B: the declaration lines (default: after the opening brace)")
    p.add_argument("--before", action="append", help="X,Y: line X must stay before line Y")
    p.add_argument("--samples", type=int, default=200, help="random orders to try (default 200)")
    common(p)

    p = sub.add_parser("group", help="sort runs of lines by prefix group, every group order")
    p.add_argument("file")
    p.add_argument("--prefixes", required=True, help="comma list of line prefixes, e.g. 'col.,uv.,v[,q['")
    common(p)

    p = sub.add_parser("sub", help="apply OLD NEW replacements to a copy and score it")
    p.add_argument("file")
    p.add_argument("pairs", nargs="+", metavar="OLD NEW")
    p.add_argument("-o", help="output file (default <stem>.sub.c beside the input)")
    p.add_argument("--diff", action="store_true")
    p.add_argument("--regs", action="store_true")
    p.add_argument("--drift", action="store_true")
    common(p, search=False)

    p = sub.add_parser("dump", help="gcc RTL / scheduler dumps with the build's flags")
    p.add_argument("file")
    p.add_argument("--passes", choices=("sched", "all"), default="sched",
                   help="sched: -dS -dR (.sched, .sched2; default); all: -da (every pass)")
    p.add_argument("--verbose", type=int, default=6, help="-fsched-verbose level (default 6)")
    p.add_argument("--cflags", default="", help="extra gcc flags (experiments only)")
    p.add_argument("--out", help="folder for the dumps (default <input dir>/<stem>_dump)")

    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)
    try:
        if args.mode == "score":
            return mode_score(args)
        if args.mode == "perm":
            return mode_perm(args)
        if args.mode == "decl":
            return mode_perm(args, decl=True)
        if args.mode == "climb":
            return mode_climb(args)
        if args.mode == "switch":
            return mode_switch(args)
        if args.mode == "group":
            return mode_group(args)
        if args.mode == "variants":
            return mode_variants(args)
        if args.mode == "dump":
            return mode_dump(args)
        return mode_sub(args)
    except SystemExit as e:
        if isinstance(e.code, str):
            print(e.code, file=sys.stderr)
            return 2
        raise


if __name__ == "__main__":
    sys.exit(main())
