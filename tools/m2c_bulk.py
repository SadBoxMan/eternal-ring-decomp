"""ER-050: m2c bulk driver for EE code: draft, clean, compile and compare many functions at once.

For each function (named, read from a list file, or every INCLUDE_ASM function of a src/ file):

1. copy splat's asm (asm/nonmatchings/<file>/<func>.s) and rewrite what m2c cannot parse:
   - `li.s $fN, <float>` (a .lit4 load, written so by tools/split.py) -> `lwc1 $fN, (M2CLIT_<bits>)`; the cleaner
     turns M2CLIT_<bits> back into a float literal in the draft (one instruction stays one instruction, so delay
     slots are untouched)
   - rabbitizer's `c1 0x...` for the R5900 square root (COP1 S-format funct 4, fs 0) -> `sqrt.s $fd, $ft`
   - `.word 0x... # cvt.w.s $fd, $fs` (rabbitizer prints the R5900 cvt.w.s as a word) -> `cvt.w.s $fd, $fs`
   - `j func_X; insn` (a tail call with an instruction in its delay slot, which m2c drops) -> `insn; j func_X; nop`
     (ER-084; not for a nop or the stack pop; --no-slot-move turns it off)
2. run m2c (Twinblade's copy, `mipsee-gcc-c` target, `--valid-syntax`) natively, with a context header (--ctx):
   none / src (prototypes and externs of the C already in src/) / harvest (src plus the signature m2c infers for each
   callee from the callee's own asm; the default)
3. clean the draft into a compilable scratch file: #include common.h, m2c's own macro header (from the m2c folder,
   never copied into this repo: m2c is GPL-3) and the context prototypes; float literals; undeclared stack
   variables declared (flagged: m2c lost a stack buffer); m2c's byte offsets made C-correct (ER-097): a %hi / %lo
   data base in index arithmetic becomes `extern char D_X[]` (or a `(char *)` cast when src/ declares it), a typed
   pointer stepped or offset by a literal steps by elements
   (ER-114: a dereferenced byte base gets its type back, or the width of the function's only memory access)
4a. store variants on a same-size draft: each run of consecutive field stores sorted by offset (three modes, ER-124;
   --no-store-sort skips it), then permutations of short runs
4b. const variant (ER-101): a draft that does not match is compiled again with the .sdata floats it reads that
   tools/sdata_consts.py finds never stored declared `extern const` (kept only on a MATCH; --no-const skips it)
4c. ruling 3.14 variant (ER-121): where our build has a sibling `j` and the game's function has none,
   `(void)&arg0; /* ruling 3.14 */` (kept only on a MATCH; --no-ruling-314 skips it)
4d. ER-147 / ER-151 draft fixes, kept when they match or come no further off: stack locals as one array sized from
   the game's frame (lowest saved-register slot; --no-frame), a run of ORed terms flattened into parameter order
   (GS register packers; --no-packer). ER-138 / ER-147 / ER-160 widen the variants: offset order also for `*p = x` /
   `p[N] = x` and per field-type sub-run, permuted orders inside blocks too (--store-limit), ruling 3.14 also with
   the first locals, a register added to a typed pointer written in bytes (--no-reg-bytes), and a retry with callees
   widened to take the argument registers the function passes on untouched (--no-untouched)
4e. ER-172 narrow parameters: a parameter the game narrows at entry (`andi $d, $aN, 0xFFFF` / 0xFF, or an `sll` +
   `sra` / `srl` pair by 16 / 24) typed u16 / u8 / s16 / s8 and m2c's masks of it dropped, then the store orders of
   that form; kept only on a MATCH (--no-narrow)
(ER-130: src/ declarations whose type is a file-local typedef are left out of m2c's context, which otherwise fails
   to parse; --plain uses, adopts or leaves out the target file's own extern for the function. ER-114: --plain
   writes the GOFAST soft-double calls m2c names (fptodp, dpadd, ...) as plain double C, recipes 2.11. ER-160: --plain
   also takes the file's non-extern prototype of the target, and gives every plain draft of one file one form of a
   name they declare differently when all still match with it)
4f. ER-224 .rodata vector copies, first of the draft fixes: m2c's `spN = M2C_FIELD(&D_x, s64 *, 0);` and
   `sp<N+8> = M2C_FIELD(&D_x, s64 *, 8);` (or the frame array's `(*(s64 *)&sp[k]) = ...`) for an INCLUDE_RODATA D_x
   -> `Vec4 spN = D_x;` (assignment, or initializer) with `extern const Vec4 D_x;` and the target file's own vector
   typedef (recipes 2.49 / 2.52; --no-rodata-vec)
4. compile with the Makefile's own settings (tools/eematch.py) into the output folder and compare with the original
   (eematch.compare_function: masked relocations with targets checked); a compile that runs past --gcc-timeout is
   killed with its children (cc1) and marked TIMEOUT (ER-066 / ER-079)
5. pass-through retry (ER-066): a same-size DIFF whose call arguments sit one register higher in the game is drafted
   again with the callee's prototype widened by a leading `void *`, so m2c passes the function's own $a0 on
6. write a per-function table: function, size band, draft compiles, exact match, differing words, failure class
   (rows printed as each function finishes; --rerun-diffs / --chunk rerun same-size DIFFs in small lists)

Nothing is placed in src/. Output (drafts are game code: gitignored, under build/): build/m2c_bulk/<label>/
  results.csv, results.md, results.json, asm/, c/, obj/, m2c_ctx.h, protos.h

A masked MATCH is a working aid, never proof (ruling docs/decomp_recipes.md 3.1): place the draft with
tools/place.py and run make regress. Usage notes: docs/matching_tools.md 6.
"""
from __future__ import annotations

import argparse
import collections
import concurrent.futures as cf
import csv
import itertools
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import time
from dataclasses import dataclass, field, asdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import eematch  # noqa: E402
from census import band_of  # noqa: E402

ROOT = eematch.ROOT
DEFAULT_M2C = Path(r"C:\Users\lukea\Claude\Projects\Project Twinblade\tools\m2c")
OUT_BASE = ROOT / "build" / "m2c_bulk"
FUNC_RE = re.compile(r"\bfunc_[0-9A-F]{8}\b")
INCLUDE_ASM_RE = re.compile(r'^INCLUDE_ASM\("asm/nonmatchings/([^"]+)",\s*(\w+)\);', re.M)

# asm rewrites
LIS_RE = re.compile(r"^(\s*/\*[^*]*\*/\s*)li\.s(\s+)(\$f\d+),\s*(\S+)\s*$")
C1_RE = re.compile(r"^(\s*/\*[^*]*\*/\s*)c1(\s+)0x([0-9A-Fa-f]+)\s*$")
# rabbitizer writes the R5900 cvt.w.s as `.word 0x... # cvt.w.s $fd, $fs # ...` (splat keeps the word)
WORD_INSN_RE = re.compile(r"^(\s*/\*[^*]*\*/\s*)\.word\s+0x[0-9A-Fa-f]+\s+#\s*(cvt\.w\.s)(\s+\$f\d+,\s*\$f\d+)\s*(#.*)?$")
OP_RE = re.compile(r"^\s*/\*[^*]*\*/\s*([a-z0-9.]+)\b(.*)$")
VU0_OPS = re.compile(r"^(lqc2|sqc2|qmfc2|qmtc2|cfc2|ctc2|v[a-z0-9]+(\.[xyzw]+)?|bc2[tf]l?)$")
# m2c draft parts
SIG_RE = re.compile(r"^([A-Za-z_][\w \*]*?)\s*\b(func_[0-9A-F]{8})\(([^)]*)\)\s*\{\s*$", re.M)
EXTERN_LINE_RE = re.compile(r"^.*/\* extern \*/\s*$|^extern [^;]*;\s*$")
LIT_RE = re.compile(r"\bM2CLIT_([0-9A-F]{8})\b")


# --- helpers ------------------------------------------------------------------------------------------------

def f32_literal(bits: int) -> str:
    """Shortest decimal that gives the same float32 bits, as a C float literal."""
    val = struct.unpack("<f", struct.pack("<I", bits))[0]
    if val != val or val in (float("inf"), float("-inf")):
        raise ValueError(f"non-finite literal 0x{bits:08X}")
    for digits in range(1, 18):
        s = f"{val:.{digits}g}"
        if struct.unpack("<I", struct.pack("<f", float(s)))[0] == bits:
            break
    if "e" not in s and "." not in s:
        s += ".0"
    return s + "f"


def asm_index() -> dict[str, Path]:
    out = {}
    for p in (ROOT / "asm" / "nonmatchings").glob("*/*.s"):
        out.setdefault(p.stem, p)
    return out


def file_functions(name: str) -> list[str]:
    """INCLUDE_ASM function names of src/<name>.c in file order (the file's still-asm functions)."""
    p = ROOT / "src" / (name if name.endswith(".c") else name + ".c")
    if not p.exists():
        raise SystemExit(f"m2c_bulk: no such file {p.relative_to(ROOT)}")
    return [m[2] for m in INCLUDE_ASM_RE.finditer(p.read_text(encoding="utf-8"))]


def placed_c_functions() -> set[str]:
    out = set()
    for p in (ROOT / "src").glob("*.c"):
        for m in SIG_RE.finditer(p.read_text(encoding="utf-8")):
            out.add(m[2])
    return out


@dataclass
class AsmInfo:
    text: str                       # rewritten asm for m2c
    n_lis: int = 0                  # li.s rewritten
    n_sqrt: int = 0                 # c1 -> sqrt.s rewritten
    n_cvtws: int = 0                # .word -> cvt.w.s rewritten
    unknown_c1: list[str] = field(default_factory=list)
    callees: set[str] = field(default_factory=set)
    has_div_sqrt: bool = False      # div.s / sqrt.s: two nops in the game, none from our gcc (ER-034)
    has_vu0: bool = False
    has_jtable: bool = False
    hilo_syms: set[str] = field(default_factory=set)   # symbols the asm reaches through %hi / %lo
    slot_moves: list[str] = field(default_factory=list)  # ER-084: mnemonics moved out of a tail j's delay slot
    mem_ops: list[str] = field(default_factory=list)     # ER-114: load / store mnemonics, stack accesses left out


def prepare_asm(src: Path) -> AsmInfo:
    out_lines = []
    info = AsmInfo("")
    for line in src.read_text(encoding="utf-8").splitlines():
        m = LIS_RE.match(line)
        if m:
            bits = struct.unpack("<I", struct.pack("<f", float(m[4])))[0]
            line = f"{m[1]}lwc1{m[2]}{m[3]}, (M2CLIT_{bits:08X})"
            info.n_lis += 1
        m = WORD_INSN_RE.match(line)
        if m:
            line = f"{m[1]}{m[2]}{m[3]}"
            info.n_cvtws += 1
        m = C1_RE.match(line)
        if m:
            imm = int(m[3], 16)
            fmt, ft, fs, fd, funct = (imm >> 21) & 0xF, (imm >> 16) & 31, (imm >> 11) & 31, (imm >> 6) & 31, imm & 63
            if fmt == 0 and funct == 4 and fs == 0:      # R5900 SQRT.S fd, ft (COP1 S-format, the source is ft)
                line = f"{m[1]}sqrt.s{m[2]}$f{fd}, $f{ft}"
                info.n_sqrt += 1
            else:
                info.unknown_c1.append(f"c1 0x{imm:X}")
        m = OP_RE.match(line)
        if m:
            op = m[1]
            if op in ("div.s", "sqrt.s", "rsqrt.s"):
                info.has_div_sqrt = True
            if VU0_OPS.match(op):
                info.has_vu0 = True
            if op in ("jal", "j"):
                info.callees.update(FUNC_RE.findall(m[2]))
            if op == "jr" and "$31" not in m[2]:
                info.has_jtable = True
            info.hilo_syms.update(re.findall(r"%(?:hi|lo)\((\w+)\)", m[2]))
            if op in MEM_TYPES and not re.search(r"\(\$(?:29|sp)\)", m[2]):
                info.mem_ops.append(op)
        out_lines.append(line)
    if SLOT_MOVE:
        out_lines = move_tail_slots(out_lines, info)
    info.text = "\n".join(out_lines) + "\n"
    return info


SLOT_MOVE = True            # --no-slot-move turns the tail `j` delay-slot rewrite off (ER-084)
J_FUNC_RE = re.compile(r"^(\s*/\*[^*]*\*/\s*)j(\s+)(?!\.)(\S+)\s*$")
SP_POP_RE = re.compile(r"^addiu\s+\$29,\s*\$29,")


def move_tail_slots(lines: list[str], info: AsmInfo) -> list[str]:
    """ER-084 / ER-055: m2c drops the instruction in the delay slot of a tail `j func_X` (func_001501D8: the game's
    `daddu $a1, $zero, $zero` became `arg1`; func_00146EF0: a `swc1` left out). Rewrite `j X; insn` as
    `insn; j X; nop`, one line for one, so the instruction count is unchanged. Safe for `j`, which reads no register.
    Left alone: a `nop` slot and the stack pop `addiu $29, $29, N` (m2c's epilogue, handled already)."""
    out: list[str] = []
    i = 0
    while i < len(lines):
        line = lines[i]
        sm = OP_RE.match(lines[i + 1]) if i + 1 < len(lines) and J_FUNC_RE.match(line) else None
        if sm and sm[1] != "nop" and not SP_POP_RE.match((sm[1] + sm[2]).strip()):
            slot_prefix = re.match(r"^(\s*/\*[^*]*\*/\s*)", lines[i + 1])[1]
            out += [lines[i + 1], line, f"{slot_prefix}nop"]     # insn; j X; nop
            info.slot_moves.append(sm[1])
            i += 2
            continue
        out.append(line)                        # (a label or directive after the j is not a plain delay slot)
        i += 1
    return out


# --- context ------------------------------------------------------------------------------------------------

CTX_TYPEDEFS = """typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;
typedef int s128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));
"""
# types m2c emits that common.h does not define (included by each draft after common.h)
DRAFT_EXTRA_H = """/* m2c_bulk: types m2c emits that common.h does not define */
typedef double f64;
typedef int s128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));
"""


# words m2c's context (CTX_TYPEDEFS + C) knows as types or qualifiers; any other identifier in a declaration's type
# is a file-local typedef (e.g. ER-092's `typedef struct {...} G390;` in text_001000C0.c), which the context lacks
CTX_TYPE_WORDS = {"extern", "const", "volatile", "unsigned", "signed", "char", "short", "int", "long", "float",
                  "double", "void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "s128", "u128",
                  "M2C_UNK", "M2C_UNK8", "M2C_UNK16", "M2C_UNK32", "M2C_UNK64"}


def unknown_types(decl: str, name: str) -> set[str]:
    """Identifiers in a declaration line that are neither the declared name, a parameter name nor a type the context
    knows (CTX_TYPE_WORDS). A parameter's name is its last identifier when another word comes before it."""
    m = re.match(rf"^(.*?)\b{re.escape(name)}\b(.*)$", decl)
    if not m:
        return set()
    words = set(re.findall(r"[A-Za-z_]\w*", m[1]))
    rest = m[2].strip()
    if rest.startswith("("):
        inner = rest[1:rest.rfind(")")] if ")" in rest else rest[1:]
        for p in _split_top(inner):
            ids = re.findall(r"[A-Za-z_]\w*", p)
            if len(ids) > 1 and ids[-1] not in CTX_TYPE_WORDS:
                ids = ids[:-1]                    # the parameter's name
            words |= set(ids)
    words.discard("__attribute__")
    return {w for w in words if w not in CTX_TYPE_WORDS and not w.startswith("__")}


def src_context() -> tuple[dict[str, str], dict[str, str]]:
    """(function prototypes, data extern lines) from src/: definitions first, then extern prototypes; the first
    declaration of a name wins (a clash between files is reported, not resolved). A declaration whose type is a
    file-local typedef (struct types: m2c's context has none of them) is left out and reported: one such line made
    m2c refuse the whole context ("Syntax error when parsing C context"), so from ER-092's G390 (22 Nov 2026, 17:04)
    every src / harvest attempt failed and each draft fell back to `none` (found by ER-130)."""
    protos: dict[str, str] = {}
    data: dict[str, str] = {}
    clashes = []
    skipped: dict[str, set[str]] = {}
    files = sorted((ROOT / "src").glob("*.c"))

    def known(line: str, name: str) -> bool:
        bad = unknown_types(line, name)
        if bad:
            skipped.setdefault(name, set()).update(bad)
        return not bad
    for p in files:
        for m in SIG_RE.finditer(p.read_text(encoding="utf-8")):
            ret = " ".join(m[1].split())
            if ret.startswith("static"):
                continue
            line = f"{ret} {m[2]}({m[3].strip() or 'void'});"
            if m[2] not in protos and known(line, m[2]):
                protos[m[2]] = line
    for p in files:
        for line in p.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if not line.startswith("extern ") or not line.endswith(";"):
                continue
            names = FUNC_RE.findall(line) or re.findall(r"\bD_[0-9A-F]{8}\b", line)
            if not names:
                continue
            name = names[0]
            if not known(line, name):
                continue
            if "(" in line:
                if name not in protos:
                    protos[name] = line[len("extern "):]
                elif " ".join(protos[name].split()) != " ".join(line[len("extern "):].split()):
                    clashes.append(name)
            else:
                data.setdefault(name, line)
    if clashes:
        print(f"m2c_bulk: note: {len(set(clashes))} prototype(s) differ between src/ files; first kept: "
              + ", ".join(sorted(set(clashes))[:8]))
    if skipped:
        types = sorted(set().union(*skipped.values()))
        print(f"m2c_bulk: note: {len(skipped)} src/ declaration(s) left out of the context (type unknown to it: "
              + ", ".join(types[:8]) + "): " + ", ".join(sorted(skipped)[:8]))
    return protos, data


CTX_FAIL_RE = re.compile(r"Syntax error when parsing C context\.?\s*\n?(.*)")


def check_context(m2c_dir: Path, ctx: Path, asm: Path, timeout: int) -> None:
    """Run m2c once with the context; a context m2c cannot parse makes every attempt that uses it fail silently (each
    draft falls back to `none`), so say so loudly."""
    _rc, out, err = run_m2c(m2c_dir, asm, ctx, timeout)
    m = CTX_FAIL_RE.search(out + "\n" + err)
    if m:
        print(f"m2c_bulk: WARNING: m2c cannot parse the context {ctx.name} ({m[1].strip()[:120]}); every src / "
              "harvest attempt will fall back to `none`", file=sys.stderr)


def harvest_signature(draft: str, name: str) -> str | None:
    for m in SIG_RE.finditer(draft):
        if m[2] == name:
            ret = " ".join(m[1].split())
            params = re.sub(r"\barg\d+\b", "", m[3]).strip()
            params = ", ".join(" ".join(x.split()) for x in params.split(",")) if params else "void"
            return f"{ret} {name}({params});"
    return None


def ctx_body(protos: dict[str, str], data: dict[str, str]) -> str:
    return "\n".join(protos[k] for k in sorted(protos)) + "\n" + "\n".join(data[k] for k in sorted(data)) + "\n"


def drop_names(text: str, names: set[str]) -> str:
    return "".join(ln for ln in text.splitlines(keepends=True)
                   if not (m := re.search(r"\b(func_[0-9A-F]{8})\(", ln)) or m[1] not in names)


def without_own(text: str, name: str) -> str:
    return re.sub(rf"^[^\n]*\b{name}\([^\n]*\n", "", text, flags=re.M)


def harvest(names: list[str], idx: dict[str, Path], src_protos: dict[str, str], data: dict[str, str], out: Path,
            m2c_dir: Path, args) -> dict[str, str]:
    """Signatures m2c infers for the targets and their callees (to --depth levels), in rounds: each round runs m2c
    on every function with the previous round's signatures as context, so a wrapper that passes its arguments on
    sees its callee's parameters (round 1 alone gives such wrappers `(void)`). Stops when nothing changes or after
    --rounds. Functions with a src/ prototype keep it."""
    want = set(names)
    frontier = set(names)
    for _ in range(args.depth):
        nxt = set()
        for n in frontier:
            if n in idx:
                nxt |= prepare_asm(idx[n]).callees
        nxt -= want
        want |= nxt
        frontier = nxt
    want = sorted(n for n in want if n in idx and n not in src_protos)
    hdir = out / "harvest"
    for n in want:
        (hdir / f"{n}.s").write_text(prepare_asm(idx[n]).text, encoding="utf-8", newline="\n")
    sigs: dict[str, str] = {}
    for rnd in range(1, args.rounds + 1):
        ctx_text = CTX_TYPEDEFS + "\n" + ctx_body({**src_protos, **sigs}, data)

        def h(n):
            ctx = None
            if rnd > 1 or src_protos:
                ctx = hdir / f"{n}.ctx.h"
                ctx.write_text(without_own(ctx_text, n), encoding="utf-8", newline="\n")
            _rc, d, _e = run_m2c(m2c_dir, hdir / f"{n}.s", ctx, args.timeout)
            return n, harvest_signature(d, n)
        new = dict(sigs)
        failed = []
        with cf.ThreadPoolExecutor(args.par) as ex:
            for n, sig in ex.map(h, want):
                if sig:
                    new[n] = sig
                else:
                    failed.append(n)
        changed = sum(new.get(n) != sigs.get(n) for n in want)
        sigs = new
        print(f"m2c_bulk: harvest round {rnd}: {len(want)} function(s), {len(sigs)} signatures, {changed} changed, "
              f"{len(failed)} without a draft")
        if changed == 0:
            break
    return sigs


# --- callee return types from the game's call sites (ER-175) ---------------------------------------------------

RT_FIX = True               # --no-rt-fix keeps m2c's harvested return types as m2c inferred them (ER-175)
RT_WINDOW = 24              # instructions looked at after a jal's delay slot (ER-164's rtscan.py)
RT_LEAVE_ONE_OUT = 4        # most changed draft prototypes for which each is also tried as m2c wrote it
_RT_SCAN: dict[str, dict] | None = None
RT_NOTES: dict[str, str] = {}   # function -> "return types from the game: func_X int -> void, ..." (its Notes)


def _rt_effects(word: int, vram: int):
    """(GPRs read, GPRs written, FPRs read, FPRs written, rabbitizer instruction) of one game word."""
    import rabbitizer
    ins = rabbitizer.Instruction(word, vram=vram, category=rabbitizer.InstrCategory.R5900)
    rs, rt, rd, fd = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, (word >> 6) & 31
    gr = {r for f, r in ((ins.readsRs, rs), (ins.readsRt, rt), (ins.readsRd, rd)) if f()}
    gw = {r for f, r in ((ins.modifiesRs, rs), (ins.modifiesRt, rt), (ins.modifiesRd, rd)) if f()}
    fr = {r for f, r in ((ins.readsFs, rd), (ins.readsFt, rt), (ins.readsFd, fd)) if f()}
    fw = {r for f, r in ((ins.modifiesFs, rd), (ins.modifiesFt, rt), (ins.modifiesFd, fd)) if f()}
    return gr, gw, fr, fw, ins


def game_return_scan(game: eematch.Game) -> dict[str, dict]:
    """ER-175 (ER-164's rtscan.py, recipes 2.38: the game's callers show a callee's return type): for every function
    of the game, from the original ELF's code (function bounds from the build's symbols), its call sites and what
    each caller does with `$v0` / `$f0` after the `jal`: read before written (a reader), passed back to the caller's
    own caller untouched (`jr $ra` with the register unwritten: a pass), or neither. The scan is a straight line from
    the instruction after the delay slot, at most RT_WINDOW instructions, stopping at a branch target (a join), after
    a branch's delay slot, or at the next call. Callee side (rtscan's too): whether the function writes `$v0` / `$f0`
    in the 6 instructions before a `jr $ra` or in its delay slot, makes a call there, or leaves by a tail `j`.
    Returns {name: {"sites": n, "v0": n, "f0": n, "pass": [caller, ...], "sets": {"v0", "f0", "call", "tail"}}}
    (cached)."""
    global _RT_SCAN
    if _RT_SCAN is not None:
        return _RT_SCAN
    scan: dict[str, dict] = {}

    def entry(n):
        return scan.setdefault(n, {"sites": 0, "v0": 0, "f0": 0, "pass": [], "sets": set()})
    for start, size in sorted(game.size_of.items()):
        name = game.name_at.get(start)
        raw = game.bytes_at(start, size)
        if not name or raw is None:
            continue
        words = struct.unpack(f"<{size // 4}I", raw[:size // 4 * 4])
        insns = [_rt_effects(w, start + 4 * i) for i, w in enumerate(words)]
        targets = set()
        sets = entry(name)["sets"]
        for i, (_gr, _gw, _fr, _fw, ins) in enumerate(insns):
            if ins.isBranch():
                targets.add(ins.getBranchVramGeneric())
            if ins.isJrRa():
                # the 6 instructions before the epilogue's restores (loads from $sp into saved registers, $ra or
                # $f20+, and the $sp pop: func_0014FE30 sets $v0 before five `ld $sN`), plus the jr's delay slot
                k, n_body = i - 1, 0
                while k >= 0 and n_body < 6:
                    w2 = words[k]
                    op2, base, dest = w2 >> 26, (w2 >> 21) & 31, (w2 >> 16) & 31
                    restore = base == 29 and ((op2 in (0x23, 0x37, 0x1E) and dest in SAVED_GPRS)
                                              or (op2 == 0x31 and dest >= 20)
                                              or (op2 == 0x09 and dest == 29))     # lw / ld / lq, lwc1, addiu sp
                    if not restore:
                        n_body += 1
                    k -= 1
                for _gr2, gw2, _fr2, fw2, i2 in insns[k + 1:i + 2]:
                    if 2 in gw2:
                        sets.add("v0")
                    if 0 in fw2:
                        sets.add("f0")
                    if i2.isFunctionCall():         # `jal g; ...; jr $ra`: g's value may be the return value
                        sets.add("call")
        for i, (_gr, _gw, _fr, _fw, ins) in enumerate(insns):
            if ins.isJumpWithAddress() and not ins.doesLink():          # j: a tail call when it leaves the function
                t = ins.getInstrIndexAsVram()
                if not (start <= t < start + size):
                    sets.add("tail")
                    if t in game.name_at:
                        entry(game.name_at[t])["pass"].append(name)
                continue
            if not (ins.isJumpWithAddress() and ins.doesLink()):
                continue
            t = ins.getInstrIndexAsVram()
            callee = game.name_at.get(t)
            if not callee:
                continue
            e = entry(callee)
            e["sites"] += 1
            state = {"v0": None, "f0": None}
            stop_after = None
            j = i + 2
            while j < len(insns) and j < i + 2 + RT_WINDOW:
                gr, gw, fr, fw, ins2 = insns[j]
                if start + 4 * j in targets:
                    break
                for reg, rd_, wr_ in (("v0", 2 in gr, 2 in gw), ("f0", 0 in fr, 0 in fw)):
                    if state[reg] is None:
                        if rd_:
                            state[reg] = "pass" if ins2.isJrRa() else "read"
                        elif wr_:
                            state[reg] = "killed"
                if ins2.isJrRa():
                    slot = insns[j + 1] if j + 1 < len(insns) else None
                    for reg, killed in (("v0", slot and 2 in slot[1]), ("f0", slot and 0 in slot[3])):
                        if state[reg] is None and not killed:
                            state[reg] = "pass"
                    break
                if stop_after is not None and j >= stop_after:
                    break
                if ins2.isFunctionCall():
                    break
                if ins2.isBranch() or ins2.isJump():
                    stop_after = j + 1
                j += 1
            if state["v0"] == "read":
                e["v0"] += 1
            if state["f0"] == "read":
                e["f0"] += 1
            if "pass" in (state["v0"], state["f0"]) and "read" not in (state["v0"], state["f0"]):
                e["pass"].append(name)
    _RT_SCAN = scan
    return scan


def game_return_class(name: str, scan: dict[str, dict]) -> str | None:
    """The register class the game's callers treat `name`'s return value as: "f32" (some caller reads $f0, none
    $v0), "s32" (some caller reads $v0, none $f0), "void" (it has call sites, no caller reads either register, and
    the function itself writes neither near a `jr $ra`, makes no call just before it and has no tail `j`:
    func_001C9B18, a 16-byte copy), or None (not known: no direct call sites, e.g. reached only through a function
    pointer; both registers read; or no reader but the function sets $v0 / $f0 at its end, ends in a call or
    tail-calls, which may be a value nobody uses: func_0012E820 ends `mov.s $f0` and its callers need it `float`,
    func_0012FAA8 ends `jal func_00165170; jr $ra` and its callers need it `s32`, recipes 2.36). Passes (a caller returning with the register untouched)
    are not followed: a void caller that ends in a call looks the same (ER-175 tried it: func_00101238, a void word
    copy, came out s32 through one such caller)."""
    e = scan.get(name)
    if not e or not e["sites"]:
        return None
    v0, f0 = e["v0"] > 0, e["f0"] > 0
    if v0 and f0:
        return None
    if f0:
        return "f32"
    if v0:
        return "s32"
    return None if e["sets"] else "void"


def _ret_class(ret: str) -> str:
    r = " ".join(ret.split())
    if "*" in r:
        return "s32"
    if r == "void":
        return "void"
    if r in ("f32", "float"):
        return "f32"
    return "s32"                # integers, M2C_UNK, f64 / double (returned in $v0 / $v1 on the EE)


def rt_fix_signatures(sigs: dict[str, str], game: eematch.Game) -> tuple[dict[str, str], dict[str, str]]:
    """ER-175: m2c's harvested signatures with the return type read from the game's call sites where the register
    class differs (func_001C9B18 came out `int`; no caller reads its $v0). Only harvested signatures change: a src/
    prototype is placed code's own view and keeps its type (the as-the-caller-saw-it cases of recipes 2.36 / 2.38 and
    ruling 3.19 live there). A GPR return of the right class keeps m2c's type (a pointer stays a pointer). Returns
    (fixed signatures, {name: "old -> new"})."""
    scan = game_return_scan(game)
    fixed, changed = dict(sigs), {}
    for n, sig in sigs.items():
        m = re.match(rf"^(.*?)\s*\b{n}\(", sig)
        if not m:
            continue
        want = game_return_class(n, scan)
        old = " ".join(m[1].split())
        if want is None or _ret_class(old) == want:
            continue
        fixed[n] = want + " " + sig[m.end() - len(n) - 1:]
        changed[n] = f"{old} -> {want}"
    return fixed, changed


DRAFT_PROTO_RE = re.compile(r"^(?!extern\b|static\b|typedef\b|return\b)([A-Za-z_][\w \*]*?)\s*\b(func_[0-9A-F]{8})"
                            r"\(([^;\n]*)\);([ \t]*/\* extern \*/)[ \t]*$", re.M)
_M2C_CACHE: dict[tuple[str, str], tuple[int, str, str]] = {}


def rt_fix_draft(c_text: str, game: eematch.Game, skip: frozenset = frozenset()) -> tuple[str, list[str]]:
    """ER-175: the callee prototypes m2c writes into a draft itself (`M2C_UNK func_X(..);  /* extern */`, for a
    callee the context does not declare) with the return type the game's call sites show, as rt_fix_signatures does
    for the harvest (ER-164's func_001C4838 draft had func_001C9B18 so, as M2C_UNK). Returns (text, ["func_X M2C_UNK
    -> void", ...]); the text is unchanged when the list is empty. `skip`: callees left as m2c wrote them."""
    scan = game_return_scan(game)
    used: list[str] = []

    def fix(m):
        want = game_return_class(m[2], scan)
        old = " ".join(m[1].split())
        if want is None or _ret_class(old) == want or m[2] in skip:
            return m[0]
        used.append(f"{m[2]} {old} -> {want}")
        return f"{want} {m[2]}({m[3]});{m[4]}"
    return DRAFT_PROTO_RE.sub(fix, c_text), used


# --- m2c ----------------------------------------------------------------------------------------------------

def run_m2c(m2c_dir: Path, asm_path: Path, ctx: Path | None, timeout: int) -> tuple[int, str, str]:
    cmd = [sys.executable, "-E", "-s", str(m2c_dir / "m2c.py"), "-t", "mipsee-gcc-c", "--valid-syntax"]
    if ctx:
        cmd += ["--context", str(ctx)]
    cmd.append(str(asm_path))
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout, stdin=subprocess.DEVNULL)
    except subprocess.TimeoutExpired:
        return -1, "", f"m2c timed out after {timeout} s"
    return r.returncode, r.stdout, r.stderr


# --- cleaning -----------------------------------------------------------------------------------------------

STACK_RE = re.compile(r"\bsp([0-9A-F]*)\b")


DATA_EXTERN_RE = re.compile(r"^extern ([\w \*]+?)\s*\b((?:func|D)_[0-9A-F]{8})\s*;(.*)$")
HILO_DRAFT_RE = re.compile(r'^extern ([\w \*]+?)\s*\b(D_[0-9A-F]{8}) __attribute__\(\(section\("\.data"\)\)\);.*$', re.M)


def _in_arith(text: str, ref: str) -> bool:
    """`ref` (e.g. `&D_X`) used as an operand of a binary + / - somewhere in text."""
    r = re.escape(ref)
    return re.search(rf"(?<![\w&]){r}\b\s*[-+](?![-+=>])|(?<![-+])[-+]\s*{r}\b(?!\s*\[)", text) is not None


MEM_TYPES = {"lb": "s8", "lbu": "u8", "sb": "s8", "lh": "s16", "lhu": "u16", "sh": "s16", "lw": "s32", "sw": "s32",
             "lwu": "u32", "lwc1": "f32", "swc1": "f32", "ld": "s64", "sd": "s64"}
ASM_MEM_OPS: dict[str, list[str]] = {}    # function -> its memory-access mnemonics (job() fills it, ER-114)


def deref_widths(body: str, n: str, t: str, mem_ops: list[str]) -> tuple[str, str]:
    """ER-114 (func_001BCF48): after a byte base `D_X` became `char D_X[]`, a dereference of a sum with it
    (`*((arg0 * 0xC4C) + D_X)`) reads a char. Give it m2c's type back, `*(T *)(...)`; where m2c had no width
    (M2C_UNK: func_001BCF48's `lhu`), the width of the function's only memory access in the asm, if it has exactly
    one. Works on the body with the `D_X\\0` markers of byte_bases. Returns (body, type used or '')."""
    if t == "M2C_UNK":
        ops = {MEM_TYPES[o] for o in mem_ops if o in MEM_TYPES}
        if len(mem_ops) != 1 or len(ops) != 1:
            return body, ""
        t = ops.pop()
    if BYTE_TYPE_RE.fullmatch(t):
        return body, ""
    out, i, used = [], 0, ""
    while (k := body.find("*(", i)) >= 0:
        prev = body[:k].rstrip()[-1:]
        try:
            j = _close_paren(body, k + 2)
        except ValueError:
            break
        inner = body[k + 2:j]
        if f"{n}\0" in inner and \
                (not prev or prev in "=(,!&|?:;{}<>+-*/%^~" or body[:k].rstrip().endswith("return")):
            out.append(body[i:k] + f"*({t} *)({inner})")
            used = t
        else:
            out.append(body[i:j + 1])
        i = j + 1
    out.append(body[i:])
    return "".join(out), used


def byte_bases(text: str, hilo: set[str], includes: list[Path], mem_ops: list[str] | None = None
               ) -> tuple[str, list[str]]:
    """ER-097: m2c writes a %hi / %lo data base used in index arithmetic in bytes (`(arg0 * 0x2C8) + &D_X`,
    `&D_X + 0x252`: evaluate.py handle_add_real / add_imm print the raw byte count), but C scales it by the element
    type it declared (M2C_UNK: by 4). docs/decomp_recipes.md 2.20.
    - a base the draft declares itself: `extern char D_X[];` (the recipe form), `&D_X` -> `D_X`, a plain read of the
      value `D_X` -> `(*(T *)D_X)` with m2c's type T;
    - a base the context declares (src/ already has an extern for it): the arithmetic operand gets a cast instead,
      `((char *)&D_X)` (scalar) or `((char *)D_X)` (array), so the file's declaration stays as it is.
    Returns (text, bases rewritten)."""
    done = []
    ctx_arrays = set()
    for p in includes:
        if p.name.endswith("protos.h") and p.exists():
            ctx_arrays |= set(re.findall(r"^extern [^;(]*\b(D_[0-9A-F]{8})\s*\[", p.read_text(encoding="utf-8"),
                                         re.M))
    own = {m[2]: m[1].strip() for m in HILO_DRAFT_RE.finditer(text)}
    for n in sorted(hilo):
        if not n.startswith("D_"):
            continue
        if n in own:
            if not _in_arith(text, "&" + n):
                continue
            t = own[n]
            text = HILO_DRAFT_RE.sub(lambda m: f"extern char {n}[];" if m[2] == n else m[0], text)
            body_start = text.find(f"extern char {n}[];") + len(f"extern char {n}[];")
            head, body = text[:body_start], text[body_start:]
            body = re.sub(rf"&{n}\b", f"{n}\0", body)
            body = re.sub(rf"(?<![\w&]){n}\b(?![\0\[])", f"(*({t} *){n})", body)
            body, k = deref_widths(body, n, t, mem_ops)
            text = head + body.replace("\0", "")
            if k:
                done.append(f"{n} (deref as {k})")
                continue
            done.append(n)
        elif n in ctx_arrays and _in_arith(text, n):
            text = re.sub(rf"(?<![\w&]){n}\b(?=\s*[-+](?![-+=>]))", f"((char *){n})", text)
            text = re.sub(rf"(?<=[-+])(\s*){n}\b(?!\s*\[)", rf"\1((char *){n})", text)
            done.append(n)
        elif _in_arith(text, "&" + n):
            text = re.sub(rf"(?<![\w&])&{n}\b(?=\s*[-+](?![-+=>]))", f"((char *)&{n})", text)
            text = re.sub(rf"(?<=[-+])(\s*)&{n}\b", rf"\1((char *)&{n})", text)
            done.append(n)
    return text, done


ELEM_SIZE = {"s8": 1, "u8": 1, "char": 1, "M2C_UNK8": 1, "void": 1, "s16": 2, "u16": 2, "short": 2,
             "M2C_UNK16": 2, "s32": 4, "u32": 4, "int": 4, "f32": 4, "float": 4, "M2C_UNK": 4, "M2C_UNK32": 4,
             "s64": 8, "u64": 8, "f64": 8, "double": 8, "M2C_UNK64": 8}
PTR_DECL_RE = re.compile(r"(?:^|[(,])\s*((?:const\s+|unsigned\s+|signed\s+)*\w+)\s*(\*+)\s*(\w+)\s*(?=[,);])", re.M)
LIT = r"(0x[0-9A-Fa-f]+|\d+)"


REG_BYTES = True            # --no-reg-bytes leaves a register added to a typed pointer as m2c wrote it (ER-138)
INT_DECL_RE = re.compile(r"(?:^|[(,])\s*((?:unsigned\s+|signed\s+)*\w+)\s+(\w+)\s*(?=[,);])", re.M)
ARRAY_DECL_RE = re.compile(r"^    ((?:const\s+|unsigned\s+|signed\s+)*\w+)\s+(\w+)\[[^\]]*\];", re.M)
INT_TYPES = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "int", "char", "short", "long", "M2C_UNK",
             "M2C_UNK8", "M2C_UNK16", "M2C_UNK32", "M2C_UNK64"}


def func_decls(text: str, name: str | None = None):
    """(signature match, pointer declarations {v: (base type, stars)}, int declarations {v}) of the function `name`
    (the first function when None): parameters and 4-space locals; an array `T v[N];` counts as `T *` (ER-138)."""
    m = SIG_RE.search(text)
    while m and name and m[2] != name:
        m = SIG_RE.search(text, m.end())
    if not m:
        return None, {}, set()
    body = text[m.end():]
    decls: dict[str, tuple[str, str]] = {}
    ints: set[str] = set()
    for d in PTR_DECL_RE.finditer("(" + m[3] + ")"):
        decls[d[3]] = (d[1], d[2])
    for d in INT_DECL_RE.finditer("(" + m[3] + ")"):
        if d[1].split()[-1] in INT_TYPES:
            ints.add(d[2])
    for d in re.finditer(r"^    ((?:const\s+|unsigned\s+|signed\s+)*\w+)\s*(\*+)\s*(\w+);", body, re.M):
        decls.setdefault(d[3], (d[1], d[2]))
    for d in ARRAY_DECL_RE.finditer(body):
        decls.setdefault(d[2], (d[1], "*"))
    for d in re.finditer(r"^    ((?:unsigned\s+|signed\s+)*\w+)\s+(\w+);", body, re.M):
        if d[1].split()[-1] in INT_TYPES:
            ints.add(d[2])
    return m, decls, ints - set(decls)


def byte_steps(text: str, name: str) -> tuple[str, int, int]:
    """ER-097: m2c adds byte counts to typed pointers as well (`var += 4` on an `s32 *` in a loop, `arg0 + 4` on an
    `s32 *` argument: add_imm keeps the raw immediate). For a parameter or local `T *v` with sizeof(T) > 1 known:
    `v += N;` / `v -= N;` and `v + N` / `v - N` become element counts (N / sizeof(T)), or byte arithmetic through
    `char *` when N is not a multiple.
    ER-138: the same for a register added to such a pointer (handle_add_real prints the raw register): `v + i` /
    `i + v` with `i` a declared integer -> `(T *)((char *)v + i)`; `v + (x * N)` / `v + (x << k)` -> element counts
    when N is a multiple of sizeof(T) (`v + x` when N == sizeof(T)), else the byte form. Arrays (m2c's `sp`, a stack
    area) count as `T *`. Returns (text, literal rewrites, register rewrites)."""
    m, decls, ints = func_decls(text, name)
    if not m:
        return text, 0, 0
    head, body = text[:m.start()], text[m.start():]
    count = nreg = 0
    for v, (base, stars) in decls.items():
        size = 4 if len(stars) > 1 else ELEM_SIZE.get(base.split()[-1])
        if not size or size == 1:
            continue
        ptype = f"{base} {stars}"

        def step(mm):
            nonlocal count
            n = int(mm[3], 0)
            count += 1
            if n % size == 0:
                return f"{mm[1]}{v} {mm[2]}= {n // size};"
            return f"{mm[1]}{v} = ({ptype})((char *){v} {mm[2]} {mm[3]});"
        body = re.sub(rf"^(\s+){v} ([-+])= {LIT};$", step, body, flags=re.M)

        def expr(mm):
            nonlocal count
            n = int(mm[2], 0)
            count += 1
            if n % size == 0:
                return f"{v} {mm[1]} {n // size}"
            return f"({ptype})((char *){v} {mm[1]} {mm[2]})"
        # `v + N` as an operand: not after a deref / cast / member access, not before a higher-precedence operator
        body = re.sub(rf"(?<![\w*.>)\]])(?<!\) )(?<!\* )\b{v}\s*([-+])\s*{LIT}\b(?!\s*[*/%\[(.])(?![\w])", expr, body)

        if not REG_BYTES:
            continue

        # ER-138: a register operand: `v + i`, `v - i`, `i + v` (i a declared integer), `v + (x * N)`, `v + (x << k)`
        def scaled(op: str, x: str, n: int, shift: bool) -> str | None:
            if n % size:
                return None
            k = n // size
            if k == 1:
                return f"{v} {op} {x}"
            if shift and k & (k - 1) == 0:
                return f"{v} {op} ({x} << {k.bit_length() - 1})"
            return f"{v} {op} ({x} * {k})"

        def reg(mm):
            nonlocal nreg
            op, e = mm[1], mm[2]
            pm = re.fullmatch(r"\((\w+) (\*|<<) " + LIT + r"\)", e)
            if pm:
                n = int(pm[3], 0)
                rep = scaled(op, pm[1], n if pm[2] == "*" else 1 << n, pm[2] == "<<")
                if rep is None:
                    rep = f"({ptype})((char *){v} {op} {e})"
            elif e in ints:
                rep = f"({ptype})((char *){v} {op} {e})"
            else:
                return mm[0]
            nreg += 1
            return rep
        lead = r"(?<![\w*.>)\]&])(?<!\) )(?<!\* )"
        tail = r"(?!\s*[*/%\[(.])(?![\w])"
        body = re.sub(rf"{lead}\b{v}\s*([-+])\s*(\(\w+ (?:\*|<<) {LIT}\)|[A-Za-z_]\w*\b){tail}", reg, body)

        def reg_first(mm):
            nonlocal nreg
            if mm[1] not in ints:
                return mm[0]
            nreg += 1
            return f"({ptype})((char *){v} + {mm[1]})"
        body = re.sub(rf"{lead}\b([A-Za-z_]\w*)\s*\+\s*{v}\b{tail}", reg_first, body)
    return head + body, count, nreg


def clean_draft(draft: str, name: str, includes: list[Path], hilo: set[str]) -> tuple[str, list[str]]:
    """Make m2c's --valid-syntax output a compilable scratch file (common.h, then `includes`). Returns (C text,
    notes). `hilo`: symbols the asm reaches through %hi / %lo.

    - `extern T func_X;` (m2c declares a function whose address is taken as data): `extern void func_X();`, else
      gcc -G8 reads a 4-byte extern $gp-relative while the game uses lui / addiu
    - `extern T D_X;` reached through %hi / %lo: `__attribute__((section(".data")))` added, which makes ee-gcc use
      %hi / %lo for it (a draft aid only: the placed form follows docs/decomp_recipes.md 2.6, `extern T D_X[];`)"""
    notes = []
    lines = []
    n_func = n_hilo = 0
    for line in draft.splitlines():
        if re.match(r"^extern f32 M2CLIT_[0-9A-F]{8};", line):
            continue
        m = DATA_EXTERN_RE.match(line)
        if m and m[2].startswith("func_"):
            line = f"extern void {m[2]}();{m[3]}"
            n_func += 1
        elif m and m[2] in hilo and "__attribute__" not in line:
            line = f'extern {m[1]} {m[2]} __attribute__((section(".data")));{m[3]}'
            n_hilo += 1
        lines.append(line)
    if n_func:
        notes.append(f"{n_func} function address extern(s)")
    if n_hilo:
        notes.append(f"{n_hilo} %hi/%lo data extern(s)")
    text = "\n".join(lines)
    # undeclared stack variables (m2c names them spXX; a bare `sp` is the address of a stack area); declared before
    # byte_steps, which treats the `sp` array as a typed pointer (ER-138)
    m = SIG_RE.search(text)
    if m and m[2] == name:
        body_start = m.end()
        body = text[body_start:]
        declared = set(re.findall(r"^\s+[\w \*]+?\b(sp[0-9A-F]*)(?:\[[^\]]*\])?;", body, re.M))
        params = set(re.findall(r"\b(sp[0-9A-F]*)\b", m[3]))
        used = {"sp" + x for x in STACK_RE.findall(body)}
        missing = sorted(used - declared - params)
        if missing:
            decl = "".join(f"    M2C_UNK {v};\n" if v != "sp" else "    M2C_UNK sp[64];\n" for v in missing)
            text = text[:body_start] + "\n" + decl + text[body_start:].lstrip("\n")
            notes.append("stack vars declared: " + " ".join(missing))
    text, bases = byte_bases(text, hilo, includes, ASM_MEM_OPS.get(name, []))
    if bases:
        notes.append("byte base: " + " ".join(bases))
    text, steps, regs = byte_steps(text, name)
    if steps:
        notes.append(f"{steps} pointer step(s) in elements")
    if regs:
        notes.append(f"{regs} register offset(s) on a typed pointer in bytes")

    def lit(m):
        return f32_literal(int(m[1], 16))
    n_lits = len(LIT_RE.findall(text))
    text = LIT_RE.sub(lit, text)
    if n_lits:
        notes.append(f"{n_lits} .lit4 literal(s)")
    # ER-224: m2c reads a literal address of unknown type as `*(void *)0x70000010` (EE scratchpad), which gcc refuses
    # ("void value not ignored"); a word load
    text, n_vd = re.subn(r"\*\(void \*\)(0x[0-9A-Fa-f]+)\b", r"*(s32 *)\1", text)
    if n_vd:
        notes.append(f"{n_vd} literal-address load(s) of type void as s32")
    head =['#include "common.h"'] + [f'#include "{p.as_posix()}"' for p in includes]
    return "\n".join(head) + "\n\n" + text.strip() + "\n", notes


# --- compile ------------------------------------------------------------------------------------------------

GCC_TIMEOUT = 120          # seconds per gcc / ee-as call (--gcc-timeout); ER-060 met a draft cc1 looped on for hours
TIMEOUT_TAG = "TIMEOUT"


def kill_tree(pid: int) -> None:
    """Kill a process and its Windows children by PID (taskkill /T). Not enough for gcc: the MSYS-built driver
    starts cc1.exe outside the Windows parent chain (seen on func_00179B30: cc1's parent was not under the driver's
    PID, and every timed-out cc1 kept running), so run_killable puts the driver in a job object first."""
    if os.name == "nt":
        subprocess.run(["taskkill", "/PID", str(pid), "/T", "/F"], capture_output=True, stdin=subprocess.DEVNULL)
    else:
        try:
            os.kill(pid, 9)
        except OSError:
            pass


class _Job:
    """A Windows job object with kill-on-close: every process started inside it (cc1 included, whatever its parent
    PID says) belongs to it, and terminate() / close() ends them all."""

    def __init__(self):
        import ctypes
        from ctypes import wintypes
        self.k32 = ctypes.WinDLL("kernel32", use_last_error=True)
        self.k32.CreateJobObjectW.restype = wintypes.HANDLE
        self.k32.CreateJobObjectW.argtypes = (wintypes.LPVOID, wintypes.LPCWSTR)
        self.k32.SetInformationJobObject.argtypes = (wintypes.HANDLE, ctypes.c_int, wintypes.LPVOID, wintypes.DWORD)
        self.k32.AssignProcessToJobObject.argtypes = (wintypes.HANDLE, wintypes.HANDLE)
        self.k32.TerminateJobObject.argtypes = (wintypes.HANDLE, wintypes.UINT)
        self.k32.CloseHandle.argtypes = (wintypes.HANDLE,)

        class BASIC(ctypes.Structure):
            _fields_ = [("PerProcessUserTimeLimit", ctypes.c_int64), ("PerJobUserTimeLimit", ctypes.c_int64),
                        ("LimitFlags", wintypes.DWORD), ("MinimumWorkingSetSize", ctypes.c_size_t),
                        ("MaximumWorkingSetSize", ctypes.c_size_t), ("ActiveProcessLimit", wintypes.DWORD),
                        ("Affinity", ctypes.c_size_t), ("PriorityClass", wintypes.DWORD),
                        ("SchedulingClass", wintypes.DWORD)]

        class IO(ctypes.Structure):
            _fields_ = [(n, ctypes.c_uint64) for n in ("r", "w", "o", "rb", "wb", "ob")]

        class EXT(ctypes.Structure):
            _fields_ = [("BasicLimitInformation", BASIC), ("IoInfo", IO), ("ProcessMemoryLimit", ctypes.c_size_t),
                        ("JobMemoryLimit", ctypes.c_size_t), ("PeakProcessMemoryUsed", ctypes.c_size_t),
                        ("PeakJobMemoryUsed", ctypes.c_size_t)]

        self.handle = self.k32.CreateJobObjectW(None, None)
        if not self.handle:
            raise OSError(ctypes.get_last_error(), "CreateJobObjectW")
        info = EXT()
        info.BasicLimitInformation.LimitFlags = 0x2000          # JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
        if not self.k32.SetInformationJobObject(self.handle, 9, ctypes.byref(info), ctypes.sizeof(info)):
            raise OSError(ctypes.get_last_error(), "SetInformationJobObject")

    def add_and_resume(self, proc: subprocess.Popen) -> None:
        import ctypes
        if not self.k32.AssignProcessToJobObject(self.handle, int(proc._handle)):
            raise OSError(ctypes.get_last_error(), "AssignProcessToJobObject")
        ntdll = ctypes.WinDLL("ntdll")
        ntdll.NtResumeProcess.argtypes = (ctypes.c_void_p,)
        ntdll.NtResumeProcess(int(proc._handle))                 # started suspended: nothing escaped the job

    def terminate(self) -> None:
        self.k32.TerminateJobObject(self.handle, 1)

    def close(self) -> None:
        self.k32.CloseHandle(self.handle)


def run_killable(cmd: list[str], timeout: int) -> tuple[int | None, str, str]:
    """subprocess.run with a timeout that also kills every process the command started; returncode None on a
    timeout. On Windows the command starts suspended inside a kill-on-close job object (_Job), so cc1 is ended with
    the driver however it was spawned; elsewhere the driver is killed (not tested)."""
    job = _Job() if os.name == "nt" else None
    p = subprocess.Popen(cmd, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
                         stdin=subprocess.DEVNULL, creationflags=0x4 if job else 0)     # 0x4: CREATE_SUSPENDED
    try:
        if job:
            try:
                job.add_and_resume(p)
            except OSError:
                p.kill()
                job.close()
                raise
        try:
            out, err = p.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            if job:
                job.terminate()
            kill_tree(p.pid)
            try:
                p.communicate(timeout=15)
            except subprocess.TimeoutExpired:
                p.kill()
            return None, "", ""
        return p.returncode, out, err
    finally:
        if job:
            job.close()                                           # kill-on-close: no leftovers either way


def compile_draft(c_path: Path, obj_dir: Path) -> tuple[Path | None, str]:
    """Compile with exactly the Makefile's CC / CFLAGS / CPPFLAGS / AS / ASFLAGS (eematch.settings; CFLAGS those of
    the function's home src file, eematch.scratch_cflags) into obj_dir.
    A gcc or ee-as call that runs longer than GCC_TIMEOUT is killed with its children; the message then starts with
    TIMEOUT_TAG."""
    st = eematch.settings()
    s_path, o_path = obj_dir / (c_path.stem + ".s"), obj_dir / (c_path.stem + ".o")
    try:
        cflags = eematch.scratch_cflags(c_path)     # the home src file's flags (ER-205)
    except SystemExit as e:
        return None, str(e)
    cc = [st["CC"], f"-B{st['GCC_DIR']}/", *cflags,
          *[f.replace("-I", "-I" + str(ROOT) + "/", 1) if f.startswith("-I") else f for f in st["CPPFLAGS"].split()],
          "-S", str(c_path), "-o", str(s_path)]
    rc, out, err = run_killable(cc, GCC_TIMEOUT)
    if rc is None:
        return None, f"{TIMEOUT_TAG}: gcc killed after {GCC_TIMEOUT} s"
    if rc != 0:
        return None, out + err
    rc2, out2, err2 = run_killable([st["AS"], *st["ASFLAGS"].split(), "-o", str(o_path), str(s_path)], GCC_TIMEOUT)
    if rc2 is None:
        return None, f"{TIMEOUT_TAG}: ee-as killed after {GCC_TIMEOUT} s"
    if rc2 != 0:
        return None, "ee-as: " + out2 + err2
    return o_path, err


def gcc_error_class(msg: str) -> str:
    """First gcc error, with file / line and names taken out, as a class label."""
    for line in msg.splitlines():
        if "warning:" in line or "In function" in line or not line.strip():
            continue
        m = re.match(r"^.*?:\d+:\s*(.*)$", line)
        if not m:
            continue
        s = m[1]
        s = re.sub(r"`[^']*'", "X", s)
        s = re.sub(r"'[^']*'", "X", s)
        s = re.sub(r"\b(func|D)_[0-9A-F]{8}\b", "X", s)
        return s.strip()[:80]
    return (msg.strip().splitlines() or ["?"])[0][:80]


# --- diff classes -------------------------------------------------------------------------------------------

def mnemonic(word: int | None, vram: int) -> str:
    if word is None:
        return "-"
    return eematch.disasm(word, vram).split()[0] if word else "nop"


def diff_class(res: eematch.FuncResult) -> str:
    if res.match:
        return "match"
    if res.game_size is not None and res.our_size != res.game_size:
        d = (res.our_size - res.game_size) // 4
        return f"size {d:+d} instr"
    ours = [mnemonic(w.ours, res.addr + w.off) for w in res.words if w.ours is not None]
    game = [mnemonic(w.game, res.addr + w.off) for w in res.words if w.game is not None]
    if ours == game:
        return "same instructions, operands differ (registers / offsets / targets)"
    if sorted(ours) == sorted(game):
        return "same instructions, order differs (scheduling)"
    return "different instructions, same size"


# --- the per-function job -----------------------------------------------------------------------------------

@dataclass
class Row:
    func: str
    file: str = ""
    addr: str = ""
    instr: int = 0
    band: str = ""
    m2c_ok: bool = False
    compiles: bool = False
    match: bool = False
    diff_words: int | None = None
    size_ours: int | None = None
    size_game: int | None = None
    klass: str = ""
    detail: str = ""
    flags: str = ""
    notes: str = ""
    seconds: float = 0.0
    plain: str = ""                 # --plain: result of the plain-C form (MATCH / diff / compile error / skipped)


UNSET_RE = re.compile(r"M2C_ERROR\(/\* Read from unset register")


@dataclass
class Attempt:
    ctx_label: str
    m2c_ok: bool = False
    compiles: bool = False
    match: bool = False
    n_err: int = 0
    klass: str = ""
    detail: str = ""
    notes: list[str] = field(default_factory=list)
    diff_words: int | None = None
    size_ours: int | None = None
    size_game: int | None = None
    files: dict[str, str] = field(default_factory=dict)   # suffix -> text (written for the kept attempt only)
    protos_h: Path | None = None                           # the prototypes header the draft was compiled with
    ctx: Path | None = None                                # the m2c context it was drafted with
    res: object = None                                     # the eematch.FuncResult of the kept draft
    widened: dict[str, str] = field(default_factory=dict)  # pass-through retry: callee -> widened prototype
    consts: set[str] = field(default_factory=set)          # ER-101: floats the kept draft declares const
    rt_used: list[str] = field(default_factory=list)       # ER-175: callees given the game's return type in the draft

    def score(self) -> tuple:
        size_off = abs((self.size_ours or 0) - (self.size_game or 0)) if self.compiles else 10 ** 6
        return (not self.match, not self.compiles, not self.m2c_ok, self.n_err, size_off,
                self.diff_words if self.diff_words is not None else 10 ** 6)


RET_WRAP_RE = re.compile(r"^void (func_[0-9A-F]{8}\([^)]*\) \{\n(?:.*\n)*?)    (func_[0-9A-F]{8}\(.*\));\n\}\s*$",
                         re.M)


def return_wrapper(c_text: str) -> str | None:
    """`void f(..) { ...; g(..); }` -> `M2C_UNK f(..) { ...; return g(..); }`: a call whose value is returned
    gets a frame and jal, a void tail call a sibling `j` (docs/decomp_recipes.md 2.1). m2c gives the void form when
    it cannot tell that $v0 is passed on."""
    m = RET_WRAP_RE.search(c_text)
    if not m:
        return None
    return c_text[:m.start()] + f"M2C_UNK {m[1]}    return {m[2]};\n}}\n"


CALL_RE = re.compile(r"\bfunc_[0-9A-F]{8}\(")
CONST_VARIANT = True        # --no-const turns the ER-101 variant off


def _const_decl(text: str, n: str) -> tuple[str, int]:
    """`extern f32 D_X;` / `extern float D_X;` (also inside a line of several externs) -> `extern const ...`."""
    return re.subn(rf"\bextern\s+((?:f32|float))\s+({n})\s*;", r"extern const \1 \2;", text)


def const_variant(c_text: str, name: str, protos_h: Path | None, out: Path, stem: str | None = None):
    """ER-101: the draft with every census-const .sdata float it reads (tools/sdata_consts.py: read by lwc1, never
    stored, address never taken) declared `extern const`. Recipes 2.21: with -fno-strict-aliasing gcc reloads a plain
    extern float after each store; a const one keeps the game's early loads (func_00145918, func_00147CF8). The
    census does not tell which ones the original declared const (func_00147F78's D_0035BD40 is never stored either
    but must stay plain), so this is a variant, kept only on a MATCH. The header copy is named per attempt (`stem`:
    function, context and base; ER-217), as the ER-188 copies are. Returns (text, protos header, names) or None."""
    import sdata_consts
    consts = sdata_consts.const_floats()
    used = sorted({n for n in re.findall(r"\bD_[0-9A-F]{8}\b", c_text) if n in consts})
    if not used:
        return None
    ph_text = protos_h.read_text(encoding="utf-8") if protos_h and protos_h.exists() else ""
    done = set()
    for n in used:
        c_text, k1 = _const_decl(c_text, n)
        ph_text, k2 = _const_decl(ph_text, n)
        if k1 or k2:
            done.add(n)
    if not done:
        return None
    ph2 = protos_h
    if protos_h and ph_text:
        ph2 = out / "obj" / f"{stem or name}.const.protos.h"
        ph2.write_text(ph_text, encoding="utf-8", newline="\n")
        c_text = c_text.replace(f'#include "{protos_h.as_posix()}"', f'#include "{ph2.as_posix()}"')
    return c_text, ph2, done


STORE_LIMIT = 48            # most permuted-order compiles per draft (ER-147: runs inside blocks too)
ASSIGN_RE = re.compile(r"^( {4,})[^ /].* = .*;$")


def store_orders(c_text: str, limit: int | None = None):
    """Other orders of each run of 2+ plain assignments at one indentation (`x = y;`, no call; ER-147: inside
    if-blocks and loops too, as func_0012FF78's three stores after a NULL test): every order for runs up to 4
    statements, else the reversed run; at most 24 per run and `limit` (STORE_LIMIT) in all. The scheduler does not
    keep source order for stores (docs/decomp_recipes.md 2.1, "Store order is not source order"); m2c writes them in
    machine order."""
    limit = STORE_LIMIT if limit is None else limit
    lines = c_text.split("\n")
    runs, cur, cur_ind = [], [], None
    for i, ln in enumerate(lines):
        am = ASSIGN_RE.match(ln)
        ok = am and not CALL_RE.search(ln) and not ln.lstrip().startswith(
            ("return", "if", "for", "while", "case", "default", "}"))
        if ok and cur and am[1] == cur_ind:
            cur.append(i)
            continue
        if len(cur) >= 2:
            runs.append(cur)
        cur, cur_ind = ([i], am[1]) if ok else ([], None)
    if len(cur) >= 2:
        runs.append(cur)
    n = 0
    for run in runs:
        block = [lines[i] for i in run]
        orders = itertools.permutations(block) if len(block) <= 4 else [tuple(reversed(block))]
        k = 0
        for order in orders:
            if list(order) == block:
                continue
            new = list(lines)
            for i, ln in zip(run, order):
                new[i] = ln
            yield "\n".join(new)
            n += 1
            k += 1
            if n >= limit:
                return
            if k >= 24:
                break


STORE_SORT = True           # --no-store-sort turns the ER-124 offset-order variant off
FIELD_STORE_RE = re.compile(r"^(\s+)M2C_FIELD\(")
BYTE_TYPE_RE = re.compile(r"\b(?:[su]8|M2C_UNK8|char)\b")


DEREF_STORE_RE = re.compile(r"^(\s+)\*([A-Za-z_]\w*) = (?!=)(.*);\s*$")
INDEX_STORE_RE = re.compile(r"^(\s+)([A-Za-z_]\w*)\[" + LIT + r"\] = (?!=)(.*);\s*$")


def _elem_types(c_text: str) -> dict[str, tuple[str, int]]:
    """ER-138: {pointer or array name: (element type, element size)} for `*p = x` / `p[N] = x` stores: the
    function's parameters and locals (func_decls) and the draft's `extern T D_X[];` lines."""
    out: dict[str, tuple[str, int]] = {}
    _m, decls, _ints = func_decls(c_text)
    for v, (base, stars) in decls.items():
        t = base.split()[-1]
        size = 4 if len(stars) > 1 else ELEM_SIZE.get(t)
        if size:
            out[v] = (t if len(stars) == 1 else "void *", size)
    for d in re.finditer(r"^extern ((?:unsigned\s+|signed\s+)*\w+)\s+(D_[0-9A-F]{8})\[\];", c_text, re.M):
        if d[1].split()[-1] in ELEM_SIZE:
            out.setdefault(d[2], (d[1].split()[-1], ELEM_SIZE[d[1].split()[-1]]))
    return out


def _field_store(line: str, elems: dict[str, tuple[str, int]] | None = None):
    """`<indent>M2C_FIELD(base, T *, off) = rhs;` -> (indent, base, type, offset, rhs), else None. ER-138: also
    `*p = rhs;` (offset 0) and `p[N] = rhs;` (offset N * sizeof element) for a pointer or array whose element type
    `elems` knows."""
    if elems:
        dm = DEREF_STORE_RE.match(line)
        if dm and dm[2] in elems:
            return dm[1], dm[2], elems[dm[2]][0] + " *", 0, dm[3].strip()
        im = INDEX_STORE_RE.match(line)
        if im and im[2] in elems:
            return im[1], im[2], elems[im[2]][0] + " *", int(im[3], 0) * elems[im[2]][1], im[4].strip()
    m = FIELD_STORE_RE.match(line)
    if not m or not line.rstrip().endswith(";"):
        return None
    i = m.end()
    try:
        j = _close_paren(line, i)
    except ValueError:
        return None
    args = _split_top(line[i:j])
    rest = line[j + 1:].strip()
    if len(args) != 3 or not rest.startswith("= ") or rest.startswith("=="):
        return None
    try:
        off = int(args[2].strip(), 0)
    except ValueError:
        return None
    return m[1], args[0].strip(), args[1].strip(), off, rest[2:-1].strip()


def offset_orders(c_text: str):
    """ER-124 (recipes 2.25, ER-098's asc_sort.py): m2c lists a setter's stores in the game's schedule order, and the
    C that compiles to that schedule writes them in ascending offset order. Each run of 2+ consecutive
    `M2C_FIELD(base, T *, off) = rhs;` lines at one indentation (inside blocks too; ER-138: also `*p = x;` and
    `p[N] = x;` for a pointer or array of known element type) is sorted by offset, in three modes: offset order
    (`asc`), bytes last (`bytelast`), bytes first (`bytefirst`); a stable sort keeps two stores to one field in order.
    ER-160: then, for runs made of 2-3 sub-runs of one field type each (func_001359C0: five function pointers, then
    two floats), each sub-run sorted or left as it is, every combination (`by type`, at most 7). Left alone: a run in
    which a store's right side reads a field the run writes (func_00181210's x58 pair) or calls a function. Yields
    (mode, text) for each mode that changes the text."""
    lines = c_text.split("\n")
    elems = _elem_types(c_text)
    runs, cur = [], []
    for i, ln in enumerate(lines):
        st = _field_store(ln, elems)
        if st and CALL_RE.search(ln):
            st = None
        if st and cur and st[0] == cur[-1][1][0] and i == cur[-1][0] + 1:
            cur.append((i, st))
            continue
        if len(cur) >= 2:
            runs.append(cur)
        cur = [(i, st)] if st else []
    if len(cur) >= 2:
        runs.append(cur)
    good = []
    for run in runs:
        written = {(st[1], st[3]) for _i, st in run}
        bases = {st[1] for _i, st in run}
        reads = False
        for _i, st in run:
            for mm in re.finditer(r"M2C_FIELD\(", st[4]):
                try:
                    a = _split_top(st[4][mm.end():_close_paren(st[4], mm.end())])
                    if len(a) == 3 and (a[0].strip(), int(a[2].strip(), 0)) in written:
                        reads = True
                except ValueError:
                    reads = True               # not parsable: leave the run alone
            for b in bases:                    # ER-138: `*p` / `p[N]` read on the right side
                if re.search(rf"(?<![\w)\]])\*\s*{re.escape(b)}\b|\b{re.escape(b)}\s*\[", st[4]):
                    reads = True
        if not reads:
            good.append(run)
    seen = {c_text}
    for mode in ("asc", "bytelast", "bytefirst"):
        new = list(lines)
        for run in good:
            def key(item, mode=mode):
                b = 1 if BYTE_TYPE_RE.search(item[1][2]) else 0
                o = item[1][3]
                return (o,) if mode == "asc" else ((b, o) if mode == "bytelast" else (-b, o))
            for (i, _st), (j, _st2) in zip(run, sorted(run, key=key)):
                new[i] = lines[j]
        text = "\n".join(new)
        if text not in seen:
            seen.add(text)
            yield mode, text
    # ER-160: sub-runs of one field type, each sorted or not
    subs = []
    for run in good:
        groups = [list(g) for _t, g in itertools.groupby(run, key=lambda it: " ".join(it[1][2].split()))]
        if 2 <= len(groups) <= 3:
            subs.append(groups)
    if not subs:
        return
    width = max(len(g) for g in subs)
    for mask in range(1, 2 ** width):
        new = list(lines)
        for groups in subs:
            for gi, g in enumerate(groups):
                if mask >> gi & 1:
                    for (i, _st), (j, _st2) in zip(g, sorted(g, key=lambda it: it[1][3])):
                        new[i] = lines[j]
        text = "\n".join(new)
        if text not in seen:
            seen.add(text)
            yield "by type", text


SIBCALL_314 = True          # --no-ruling-314 turns the ER-121 variant off
SIBCALL_ARG_RE = re.compile(r"\(void\)&(\w+); /\* ruling 3\.14 \*/")
LOCAL_DECL_RE = re.compile(r"^    (?!return\b|goto\b)(?:(?:const|unsigned|signed|volatile)\s+)*[A-Za-z_]\w*[\s*]+\w+"
                           r"(?:\[[^\]]*\])?;\s*$")


def our_sibcalls(s_path: Path, name: str) -> list[str]:
    """Function names our compiled .s reaches by a plain `j` (a sibling call) inside `name`."""
    if not s_path.exists():
        return []
    out, inside = [], False
    for ln in s_path.read_text(encoding="utf-8", errors="replace").splitlines():
        if re.match(rf"^{name}:", ln):
            inside = True
        elif inside and re.match(rf"^\s*\.end\s+{name}\b", ln):
            break
        elif inside:
            m = re.match(r"^\s*j\s+([A-Za-z_]\w*)\s*$", ln)
            if m and not m[1].startswith(("$", ".")):
                out.append(m[1])
    return out


def game_has_sibcall(game: eematch.Game, addr: int | None) -> bool:
    """A `j` in the game's function whose target lies outside it (a sibling call)."""
    size = game.size_of.get(addr) if addr is not None else None
    if not size:
        return True                                    # unknown: do not try the variant
    for off in range(0, size, 4):
        t = _call_target(game.word(addr + off), addr + off)
        if t is not None and game.word(addr + off) >> 26 == 2 and not (addr <= t < addr + size):
            return True
    return False


NARROW_RE = re.compile(r"\b(?:[su]8|[su]16|char|short|M2C_UNK8|M2C_UNK16)\b")
SIBCALL_LOCALS = 3          # ER-138: most locals tried after the first parameter


def sibcall_variants(c_draft: str, name: str, addr: int | None, s_path: Path, game: eematch.Game
                     ) -> list[tuple[str, str]]:
    """ER-121 (ruling 3.14, recipes 2.24): the game has `jal` + epilogue where our build of the draft has a sibling
    `j`, because Cygnus' sibcall pass refuses sibcalls in a function that takes a local's or parameter's address.
    `(void)&arg0; /* ruling 3.14 */` after the declarations, then (ER-138, as func_0014B528's `(void)&g`) the same
    with each of the first SIBCALL_LOCALS locals declared at the top of the body, in declaration order; narrow
    locals (8 / 16 bits) are skipped: ER-100 found such a local given `&` goes to the stack. Only when our .s has a
    sibling `j` and the game's function has none. Returns [(variable, text)], empty otherwise."""
    if not our_sibcalls(s_path, name) or game_has_sibcall(game, addr):
        return []
    m = SIG_RE.search(c_draft)
    while m and m[2] != name:
        m = SIG_RE.search(c_draft, m.end())
    if not m:
        return []
    names = []
    if m[3].strip() and m[3].strip() != "void":
        ids = re.findall(r"[A-Za-z_]\w*", _split_top(m[3])[0])
        if len(ids) >= 2:
            names.append(ids[-1])
    lines = c_draft[m.end():].split("\n")
    k = 1 if lines and lines[0].strip() == "" else 0     # the rest of the `{` line
    local_names = []
    while k < len(lines) and (LOCAL_DECL_RE.match(lines[k]) or not lines[k].strip()):
        dm = re.match(r"^    (.*?)\b(\w+)(\[[^\]]*\])?;\s*$", lines[k])
        if dm and not NARROW_RE.search(dm[1]):
            local_names.append(dm[2])
        k += 1
    names += local_names[:SIBCALL_LOCALS]
    out = []
    for v in names:
        new = list(lines)
        new.insert(k, f"    (void)&{v}; /* ruling 3.14 */")
        out.append((v, c_draft[:m.end()] + "\n".join(new)))
    return out


def sibcall_variant(c_draft: str, name: str, addr: int | None, s_path: Path, game: eematch.Game) -> str | None:
    """The first form of sibcall_variants (ER-121's `(void)&arg0`), or None (kept for scratch scripts)."""
    v = sibcall_variants(c_draft, name, addr, s_path, game)
    return v[0][1] if v else None


# --- ER-147: locals sized from the game's frame -------------------------------------------------------------

SAVED_GPRS = set(range(16, 24)) | {30, 31}
FRAME_FIX = True            # --no-frame turns the ER-147 frame-sized locals off


def game_locals_size(game: eematch.Game, addr: int | None) -> int | None:
    """Bytes of stack locals in the game's function: the offset of the lowest callee-saved register slot (`sd` /
    `sw` / `sq` of $16-$23, $30, $31 or `swc1` of $f20+ at `N($sp)`, before the first jump or branch), or the whole
    frame when nothing is saved (recipes 2.29). None without an `addiu $sp, $sp, -N` first."""
    size = game.size_of.get(addr) if addr is not None else None
    if not size:
        return None
    w0 = game.word(addr)
    if w0 >> 16 != 0x27BD or (w0 & 0xFFFF) < 0x8000:
        return None
    frame = 0x10000 - (w0 & 0xFFFF)
    saves = []
    for off in range(4, size, 4):
        w = game.word(addr + off)
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op in (1, 2, 3, 4, 5, 6, 7, 20, 21, 22, 23) or (op == 0 and (w & 0x3F) in (8, 9)) or \
                (op == 17 and rs == 8):
            break                                       # j / jal / branch / jr / jalr / bc1x: past the prologue
        if rs == 29 and ((op in (0x3F, 0x2B, 0x1F) and rt in SAVED_GPRS) or (op == 0x39 and rt >= 20)):
            saves.append(w & 0xFFFF)
    return min(saves) if saves else frame


SPVAR_DECL_RE = re.compile(r"^    (.*?)\s*\b(sp[0-9A-F]*)\s*(\[[^\]]*\])?;\s*$")


def frame_variant(c_text: str, name: str, locals_size: int | None) -> str | None:
    """ER-147 (recipes 2.29 / 2.31: 13 of ER-125's 27 hand matches): m2c writes the stack locals as `M2C_UNK sp[64]`
    (a bare `sp`, declared by clean_draft) and `spXX` variables, so the frame comes out 0x100 bytes too big or the
    locals land elsewhere. One array sized from the game's frame takes their place: `M2C_UNK sp[size / 4];`,
    `&spXX` -> `&sp[XX / 4]`, `spXX` -> `sp[XX / 4]` (another scalar type T: `(*(T *)&sp[XX / 4])`). None when the
    draft has no stack locals, an `spXX` array, an offset outside the game's locals, or already the right array."""
    if not locals_size or locals_size % 4:
        return None
    m = SIG_RE.search(c_text)
    while m and m[2] != name:
        m = SIG_RE.search(c_text, m.end())
    if not m:
        return None
    lines = c_text[m.end():].split("\n")
    k = 1 if lines and lines[0].strip() == "" else 0
    found: dict[str, tuple[str, str, int]] = {}
    while k < len(lines) and (LOCAL_DECL_RE.match(lines[k]) or not lines[k].strip()):
        dm = SPVAR_DECL_RE.match(lines[k])
        if dm:
            found[dm[2]] = (dm[1].strip(), dm[3] or "", k)
        k += 1
    if not found:
        return None
    offs = {}
    for v, (t, arr, _k) in found.items():
        if v == "sp":
            continue
        if arr:
            return None
        offs[v] = int(v[2:], 16)
    n = locals_size // 4
    if any(o >= locals_size or o % 4 for o in offs.values()):
        return None
    if not offs and found.get("sp", ("", "", 0))[1] == f"[{n}]":
        return None
    first = min(k2 for _t, _a, k2 in found.values())
    drop = {k2 for _t, _a, k2 in found.values()}
    head = [ln for j, ln in enumerate(lines[:k]) if j not in drop]
    head.insert(first, f"    M2C_UNK sp[{n}];")
    body = "\n".join(lines[k:])
    for v, o in sorted(offs.items(), key=lambda x: -len(x[0])):
        t = found[v][0]
        body = re.sub(rf"&{v}\b", f"&sp[{o // 4}]", body)
        rep = f"sp[{o // 4}]" if t in ("M2C_UNK", "s32", "int", "M2C_UNK32") else f"(*({t} *)&sp[{o // 4}])"
        body = re.sub(rf"(?<![\w&]){v}\b", rep, body)
    return c_text[:m.end()] + "\n".join(head) + "\n" + body


# --- ER-151: GS register packers -----------------------------------------------------------------------------

PACKER_VARIANT = True       # --no-packer turns the ER-151 variant off


def _strip_parens(e: str) -> str:
    e = e.strip()
    while e.startswith("("):
        try:
            if _close_paren(e, 1) != len(e) - 1:
                break
        except ValueError:
            break
        e = e[1:-1].strip()
    return e


def _top_level(e: str, pattern: str) -> bool:
    """`pattern` (a regex) matches somewhere in e outside parentheses."""
    depth, flat = 0, []
    for ch in e:
        depth += ch == "("
        depth -= ch == ")"
        flat.append(ch if depth == 0 and ch not in "()" else " ")
    return re.search(pattern, "".join(flat)) is not None


def or_terms(e: str) -> list[str] | None:
    """The terms of a run of bitwise ORs with every grouping flattened (`a | (b | c) | d` -> [a, b, c, d]); None when
    the expression has an operator weaker than `|` outside parentheses (&&, ||, ?:, assignment, comma)."""
    e = _strip_parens(e)
    if _top_level(e, r"\|\||&&|\?|,|(?<![=!<>])=(?!=)"):
        return None
    parts, depth, cur = [], 0, ""
    for ch in e:
        depth += ch == "("
        depth -= ch == ")"
        if ch == "|" and depth == 0:
            parts.append(cur)
            cur = ""
            continue
        cur += ch
    parts.append(cur)
    if len(parts) == 1:
        return [e]
    out = []
    for p in parts:
        t = or_terms(p)
        if t is None:
            return None
        out += t
    return out


def packer_variant(c_text: str, name: str) -> str | None:
    """ER-151 (recipes 2.31: all nine GS register packers that call func_001C6880): an expression that is a run of
    3+ ORed terms, each reading a parameter, written as the flat list in parameter order (`a1 | a2 << k2 | ...`, as
    Sony's SCE_GS_SET_* macros are written); m2c groups the terms as the scheduler left them (`a | d | (b | c)`).
    Every return / assignment right side and call argument is looked at, the outermost first. None when nothing
    changes."""
    m = SIG_RE.search(c_text)
    while m and m[2] != name:
        m = SIG_RE.search(c_text, m.end())
    if not m:
        return None
    params = []
    for p in _split_top(m[3]):
        ids = re.findall(r"[A-Za-z_]\w*", p)
        if len(ids) >= 2:
            params.append(ids[-1])
    if len(params) < 2:
        return None
    pidx = {p: i for i, p in enumerate(params)}
    start = m.end()
    body = c_text[start:]
    # candidate spans: the inside of every parenthesis group split at its top-level commas, and `return X;` / `= X;`
    spans = []
    for i, ch in enumerate(body):
        if ch == "(":
            try:
                j = _close_paren(body, i + 1)
            except ValueError:
                continue
            pos = i + 1
            for part in _split_top(body[i + 1:j]):
                spans.append((pos, pos + len(part)))
                pos += len(part) + 1
    for sm in re.finditer(r"(?:\breturn\s+|[^=!<>]=\s+)([^;]+);", body):
        spans.append((sm.start(1), sm.end(1)))
    spans.sort(key=lambda s: (s[0], -(s[1] - s[0])))
    edits, last_end = [], -1
    for a, b in spans:
        if a < last_end:
            continue
        expr = body[a:b]
        terms = or_terms(expr)
        if not terms or len(terms) < 3:
            continue
        keys = []
        for t in terms:
            used = [pidx[x] for x in re.findall(r"[A-Za-z_]\w*", t) if x in pidx]
            if not used:
                break
            keys.append(min(used))
        else:
            order = [t for _k, t in sorted(zip(keys, terms), key=lambda kt: kt[0])]
            new = " | ".join(t.strip() if re.fullmatch(r"\w+", t.strip()) else f"({_strip_parens(t)})"
                             for t in order)
            lead = expr[:len(expr) - len(expr.lstrip())]
            trail = expr[len(expr.rstrip()):]
            if " ".join(new.split()) != " ".join(_strip_parens(expr).split()):
                edits.append((a, b, lead + new + trail))
                last_end = b
    if not edits:
        return None
    for a, b, rep in reversed(edits):
        body = body[:a] + rep + body[b:]
    return c_text[:start] + body


# --- ER-172: narrow parameters ---------------------------------------------------------------------------------

NARROW_VARIANT = True       # --no-narrow turns the ER-172 variant off
NARROW_KEEP = True          # --no-narrow-keep keeps it only on a MATCH, as ER-172 did (ER-180)
NARROW_TYPES = {("andi", 0xFFFF): "u16", ("andi", 0xFF): "u8", ("sra", 16): "s16", ("srl", 16): "u16",
                ("sra", 24): "s8", ("srl", 24): "u8"}
WIDE_INT_TYPES = {"s32", "u32", "int", "unsigned int", "signed int", "M2C_UNK", "M2C_UNK32", "s16", "u16", "s8",
                  "u8", "s64", "u64", "long", "unsigned long"}


def _imm(s: str) -> int | None:
    try:
        return int(s, 0)
    except ValueError:
        return None


def game_entry_narrows(name: str) -> dict[int, str]:
    """ER-172 (recipes 2.18: a `u16` parameter is narrowed once on entry): {argument register: narrow type} for each
    argument register ($a0-$t3) the game's function narrows before writing it and before its first jump or branch:
    `andi $d, $aN, 0xFFFF` -> u16 (0xFF -> u8); `sll $d, $aN, 16` then `sra` / `srl $e, $d, 16` -> s16 / u16 (24:
    s8 / u8). The first narrowing of each register counts."""
    out: dict[int, str] = {}
    written: set[int] = set()
    shifted: dict[int, tuple[int, int]] = {}        # sll destination -> (argument register, shift)
    for op, ops in asm_insns(name):
        if op in ("j", "jal", "jr", "jalr") or BRANCH_OP_RE.match(op):
            break
        w, _r = reg_effects(op, ops)
        regs = [GPR_RE.fullmatch(o) for o in ops]
        new_shift = None
        if len(ops) == 3 and regs[0] and regs[1]:
            d, s, k = int(regs[0][1]), int(regs[1][1]), _imm(ops[2])
            if op == "andi" and 4 <= s <= 11 and s not in written and (op, k) in NARROW_TYPES:
                out.setdefault(s, NARROW_TYPES[(op, k)])
            elif op == "sll" and 4 <= s <= 11 and s not in written and k in (16, 24):
                new_shift = (d, (s, k))
            elif op in ("sra", "srl") and s in shifted and shifted[s][1] == k:
                out.setdefault(shifted[s][0], NARROW_TYPES[(op, k)])
        for r in w:
            shifted.pop(r, None)
        if new_shift:
            shifted[new_shift[0]] = new_shift[1]
        written |= w
    return out


def _narrow_forms(v: str, t: str) -> list[str]:
    """Regexes for the expressions that narrow `v` to type `t` as m2c writes them (removed once `v` has type t)."""
    bits = {"u16": 16, "s16": 16, "u8": 24, "s8": 24}[t]
    sh = rf"(?:0x{bits:X}|{bits})"
    # the shift pair: unsigned (srl) only with an unsigned cast; signed (sra) with a signed cast or none (v is s32)
    cast = r"\(\s*(?:u32|unsigned int)\s*\)\s*" if t[0] == "u" else r"(?:\(\s*(?:s32|int)\s*\)\s*)?"
    one = rf"{cast}\(\s*{v}\s*<<\s*{sh}\s*\)\s*>>\s*{sh}(?!\w)"
    # a bare form only as a whole operand (call argument, right side, return value): `a + v & 0xFFFF` is not v's
    whole = (r"(?:(?<=[(,=])|(?<=\breturn))(\s*)", r"(?=\s*[),;])")      # group 1: the space kept
    forms = [rf"\(\s*{one}\s*\)", whole[0] + one + whole[1], rf"\(\s*{t}\s*\)\s*\(\s*{v}\s*\)",
             rf"\(\s*{t}\s*\)\s*{v}\b"]
    if t[0] == "u":
        mask = "0xFFFF" if t == "u16" else "0xFF"
        forms = [rf"\(\s*{v}\s*&\s*{mask}\s*\)", rf"{whole[0]}\b{v}\s*&\s*{mask}{whole[1]}"] + forms
    return forms


def narrow_variant(c_text: str, name: str) -> tuple[str, list[str]] | None:
    """ER-172 (recipes 2.18; func_0012FCA0, func_0012FD70, func_0012FE48 in matching_tools 6.18): where the game
    narrows an argument register at entry, the draft's parameter `argN` of a wider (or other) integer type gets that
    type (u16 / s16 / u8 / s8) and the body's narrowings of it as m2c writes them (`argN & 0xFFFF`, `(s16) argN`, the
    shift pair) become plain `argN`. m2c types the parameter `s32` and writes the mask at each use, so gcc masks at
    the use instead of once at entry. Returns (text, ["arg1 u16", ...]) or None when nothing changes."""
    narrows = game_entry_narrows(name)
    if not narrows:
        return None
    m = SIG_RE.search(c_text)
    while m and m[2] != name:
        m = SIG_RE.search(c_text, m.end())
    if not m:
        return None
    parts = _split_top(m[3]) if m[3].strip() not in ("", "void") else []
    new_parts, used = [], []
    swaps: list[tuple[str, str]] = []
    for p in parts:
        pm = re.fullmatch(r"\s*(.*?)\s*\b(arg([0-7]))\s*", p)
        t = narrows.get(int(pm[3]) + 4) if pm else None
        if t and " ".join(pm[1].split()) in WIDE_INT_TYPES and " ".join(pm[1].split()) != t:
            new_parts.append(f"{t} {pm[2]}")
            used.append(f"{pm[2]} {t}")
            swaps.append((pm[2], t))
        else:
            new_parts.append(p.strip())
    if not swaps:
        return None
    body = c_text[m.end():]
    for v, t in swaps:
        for form in _narrow_forms(v, t):
            body = re.sub(form, lambda mo, v=v: ((mo[1] or "") if mo.re.groups else "") + v, body)
    ret = " ".join(m[1].split())
    head = (ret + name if ret.endswith("*") else f"{ret} {name}") + f"({', '.join(new_parts)}) {{"
    return c_text[:m.start()] + head + body, used


# --- ER-188: early exits, s16 sign tests, the game's return type, stack vectors, global bases -----------------
# Draft fixes from hand matches (recipes 2.42, 2.43, 2.45, 2.46). Each yields candidate texts; attempt() keeps the
# best one when it matches or comes strictly closer, and the variants kept only on a MATCH then start from it (if
# none of them matches, they run again from the draft as it was, so no match is lost).

EARLY_EXIT = True           # --no-early-exit
S16_SIGN = True             # --no-s16-sign
GAME_RET = True             # --no-game-ret
STACK_VEC = True            # --no-stack-vec
GLOBAL_BASE = True          # --no-global-base
WHILE_FIX = True            # --no-while (ER-217)
UNTOUCHED_DRAFT = True      # --no-untouched-draft (ER-217)
RODATA_VEC = True           # --no-rodata-vec (ER-224)
KEEP_WORDS = True           # --no-keep-words: the ER-188 keep rule (size first) for the draft fixes (ER-217)


def _closer(new, old) -> bool:
    """ER-217 keep rule for the draft fixes (ER-188's compared (size off, words) as a tuple, so a fix that brought
    the size closer was kept however many words it added: func_00132108 12 -> 4 bytes off, 33 -> 50 words). Closer
    is: a MATCH; reaching the game's size from a size difference (the store orders and later variants then run);
    else no farther in size and in words and nearer in one of them."""
    if new.match:
        return True
    (ns, nw), (os_, ow) = _off(new), _off(old)
    if not KEEP_WORDS:
        return (ns, nw) < (os_, ow)
    if ns == 0 and os_ > 0:
        return True
    return ns <= os_ and nw <= ow and (ns, nw) != (os_, ow)


def _func_span(c_text: str, name: str):
    """(signature match, body lines without the closing brace, text after the body) of `name`'s definition."""
    m = SIG_RE.search(c_text)
    while m and m[2] != name:
        m = SIG_RE.search(c_text, m.end())
    if not m:
        return None
    rest = c_text[m.end():]
    end = rest.rfind("\n}")
    if end < 0:
        return None
    return m, rest[:end].split("\n"), rest[end:]


def _likely_forward(word: int) -> bool:
    """A branch-likely with a forward target (backward ones are gcc's loop rotation, recipes 2.46)."""
    op, rt = word >> 26, (word >> 16) & 31
    off = word & 0xFFFF
    fwd = 0 < off < 0x8000
    if op in (0x14, 0x15, 0x16, 0x17):                       # beql / bnel / blezl / bgtzl
        return fwd
    if op == 1 and rt in (0x02, 0x03, 0x12, 0x13):           # bltzl / bgezl / bltzall / bgezall
        return fwd
    if op == 17 and (word >> 21) & 31 == 8 and rt & 2:       # bc1fl / bc1tl
        return fwd
    return False


def likely_gap(res, game: eematch.Game, addr: int | None) -> bool:
    """The game's function has more forward branch-likelies than our build of the draft (ER-184 / ER-189's scan)."""
    size = game.size_of.get(addr) if addr is not None else None
    if not size:
        return False
    g = sum(_likely_forward(game.word(addr + o)) for o in range(0, size, 4))
    ours = sum(_likely_forward(w.ours) for w in res.words if w.ours is not None)
    return g > ours


_REL_SWAP = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}


def _top_ops(e: str) -> list[tuple[int, str]]:
    """Top-level comparison, logical, ?: and assignment operators of an expression (outside () and [])."""
    depth, i, ops = 0, 0, []
    while i < len(e):
        c, two = e[i], e[i:i + 2]
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        elif depth == 0:
            if two in ("&&", "||", "==", "!=", "<=", ">="):
                ops.append((i, two))
                i += 2
                continue
            if two in ("<<", ">>", "->"):
                i += 2
                continue
            if c in "<>?=":
                ops.append((i, c))
        i += 1
    return ops


def negations(cond: str) -> list[str]:
    """The inverted test: `==` / `!=` swapped (exact for floats too); `<` `<=` `>` `>=` swapped and also as `!(..)`
    (not the same for a float NaN: a byte match decides); `!x` -> `x`; anything else `!(..)`."""
    c = _strip_parens(cond)
    ops = _top_ops(c)
    if len(ops) == 1 and ops[0][1] in _REL_SWAP:
        i, op = ops[0]
        swapped = f"{c[:i].rstrip()} {_REL_SWAP[op]} {c[i + len(op):].lstrip()}"
        return [swapped] if op in ("==", "!=") else [swapped, f"!({c})"]
    if not ops and c.startswith("!") and not c.startswith("!="):
        inner = c[1:].strip()
        if re.fullmatch(r"[A-Za-z_]\w*", inner) or _strip_parens(inner) != inner:
            return [_strip_parens(inner)]
    if not ops and re.fullmatch(r"[A-Za-z_]\w*", c):
        return [f"!{c}"]
    return [f"!({c})"]


IF_OPEN_RE = re.compile(r"^    if \((.*)\) \{$")


def _early_exit_once(lines: list[str], form: int) -> list[str] | None:
    """One top-level `if (c) { body; return X; } return Y;` at the end of the body -> `if (!c) { return Y; } body;
    return X;` (a void function: `if (c) { body }` last -> `if (!c) { return; } body`). None when the tail is not so."""
    k = len(lines)
    while k and not lines[k - 1].strip():
        k -= 1
    tail = lines[:k]
    if not tail:
        return None
    ret_y = None
    if re.fullmatch(r"    return\b.*;", tail[-1]):
        ret_y, close = tail[-1].strip(), len(tail) - 2
    else:
        close = len(tail) - 1
    if close < 1 or tail[close] != "    }":
        return None
    j = close - 1
    while j >= 0 and not (tail[j].startswith("    ") and not tail[j].startswith("     ") and tail[j].strip()):
        j -= 1
    if j < 0:
        return None
    om = IF_OPEN_RE.match(tail[j])
    if not om:
        return None
    block = tail[j + 1:close]
    if not block or any(ln.strip() and not ln.startswith("        ") for ln in block):
        return None
    if ret_y is not None and not re.fullmatch(r"        return\b.*;", block[-1]):
        return None                                   # the block falls through to `return Y`: not this shape
    if ret_y is None and re.fullmatch(r"        return\b.*;", block[-1]):
        return None
    negs = negations(om[1])
    neg = negs[min(form, len(negs) - 1)]
    exit_ = ret_y if ret_y is not None else "return;"
    return tail[:j] + [f"    if ({neg}) {{", f"        {exit_}", "    }"] + [ln[4:] if ln.strip() else ln
                                                                                for ln in block]


def early_exit_variants(c_text: str, name: str) -> list[tuple[str, str]]:
    """Recipes 2.43 / 2.46 (ER-184: 16 of 16 hits, ER-189: 12 more): m2c writes `if (p != NULL) { ...; return 0; }
    return -1;`, which gcc compiles to a `beqz` past the body; the game's `bnel p, $zero` with the body's first load
    in the likely slot needs the short path first, `if (p == NULL) return -1;`. Applied at the end of the body up to
    3 times (a second test inside the first: `if (x <= 0) return -1;`), each depth a candidate; a relational test
    gives a second spelling (`!(a < b)`). Returns [(note, text)]."""
    sp = _func_span(c_text, name)
    if not sp:
        return []
    m, lines, after = sp
    head = c_text[:m.end()]
    out = []
    for form in (0, 1):
        cur = lines
        for depth in range(1, 4):
            nxt = _early_exit_once(cur, form)
            if nxt is None:
                break
            cur = nxt
            text = head + "\n".join(cur) + after
            if all(text != t for _n, t in out):
                out.append((f"early exit first (x{depth}" + (", !(..)" if form else "") + ")", text))
    return out


def s16_sign_variants(c_text: str, name: str) -> list[tuple[str, str]]:
    """Recipes 2.46 (func_00141E20, func_00143C10): m2c's `t = x - 1; x = t; if (t & 0x8000)` on a halfword is the
    game's `if (--x < 0)` on an s16 (`sll 16` / `bgez`); also a bare `if (t & 0x8000)` -> `if ((s16) t < 0)`.
    Returns [(note, text)]: the fused decrement first when the temporary is used nowhere else."""
    sp = _func_span(c_text, name)
    if not sp or "0x8000" not in c_text:
        return []
    m, lines, after = sp
    head = c_text[:m.end()]
    body = "\n".join(lines)
    out = []
    fused = list(lines)
    n_fused = 0
    for i in range(len(fused) - 2, 1, -1):
        im = re.fullmatch(r"(\s+)if \((!?)\(?(\w+) & 0x8000\)?\) \{", fused[i])
        if not im:
            continue
        ind, neg, t = im[1], im[2], im[3]
        am = re.fullmatch(rf"{ind}{t} = (.+) - 1;", fused[i - 2])
        sm = re.fullmatch(rf"{ind}(.+) = (?:\([us]16\) )?{t};", fused[i - 1])
        if not (am and sm and am[1] == sm[1]):
            continue
        if len(re.findall(rf"\b{t}\b", "\n".join(fused))) != 3 + 1:   # declaration, assignment, store, test
            continue
        lv = sm[1]
        fm = re.fullmatch(r"M2C_FIELD\((.+), [us]16 \*, (\w+)\)", lv)
        s16lv = f"M2C_FIELD({fm[1]}, s16 *, {fm[2]})" if fm else f"(*(s16 *)&{lv})"
        fused[i] = f"{ind}if (--{s16lv} {'>=' if neg else '<'} 0) {{"
        del fused[i - 2:i]
        fused = [ln for ln in fused if not re.fullmatch(rf"    [\w ]+\b{t};", ln)]
        n_fused += 1
        break
    if n_fused:
        out.append(("s16 sign test: --x < 0", head + "\n".join(fused) + after))
    simple, k = re.subn(r"if \(\(?(\w+) & 0x8000\)?\)", r"if ((s16) \1 < 0)", body)
    simple, k2 = re.subn(r"if \(!\((\w+) & 0x8000\)\)", r"if ((s16) \1 >= 0)", simple)
    if k + k2:
        out.append(("s16 sign test: (s16) t < 0", head + simple + after))
    return out


TAIL_CALL_RE = re.compile(r"^    ((func_[0-9A-F]{8})\(.*\));$")


def _retype_callee(c_text: str, protos_h: Path | None, callee: str, ret: str, out: Path, name: str, tag: str,
                   stem: str | None = None):
    """`callee`'s prototype with return type `ret`: the draft's own line (`M2C_UNK func_X(..);  /* extern */`) or the
    prototypes header's (a copy under obj/, the draft's include switched to it). Returns (text, header) or None."""
    pat = re.compile(rf"^(?:extern\s+)?[A-Za-z_][\w \*]*?\s*\b{callee}\(", re.M)
    sm = SIG_RE.search(c_text)
    while sm and sm[2] != name:
        sm = SIG_RE.search(c_text, sm.end())
    pre = c_text[:sm.start()] if sm else c_text
    if pat.search(pre):
        new_pre = pat.sub(f"{ret} {callee}(", pre, count=1)
        return new_pre + c_text[len(pre):], protos_h
    if protos_h and protos_h.exists():
        ph = protos_h.read_text(encoding="utf-8")
        ph2, k = pat.subn(f"{ret} {callee}(", ph, count=1)
        if not k:
            return None
        p2 = out / "obj" / f"{stem or name}.{tag}.protos.h"
        p2.write_text(ph2, encoding="utf-8", newline="\n")
        return c_text.replace(f'#include "{protos_h.as_posix()}"', f'#include "{p2.as_posix()}"'), p2
    return None


def game_ret_variants(c_text: str, name: str, game: eematch.Game, protos_h: Path | None, out: Path,
                      stem: str | None = None):
    """Recipes 2.42 / 2.46: a `void` draft ending in a call whose value the game leaves in `$v0` / `$f0` to the
    `jr $ra`, where the game's callers read that register (game_return_class: func_00130378 and seven more are
    `float`, m2c drafted `void`): `T f(..) { ...; return g(..); }`, the callee's prototype given T as well when it is
    not. Returns [(note, text, protos header)]."""
    cls = game_return_class(name, game_return_scan(game))
    if cls not in ("f32", "s32"):
        return []
    sp = _func_span(c_text, name)
    if not sp or " ".join(sp[0][1].split()) != "void":
        return []
    m, lines, after = sp
    k = len(lines)
    while k and not lines[k - 1].strip():
        k -= 1
    tm = TAIL_CALL_RE.match(lines[k - 1]) if k else None
    if not tm:
        return []
    t = cls if cls == "f32" else "s32"
    body = lines[:k - 1] + [f"    return {tm[1]};"] + lines[k:]
    text = c_text[:m.start()] + f"{t} {name}({m[3]}) {{" + "\n".join(body) + after
    outs = [(f"returns the last call's value ({t}: the game's callers read it)", text, protos_h)]
    callee = tm[2]
    cur = _ret_class(_callee_ret(text, protos_h, callee) or "")
    if cur != ("f32" if t == "f32" else "s32"):
        rt = _retype_callee(text, protos_h, callee, t, out, name, "gret", stem)
        if rt:
            outs.insert(0, (f"returns the last call's value ({t}; {callee} declared {t})", rt[0], rt[1]))
    return outs


def _callee_ret(c_text: str, protos_h: Path | None, callee: str) -> str | None:
    pat = re.compile(rf"^(?:extern\s+)?([A-Za-z_][\w \*]*?)\s*\b{callee}\(", re.M)
    for src in (c_text, protos_h.read_text(encoding="utf-8") if protos_h and protos_h.exists() else ""):
        mm = pat.search(src)
        if mm and not mm[1].strip().startswith("return"):
            return " ".join(mm[1].split())
    return None


def game_sp_floats(game: eematch.Game, addr: int | None) -> bool:
    """The game's function moves floats through its stack locals (`lwc1` / `swc1` at N($sp) below the saves)."""
    size = game.size_of.get(addr) if addr is not None else None
    loc = game_locals_size(game, addr)
    if not size or not loc:
        return False
    for o in range(0, size, 4):
        w = game.word(addr + o)
        if w >> 26 in (0x31, 0x39) and (w >> 21) & 31 == 29 and (w & 0xFFFF) < loc:
            return True
    return False


def stack_vec_variants(c_text: str, name: str, game: eematch.Game, addr: int | None, protos_h: Path | None,
                       out: Path, stem: str | None = None):
    """Recipes 2.46 (func_0012D388, func_00130120, func_0012FFD8, func_00139A70, ...; ER-189 follow-up 5): m2c's
    stack array of a function whose game code moves floats through it (`lwc1` / `swc1` at N($sp)) typed `f32`; and a
    call that passes m2c's `sp` followed by 1-4 values (m2c read the vector's stack stores as more arguments) as the
    stores into the vector, then the call with `sp` last, its prototype cut after that parameter (`f32 *`). Returns
    [(note, text, protos header)]."""
    if not game_sp_floats(game, addr):
        return []
    sp = _func_span(c_text, name)
    if not sp:
        return []
    m, lines, after = sp
    head = c_text[:m.end()]
    outs = []
    typed = [re.sub(r"^    (?:M2C_UNK|s32|u32|int)\s+sp\[", "    f32 sp[", ln) for ln in lines]
    if typed != lines:
        outs.append(("stack array typed f32", head + "\n".join(typed) + after, protos_h))
    base = typed
    for i, ln in enumerate(base):
        cm = re.fullmatch(r"(\s+)((?:[\w\[\]]+ = |return )?)(func_[0-9A-F]{8})\((.*)\);", ln)
        if not cm:
            continue
        args = _split_top(cm[4])
        args = [a.strip() for a in args]
        if "sp" not in args:
            continue
        j = args.index("sp")
        vals = args[j + 1:]
        if not 1 <= len(vals) <= 4:
            continue
        ind = cm[1]
        stores = [f"{ind}sp[{n}] = {v};" for n, v in enumerate(vals)]
        new = base[:i] + stores + [f"{ind}{cm[2]}{cm[3]}({', '.join(args[:j + 1])});"] + base[i + 1:]
        text = head + "\n".join(new) + after
        if not any(re.match(r"^    f32 sp\[", x) for x in new):
            text = re.sub(r"^    (?:M2C_UNK|s32|u32|int)\s+sp\[", "    f32 sp[", text, count=1, flags=re.M)
        callee = cm[3]
        cut = _cut_callee(text, protos_h, callee, j, out, name, stem)
        if cut:
            outs.insert(0, (f"stack vector: {len(vals)} value(s) stored, {callee} takes f32 * (arg {j})",
                            cut[0], cut[1]))
        break
    return outs


def _cut_callee(c_text: str, protos_h: Path | None, callee: str, j: int, out: Path, name: str,
                stem: str | None = None):
    """`callee`'s prototype cut to its first j + 1 parameters, the last one `f32 *` (draft line or header copy)."""
    pat = re.compile(rf"^((?:extern\s+)?[A-Za-z_][\w \*]*?\s*\b{callee})\(([^)]*)\)", re.M)

    def fix(mm):
        ps = [p.strip() for p in _split_top(mm[2])] if mm[2].strip() not in ("", "void") else []
        ps = (ps + ["s32"] * (j + 1))[:j] + ["f32 *"]
        return f"{mm[1]}({', '.join(ps)})"
    if pat.search(c_text):
        return pat.sub(fix, c_text, count=1), protos_h
    if protos_h and protos_h.exists():
        ph2, k = pat.subn(fix, protos_h.read_text(encoding="utf-8"), count=1)
        if not k:
            return None
        p2 = out / "obj" / f"{stem or name}.svec.protos.h"
        p2.write_text(ph2, encoding="utf-8", newline="\n")
        return c_text.replace(f'#include "{protos_h.as_posix()}"', f'#include "{p2.as_posix()}"'), p2
    return None


_TSIZE = {"s8": 1, "u8": 1, "char": 1, "s16": 2, "u16": 2, "s32": 4, "u32": 4, "f32": 4, "int": 4, "M2C_UNK": 4,
          "s64": 8, "u64": 8, "f64": 8}
GFIELD_RE = re.compile(r"M2C_FIELD\((&?D_[0-9A-F]{8}), ([\w ]+?) \*, (0x[0-9A-Fa-f]+|\d+)\)")


def global_base_variants(c_text: str, name: str, protos_h: Path | None = None, out: Path | None = None,
                         stem: str | None = None) -> list[tuple[str, str, Path | None]]:
    """Recipes 2.45 (func_001C1880, func_001C1630, func_001BCCF8, func_001BCD30, func_001BF198; ER-186 follow-up 3):
    `M2C_FIELD(D_x, T *, off)` folds the offset into `%lo(D_x + off)`; the game builds `D_x` in a register and loads
    off it. The forms that keep the base: one typed pointer set from the global (`s16 *p = (s16 *)D_x;`, each access
    `p[off / size]`, another type through `(char *)p + off`), tried with each access type in turn (most used first,
    at most 3); and D_x's extern as an array of that type read by index (`D_001F6B90[9]`): the draft's own line or
    (ER-217, under --no-while's switch) the prototypes header's, as a copy under obj/. Returns [(note, text,
    protos header)]."""
    sp = _func_span(c_text, name)
    if not sp or "M2C_FIELD(" not in c_text:
        return []
    m, lines, after = sp
    body = "\n".join(lines)
    uses = GFIELD_RE.findall(body)
    if not uses:
        return []
    by_g: dict[str, list[tuple[str, int]]] = {}
    for g, t, off in uses:
        by_g.setdefault(g, []).append((" ".join(t.split()), int(off, 0)))
    k = 1 if lines and not lines[0].strip() else 0
    while k < len(lines) and LOCAL_DECL_RE.match(lines[k]):
        k += 1
    outs = []
    # one candidate per type choice of the most used global; every other global takes its own most used type
    gl = sorted(by_g, key=lambda g: -len(by_g[g]))
    first_types = []
    for t, _n in sorted(((t, sum(a == t for a, _o in by_g[gl[0]])) for t in {a for a, _o in by_g[gl[0]]}),
                        key=lambda x: -x[1])[:3]:
        first_types.append(t)
    for t0, unify in [(t, False) for t in first_types] + [(t, True) for t in first_types[:2]]:
        if unify and not any(a != t0 and _TSIZE.get(a) == _TSIZE.get(t0) for a, _o in by_g[gl[0]]):
            continue
        choice = {gl[0]: t0}
        for g in gl[1:]:
            choice[g] = max({a for a, _o in by_g[g]}, key=lambda t: sum(a == t for a, _o in by_g[g]))
        decls, new_body = [], body
        for n, g in enumerate(gl):
            t = choice[g]
            v = f"gb{n}"
            decls.append(f"    {t} *{v} = ({t} *){g};")

            def rep(mm, g=g, t=t, v=v, uni=unify and n == 0):
                if mm[1] != g:
                    return mm[0]
                ta, off = " ".join(mm[2].split()), int(mm[3], 0)
                size = _TSIZE.get(ta)
                if (ta == t or (uni and size == _TSIZE.get(t))) and size and off % size == 0:
                    return f"{v}[{off // size}]"
                return f"(*({ta} *)((char *){v} + 0x{off:X}))"
            new_body = GFIELD_RE.sub(rep, new_body)
        nl = new_body.split("\n")
        text = c_text[:m.end()] + "\n".join(nl[:k] + decls + nl[k:]) + after
        outs.append((f"global base pointer: " + ", ".join(f"{choice[g]} *{g}" for g in gl)
                     + (f" (same-size fields as {t0})" if unify else ""), text, protos_h))
    # index form: a global the draft declares itself, every access one type at aligned offsets
    for g in gl:
        if g.startswith("&"):
            continue
        ts = {a for a, _o in by_g[g]}
        t = next(iter(ts))
        size = _TSIZE.get(t)
        if len(ts) != 1 or not size or any(o % size for _a, o in by_g[g]):
            continue
        ext_re = re.compile(rf"^extern [^;\n(]*\b{g}\b[^;\n(]*;[^\n]*$", re.M)
        em = ext_re.search(c_text[:m.start()])
        ph = protos_h
        if em:
            text = c_text[:em.start()] + f"extern {t} {g}[];" + c_text[em.end():m.start()]
        elif WHILE_FIX and protos_h and protos_h.exists() and out is not None and \
                ext_re.search(protos_h.read_text(encoding="utf-8")):
            # ER-217: the extern is in the prototypes header (func_001C1880): a copy with the array form
            ph = out / "obj" / f"{stem or name}.garr.protos.h"
            ph.write_text(ext_re.sub(f"extern {t} {g}[];", protos_h.read_text(encoding="utf-8"), count=1),
                          encoding="utf-8", newline="\n")
            text = c_text[:m.start()].replace(f'#include "{protos_h.as_posix()}"', f'#include "{ph.as_posix()}"')
        else:
            continue
        text += c_text[m.start():m.end()] + GFIELD_RE.sub(
            lambda mm: f"{g}[{int(mm[3], 0) // size}]" if mm[1] == g else mm[0], body) + after
        outs.append((f"global as an array: extern {t} {g}[]", text, ph))
    return outs


IF_ANY_RE = re.compile(r"^(\s*)if \((.*)\) \{$")


def while_variants(c_text: str, name: str) -> list[tuple[str, str]]:
    """ER-217 (ER-186's func_001C1880, recipes 2.45): m2c writes a `while` loop as `if (c) { do { .. } while (c); }`;
    the game's source had `while (c) { .. }`. Only where the if-block holds nothing but the do-while and both tests
    are the same text. Candidates: every such loop rewritten, then each alone when there are several (at most 3).
    Returns [(note, text)]."""
    sp = _func_span(c_text, name)
    if not sp or "do {" not in c_text:
        return []
    m, lines, after = sp
    head = c_text[:m.end()]
    spots = []
    for i in range(len(lines) - 3):
        im = IF_ANY_RE.match(lines[i])
        if not im or lines[i + 1] != im[1] + "    do {":
            continue
        ind, c = im[1], im[2]
        j = next((j for j in range(i + 2, len(lines) - 1) if lines[j].startswith(ind + "    }")), None)
        if j is None or lines[j] != f"{ind}    }} while ({c});" or lines[j + 1] != ind + "}":
            continue
        if any(ln.strip() and not ln.startswith(ind + "        ") for ln in lines[i + 2:j]):
            continue
        spots.append((i, j))

    def rewrite(sel):
        new = list(lines)
        for i, j in sorted(sel, reverse=True):
            ind = IF_ANY_RE.match(new[i])[1]
            c = IF_ANY_RE.match(new[i])[2]
            inner = [ln[4:] if ln.strip() else ln for ln in new[i + 2:j]]
            new[i:j + 2] = [f"{ind}while ({c}) {{"] + inner + [ind + "}"]
        return head + "\n".join(new) + after
    if not spots:
        return []
    outs = [(f"while loop for m2c's if + do-while (x{len(spots)})", rewrite(spots))]
    if len(spots) > 1:
        for k, s in enumerate(spots[:3]):
            outs.append((f"while loop for m2c's if + do-while (loop {k + 1} of {len(spots)})", rewrite([s])))
    return outs


# --- ER-224: a .rodata vector copied with ld / sd as a struct copy from an `extern const` -------------------------
_RODATA_NAMES: set[str] | None = None
_VEC_TYPES: dict[str, tuple[str, str, int | None]] = {}
VEC_TYPEDEF_RE = re.compile(r"typedef\s+struct\s*\{\s*(float|f32)\s+v\s*\[\s*4\s*\]\s*;\s*\}\s*"
                            r"__attribute__\s*\(\(\s*aligned\s*\(\s*16\s*\)\s*\)\)\s*(\w+)\s*;")
VEC_HALF_RE = re.compile(r"^(\s+)(sp([0-9A-F]+)|\(\*\(s64 \*\)&sp\[(\d+)\]\)|\*\(s64 \*\)\(\(s8 \*\)sp \+ 0x([0-9A-F]+)\))"
                         r" = M2C_FIELD\(&(D_[0-9A-F]{8}), s64 \*, (0x0|0|0x8|8)\);$")
VEC_TYPEDEF_LINE_RE = re.compile(r"^typedef struct \{ (?:float|f32) v\[4\]; \} __attribute__\(\(aligned\(16\)\)\) "
                                 r"(\w+);$")


def rodata_names() -> set[str]:
    """Every INCLUDE_RODATA symbol of src/ (read once)."""
    global _RODATA_NAMES
    if _RODATA_NAMES is None:
        _RODATA_NAMES = set()
        for p in (ROOT / "src").rglob("*.c"):
            _RODATA_NAMES.update(re.findall(r'INCLUDE_RODATA\("[^"]*",\s*(D_[0-9A-F]{8})\)',
                                            p.read_text(encoding="utf-8", errors="replace")))
    return _RODATA_NAMES


def vec_type(src_file: str | None) -> tuple[str, str, int | None]:
    """(type name, one-line typedef, the file's line of its own typedef or None) for a 16-byte aligned float vector:
    the target file's own name and field type where it has one (text_001000C0 `Vec4`, text_00150878 `Vec4A`), else
    `Vec4` (Unofficial; `Vec4R` if the file already uses the name Vec4 for something else)."""
    key = src_file or ""
    if key not in _VEC_TYPES:
        p = ROOT / "src" / f"{key}.c"
        text = p.read_text(encoding="utf-8") if key and p.exists() else ""
        m = VEC_TYPEDEF_RE.search(text)
        if m:
            _VEC_TYPES[key] = (m[2], f"typedef struct {{ {m[1]} v[4]; }} __attribute__((aligned(16))) {m[2]};",
                               text.count("\n", 0, m.start()) + 1)
        else:
            n = "Vec4R" if re.search(r"\bVec4\b", text) else "Vec4"
            _VEC_TYPES[key] = (n, f"typedef struct {{ f32 v[4]; }} __attribute__((aligned(16))) {n};", None)
    return _VEC_TYPES[key]


def rodata_vec_variants(c_text: str, name: str, protos_h: Path | None = None, out: Path | None = None,
                        stem: str | None = None) -> list[tuple[str, str, Path | None]]:
    """ER-224 (recipes 2.49 / 2.52: func_001A3D00, func_00126F00, func_00125B68): the game copies an INCLUDE_RODATA
    vector into a stack local with two `ld` / `sd` pairs; m2c writes the halves as `spN = M2C_FIELD(&D_x, s64 *, 0);`
    and `sp<N+8> = M2C_FIELD(&D_x, s64 *, 8);` (after the frame fix: `(*(s64 *)&sp[k]) = ...`). The prologue takes the
    game's order only from a struct copy of a `const` extern: `Vec4 spN = D_x;` with `extern const Vec4 D_x;` and the
    file's own vector typedef (one line after `#include "common.h"`). A named local must be used only as `&spN`
    elsewhere and its upper half not at all, else that pair is left alone. Candidates: every pair as an assignment at
    m2c's line; then, when every pair is a named local copied at the top level of the body above any label, as
    initializers. Returns [(note, text, protos header)]."""
    sp = _func_span(c_text, name)
    if not sp or "s64 *, " not in c_text:
        return []
    m, lines, after = sp
    rod = rodata_names()
    halves: dict[tuple, int] = {}          # (D_x, kind, start of the vector, half offset) -> line
    for i, ln in enumerate(lines):
        hm = VEC_HALF_RE.match(ln)
        if not hm or hm[6] not in rod:
            continue
        off = int(hm[7], 0)
        if hm[3] is not None:
            key = (hm[6], "var", int(hm[3], 16) - off, off)
        elif hm[5] is not None:                        # ER-224's extra-argument stores: a byte offset from sp
            key = (hm[6], "spb", int(hm[5], 16) - off, off)
        else:
            key = (hm[6], "arr", int(hm[4]), off)      # array index of the half; matched below by element size
        halves.setdefault(key, i)
    if not halves:
        return []
    dm = re.search(r"^    (M2C_UNK|s32|u32|f32|int|s64|u64)\s+sp\[", "\n".join(lines), re.M)
    elem = _TSIZE.get(dm[1], 4) if dm else None
    ok, used = [], set()
    for (g, kind, base, off), i0 in sorted(halves.items(), key=lambda kv: kv[1]):
        if off != 0:
            continue
        if kind in ("var", "spb"):
            i8 = halves.get((g, kind, base, 8))
        else:
            i8 = halves.get((g, kind, base + 8 // elem, 8)) if elem and elem <= 8 else None
        if i8 is None or i0 in used or i8 in used:
            continue
        dlo = dhi = None
        if kind == "var":
            lo_n, hi_n = f"sp{base:X}", f"sp{base + 8:X}"
            dl = [k for k, ln in enumerate(lines) if re.fullmatch(rf"    [A-Za-z_][\w ]*?[\s*]{lo_n};", ln)]
            dh = [k for k, ln in enumerate(lines) if re.fullmatch(rf"    [A-Za-z_][\w ]*?[\s*]{hi_n};", ln)]
            if len(dl) != 1 or len(dh) != 1:
                continue
            dlo, dhi = dl[0], dh[0]
            others = "\n".join(ln for k, ln in enumerate(lines) if k not in (i0, i8, dlo, dhi))
            if re.search(rf"\b{hi_n}\b", others) or re.search(rf"(?<![&\w]){lo_n}\b", others):
                continue
        ok.append((g, kind, base, i0, i8, dlo, dhi))
        used.update((i0, i8))
    if not ok:
        return []
    src_file = ASM_INDEX[name].parent.name if name in ASM_INDEX else None
    vt, tdef, _tline = vec_type(src_file)
    names = list(dict.fromkeys(p[0] for p in ok))

    # declarations: the typedef after common.h (before every other include: the prototypes header may hold an extern
    # that uses it), each D_x's extern (draft line, else the prototypes header's as a copy) as `extern const vt D_x;`
    pre = c_text[:m.start()]
    ph, ph_text = protos_h, (protos_h.read_text(encoding="utf-8") if protos_h and protos_h.exists() else None)
    ph_new = ph_text
    for g in names:
        # (a data extern only: no `(` before the name; the section attribute after it is fine)
        ext_re = re.compile(rf"^extern [^;\n(]*\b{g}\b(?:\s*\[\w*\])?(?:\s+__attribute__\(\([^;\n]*\)\))?\s*;[^\n]*$",
                            re.M)
        if ext_re.search(pre):
            pre = ext_re.sub(f"extern const {vt} {g};", pre, count=1)
        elif ph_new is not None and ext_re.search(ph_new):
            ph_new = ext_re.sub(f"extern const {vt} {g};", ph_new, count=1)
        else:
            pre = pre.rstrip("\n") + f"\nextern const {vt} {g};\n\n"
    if ph_new != ph_text and out is not None:
        ph = out / "obj" / f"{stem or name}.rvec.protos.h"
        ph.write_text(ph_new, encoding="utf-8", newline="\n")
        pre = pre.replace(f'#include "{protos_h.as_posix()}"', f'#include "{ph.as_posix()}"')
    cm = re.search(r'^#include "common\.h"\n', pre, re.M)
    pre = (pre[:cm.end()] + tdef + "\n" + pre[cm.end():]) if cm else tdef + "\n" + pre
    head = pre + c_text[m.start():m.end()]

    def build(init: bool) -> str:
        new, drop = list(lines), set()
        for g, kind, base, i0, i8, dlo, dhi in ok:
            ind = VEC_HALF_RE.match(lines[i0])[1]
            drop.add(i8)
            if kind == "arr":
                new[i0] = f"{ind}*({vt} *)&sp[{base}] = {g};"
                continue
            if kind == "spb":
                new[i0] = f"{ind}*({vt} *)((s8 *)sp + 0x{base:X}) = {g};"
                continue
            lo_n = f"sp{base:X}"
            drop.add(dhi)
            if init:
                new[dlo] = f"    {vt} {lo_n} = {g};"
                drop.add(i0)
            else:
                new[dlo] = f"    {vt} {lo_n};"
                new[i0] = f"{ind}{lo_n} = {g};"
        return head + "\n".join(ln for k, ln in enumerate(new) if k not in drop) + after

    note = f"rodata vector copy: extern const {vt} " + ", ".join(names) + \
        f" ({len(ok)} cop{'y' if len(ok) == 1 else 'ies'}"
    outs = [(note + ")", build(False), ph)]
    first_label = next((k for k, ln in enumerate(lines) if re.fullmatch(r"\w+:", ln.strip())), len(lines))
    if all(p[1] == "var" and p[3] < first_label and VEC_HALF_RE.match(lines[p[3]])[1] == "    " for p in ok):
        outs.append((note + ", initializers)", build(True), ph))
    return outs


EXTRA_STORES = True         # --no-extra-stores (ER-224)
_ST_TYPES = {"sw": "s32", "swc1": "f32", "sd": "s64", "sh": "s16", "sb": "s8"}
SP_ST_RE = re.compile(r"^(-?0x[0-9A-Fa-f]+|\d+)\(\$(?:29|sp)\)$")


def game_call_stores(name: str) -> dict[str, list[dict[int, str]]]:
    """For each `jal` of the function's splat asm, in order: callee -> [{sp offset: store mnemonic}] of the stores to
    N($sp) since the previous jal (its delay slot excluded), this jal's delay slot included; the last store to an
    offset wins. A straight-line scan (branches ignored)."""
    p = ASM_INDEX.get(name)
    if not p or not p.exists():
        return {}
    ins = []
    for line in p.read_text(encoding="utf-8", errors="replace").splitlines():
        om = OP_RE.match(line)
        if om:
            ins.append((om[1], [o.strip() for o in om[2].split(",")] if om[2].strip() else []))
    out: dict[str, list[dict[int, str]]] = {}
    start = 0
    for i, (op, ops) in enumerate(ins):
        if op != "jal" or not ops:
            continue
        st: dict[int, str] = {}
        for op2, ops2 in ins[start:i + 2]:
            if op2 in _ST_TYPES and len(ops2) == 2 and (sm := SP_ST_RE.match(ops2[1])):
                st[int(sm[1], 0)] = op2
        out.setdefault(ops[0], []).append(st)
        start = i + 2
    return out


def extra_arg_stores(c_text: str, name: str) -> tuple[str, str] | None:
    """ER-224: m2c passes the stores a function makes into its lowest stack slots before a call (a vector at 0($sp)
    built for the callee, func_00125D00's `sp[0..2]`; the halves of a .rodata vector copy) as `/* extra? */`
    arguments, and the draft does not compile (too many arguments). Each such call is matched to the game's jal of
    the same callee by order (only when the draft has as many calls to it as the asm has jals); its n extras become
    `*(T *)((s8 *)sp + N) = value;` before the statement, T from the store (sw / swc1 / sd / sh / sb), N the n lowest
    stored offsets below the draft's first named stack local; where that does not fit (or the call sits in a loop
    test), the extras are dropped. Returns (text, note) or None."""
    sp = _func_span(c_text, name)
    if not sp:
        return None
    m, lines, after = sp
    sites = game_call_stores(name)
    named = [int(x, 16) for x in re.findall(r"^    [^;\n]*?\bsp([0-9A-F]+)\b(?:\[[^\]]*\])?;", "\n".join(lines),
                                            re.M)]
    limit = min(named) if named else 1 << 16
    calls: dict[str, list[tuple[int, int, int]]] = {}       # callee -> [(line, start of name, end paren)]
    for i, ln in enumerate(lines):
        for cm in re.finditer(r"\b(func_[0-9A-F]{8})\(", ln):
            calls.setdefault(cm[1], []).append((i, cm.start(), _close_paren(ln, cm.end())))
    new = list(lines)
    inserts: dict[int, list[str]] = {}
    n_st = n_drop = 0
    for callee, cl in calls.items():
        mapped = len(sites.get(callee, [])) == len(cl)
        for k, (i, s0, j) in enumerate(cl):
            ln = new[i]
            if "/* extra? */" not in lines[i][s0:j + 1]:
                continue
            # find this call again in the line (an earlier call on it may have been edited): its nth occurrence
            occ = list(re.finditer(rf"\b{callee}\(", ln))
            nth = sum(1 for (i2, s2, _j2) in cl if i2 == i and s2 < s0)
            if nth >= len(occ):
                continue
            cm = occ[nth]
            jj = _close_paren(ln, cm.end())
            args = _split_top(ln[cm.end():jj])
            keep = [a for a in args if "/* extra? */" not in a]
            extras = [a.replace("/* extra? */", "").strip() for a in args if "/* extra? */" in a]
            stores = []
            st = sites[callee][k] if mapped else {}
            offs = sorted(o for o in st if 0 <= o < limit)
            loop_test = re.match(r"^\s*(?:\}|while \(|for \()", ln) is not None   # no statement slot before it
            if mapped and len(offs) == len(extras) and st and not loop_test and \
                    all(st[o] in _ST_TYPES for o in offs):
                ind = re.match(r"^(\s*)", ln)[1]
                stores = [f"{ind}*({_ST_TYPES[st[o]]} *)((s8 *)sp + 0x{o:X}) = {v};" for o, v in zip(offs, extras)]
                n_st += len(stores)
            else:
                n_drop += len(extras)
            new[i] = ln[:cm.end()] + ", ".join(a.strip() for a in keep) + ln[jj:]
            if stores:
                inserts.setdefault(i, []).extend(stores)
    if not n_st and not n_drop:
        return None
    body = []
    for i, ln in enumerate(new):
        body.extend(inserts.get(i, []))
        body.append(ln)
    if n_st and not re.search(r"^    [^;\n]*\bsp\[[^\]]*\];", "\n".join(body), re.M):
        k = 1 if body and not body[0].strip() else 0
        body.insert(k, "    M2C_UNK sp[64];")
    note = "extra? arguments" + (f": {n_st} as stores into sp" if n_st else "") + \
        (f"{',' if n_st else ':'} {n_drop} dropped" if n_drop else "")
    return c_text[:m.end()] + "\n".join(body) + after, note


_FLOAT_PARAM_RE = re.compile(r"^(?:const\s+)?(?:f32|f64|float|double)\s+\w+$")


def shifted_calls(res, game: eematch.Game) -> dict[str, int]:
    """ER-217: calls whose arguments sit d >= 1 registers higher in the game than in our build ({callee: d}): a
    differing word where the game writes $a<i+d> and ours is the same instruction writing $a<i> (other operands
    equal). The callee is found as in passthrough_calls (the j / jal whose delay slot holds the word, else the next
    within 16 words); a callee seen with two different shifts is left out. passthrough_calls is the d = 1 case with
    looser operand checks; this one also reads func_00130120 ($a3 for our $a0: three arguments passed on)."""
    found: dict[str, set[int]] = {}
    if res is None or res.game_size is None or res.our_size != res.game_size:
        return {}
    for w in res.words:
        if w.ok or not w.ours or w.game is None:
            continue
        vram = res.addr + w.off
        gm, gops = _ops(w.game, vram)
        om, oops = _ops(w.ours, vram)
        if om != gm or len(oops) != len(gops) or not gops or gops[0] not in ARG_REGS or oops[0] not in ARG_REGS \
                or oops[1:] != gops[1:]:
            continue
        d = ARG_REGS.index(gops[0]) - ARG_REGS.index(oops[0])
        if d < 1:
            continue
        target = _call_target(game.word(vram - 4), vram - 4) if w.off >= 4 else None
        off = w.off
        while target is None and off < min(res.game_size, w.off + 64):
            target = _call_target(game.word(res.addr + off), res.addr + off)
            off += 4
        if target is None:
            continue
        found.setdefault(game.name_at.get(target) or f"func_{target:08X}", set()).add(d)
    return {c: next(iter(ds)) for c, ds in found.items() if len(ds) == 1}


def untouched_draft_variants(c_text: str, name: str, res, game: eematch.Game, protos_h: Path | None, out: Path,
                             stem: str | None = None):
    """ER-217 (ER-188 follow-up 4: func_0012D388, func_00130120 at 1 word after the stack vector): a call whose
    arguments sit d registers higher in the game (shifted_calls) gets the function's own first d integer parameters
    as new first arguments (the function hands $a0.. on untouched), the callee's prototype d leading `s32`. The
    ER-147 untouched-args retry re-drafts with m2c before the draft fixes, and its callee scan needs the callee's
    splat asm (a placed callee has none), so it cannot follow the stack-vector fix; this is the same change on the
    draft. Integer parameters the function lacks are added (`s32 argN`, after its last integer parameter, or first):
    floats and integers take separate registers in this ABI (func_0012D388 takes only `f32 fparg0`, in $f12).
    Returns [(note, text, protos header)]."""
    calls = shifted_calls(res, game)
    sp = _func_span(c_text, name)
    if not calls or not sp:
        return []
    m, lines, after = sp
    params = [] if m[3].strip() in ("", "void") else [" ".join(p.split()) for p in _split_top(m[3])]
    if any("..." in p or "(" in p for p in params):
        return []
    ints = [k for k, p in enumerate(params) if not _FLOAT_PARAM_RE.match(p)]
    need = max(calls.values())
    names = [re.findall(r"\w+", params[k])[-1] for k in ints[:need]]
    new_params = list(params)
    added = []
    n = len(ints)
    while len(names) < need:
        v = f"arg{n}" if not re.search(rf"\barg{n}\b", c_text) else f"uarg{n}"
        names.append(v)
        added.append(f"s32 {v}")
        n += 1
    if added:
        at = ints[-1] + 1 if ints else 0
        new_params[at:at] = added
    sig = c_text[m.start():m.end()].replace(f"{name}({m[3]})", f"{name}({', '.join(new_params)})", 1)
    body = "\n".join(lines)
    pre = c_text[:m.start()]
    ph = protos_h
    ph_text = ph.read_text(encoding="utf-8") if ph and ph.exists() else None
    ph_changed = False
    for callee in sorted(calls):
        d = calls[callee]
        lead = ", ".join(names[:d])
        body, k = re.subn(rf"\b{callee}\((\s*\))?",
                          lambda mm, c=callee: f"{c}({lead})" if mm[1] else f"{c}({lead}, ", body)
        if not k:
            return []
        pat = re.compile(rf"^((?:extern\s+)?[A-Za-z_][\w \*]*?\s*\b{callee})\(([^)]*)\)", re.M)

        def fix(mm, d=d):
            ps = [] if mm[2].strip() in ("", "void") else [mm[2].strip()]
            return f"{mm[1]}({', '.join(['s32'] * d + ps)})"
        if pat.search(pre):
            pre = pat.sub(fix, pre, count=1)
        elif ph_text is not None and pat.search(ph_text):
            ph_text = pat.sub(fix, ph_text, count=1)
            ph_changed = True
        else:
            return []
    if added:
        own_pat = re.compile(rf"^((?:extern\s+)?[A-Za-z_][\w \*]*?\s*\b{name})\([^)]*\)\s*;", re.M)
        pre = own_pat.sub(lambda mm: f"{mm[1]}({', '.join(new_params)});", pre, count=1)
    if ph_changed:
        p2 = out / "obj" / f"{stem or name}.uarg.protos.h"
        p2.write_text(ph_text, encoding="utf-8", newline="\n")
        pre = pre.replace(f'#include "{ph.as_posix()}"', f'#include "{p2.as_posix()}"')
        ph = p2
    note = "untouched argument(s) passed on: " + ", ".join(f"{c} gets {', '.join(names[:calls[c]])} first"
                                                           for c in sorted(calls))
    return [(note + (f" ({', '.join(added)} added)" if added else ""), pre + sig + body + after, ph)]


def store_variants(c_text: str):
    """Offset orders first (ER-124: at most 3 compiles), then the permutations of store_orders. Yields (note, text)."""
    if STORE_SORT:
        for mode, text in offset_orders(c_text):
            yield f"store order by offset ({mode})", text
    for text in store_orders(c_text):
        yield "store order permuted", text


def _off(res) -> tuple[int, int]:
    """How far a compared draft is off: (size difference in bytes, differing words)."""
    return abs(res.our_size - (res.game_size or 0)), sum(not w.ok for w in res.words)


def compile_compare(name: str, addr: int | None, c_text: str, c_path: Path, out: Path, game: eematch.Game):
    c_path.write_text(c_text, encoding="utf-8", newline="\n")
    obj_path, msg = compile_draft(c_path, out / "obj")
    if obj_path is None:
        return None, msg
    try:
        return eematch.compare_function(eematch.Obj(obj_path), game, name, addr), ""
    except SystemExit as e:
        return False, str(e)


def attempt(name: str, addr: int | None, a_out: Path, out: Path, m2c_dir: Path, ctx: Path | None,
            protos_h: Path | None, ctx_label: str, game: eematch.Game, timeout: int, hilo: set[str],
            rt: bool = False, rt_skip: frozenset = frozenset()) -> Attempt | None:
    """One draft from one context through the variants. rt (ER-175): the draft's own callee prototypes with the
    game's return types (rt_fix_draft; `rt_skip`: callees left as m2c wrote them); returns None when that changes
    nothing. m2c runs once per (asm, context)."""
    at = Attempt(ctx_label, protos_h=protos_h, ctx=ctx)
    key = (str(a_out), str(ctx))
    if key not in _M2C_CACHE:
        _M2C_CACHE[key] = run_m2c(m2c_dir, a_out, ctx, timeout)
    rc, draft, err = _M2C_CACHE[key] if rt else _M2C_CACHE.pop(key)
    at.files["m2c.txt"] = draft + ("\n/* stderr:\n" + err + "*/\n" if err.strip() else "")
    fail = re.search(r"Decompilation failure:\s*\n\s*\n?(.*)", draft)
    if rc != 0 or fail or not draft.strip():
        at.klass = "m2c failed"
        reason = (fail[1] if fail else (err.strip().splitlines() or ["no output"])[-1]).strip()
        pm = re.search(r"Failed to parse instruction at .*?: (\S+)", reason)
        at.detail = f"cannot parse `{pm[1]}`" if pm else reason[:120]
        return at
    at.m2c_ok = True
    at.n_err = len(re.findall(r"\bM2C_ERROR\(", draft))
    includes = [out / "m2c_extra.h", m2c_dir / "m2c_macros.h"] + ([protos_h] if protos_h else [])
    c_text, at.notes = clean_draft(draft, name, includes, hilo)
    if rt:
        c_text, used = rt_fix_draft(c_text, game, rt_skip)
        if not used:
            return None
        at.rt_used = [u.split()[0] for u in used]
        at.notes.append("draft prototypes with the game's return types: " + ", ".join(used)
                        + (" (kept as m2c wrote: " + ", ".join(sorted(rt_skip)) + ")" if rt_skip else ""))
    if at.n_err:
        at.notes.append(f"{at.n_err} M2C_ERROR")
    at.files["c"] = c_text
    # compile under a per-attempt name: the variants of one function share the folder
    c_path = out / "obj" / f"{name}.{re.sub(r'[^a-z]+', '_', ctx_label)}.c"
    c_draft = c_text
    res, msg = compile_compare(name, addr, c_text, c_path, out, game)
    if res is None and EXTRA_STORES and "too many arguments to function" in msg and "/* extra? */" in c_text:
        # ER-224: m2c's `/* extra? */` arguments are the game's stack stores before the call (a vector at 0($sp));
        # as stores into `sp` (or dropped when they cannot be placed), compiled under the same name (the variants
        # below then start from it)
        xs = extra_arg_stores(c_text, name)
        if xs:
            r2, m2 = compile_compare(name, addr, xs[0], c_path, out, game)
            if r2 is not None:
                res, msg, c_text, c_draft = r2, m2, xs[0], xs[0]
                at.files["c"] = c_text
                at.notes.append(xs[1])
            else:
                at.notes.append(xs[1] + ": still no compile")
    if res is None:
        at.files["err.txt"] = msg
        if msg.startswith(TIMEOUT_TAG):
            at.klass, at.detail = TIMEOUT_TAG, msg
            at.notes.append(f"{TIMEOUT_TAG} ({ctx_label})")
            return at
        at.klass = "compile error"
        at.detail = gcc_error_class(msg)
        return at
    at.compiles = True
    if res is False:
        at.klass = "compare failed"
        at.detail = msg[:120]
        return at
    if not res.match and res.game_size and res.our_size < res.game_size:
        alt = return_wrapper(c_text)
        if alt:
            res2, _msg2 = compile_compare(name, addr, alt, c_path.with_suffix(".ret.c"), out, game)
            if res2 and (res2.match or abs(res2.our_size - res2.game_size) < res.game_size - res.our_size):
                res, c_text = res2, alt
                at.files["c"] = alt
                at.notes.append("return-wrapper form")
    s_base = c_path.with_suffix(".s")
    # ER-147 / ER-151: draft fixes kept when they match or come no further off (then the base of the variants below)
    if not res.match and FRAME_FIX:
        size = game_locals_size(game, addr)
        fv = frame_variant(c_text, name, size)
        if fv:
            fpath = c_path.with_suffix(".frame.c")
            res2, _msg2 = compile_compare(name, addr, fv, fpath, out, game)
            if res2 and (res2.match or _off(res2) <= _off(res)):
                res, c_text, c_draft, s_base = res2, fv, fv, fpath.with_suffix(".s")
                at.files["c"] = fv
                at.notes.append(f"locals sized from the game's frame (0x{size:X} bytes)")
            else:
                at.notes.append(f"frame-sized locals tried (0x{size:X} bytes), no closer")
    if not res.match and PACKER_VARIANT:
        pv = packer_variant(c_text, name)
        if pv:
            ppath = c_path.with_suffix(".pack.c")
            res2, _msg2 = compile_compare(name, addr, pv, ppath, out, game)
            if res2 and (res2.match or _off(res2) <= _off(res)):
                res, c_text, c_draft, s_base = res2, pv, pv, ppath.with_suffix(".s")
                at.files["c"] = pv
                at.notes.append("OR terms in parameter order (packer)")
            else:
                at.notes.append("packer order tried, no closer")
    # ER-188: draft fixes from hand matches (early exit, s16 sign test, the game's return type, stack vector, global
    # base), each kept when it matches or comes strictly closer; `pres` holds the states before each kept change, so
    # the variants kept only on a MATCH can run again from them (no match lost)
    # ER-217: `while` loops first (kept when no farther: the same loop in the game's spelling, which the global base
    # may need, func_001C1880), the untouched argument after the stack vector; the keep rule is _closer
    # ER-224: the .rodata vector copies first of all, kept by _closer only (kept when exactly as far off, they took
    # the M2C_FIELD halves the global base would have used: func_001254F8 80 -> 154 words)
    pres: list[tuple] = []
    for fix in ("rvec", "while", "early", "s16", "gret", "svec", "uarg", "gbase"):
        if res.match:
            break
        cands: list[tuple[str, str, Path | None]] = []
        if fix == "rvec" and RODATA_VEC:
            cands = rodata_vec_variants(c_text, name, at.protos_h, out, c_path.stem)
        elif fix == "while" and WHILE_FIX:
            cands = [(n, t, at.protos_h) for n, t in while_variants(c_text, name)]
        elif fix == "uarg" and UNTOUCHED_DRAFT:
            cands = untouched_draft_variants(c_text, name, res, game, at.protos_h, out, c_path.stem)
        elif fix == "early" and EARLY_EXIT and likely_gap(res, game, addr):
            cands = [(n, t, at.protos_h) for n, t in early_exit_variants(c_text, name)]
        elif fix == "s16" and S16_SIGN:
            cands = [(n, t, at.protos_h) for n, t in s16_sign_variants(c_text, name)]
        elif fix == "gret" and GAME_RET:
            cands = game_ret_variants(c_text, name, game, at.protos_h, out, c_path.stem)
        elif fix == "svec" and STACK_VEC:
            cands = stack_vec_variants(c_text, name, game, addr, at.protos_h, out, c_path.stem)
        elif fix == "gbase" and GLOBAL_BASE:
            cands = global_base_variants(c_text, name, at.protos_h, out, c_path.stem)
        best = None
        for k, (note, alt, ph) in enumerate(cands[:8]):
            fpath = c_path.with_suffix(f".{fix}{k}.c")
            r2 = compile_compare(name, addr, alt, fpath, out, game)[0]
            if r2 and (best is None or (not best[0].match and _closer(r2, best[0]))):
                best = (r2, alt, note, ph, fpath)
            if r2 and r2.match:
                break
        if not cands:
            continue
        if best and (_closer(best[0], res) or (fix == "while" and _off(best[0]) == _off(res))):
            was = sum(not w.ok for w in res.words)
            pres.append((res, c_text, c_draft, s_base, at.protos_h))
            res, c_text, c_draft, s_base = best[0], best[1], best[1], best[4].with_suffix(".s")
            at.protos_h = best[3]
            at.files["c"] = c_text
            at.notes.append(best[2] + ("" if res.match else f" ({was} -> {sum(not w.ok for w in res.words)} words)"))
        else:
            at.notes.append(f"{cands[0][0].split(':')[0].split(' (')[0]} tried, no closer")
    # ER-172: parameters the game narrows at entry typed u16 / s16 / u8 / s8 (then the store orders of that form);
    # ER-180: kept when it matches or brings the draft closer (the variants below then start from it; if none of them
    # matches, they run again from the draft as it was before the narrowing, so no match is lost)
    narrow_pre = None
    if not res.match and NARROW_VARIANT:
        nv = narrow_variant(c_text, name)
        if nv:
            alt, used = nv
            npath = c_path.with_suffix(".narrow.c")
            r0 = compile_compare(name, addr, alt, npath, out, game)[0]
            tries = [(r0, alt, "", npath)]
            if r0 and not r0.match and r0.game_size == r0.our_size:
                for k, (note, alt2) in enumerate(store_variants(alt)):
                    if k >= STORE_LIMIT:
                        break
                    opath = c_path.with_suffix(f".narrow.ord{k}.c")
                    r3 = compile_compare(name, addr, alt2, opath, out, game)[0]
                    tries.append((r3, alt2, note, opath))
                    if r3 and r3.match:
                        break
            hit = next(((r, t, nt) for r, t, nt, _p in tries if r and r.match), None)
            if hit:
                res, c_text = hit[0], hit[1]
                at.files["c"] = c_text
                at.notes.append("narrow parameter: " + ", ".join(used) + (f"; {hit[2]}" if hit[2] else ""))
            else:
                best = min((x for x in tries if x[0]), default=None, key=lambda x: _off(x[0]))
                if NARROW_KEEP and best and _off(best[0]) < _off(res):
                    narrow_pre = (res, c_text, c_draft, s_base, at.protos_h)
                    was = sum(not w.ok for w in res.words)
                    res, c_text, c_draft, s_base = best[0], best[1], best[1], best[3].with_suffix(".s")
                    at.files["c"] = c_text
                    at.notes.append("narrow parameter kept, closer: " + ", ".join(used) + (
                        f"; {best[2]}" if best[2] else "") + f" ({was} -> {sum(not w.ok for w in res.words)} words)")
                else:
                    at.notes.append("narrow parameter tried (" + ", ".join(used) + "), no match" + (
                        f" (best: {diff_class(best[0])}, {sum(not w.ok for w in best[0].words)} words)"
                        if best else ""))

    def later(res, c_text, c_draft, s_base, tag=""):
        """The variants kept only on a MATCH (store orders, ruling 3.14, const floats) on one base draft. Returns
        (res, c_text, notes, protos_h or None, consts or None)."""
        notes: list[str] = []
        ph_new = consts = None
        if not res.match and res.game_size == res.our_size:
            for k, (note, alt) in enumerate(store_variants(c_text)):
                res2, _msg2 = compile_compare(name, addr, alt, c_path.with_suffix(f".ord{k}.c"), out, game)
                if res2 and res2.match:
                    res, c_text = res2, alt
                    notes.append(note)
                    break
        if not res.match and SIBCALL_314:
            svs = sibcall_variants(c_draft, name, addr, s_base, game)
            tries = []
            n_ord = 0
            for v, sv in svs:
                r0 = compile_compare(name, addr, sv, c_path.with_suffix(f".r314_{v}.c"), out, game)[0]
                tries.append((r0, sv, ""))
                if r0 and r0.match:
                    break
                if r0 and r0.game_size == r0.our_size:
                    for k, (note, alt2) in enumerate(store_variants(sv)):
                        if n_ord >= STORE_LIMIT:
                            break
                        n_ord += 1
                        r3, _m3 = compile_compare(name, addr, alt2, c_path.with_suffix(f".r314_{v}.ord{k}.c"), out,
                                                  game)
                        tries.append((r3, alt2, note))
                        if r3 and r3.match:
                            break
                if any(r and r.match for r, _t, _n in tries):
                    break
            if svs:
                hit = next(((r, t, nt) for r, t, nt in tries if r and r.match), None)
                if hit:
                    res, c_text = hit[0], hit[1]
                    notes.append("ruling 3.14 (void)&" + SIBCALL_ARG_RE.search(c_text)[1]
                                 + (f"; {hit[2]}" if hit[2] else ""))
                else:
                    best = min((r for r, _t, _n in tries if r), default=None, key=_off)
                    notes.append(f"ruling 3.14 tried ({', '.join(v for v, _s in svs)}), no match" + (
                        f" (best: {diff_class(best)}, {sum(not w.ok for w in best.words)} words)" if best else ""))
        if not res.match and CONST_VARIANT:
            cv = const_variant(c_text, name, at.protos_h, out, c_path.stem + tag)
            if cv:
                alt, ph2, used = cv
                res2, _msg2 = compile_compare(name, addr, alt, c_path.with_suffix(".const.c"), out, game)
                tries = [(res2, alt, "")]
                if res2 and not res2.match and res2.game_size == res2.our_size:
                    for k, (note, alt2) in enumerate(store_variants(alt)):
                        r3, _m3 = compile_compare(name, addr, alt2, c_path.with_suffix(f".const.ord{k}.c"), out, game)
                        tries.append((r3, alt2, note))
                        if r3 and r3.match:
                            break
                hit = next(((r, t, nt) for r, t, nt in tries if r and r.match), None)
                if hit:
                    res, c_text = hit[0], hit[1]
                    ph_new, consts = ph2, used
                    notes.append("const floats: " + " ".join(sorted(used)) + (f"; {hit[2]}" if hit[2] else ""))
                else:
                    notes.append("const variant tried, no match")
        return res, c_text, notes, ph_new, consts

    st = later(res, c_text, c_draft, s_base)
    if narrow_pre:
        pres.append(narrow_pre)
    kept_ph = at.protos_h
    n_pres = len(pres)
    for depth, pre in enumerate(reversed(pres)):
        if st[0].match:
            break
        at.protos_h = pre[4]
        st2 = later(*pre[:4], tag=f".pre{depth}")
        if st2[0].match:
            what = "narrow parameter" if narrow_pre and depth == 0 else "draft fix"
            dropped = [n for n in at.notes if n.startswith(("narrow parameter kept", "early exit first", "s16 sign",
                                                            "returns the last call", "stack ", "global ",
                                                            "while loop", "untouched argument",
                                                            "rodata vector"))
                       and "tried" not in n][-(depth + 1):]
            st = (st2[0], st2[1], [f"{what} dropped: a later variant matches without the last {depth + 1} kept "
                                   f"change(s) of {n_pres} ({'; '.join(dropped)})"] + st2[2], st2[3], st2[4])
            kept_ph = pre[4]
            break
    at.protos_h = kept_ph
    res, c_text = st[0], st[1]
    at.files["c"] = c_text
    at.notes += st[2]
    if st[3] is not None:
        at.protos_h, at.consts = st[3], st[4]
    at.files["diff.txt"] = eematch.report(res)
    at.res = res
    at.match = res.match
    at.size_ours, at.size_game = res.our_size, res.game_size
    at.diff_words = sum(not w.ok for w in res.words)
    if res.match:
        at.klass = "MATCH" if not at.n_err else "MATCH (with M2C_ERROR?)"
        if res.unchecked:
            at.detail = "unchecked: " + "; ".join(res.unchecked)[:150]
    else:
        at.klass = "M2C_ERROR in draft" if at.n_err else "diff"
        at.detail = diff_class(res)
    return at


# --- pass-through retry (ER-066) ------------------------------------------------------------------------------

ARG_REGS = ["$a0", "$a1", "$a2", "$a3"]


def _ops(word: int, vram: int) -> tuple[str, list[str]]:
    parts = eematch.disasm(word, vram).split(None, 1)
    return parts[0], [x.strip() for x in parts[1].split(",")] if len(parts) > 1 else []


def _call_target(word: int, vram: int) -> int | None:
    if word >> 26 in (2, 3):                       # j / jal
        return (vram & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
    return None


def passthrough_calls(res, game: eematch.Game) -> dict[str, int]:
    """Calls whose arguments sit one register higher in the game: {callee name: highest $aN the game sets for it}.
    A differing word counts when the game writes $a1..$a3 and ours is the same instruction writing the register one
    lower (every $a register operand one lower; other operands such as relocated immediates are not compared), or
    ours is a nop there (m2c's prototype dropped that argument altogether). The callee is the game's j / jal whose
    delay slot holds the word, else the next j / jal within 16 words. Such a function hands its own $a0 to the callee
    untouched and m2c left it out (docs/decomp_recipes.md 2.9)."""
    found: dict[str, int] = {}
    if res is None or res.game_size is None or res.our_size != res.game_size:
        return found
    for w in res.words:
        if w.ok or w.ours is None or w.game is None:
            continue
        vram = res.addr + w.off
        gm, gops = _ops(w.game, vram)
        if not gops or gops[0] not in ARG_REGS[1:]:
            continue
        n = ARG_REGS.index(gops[0])
        if w.ours != 0:
            om, oops = _ops(w.ours, vram)
            pairs = [(a, b) for a, b in zip(oops, gops) if a in ARG_REGS and b in ARG_REGS]
            if om != gm or len(oops) != len(gops) or not oops or oops[0] != ARG_REGS[n - 1] or \
                    not all(ARG_REGS.index(b) == ARG_REGS.index(a) + 1 for a, b in pairs):
                continue
        target = None
        if w.off >= 4:
            target = _call_target(game.word(vram - 4), vram - 4)
        off = w.off
        while target is None and off < min(res.game_size, w.off + 64):
            target = _call_target(game.word(res.addr + off), res.addr + off)
            off += 4
        if target is None:
            continue
        callee = game.name_at.get(target) or f"func_{target:08X}"
        found[callee] = max(found.get(callee, 0), n)
    return found


def widen_proto(line: str | None, name: str, top: int) -> str | None:
    """`T name(params);` -> `T name(void *, params[, s32 ...]);` with at least top + 1 parameters ($a0..$a<top>).
    None for a variadic prototype."""
    if line is None:
        line = f"void {name}(void);"
    m = re.match(rf"^\s*(?:extern\s+)?(.*?\b{name}\()(.*)(\);.*)$", line)
    if not m:
        return None
    inner = m[2].strip()
    params = [] if inner in ("", "void") else [" ".join(p.split()) for p in _split_top(inner)]
    if "..." in params:
        return None
    new = ["void *"] + params
    while len(new) < top + 1:
        new.append("s32")
    return m[1] + ", ".join(new) + m[3].split(";")[0] + ";"


def passthrough_retry(name: str, addr: int | None, best: Attempt, a_out: Path, out: Path, m2c_dir: Path,
                      game: eematch.Game, timeout: int, hilo: set[str], tag: str = "pt") -> Attempt | None:
    """Re-draft with each shifted callee's prototype widened by a leading `void *` in m2c's context (and the draft's
    prototypes header), so m2c itself reads the untouched $a0 as the function's own arg0 and the arguments after it
    from $a1 on (a trailing constant m2c had dropped comes back too)."""
    calls = passthrough_calls(best.res, game)
    if not calls:
        return None
    ctx_text = best.ctx.read_text(encoding="utf-8") if best.ctx else CTX_TYPEDEFS + "\n"
    ph_text = best.protos_h.read_text(encoding="utf-8") if best.protos_h else ""
    m2c_txt = best.files.get("m2c.txt", "")
    widened = {}
    for callee, top in calls.items():
        old = None
        for src in (ph_text, ctx_text, m2c_txt):
            for mm in re.finditer(rf"^[^\n]*\b{callee}\([^\n]*;", src, re.M):
                if not mm[0].startswith((" ", "\t")):          # a declaration, not a call in a body
                    old = re.sub(r"/\*\s*extern\s*\*/", "", mm[0]).strip()
                    break
            if old:
                break
        new = widen_proto(old, callee, top)
        if new is None:
            return None
        widened[callee] = new

    ctx = out / "obj" / f"{name}.{tag}.ctx.h"
    ph = out / "obj" / f"{name}.{tag}.protos.h"
    add = "".join(v + "\n" for v in widened.values())
    ctx.write_text(drop_names(ctx_text, set(widened)) + add, encoding="utf-8", newline="\n")
    ph.write_text(drop_names(ph_text, set(widened)) + add, encoding="utf-8", newline="\n")
    label = "pass-through retry (" + ", ".join(sorted(widened)) + " widened)"
    at = attempt(name, addr, a_out, out, m2c_dir, ctx, ph, label, game, timeout, hilo)
    at.widened = widened
    at.notes.append("pass-through retry: " + ", ".join(v.rstrip(";") for v in widened.values()))
    return at


# --- untouched argument registers (ER-147) --------------------------------------------------------------------

UNTOUCHED_RETRY = True      # --no-untouched turns the ER-147 retry off
ASM_INDEX: dict[str, Path] = {}                  # run_batch fills it: function -> splat asm
_INSNS: dict[str, list[tuple[str, list[str]]]] = {}
GPR_RE = re.compile(r"\$(\d+)\b")
STORE_OPS = {"sb", "sh", "sw", "sd", "sq", "swc1", "sdc1", "swl", "swr", "sdl", "sdr", "sqc2"}
BRANCH_OP_RE = re.compile(r"^(?:b|beq|bne|beqz|bnez|blez|bgtz|bltz|bgez|bltzal|bgezal|beql|bnel|beqzl|bnezl|blezl|"
                          r"bgtzl|bltzl|bgezl|bc1[tf]l?|bc0[tf]l?|bc2[tf]l?)$")
SRC_ONLY_OPS = {"mtc1", "dmtc1", "ctc1", "mthi", "mtlo", "mthi1", "mtlo1", "jr", "j", "teq", "tne", "mtsa", "mtsab",
                "mtsah", "qmtc2", "ctc2", "mtc0", "nop", "sync", "syscall", "break"}
MULDIV_OPS = {"mult", "multu", "div", "divu", "dmult", "dmultu", "ddiv", "ddivu", "mult1", "multu1", "div1", "divu1",
              "madd", "maddu", "madd1", "maddu1"}


def asm_insns(name: str) -> list[tuple[str, list[str]]]:
    """(mnemonic, operands) of a function's splat asm, labels and directives left out (cached)."""
    if name not in _INSNS:
        out = []
        p = ASM_INDEX.get(name)
        if p:
            for ln in p.read_text(encoding="utf-8").splitlines():
                m = OP_RE.match(ln)
                if m:
                    out.append((m[1], [x.strip() for x in _split_top(m[2])] if m[2].strip() else []))
        _INSNS[name] = out
    return _INSNS[name]


def reg_effects(op: str, ops: list[str]) -> tuple[set[int], set[int]]:
    """(GPRs written, GPRs read) by one instruction (FP registers are left out)."""
    regs = [{int(x) for x in GPR_RE.findall(o)} for o in ops]
    allr = set().union(*regs) if regs else set()
    if op in ("jal", "bal"):
        return {31}, set()
    if op == "jalr":
        return ({31} if len(regs) < 2 else regs[0]), regs[-1] if regs else set()
    if op in STORE_OPS or BRANCH_OP_RE.match(op) or op in SRC_ONLY_OPS:
        return set(), allr
    if op in MULDIV_OPS:
        return (regs[0], set().union(*regs[1:])) if len(regs) == 3 else (set(), allr)
    if not regs:
        return set(), set()
    first = regs[0] if re.fullmatch(r"\$\d+", ops[0]) else set()
    rest = set().union(*regs[1:]) if len(regs) > 1 else set()
    return first, rest | (regs[0] if not first else set())


def callee_reads(callee: str, reg: int, depth: int = 1) -> bool:
    """The callee's asm reads `reg` before writing it (a straight scan, branches ignored); a `jal` / `j` it reaches
    first with `reg` still untouched counts when that callee reads it (to `depth` levels)."""
    insns = asm_insns(callee)
    for i, (op, ops) in enumerate(insns):
        w, r = reg_effects(op, ops)
        if reg in r:
            return True
        if op in ("jal", "j", "jr", "jalr"):
            if i + 1 < len(insns):
                w2, r2 = reg_effects(*insns[i + 1])
                if reg in r2:
                    return True
                if reg in w2:
                    return False
            t = FUNC_RE.findall(" ".join(ops))
            return bool(op in ("jal", "j") and t and depth > 0 and callee_reads(t[0], reg, depth - 1))
        if reg in w:
            return False
    return False


def untouched_arg_calls(name: str) -> dict[str, int]:
    """ER-147 (recipes 2.29: func_0012F540 passes $a0 / $a1 on, func_0012FE10 $a0 / $a2, func_0012EAF8 $a0): calls
    to which the function hands an argument register ($a0-$t3) it never wrote, and which the callee reads:
    {callee: parameters it needs ($a0 .. the highest such register or argument register set for it that it
    reads)}. Only calls before the first `jal` (a call clobbers the argument registers)."""
    insns = asm_insns(name)
    written: set[int] = set()
    out: dict[str, int] = {}
    for i, (op, ops) in enumerate(insns):
        if op in ("jal", "j"):
            t = FUNC_RE.findall(" ".join(ops))
            if t:
                wr = set(written)
                if i + 1 < len(insns):
                    wr |= reg_effects(*insns[i + 1])[0]
                passed = [r for r in range(4, 12) if r not in wr and callee_reads(t[0], r)]
                if passed:
                    setr = [r for r in range(4, 12) if r in wr and callee_reads(t[0], r)]
                    out[t[0]] = max(out.get(t[0], 0), max(passed + setr) - 3)
            if op == "jal":
                written |= set(range(1, 16)) | {24, 25, 31}
                continue
        written |= reg_effects(op, ops)[0]
    return out


def proto_params(line: str | None, name: str) -> int | None:
    """Number of parameters of a prototype line (None: unknown / variadic / unprototyped)."""
    if not line:
        return None
    pp = _proto_parts(line, name)
    if not pp or pp[1] is None or any("..." in p for p in pp[1]):
        return None
    return len(pp[1])


def untouched_retry(name: str, addr: int | None, base: Attempt, a_out: Path, out: Path, m2c_dir: Path,
                    game: eematch.Game, timeout: int, hilo: set[str], tag: str) -> Attempt | None:
    """ER-147: re-draft with each callee that gets an untouched argument register widened (trailing `s32`
    parameters up to the register) in m2c's context and the draft's prototypes header, so m2c passes the function's
    own argument on; m2c drops it when the callee's prototype is too short (a narrow src/ prototype wins over the
    harvested one). The pass-through retry (ER-066) covers the other shape: every argument one register higher."""
    calls = untouched_arg_calls(name)
    if not calls:
        return None
    ctx_text = base.ctx.read_text(encoding="utf-8") if base.ctx else CTX_TYPEDEFS + "\n"
    ph_text = base.protos_h.read_text(encoding="utf-8") if base.protos_h else ""
    widened = {}
    for callee, need in calls.items():
        old = None
        for src in (ph_text, ctx_text):
            mm = re.search(rf"^(?!\s)[^\n]*\b{callee}\([^\n]*;", src, re.M)
            if mm:
                old = mm[0].strip()
                break
        if old is None:
            old = f"void {callee}(void);"
        have = proto_params(old, callee)
        if have is None or have >= need:
            continue
        pm = re.match(rf"^\s*(?:extern\s+)?(.*?\b{callee}\()(.*)(\);.*)$", old)
        if not pm:
            continue
        params = [] if pm[2].strip() in ("", "void") else [" ".join(p.split()) for p in _split_top(pm[2])]
        params += ["s32"] * (need - len(params))
        # a `void` callee becomes M2C_UNK: a wrapper that passes arguments on often returns its callee's value too
        # (recipes 2.18 / 2.29, func_0012FE10), and `return g(..)` does not compile against a void g
        head = re.sub(r"^void\s+(?=\w+\($)", "M2C_UNK ", pm[1])
        widened[callee] = head + ", ".join(params) + ");"
    if not widened:
        return None
    ctx = out / "obj" / f"{name}.{tag}.ctx.h"
    ph = out / "obj" / f"{name}.{tag}.protos.h"
    add = "".join(v + "\n" for v in widened.values())
    ctx.write_text(drop_names(ctx_text, set(widened)) + add, encoding="utf-8", newline="\n")
    ph.write_text(drop_names(ph_text, set(widened)) + add, encoding="utf-8", newline="\n")
    label = f"untouched-args retry on {base.ctx_label} (" + ", ".join(sorted(widened)) + " widened)"
    at = attempt(name, addr, a_out, out, m2c_dir, ctx, ph, label, game, timeout, hilo)
    at.widened = widened
    at.notes.append("untouched-args retry: " + ", ".join(v.rstrip(";") for v in widened.values()))
    return at


def job(name: str, asm_path: Path, out: Path, m2c_dir: Path, ctxs: list[tuple[Path | None, Path | None, str]],
        game: eematch.Game, timeout: int, passthrough: bool = True) -> Row:
    """ctxs: (m2c context, protos header for the draft, label) to try in order until one matches; the best attempt
    (match, compiles, fewest M2C_ERROR, nearest size, fewest differing words) is kept and written to c/."""
    t0 = time.time()
    row = Row(name, file=asm_path.parent.name)
    addr = game.addr(name)
    if addr is not None:
        row.addr = f"0x{addr:08X}"
        gs = game.size_of.get(addr)
        if gs:
            row.instr = gs // 4
            row.band = band_of(row.instr, row.instr == 2 and game.word(addr) == 0x03E00008 and game.word(addr + 4) == 0)
    info = prepare_asm(asm_path)
    ASM_MEM_OPS[name] = list(info.mem_ops)
    flags = []
    if info.has_div_sqrt:
        flags.append("div/sqrt (ER-034)")
    if info.has_vu0:
        flags.append("VU0")
    if info.has_jtable:
        flags.append("jump table" + (" in text_001343E8 (needs the file cut)" if row.file == "text_001343E8" else ""))
    if info.unknown_c1:
        flags.append("unknown " + ", ".join(sorted(set(info.unknown_c1))))
    row.flags = "; ".join(flags)
    a_out = out / "asm" / f"{name}.s"
    a_out.write_text(info.text, encoding="utf-8", newline="\n")
    tried = []
    for ctx, protos_h, ctx_label in ctxs:
        if RT_FIX:
            # ER-175: m2c's own callee prototypes with the game's return types first, then the draft as m2c wrote it
            at_rt = attempt(name, addr, a_out, out, m2c_dir, ctx, protos_h, ctx_label + ", rt", game, timeout,
                            info.hilo_syms, rt=True)
            if at_rt is not None:
                tried.append(at_rt)
                # several changed: each one also left as m2c wrote it (the caller may have seen that type: ER-164's
                # func_001C4838 matches with func_001C9B18 void and func_00101238 as m2c's int, ruling 3.19)
                if not at_rt.match and 2 <= len(at_rt.rt_used) <= RT_LEAVE_ONE_OUT:
                    for k, c in enumerate(at_rt.rt_used):
                        a2 = attempt(name, addr, a_out, out, m2c_dir, ctx, protos_h, ctx_label + f", rt{k}", game,
                                     timeout, info.hilo_syms, rt=True, rt_skip=frozenset({c}))
                        if a2 is not None:
                            tried.append(a2)
                            if a2.match:
                                break
                if tried[-1].match:
                    _M2C_CACHE.pop((str(a_out), str(ctx)), None)
                    break
        tried.append(attempt(name, addr, a_out, out, m2c_dir, ctx, protos_h, ctx_label, game, timeout,
                             info.hilo_syms))
        if tried[-1].match:
            break
    best = min(tried, key=Attempt.score)
    if passthrough and not best.match:
        # ER-130: each compiled attempt in score order (at most 3 retries), not the best alone: with a working src
        # context the best draft can take a callee's full src/ prototype and miss, while the `none` draft's
        # pass-through retry matches (func_0012F900 in text_001000C0)
        n_pt = 0
        for base in sorted([a for a in tried if a.compiles], key=Attempt.score):
            if n_pt >= 3:
                break
            pt = passthrough_retry(name, addr, base, a_out, out, m2c_dir, game, timeout, info.hilo_syms,
                                   f"pt{n_pt}")
            if pt is None:
                continue
            n_pt += 1
            tried.append(pt)
            if pt.match:
                best = pt
                break
    if passthrough and UNTOUCHED_RETRY and not best.match:
        # ER-147: the two best compiled attempts (by score) re-drafted with the untouched argument registers passed on
        uts: list[Attempt] = []
        for base in sorted([a for a in tried if a.compiles and not a.widened], key=Attempt.score):
            if len(uts) >= 2:
                break
            ut = untouched_retry(name, addr, base, a_out, out, m2c_dir, game, timeout, info.hilo_syms,
                                 f"ut{len(uts)}")
            if ut is None:
                continue
            uts.append(ut)
            tried.append(ut)
            if ut.match:
                break
        if uts:
            best = min([best] + uts, key=Attempt.score)     # a closer near miss is kept too (its Notes say so)
    KEPT[name] = (best.files.get("c", ""), best.protos_h)
    WIDENED[name] = dict(best.widened)
    CONSTED[name] = set(best.consts)
    for suffix in ("m2c.txt", "c", "err.txt", "diff.txt", "plain.c", "plain.diff.txt", "plain.err.txt"):
        p = out / "c" / f"{name}.{suffix}"
        if suffix in best.files:
            p.write_text(best.files[suffix], encoding="utf-8", newline="\n")
        elif p.exists():
            p.unlink()
    row.m2c_ok, row.compiles, row.match = best.m2c_ok, best.compiles, best.match
    row.klass, row.detail = best.klass, best.detail
    row.diff_words, row.size_ours, row.size_game = best.diff_words, best.size_ours, best.size_game
    notes = list(best.notes)
    if info.slot_moves:
        notes.append("tail-j slot moved: " + " ".join(info.slot_moves))
    notes += [n for at in tried if at is not best for n in at.notes if n.startswith(TIMEOUT_TAG)]
    if any(at.ctx_label.startswith("pass-through") for at in tried) and \
            not (best.match and best.ctx_label.startswith("pass-through")):
        notes.append("pass-through retry tried, no match")
    if any(at.ctx_label.startswith("untouched") for at in tried) and not best.match:
        notes.append("untouched-args retry tried, no match")
    if name in RT_NOTES:
        notes.append(RT_NOTES[name] + ("" if best.ctx_label.endswith("game return types") else " (not kept)"))
    if len(tried) > 1:
        notes.append(f"ctx {best.ctx_label} ({len(tried)} tried)")
    row.notes = "; ".join(notes)
    row.seconds = round(time.time() - t0, 2)
    return row


# --- plain C (--plain, ER-054) ------------------------------------------------------------------------------

KEPT: dict[str, tuple[str, Path | None]] = {}     # function -> (kept draft text, its prototypes header)
WIDENED: dict[str, dict[str, str]] = {}           # function -> {callee: prototype widened by the pass-through retry}
CONSTED: dict[str, set[str]] = {}                 # function -> floats its kept draft declares const (ER-101)
TAG = ""                                           # --tag: task code at the start of the plain comment

PLAIN_TYPES = [("M2C_UNK8", "s8"), ("M2C_UNK16", "s16"), ("M2C_UNK32", "s32"), ("M2C_UNK64", "s64"),
               ("M2C_UNK", "s32"), ("f64", "double")]
NAME_RE = re.compile(r"\b(?:func|D)_[0-9A-F]{8}\b")
PLAIN_COMMENT = ("/* {tag}drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed "
                 "readings. */")


def _close_paren(s: str, i: int) -> int:
    """Index of the ')' that closes the '(' just before s[i]."""
    depth = 1
    while i < len(s):
        if s[i] == "(":
            depth += 1
        elif s[i] == ")":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    raise ValueError("unbalanced parentheses")


def _split_top(s: str) -> list[str]:
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "," and depth == 0:
            out.append(cur)
            cur = ""
            continue
        depth += ch == "("
        depth -= ch == ")"
        cur += ch
    return out + [cur]


def _simple(e: str) -> bool:
    return re.fullmatch(r"[A-Za-z_]\w*", e) is not None


def plain_fields(text: str) -> str:
    """M2C_FIELD(e, T *, off) -> *(T *)((char *)e + off) (m2c's macro uses s8 *: the same arithmetic); offset 0 ->
    *(T *)e; &M2C_FIELD(..) -> the pointer itself; parentheses only where a postfix operator follows."""
    key = "M2C_FIELD("
    while (i := text.find(key)) >= 0:
        j = _close_paren(text, i + len(key))
        args = _split_top(text[i + len(key):j])
        if len(args) != 3:
            raise ValueError("M2C_FIELD with other than 3 arguments")
        e, t, off = (plain_fields(a).strip() for a in args)
        base = e if _simple(e) else f"({e})"
        if re.fullmatch(r"0x0*|0+", off):
            ptr = f"({t}){base}"
        else:
            ptr = f"({t})((char *){base} + {off})"
        amp = i > 0 and text[i - 1] == "&" and (i < 2 or text[i - 2] != "&")
        after = text[j + 1:].lstrip()
        if amp:
            start, rep = i - 1, f"({ptr})" if after[:1] in ("[", ".", "(") or after.startswith("->") else ptr
        else:
            rep = f"*{ptr}"
            if after[:1] in ("[", ".", "(") or after.startswith("->"):
                rep = f"({rep})"
            start = i
        text = text[:start] + rep + text[j + 1:]
    return text


def plain_types(text: str) -> str:
    for a, b in PLAIN_TYPES:
        text = re.sub(rf"\b{a}\b", b, text)
    return text


def decl_name(line: str) -> str | None:
    """The name a declaration line declares: the first func_ / D_ name, else (ER-114: `sqrtf`, `fptodp`, SDK names)
    the identifier before the parameter list of a prototype, or before the `;` / `[` of a data extern."""
    m = NAME_RE.findall(line)
    if m:
        return m[0]
    s = re.sub(r"/\*.*?\*/", "", line).strip()
    pm = re.match(r"^(?:extern\s+)?[A-Za-z_][\w\s\*]*?[\s\*]([A-Za-z_]\w*)\s*\(", s)
    if pm and pm[1] not in CTX_TYPE_WORDS:
        return pm[1]
    dm = re.match(r"^extern\s+[A-Za-z_][\w\s\*]*?[\s\*]([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;$", s)
    return dm[1] if dm and dm[1] not in CTX_TYPE_WORDS else None


# ER-114: libgcc's GOFAST soft-double routines (ER-041 names, recipes 2.11: "write plain C with doubles; never call
# these routines by name"); m2c calls them by name, so the plain form writes the C operation that compiles to them
SOFT_UNARY = {"fptodp": "double", "litodp": "double", "dptofp": "f32", "dptoli": "s32", "dptoul": "u32"}
SOFT_BINARY = {"dpadd": "+", "dpsub": "-", "dpmul": "*", "dpdiv": "/"}
SOFT_RE = re.compile(r"\b(fptodp|litodp|dptofp|dptoli|dptoul|dpadd|dpsub|dpmul|dpdiv|dpcmp)\s*\(")


def plain_softfloat(text: str) -> tuple[str, int, list[str]]:
    """`fptodp(x)` -> `((double) (x))`, `dpadd(a, b)` -> `(a + b)` (also dpsub / dpmul / dpdiv), `dptofp` /
    `dptoli` / `dptoul` / `litodp` -> casts, `dpcmp(a, b) OP 0` -> `(a OP b)`. Innermost calls first. Returns (text,
    rewrites, routines left by name: a dpcmp not compared with 0, or one with other than its arguments)."""
    n, left = 0, []
    pos = len(text)
    while True:
        ms = [m for m in SOFT_RE.finditer(text) if m.start() < pos]
        if not ms:
            break
        m = ms[-1]
        pos = m.start()
        try:
            j = _close_paren(text, m.end())
        except ValueError:
            left.append(m[1])
            continue
        args = [a.strip() for a in _split_top(text[m.end():j])]
        par = [a if _simple(a) or re.fullmatch(r"[\w.]+f?", a) else f"({a})" for a in args]
        rep, end = None, j + 1
        if m[1] in SOFT_UNARY and len(args) == 1:
            rep = f"(({SOFT_UNARY[m[1]]}) {par[0]})"
        elif m[1] in SOFT_BINARY and len(args) == 2:
            rep = f"({par[0]} {SOFT_BINARY[m[1]]} {par[1]})"
        elif m[1] == "dpcmp" and len(args) == 2:
            cm = re.match(r"\s*(<=|>=|==|!=|<|>)\s*0\b(?!\.)", text[j + 1:])
            if cm:
                rep, end = f"({par[0]} {cm[1]} {par[1]})", j + 1 + cm.end()
        if rep is None:
            left.append(m[1])
            continue
        text = text[:m.start()] + rep + text[end:]
        n += 1
    return text, n, sorted(set(left))


NONEXTERN_PROTO_RE = re.compile(r"^(?!extern\b|static\b|typedef\b|return\b)[A-Za-z_][\w \*]*?[\s\*]func_[0-9A-F]{8}\s*"
                                r"\([^;{}]*\)\s*;\s*$")


def file_decls(src_file: str) -> dict[str, str]:
    """Existing declarations of a src/ file: extern lines and (ER-160) top-level prototype lines without `extern`
    (first wins), and prototypes made from C definitions."""
    p = ROOT / "src" / f"{src_file}.c"
    out: dict[str, str] = {}
    if not p.exists():
        return out
    text = p.read_text(encoding="utf-8")
    for line in text.splitlines():
        s = line.strip()
        if s.startswith("extern ") and s.endswith(";") and (n := decl_name(s)):
            out.setdefault(n, s)
        elif NONEXTERN_PROTO_RE.match(line) and (n := decl_name(s)):
            out.setdefault(n, s)          # ER-160: `void func_0013E818(void *);` (no extern) declares it as well
    for m in SIG_RE.finditer(text):
        if not m[1].strip().startswith("static"):
            out.setdefault(m[2], f"{' '.join(m[1].split())} {m[2]}({m[3].strip() or 'void'});")
    return out


def _param_types(params: str) -> list[str]:
    """Parameter list text -> the types alone (`void *arg0, s32 b` -> ['void *', 's32']; `void` / empty -> [])."""
    params = params.strip()
    if params in ("", "void"):
        return []
    out = []
    for p in _split_top(params):
        p = " ".join(p.split())
        ids = re.findall(r"[A-Za-z_]\w*", p)
        if len(ids) > 1 and ids[-1] not in CTX_TYPE_WORDS and p.endswith(ids[-1]):
            p = p[:-len(ids[-1])].rstrip()
        out.append(re.sub(r"\s*\*\s*", " *", p).replace("* *", "**").strip())
    return out


def _proto_parts(line: str, name: str) -> tuple[str, list[str] | None] | None:
    """`extern T name(params);` -> (return type, parameter types; None for an unprototyped `()`)."""
    m = re.match(rf"^\s*(?:extern\s+)?(.*?)\s*\b{name}\s*\((.*)\)\s*;\s*$", line)
    if not m:
        return None
    ret = re.sub(r"\s*\*\s*", " *", " ".join(m[1].split())).strip()
    return ret, (None if m[2].strip() == "" else _param_types(m[2]))


def own_decl_form(func: str, name: str, own: str) -> tuple[str, str | None]:
    """ER-130: the target file's own extern for the function (an address-taking user's `(void)`, `(s8 *)`, ...)
    against the draft's definition. Returns ("same", None) when they agree (gcc accepts both; `()` agrees with any
    parameters), ("adopt", function text with the file's return and parameter types) when the parameter counts
    agree, else ("differ", None)."""
    d = SIG_RE.search(func)
    o = _proto_parts(own, name)
    if not d or not o:
        return "differ", None
    d_ret = re.sub(r"\s*\*\s*", " *", " ".join(d[1].split())).strip()
    d_params = _param_types(d[3])
    o_ret, o_params = o
    if o_ret == d_ret and (o_params is None or o_params == d_params):
        return "same", None
    if o_params is None or len(o_params) != len(d_params):
        return "differ", None
    names = []
    for p in (_split_top(d[3]) if d_params else []):
        ids = re.findall(r"[A-Za-z_]\w*", p)
        names.append(ids[-1] if len(ids) > 1 and ids[-1] not in CTX_TYPE_WORDS else None)
    if any(n is None for n in names):
        return "differ", None
    ps = ", ".join((t + n if t.endswith("*") else f"{t} {n}") for t, n in zip(o_params, names)) or "void"
    head = (o_ret + name if o_ret.endswith("*") else f"{o_ret} {name}") + f"({ps}) {{"
    return "adopt", func[:d.start()] + head + func[d.end():]


def plain_parts(c_text: str, name: str):
    """Kept draft -> (head declarations {name: plain line}, plain function text, hilo names), or a skip reason."""
    body_lines = [ln for ln in c_text.splitlines() if not ln.startswith("#include")]
    text = "\n".join(body_lines).strip() + "\n"
    m = SIG_RE.search(text)
    while m and m[2] != name:
        m = SIG_RE.search(text, m.end())
    if not m:
        return "no definition found"
    head, func = text[:m.start()], text[m.start():]
    decls: dict[str, str] = {}
    hilo: set[str] = set()
    for line in head.splitlines():
        s = re.sub(r"/\*\s*extern\s*\*/", "", line).strip()
        if not s:
            continue
        if tm := VEC_TYPEDEF_LINE_RE.match(s):    # ER-224: the vector typedef of the .rodata copy fix
            decls["typedef:" + tm[1]] = s
            continue
        n = decl_name(s)
        if not n or not s.endswith(";"):
            return f"unexpected line before the function: {s[:60]}"
        hm = re.match(r'^extern ([\w \*]+?)\s*\b(D_[0-9A-F]{8})\s+__attribute__\(\(section\("\.data"\)\)\);$', s)
        if hm:                                   # recipe form (docs/decomp_recipes.md 2.6): extern T D_X[];
            s = f"extern {hm[1].strip()} {hm[2]}[];"
            hilo.add(n)
        elif not s.startswith("extern "):
            s = "extern " + s
        decls[n] = plain_types(" ".join(s.split()))
    for n in hilo:                               # D_X (a value) -> D_X[0], then &D_X -> D_X
        func = re.sub(rf"(?<![&\w]){n}\b(?!\s*\[)", f"{n}[0]", func)
        func = re.sub(rf"&{n}\b", n, func)
    func = plain_types(plain_fields(func))
    left = sorted(set(re.findall(r"\bM2C_\w+", func)))
    if left:
        return "m2c macro left: " + ", ".join(left)
    if re.search(r"\b[su]128\b", func + "".join(decls.values())):
        return "128-bit type (common.h has no s128 / u128)"
    notes = []
    func, n_soft, soft_left = plain_softfloat(func)
    if soft_left:
        return "soft-double routine called by name (recipes 2.11): " + ", ".join(soft_left)
    if n_soft:
        notes.append(f"{n_soft} soft-double call(s) written as C")
    return decls, func, hilo, notes


def proto_lines(path: Path | None) -> dict[str, str]:
    out: dict[str, str] = {}
    if path and path.exists():
        for line in path.read_text(encoding="utf-8").splitlines():
            if (n := decl_name(line)) and line.strip().endswith(";"):
                s = line.strip()
                out.setdefault(n, plain_types(s if s.startswith("extern ") else "extern " + s))
    return out


def make_plain(rows: list[Row], out: Path, game: eematch.Game) -> None:
    """For every MATCH row: write c/<func>.plain.c (common.h only, no m2c macros or section attributes, the
    declarations it needs), compile and compare it again; row.plain gets the result. Declarations come, in this
    order, from the target src/ file (so place.py drops lines it already has), from the run's own matched
    functions (their final signatures), from the draft's own extern lines, then from its prototypes header."""
    matched = [r for r in rows if r.match]
    parts = {}
    for r in matched:
        c_text, _ph = KEPT.get(r.func, ("", None))
        parts[r.func] = plain_parts(c_text, r.func)
    run_sigs = {}
    for n, p in parts.items():
        if isinstance(p, tuple):
            m = SIG_RE.search(p[1])
            run_sigs[n] = f"extern {' '.join(m[1].split())} {n}({m[3].strip() or 'void'});"
    fdecls_cache: dict[str, dict[str, str]] = {}
    for r in rows:
        if not r.match:
            continue
        p = parts[r.func]
        if not isinstance(p, tuple):
            r.plain = f"skipped: {p}"
            continue
        head, func, _hilo, pnotes = p
        fdecls = fdecls_cache.setdefault(r.file, file_decls(r.file))
        protos = proto_lines(KEPT[r.func][1])
        widened = {n: "extern " + plain_types(v) for n, v in WIDENED.get(r.func, {}).items()}
        # ER-147: a callee the file already declares with at least as many parameters keeps the file's line (the
        # retry widened it only because the context it drafted with lacked it, e.g. the `none` attempt)
        widened = {n: v for n, v in widened.items()
                   if not (n in fdecls and (proto_params(fdecls[n], n) or 0) >= (proto_params(v, n) or 0))}
        must_widen = [n for n, v in widened.items()
                      if n in fdecls and " ".join(fdecls[n].split()) != " ".join(v.split())]
        consted = {}
        for n in CONSTED.get(r.func, set()):
            dm = re.search(rf"\bextern\s+const\s+(\w+)\s+{n}\s*;", "\n".join(head.values()) + "\n"
                           + "\n".join(protos.values()))
            consted[n] = f"extern const {plain_types(dm[1]) if dm else 'f32'} {n};"
        must_const = [n for n in consted if n in fdecls and not re.search(rf"\bconst\b[^;]*\b{n}\b", fdecls[n])]
        # ER-224: the .rodata vector copy fix: its typedef first in the plain file, its `extern const <vector> D_x;`
        # lines before every other source (the file may declare D_x another way: then say to align it)
        tdefs = {k[8:]: v for k, v in head.items() if k.startswith("typedef:")}
        vconst = {n: v for n, v in head.items() if not n.startswith("typedef:")
                  and (vm := re.match(r"^extern const (\w+) D_[0-9A-F]{8};$", v)) and vm[1] in tdefs}
        must_vconst = [n for n in vconst if n in fdecls and " ".join(fdecls[n].split()) != vconst[n]]
        consted = {**vconst, **consted}
        # ER-130: the file's own extern for the target (an address-taking user's prototype) must agree with the
        # definition, or the plain compile fails "conflicting types" (and so would the placement: place.py keeps the
        # file's line). Agrees: declare it. Same parameter count: try the definition with the file's types (kept on a
        # plain MATCH: nothing in the file changes). Else leave it out here and say which line to align.
        # ER-160: a non-extern prototype line (`void func_0013E818(void *);`) counts as the file's own declaration
        own = fdecls.get(r.func, "")
        own_state = None
        if own:
            own_state, adopted = own_decl_form(func, r.func, own)
            if own_state == "adopt":
                alt_text = plain_file(list(tdefs.values()) + own_lines_for(
                    func, r.func, own, lines_src=(consted, widened, fdecls, run_sigs, head, protos)), adopted)
                res_a, _m = compile_compare(r.func, game.addr(r.func), alt_text,
                                            out / "obj" / f"{r.func}.plain_own.c", out, game)
                if res_a and res_a.match:
                    func = adopted
                    own_state = "adopted"
        lines = own_lines_for(func, r.func, own if own_state in ("same", "adopted") else "",
                              lines_src=(consted, widened, fdecls, run_sigs, head, protos))
        lines = list(tdefs.values()) + lines
        text = plain_file(lines, func)
        res, msg = compile_compare(r.func, game.addr(r.func), text, out / "obj" / f"{r.func}.plain.c", out, game)
        (out / "c" / f"{r.func}.plain.c").write_text(text, encoding="utf-8", newline="\n")
        if res is None:
            r.plain = "compile error: " + gcc_error_class(msg)
            (out / "c" / f"{r.func}.plain.err.txt").write_text(msg, encoding="utf-8", newline="\n")
        elif res is False:
            r.plain = "compare failed: " + msg[:80]
        else:
            (out / "c" / f"{r.func}.plain.diff.txt").write_text(eematch.report(res), encoding="utf-8", newline="\n")
            r.plain = "MATCH" if res.match else "diff: " + diff_class(res)
        if own_state == "adopted":
            r.plain += f" (definition takes the file's own prototype `{own}`)"
        elif own_state in ("differ", "adopt"):
            r.plain += f" (the file's own declaration `{own}` differs from the definition and is left out here: " \
                       "align that line with the definition before placing, recipes 2.9)"
        if must_widen:
            r.plain += " (file declares " + ", ".join(must_widen) + " narrower; widen it to the plain file's line " \
                       "before placing)"
        if must_const:
            r.plain += " (file declares " + ", ".join(sorted(must_const)) + " without const; make the file's line " \
                       "const before placing, then make regress: the file's other users compile against it too)"
        if must_vconst:
            r.plain += " (file declares " + ", ".join(sorted(must_vconst)) + " otherwise; make the file's line the " \
                       "plain file's `extern const` vector before placing, then make regress)"
        for tn in tdefs:
            vt, _td, tline = vec_type(r.file)
            if tline and vt == tn:
                r.plain += f" (the file defines {tn} at line {tline}: drop the plain file's typedef line; the type " \
                           "must be declared above this function)"
            else:
                r.plain += f" (typedef {tn} is Unofficial: {r.file} has no 16-byte vector type; keep the line, " \
                           "with the comment of ruling 3.15, and list the use there)"
        if pnotes:
            r.plain += " (" + "; ".join(pnotes) + ")"
        late = late_callees(r.file, r.func, func)
        if late:
            r.plain += " (the file declares " + ", ".join(late) + " only below this function: put a prototype above " \
                       "it before placing, or the call goes unprototyped (a float argument as a double); recipes 2.45)"
        if RULING_RE.search(func):
            r.plain += " (uses ruling 3.14: list it in recipes 3.14 when placed)"
        if "locals sized from the game's frame" in r.notes and r.plain.startswith("MATCH"):
            r.plain += " (stack array sized from the game's frame: give it the type the code reads; any part never " \
                       "read is ruling 3.13's unused local)"
    plain_clashes(rows, out, game)


RULING_RE = re.compile(r"/\* ruling 3\.14 \*/")


def late_callees(src_file: str, func_name: str, func: str) -> list[str]:
    """ER-188 (recipes 2.45, func_00152D58): callees of the plain function that the target file declares (extern,
    prototype or definition) only below the function's INCLUDE_ASM line. place.py keeps the file's own line and drops
    the plain file's, so the call would compile unprototyped there although the plain draft matched."""
    p = ROOT / "src" / f"{src_file}.c"
    if not p.exists():
        return []
    lines = p.read_text(encoding="utf-8").splitlines()
    pos = next((i for i, ln in enumerate(lines) if "INCLUDE_ASM" in ln and re.search(rf"\b{func_name}\b", ln)), None)
    if pos is None:
        return []
    late = []
    for n in dict.fromkeys(re.findall(r"\b(func_[0-9A-F]{8})\s*\(", func)):
        if n == func_name:
            continue
        first = next((i for i, ln in enumerate(lines)
                      if re.match(r"[A-Za-z_]", ln) and "INCLUDE_ASM" not in ln and re.search(rf"\b{n}\s*\(", ln)),
                     None)
        if first is not None and first > pos:
            late.append(n)
    return late


def plain_file(lines: list[str], func: str) -> str:
    return '#include "common.h"\n\n' + "".join(ln + "\n" for ln in lines) + ("\n" if lines else "") + \
        PLAIN_COMMENT.format(tag=f"{TAG}: " if TAG else "") + "\n" + func.strip() + "\n"


def own_lines_for(func: str, name: str, own: str, lines_src) -> list[str]:
    """The plain file's declarations: one line per name the body uses, from the first source in lines_src that has
    it; `own` (the file's own extern for the target, when it agrees) first. Named callees (ER-114: `sqrtf`, SDK
    names) are declared like func_ / D_ names."""
    lines = [own] if own else []
    for n in dict.fromkeys(body_names(func, lines_src)):
        if n == name:
            continue
        for src in lines_src:
            if n in src:
                lines.append(src[n])
                break
        else:
            lines.append(f"/* m2c_bulk: no declaration for {n} */")
    return lines


def body_names(func: str, lines_src) -> list[str]:
    """func_ / D_ names in the body, then other identifiers the body calls that a declaration source names."""
    out = NAME_RE.findall(func)
    declared = set().union(*(set(s) for s in lines_src))
    for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", func):
        if m[1] in declared and not NAME_RE.fullmatch(m[1]):
            out.append(m[1])
    return out


def plain_clashes(rows: list[Row], out: Path, game: eematch.Game | None = None) -> None:
    """One name declared two ways by plain drafts of the same file clashes when both are placed. ER-160 (ER-154's
    `extern s32 D_0035C554` against `extern void *D_0035C554` in text_001CB528): each form in turn (the most used
    first) is put into the drafts that declare it another way and they are compiled again; the first form under which
    every draft still matches is kept in all of them (Plain column: `one declaration for the file`); when none fits,
    each draft's Plain column names the clash."""
    by_file: dict[str, dict[str, dict[str, list[Row]]]] = collections.defaultdict(
        lambda: collections.defaultdict(lambda: collections.defaultdict(list)))
    for r in rows:
        if r.plain.startswith("MATCH"):
            for ln in (out / "c" / f"{r.func}.plain.c").read_text(encoding="utf-8").splitlines():
                if ln.startswith("extern ") and (n := decl_name(ln)):
                    by_file[r.file][n][" ".join(ln.split())].append(r)
    for f, names in sorted(by_file.items()):
        for n, forms in sorted(names.items()):
            if len(forms) < 2:
                continue
            print(f"m2c_bulk: plain: {f}: {n} declared {len(forms)} ways by the drafts: " + " | ".join(sorted(forms)))
            if game is None:
                continue
            fixed = None
            for cand in sorted(forms, key=lambda d: (-len(forms[d]), d)):
                new_texts = {}
                for d, rs in forms.items():
                    if d == cand:
                        continue
                    for r in rs:
                        p = out / "c" / f"{r.func}.plain.c"
                        t = "\n".join(cand if " ".join(ln.split()) == d else ln
                                      for ln in p.read_text(encoding="utf-8").split("\n"))
                        res, _m = compile_compare(r.func, game.addr(r.func), t,
                                                  out / "obj" / f"{r.func}.plain_unify.c", out, game)
                        if not (res and res.match):
                            break
                        new_texts[r.func] = t
                    else:
                        continue
                    break
                else:
                    fixed = (cand, new_texts)
                    break
            if fixed:
                cand, new_texts = fixed
                for fn, t in new_texts.items():
                    (out / "c" / f"{fn}.plain.c").write_text(t, encoding="utf-8", newline="\n")
                for rs in forms.values():
                    for r in rs:
                        r.plain += f" (one declaration for the file: `{cand}`, which every plain draft of {f} " \
                                   "matches with)"
                print(f"m2c_bulk: plain: {f}: {n}: every draft matches with `{cand}`; their plain files use it")
            else:
                for rs in forms.values():
                    for r in rs:
                        r.plain += f" (clash: the plain drafts of {f} declare {n} {len(forms)} ways and no one form " \
                                   "fits them all; align by hand before placing)"


def run_batch(names: list[str], out: Path, args, m2c_dir: Path, idx: dict[str, Path], game: eematch.Game):
    """One run over `names` into `out` (asm/, c/, obj/, harvest/ cleared first): context (with its own harvest),
    the per-function jobs, --plain. Returns (rows, harvested signature count)."""
    if not ASM_INDEX:                         # every splat asm file, SDK subfolders too (callees of the retry)
        for p in (ROOT / "asm" / "nonmatchings").rglob("*.s"):
            ASM_INDEX.setdefault(p.stem, p)
        ASM_INDEX.update(idx)
    for sub in ("asm", "c", "obj", "harvest"):
        if (out / sub).is_dir():
            shutil.rmtree(out / sub)          # this run's own scratch folders under build/m2c_bulk/<label>/
        (out / sub).mkdir(parents=True, exist_ok=True)
    (out / "m2c_extra.h").write_text(DRAFT_EXTRA_H, encoding="utf-8", newline="\n")

    # context
    ctx_path = protos_path = None
    harvested = 0
    void_ret: set[str] = set()
    src_protos: dict[str, str] = {}
    src_data: dict[str, str] = {}
    rt_changed: dict[str, str] = {}
    m2c_protos_path = None
    if args.ctx != "none":
        protos, data = src_context()
        src_protos, src_data = dict(protos), dict(data)
        if args.ctx == "harvest":
            sigs = harvest(names, idx, protos, data, out, m2c_dir, args)
            harvested = len(sigs)
            void_ret = {n for n, sig in sigs.items() if sig.startswith("void ")}
            if RT_FIX:
                # ER-175: return types from the game's call sites; m2c's own signatures stay as a fallback context
                fixed, rt_changed = rt_fix_signatures(sigs, game)
                m2c_protos_path = out / "protos_m2c.h"
                m2c_protos_path.write_text(ctx_body({**protos, **sigs}, data), encoding="utf-8", newline="\n")
                (out / "rt_fix.txt").write_text("".join(f"{n} {rt_changed[n]}\n" for n in sorted(rt_changed)),
                                                encoding="utf-8", newline="\n")
                print(f"m2c_bulk: return types from the game's call sites: {len(rt_changed)} of {len(sigs)} harvested "
                      f"signature(s) changed ({out / 'rt_fix.txt'})")
                sigs = fixed
            protos.update(sigs)
        protos_path = out / "protos.h"
        ctx_path = out / "m2c_ctx.h"
        body = ctx_body(protos, data)
        ctx_path.write_text(CTX_TYPEDEFS + "\n" + body, encoding="utf-8", newline="\n")
        protos_path.write_text(body, encoding="utf-8", newline="\n")
        probe = out / "obj" / "_ctx_check.s"
        probe.write_text(prepare_asm(idx[names[0]]).text, encoding="utf-8", newline="\n")
        check_context(m2c_dir, ctx_path, probe, args.timeout)
    print(f"m2c_bulk: {len(names)} function(s), ctx {args.ctx}"
          + (f" ({harvested} harvested signatures)" if args.ctx == "harvest" else "") + f", output {out}")

    def variant(n, tag, drop: set[str], src_only: bool, base: Path | None = None):
        """Context files for function n: without its own prototype, minus `drop`, or src/ only (`base`: another
        prototypes file than protos.h)."""
        c = out / "obj" / f"{n}.{tag}.ctx.h"
        ph = out / "obj" / f"{n}.{tag}.protos.h"
        body = ctx_body(src_protos, src_data) if src_only else \
            drop_names((base or protos_path).read_text(encoding="utf-8"), drop)
        c.write_text(without_own(CTX_TYPEDEFS + "\n" + body, n), encoding="utf-8", newline="\n")
        ph.write_text(without_own(body, n), encoding="utf-8", newline="\n")
        return c, ph

    def run_one(n):
        # each draft's headers leave out its own prototype (m2c prints the definition itself)
        ctxs = []
        if args.ctx == "harvest":
            mine = sorted(c for c in prepare_asm(idx[n]).callees if c in rt_changed)
            if mine:
                # ER-175: the game's return types first, then m2c's own harvest (the caller may have seen m2c's)
                RT_NOTES[n] = "return types from the game: " + ", ".join(f"{c} {rt_changed[c]}" for c in mine)
                ctxs.append((*variant(n, "h", set(), False), "harvest, game return types"))
                ctxs.append((*variant(n, "hm", set(), False, m2c_protos_path), "harvest"))
            else:
                ctxs.append((*variant(n, "h", set(), False, m2c_protos_path), "harvest"))
            if void_ret:
                ctxs.append((*variant(n, "hnv", void_ret, False, m2c_protos_path), "harvest minus void returns"))
        if args.ctx in ("harvest", "src"):
            ctxs.append((*variant(n, "src", set(), True), "src"))
        ctxs.append((None, None, "none"))
        if args.single:
            ctxs = ctxs[:1]
        return job(n, idx[n], out, m2c_dir, ctxs, game, args.timeout, not args.no_passthrough)

    # results are printed as each function finishes (ER-079: in input order a slow function hid all progress after
    # it); the table keeps the input order
    done: dict[str, Row] = {}
    t0 = time.time()
    with cf.ThreadPoolExecutor(args.par) as ex:
        futs = {ex.submit(run_one, n): n for n in names}
        for k, fut in enumerate(cf.as_completed(futs), 1):
            row = fut.result()
            done[row.func] = row
            print(f"  [{k}/{len(names)} {time.time() - t0:5.0f} s] {row.func} {row.band:>8} {row.klass:<20} "
                  f"{row.detail[:90]}")
    rows: list[Row] = [done[n] for n in names]

    if args.plain:
        make_plain(rows, out, game)
    return rows, harvested


# --- main ---------------------------------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("funcs", nargs="*", help="function names")
    ap.add_argument("--file", action="append", default=[], help="every INCLUDE_ASM function of src/<file>.c")
    ap.add_argument("--list", help="a text file with one function name per line (# comments)")
    ap.add_argument("--ctx", choices=("none", "src", "harvest"), default="harvest",
                    help="m2c context: none; src = prototypes / externs of src/; harvest = src + m2c's inferred "
                         "signature of every callee (default)")
    ap.add_argument("--depth", type=int, default=2, help="harvest: callee levels below the targets (default 2)")
    ap.add_argument("--rounds", type=int, default=4, help="harvest: most rounds (default 4)")
    ap.add_argument("--single", action="store_true",
                    help="try only the --ctx context (default: fall back to harvest minus void returns, src, none "
                         "until one matches; the best attempt is kept)")
    ap.add_argument("--label", help="output folder name under build/m2c_bulk (default: from the input and --ctx)")
    ap.add_argument("--m2c", default=str(DEFAULT_M2C), help=f"m2c folder (default {DEFAULT_M2C})")
    ap.add_argument("--par", type=int, default=4, help="parallel jobs (default 4)")
    ap.add_argument("--timeout", type=int, default=120, help="m2c timeout per function, seconds")
    ap.add_argument("--plain", action="store_true",
                    help="also write c/<func>.plain.c for every MATCH: plain C for tools/place.py (no m2c macros or "
                         "headers, recipe-form %%hi/%%lo externs), compiled and compared again")
    ap.add_argument("--gcc-timeout", type=int, default=120,
                    help="kill a gcc / ee-as call (with cc1) after this many seconds and mark the draft TIMEOUT "
                         "(default 120)")
    ap.add_argument("--no-passthrough", action="store_true",
                    help="skip the pass-through retry (a same-size DIFF whose call arguments sit one register higher "
                         "in the game is re-drafted with the callee's prototype widened by a leading void *)")
    ap.add_argument("--no-slot-move", action="store_true",
                    help="keep the instruction in a tail `j func_X`'s delay slot where it is (default: move it in front "
                         "of the j, as `insn; j X; nop`: m2c drops it otherwise; ER-084)")
    ap.add_argument("--no-const", action="store_true",
                    help="skip the const variant (a non-matching draft is compiled again with the .sdata floats "
                         "tools/sdata_consts.py finds never stored declared extern const; ER-101)")
    ap.add_argument("--no-store-sort", action="store_true",
                    help="skip the offset-order store variant (each run of consecutive field stores sorted by offset, "
                         "three modes; ER-124)")
    ap.add_argument("--no-ruling-314", action="store_true",
                    help="skip the ruling 3.14 variant ((void)&arg0 where our build has a sibling j and the game has "
                         "jal + epilogue; ER-121; then (void)& of the first locals, ER-138)")
    ap.add_argument("--no-frame", action="store_true",
                    help="skip the frame-sized locals (m2c's sp[64] / spXX as one array sized from the game's lowest "
                         "saved-register slot; ER-147)")
    ap.add_argument("--no-packer", action="store_true",
                    help="skip the packer variant (a run of 3+ ORed terms flattened into parameter order; ER-151)")
    ap.add_argument("--no-narrow", action="store_true",
                    help="skip the narrow-parameter variant (a parameter the game narrows at entry, andi 0xFFFF / "
                         "0xFF or an sll + sra / srl pair, typed u16 / u8 / s16 / s8; ER-172)")
    ap.add_argument("--no-rt-fix", action="store_true",
                    help="keep m2c's harvested return types (default: a harvested callee's return type is read from "
                         "the game's call sites where they show it, m2c's own kept as a fallback context; ER-175)")
    ap.add_argument("--no-narrow-keep", action="store_true",
                    help="keep the narrow-parameter variant only on a MATCH, as ER-172 did (default: also when it "
                         "brings the draft closer, which the later variants then start from; ER-180)")
    ap.add_argument("--no-untouched", action="store_true",
                    help="skip the untouched-args retry (callees widened to take the argument registers the function "
                         "passes on untouched; ER-147)")
    ap.add_argument("--no-reg-bytes", action="store_true",
                    help="leave a register added to a typed pointer as m2c wrote it (default: byte arithmetic or "
                         "element counts; ER-138)")
    ap.add_argument("--no-early-exit", action="store_true",
                    help="skip the early-exit fix (`if (c) { ..; return X; } return Y;` -> `if (!c) return Y; ..; "
                         "return X;` where the game has more forward branch-likelies; ER-188, recipes 2.43)")
    ap.add_argument("--no-s16-sign", action="store_true",
                    help="skip the s16 sign test fix (`t = x - 1; x = t; if (t & 0x8000)` -> `if (--x < 0)` on an "
                         "s16; ER-188, recipes 2.46)")
    ap.add_argument("--no-game-ret", action="store_true",
                    help="skip the return-type fix (a void draft ending in a call returns its value where the game's "
                         "callers read $v0 / $f0; ER-188, recipes 2.42 / 2.46)")
    ap.add_argument("--no-stack-vec", action="store_true",
                    help="skip the stack-vector fix (m2c's stack array typed f32, a call's trailing vector values as "
                         "stores into it; ER-188, recipes 2.46)")
    ap.add_argument("--no-global-base", action="store_true",
                    help="skip the global-base fix (M2C_FIELD on a global through a typed pointer or as an array; "
                         "ER-188, recipes 2.45)")
    ap.add_argument("--no-while", action="store_true",
                    help="skip the while fix (m2c's `if (c) { do { .. } while (c); }` as `while (c) { .. }`; ER-217)")
    ap.add_argument("--no-untouched-draft", action="store_true",
                    help="skip the untouched-argument fix on the draft (a call whose arguments sit one register higher "
                         "in the game gets the function's own first integer argument first; ER-217)")
    ap.add_argument("--no-rodata-vec", action="store_true",
                    help="skip the .rodata vector fix (an INCLUDE_RODATA vector copied with ld / sd drafted as a struct "
                         "copy from `extern const Vec4 D_x;`; ER-224, recipes 2.49 / 2.52)")
    ap.add_argument("--no-extra-stores", action="store_true",
                    help="no rescue of a draft that does not compile for m2c's `/* extra? */` arguments (the game's "
                         "stack stores before the call written as stores into sp, else dropped; ER-224)")
    ap.add_argument("--no-keep-words", action="store_true",
                    help="ER-188's keep rule for the draft fixes (size first, then words) instead of ER-217's (no "
                         "farther in size or in words, unless it reaches the game's size)")
    ap.add_argument("--store-limit", type=int, default=48,
                    help="most permuted store orders compiled per draft (default 48; ER-147)")
    ap.add_argument("--rerun-diffs", metavar="RESULTS",
                    help="take the functions from an earlier run's results.json (or its folder): its same-size DIFFs "
                         "that are still INCLUDE_ASM; use with --chunk")
    ap.add_argument("--chunk", type=int, default=0,
                    help="run the functions in small lists of this many, each with its own harvest (sub-folders "
                         "chunkNN/; c/ files and the results table are gathered in the run folder)")
    ap.add_argument("--tag", default="",
                    help="task code for the comment above each plain function (e.g. ER-064: \"/* ER-064: drafted "
                         "with m2c ... */\")")
    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)
    global GCC_TIMEOUT, TAG, SLOT_MOVE, CONST_VARIANT, STORE_SORT, SIBCALL_314, FRAME_FIX, PACKER_VARIANT, \
        UNTOUCHED_RETRY, REG_BYTES, STORE_LIMIT, NARROW_VARIANT, RT_FIX, NARROW_KEEP, EARLY_EXIT, S16_SIGN, GAME_RET, \
        STACK_VEC, GLOBAL_BASE, WHILE_FIX, UNTOUCHED_DRAFT, KEEP_WORDS, RODATA_VEC, EXTRA_STORES
    RODATA_VEC = not args.no_rodata_vec
    EXTRA_STORES = not args.no_extra_stores
    WHILE_FIX = not args.no_while
    UNTOUCHED_DRAFT = not args.no_untouched_draft
    KEEP_WORDS = not args.no_keep_words
    EARLY_EXIT = not args.no_early_exit
    S16_SIGN = not args.no_s16_sign
    GAME_RET = not args.no_game_ret
    STACK_VEC = not args.no_stack_vec
    GLOBAL_BASE = not args.no_global_base
    FRAME_FIX = not args.no_frame
    NARROW_VARIANT = not args.no_narrow
    NARROW_KEEP = not args.no_narrow_keep
    RT_FIX = not args.no_rt_fix
    PACKER_VARIANT = not args.no_packer
    UNTOUCHED_RETRY = not args.no_untouched
    REG_BYTES = not args.no_reg_bytes
    STORE_LIMIT = args.store_limit
    STORE_SORT = not args.no_store_sort
    SIBCALL_314 = not args.no_ruling_314
    CONST_VARIANT = not args.no_const
    GCC_TIMEOUT = args.gcc_timeout
    SLOT_MOVE = not args.no_slot_move
    TAG = args.tag

    m2c_dir = Path(args.m2c)
    if not (m2c_dir / "m2c.py").exists():
        print(f"m2c_bulk: m2c not found at {m2c_dir}", file=sys.stderr)
        return 2
    names = list(args.funcs)
    for f in args.file:
        names += file_functions(f)
    if args.list:
        names += [ln.split("#")[0].strip() for ln in Path(args.list).read_text(encoding="utf-8").splitlines()
                  if ln.split("#")[0].strip()]
    if args.rerun_diffs:
        rp = Path(args.rerun_diffs)
        rp = rp / "results.json" if rp.is_dir() else rp
        prev = json.loads(rp.read_text(encoding="utf-8"))
        picked = [r["func"] for r in prev if r.get("klass") == "diff" and r.get("size_ours") is not None
                  and r.get("size_ours") == r.get("size_game")]
        print(f"m2c_bulk: {len(picked)} same-size DIFF(s) from {rp}")
        names += picked
    names = list(dict.fromkeys(names))
    if not names:
        ap.error("no functions: name some, or use --file / --list / --rerun-diffs")
    idx = asm_index()
    missing = [n for n in names if n not in idx]
    if missing:
        print(f"m2c_bulk: no asm for {len(missing)} function(s) (placed as C already?): {' '.join(missing[:10])}",
              file=sys.stderr)
        names = [n for n in names if n in idx]
        if not names:
            print("m2c_bulk: nothing left to draft", file=sys.stderr)
            return 1
    game = eematch.Game()
    if not game.have_syms:
        print("m2c_bulk: build/SLUS_200.15.syms.elf missing (run make regress first)", file=sys.stderr)
        return 2
    if CONST_VARIANT:
        import sdata_consts
        print(f"m2c_bulk: const variant: {len(sdata_consts.const_floats())} never-stored .sdata floats "
              "(tools/sdata_consts.py)")

    label = args.label or ("_".join(Path(f).stem for f in args.file) or "funcs") + f"_{args.ctx}"
    if not re.fullmatch(r"[\w.-]+", label):
        ap.error(f"--label {label!r}: letters, digits, '_', '-', '.' only")
    out = OUT_BASE / label
    t_start = time.time()
    if args.chunk and args.chunk < len(names):
        # small lists (ER-066): each with its own harvest, as ER-062's run on 11 names matched 4 drafts that the
        # file-wide run had as DIFF (the harvested signatures differ between the runs)
        for sub in ("asm", "c", "obj", "harvest"):
            if (out / sub).is_dir():
                shutil.rmtree(out / sub)
        (out / "c").mkdir(parents=True, exist_ok=True)
        for old in out.glob("chunk[0-9][0-9]"):
            shutil.rmtree(old)
        rows, harvested = [], 0
        chunks = [names[i:i + args.chunk] for i in range(0, len(names), args.chunk)]
        for k, part in enumerate(chunks, 1):
            sub = out / f"chunk{k:02d}"
            print(f"m2c_bulk: chunk {k} of {len(chunks)}: {len(part)} function(s)")
            r, h = run_batch(part, sub, args, m2c_dir, idx, game)
            for row in r:
                row.notes = "; ".join(filter(None, [row.notes, f"chunk{k:02d}"]))
            rows += r
            harvested += h
            for f in (sub / "c").iterdir():
                shutil.copy2(f, out / "c" / f.name)
    else:
        rows, harvested = run_batch(names, out, args, m2c_dir, idx, game)
    write_results(out, rows, args, harvested, time.time() - t_start)
    return 0


def write_results(out: Path, rows: list[Row], args, harvested: int, secs: float) -> None:
    cols = list(asdict(rows[0]).keys()) if rows else []
    with open(out / "results.csv", "w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=cols)
        w.writeheader()
        for r in rows:
            w.writerow(asdict(r))
    (out / "results.json").write_text(json.dumps([asdict(r) for r in rows], indent=1), encoding="utf-8",
                                      newline="\n")
    n = len(rows)
    m2c_ok = sum(r.m2c_ok for r in rows)
    comp = sum(r.compiles for r in rows)
    match = sum(r.match for r in rows)
    near = [r for r in rows if r.compiles and not r.match and r.diff_words is not None]
    lines = [f"# m2c_bulk: {out.name}", "",
             f"{n} functions, ctx `{args.ctx}`" + (f" ({harvested} harvested signatures)" if args.ctx == "harvest"
                                                    else "") + f", {secs:.0f} s, par {args.par}.", "",
             "| Stage | Functions | % |", "|---|---|---|",
             f"| m2c gave a draft | {m2c_ok} | {100 * m2c_ok / max(n, 1):.0f} |",
             f"| draft compiles | {comp} | {100 * comp / max(n, 1):.0f} |",
             f"| exact match (masked compare) | {match} | {100 * match / max(n, 1):.0f} |", ""]
    buckets = collections.Counter()
    for r in near:
        if r.size_game is not None and r.size_ours != r.size_game:
            buckets["size differs"] += 1
        elif r.diff_words <= 2:
            buckets["same size, 1-2 words differ"] += 1
        elif r.diff_words <= 8:
            buckets["same size, 3-8 words differ"] += 1
        else:
            buckets["same size, 9+ words differ"] += 1
    lines += ["| Compiled, no match | Functions |", "|---|---|"] + \
             [f"| {k} | {v} |" for k, v in sorted(buckets.items())] + [""]
    classes = collections.Counter(f"{r.klass}: {r.detail}" if r.klass in ("m2c failed", "compile error", "diff")
                                  else r.klass for r in rows)
    lines += ["| Class | Functions |", "|---|---|"] + [f"| {k} | {v} |" for k, v in classes.most_common()] + [""]
    bands = collections.OrderedDict()
    for r in sorted(rows, key=lambda r: r.instr):
        b = bands.setdefault(r.band or "?", [0, 0, 0])
        b[0] += 1
        b[1] += r.compiles
        b[2] += r.match
    lines += ["| Band | Functions | Compile | Match |", "|---|---|---|---|"] + \
             [f"| {k} | {v[0]} | {v[1]} | {v[2]} |" for k, v in bands.items()] + [""]
    flags = collections.Counter()
    for r in rows:
        for f in filter(None, r.flags.split("; ")):
            flags[f] += 1
    if flags:
        lines += ["| Flag (asm) | Functions | Of them matched |", "|---|---|---|"]
        for f, c in flags.most_common():
            lines.append(f"| {f} | {c} | {sum(r.match for r in rows if f in r.flags.split('; '))} |")
        lines.append("")
    if args.plain:
        pc = collections.Counter(r.plain.split(":")[0] for r in rows if r.match)
        lines += ["| Plain C (`--plain`, MATCH drafts) | Functions |", "|---|---|"] + \
                 [f"| {k} | {v} |" for k, v in pc.most_common()] + [""]
    lines += ["| Function | Band | Compiles | Match | Diff words | Class | Detail | Flags | Notes | Plain |",
              "|---|---|---|---|---|---|---|---|---|---|"]
    for r in rows:
        lines.append(f"| {r.func} | {r.band} | {'yes' if r.compiles else 'no'} | {'yes' if r.match else 'no'} | "
                     f"{'' if r.diff_words is None else r.diff_words} | {r.klass} | {r.detail.replace('|', '/')} | "
                     f"{r.flags} | {r.notes} | {r.plain.replace('|', '/')} |")
    (out / "results.md").write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    print("\n".join(lines[:12]))
    print(f"m2c_bulk: table in {(out / 'results.md').relative_to(ROOT)}")


if __name__ == "__main__":
    sys.exit(main())
