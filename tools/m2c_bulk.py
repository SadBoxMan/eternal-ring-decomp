"""ER-050: m2c bulk driver for EE code: draft, clean, compile and compare many functions at once.

For each function (named, read from a list file, or every INCLUDE_ASM function of a src/ file):

1. copy splat's asm (asm/nonmatchings/<file>/<func>.s) and rewrite what m2c cannot parse:
   - `li.s $fN, <float>` (a .lit4 load, written so by tools/split.py) -> `lwc1 $fN, (M2CLIT_<bits>)`; the cleaner
     turns M2CLIT_<bits> back into a float literal in the draft (one instruction stays one instruction, so delay
     slots are untouched)
   - rabbitizer's `c1 0x...` for the R5900 square root (COP1 S-format funct 4, fs 0) -> `sqrt.s $fd, $ft`
   - `.word 0x... # cvt.w.s $fd, $fs` (rabbitizer prints the R5900 cvt.w.s as a word) -> `cvt.w.s $fd, $fs`
2. run m2c (Twinblade's copy, `mipsee-gcc-c` target, `--valid-syntax`) natively, with a context header (--ctx):
   none / src (prototypes and externs of the C already in src/) / harvest (src plus the signature m2c infers for each
   callee from the callee's own asm; the default)
3. clean the draft into a compilable scratch file: #include common.h, m2c's own macro header (from the m2c folder,
   never copied into this repo: m2c is GPL-3) and the context prototypes; float literals; undeclared stack
   variables declared (flagged: m2c lost a stack buffer)
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
        out_lines.append(line)
    info.text = "\n".join(out_lines) + "\n"
    return info


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


def src_context() -> tuple[dict[str, str], dict[str, str]]:
    """(function prototypes, data extern lines) from src/: definitions first, then extern prototypes; the first
    declaration of a name wins (a clash between files is reported, not resolved)."""
    protos: dict[str, str] = {}
    data: dict[str, str] = {}
    clashes = []
    files = sorted((ROOT / "src").glob("*.c"))
    for p in files:
        for m in SIG_RE.finditer(p.read_text(encoding="utf-8")):
            ret = " ".join(m[1].split())
            if ret.startswith("static"):
                continue
            protos.setdefault(m[2], f"{ret} {m[2]}({m[3].strip() or 'void'});")
    for p in files:
        for line in p.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if not line.startswith("extern ") or not line.endswith(";"):
                continue
            names = FUNC_RE.findall(line) or re.findall(r"\bD_[0-9A-F]{8}\b", line)
            if not names:
                continue
            name = names[0]
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
    return protos, data


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

    def lit(m):
        return f32_literal(int(m[1], 16))
    n_lits = len(LIT_RE.findall(text))
    text = LIT_RE.sub(lit, text)
    if n_lits:
        notes.append(f"{n_lits} .lit4 literal(s)")
    # undeclared stack variables (m2c names them spXX; a bare `sp` is the address of a stack area)
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
    head = ['#include "common.h"'] + [f'#include "{p.as_posix()}"' for p in includes]
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
    """Compile with exactly the Makefile's CC / CFLAGS / CPPFLAGS / AS / ASFLAGS (eematch.settings) into obj_dir.
    A gcc or ee-as call that runs longer than GCC_TIMEOUT is killed with its children; the message then starts with
    TIMEOUT_TAG."""
    st = eematch.settings()
    s_path, o_path = obj_dir / (c_path.stem + ".s"), obj_dir / (c_path.stem + ".o")
    cc = [st["CC"], f"-B{st['GCC_DIR']}/", *st["CFLAGS"].split(),
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


def store_orders(c_text: str, limit: int = 24):
    """Other orders of each run of 2+ plain top-level assignments (4-space indent, `x = y;`, no call): every order
    for runs up to 4 statements, else the reversed run. The scheduler does not keep source order for stores
    (docs/decomp_recipes.md 2.1, "Store order is not source order"); m2c writes them in machine order."""
    lines = c_text.split("\n")
    runs, cur = [], []
    for i, ln in enumerate(lines):
        if re.match(r"^    [^ /].* = .*;$", ln) and not CALL_RE.search(ln) and not ln.lstrip().startswith(
                ("return", "if", "for", "while", "case", "default")):
            cur.append(i)
        else:
            if len(cur) >= 2:
                runs.append(cur)
            cur = []
    n = 0
    for run in runs:
        block = [lines[i] for i in run]
        orders = itertools.permutations(block) if len(block) <= 4 else [tuple(reversed(block))]
        for order in orders:
            if list(order) == block:
                continue
            new = list(lines)
            for i, ln in zip(run, order):
                new[i] = ln
            yield "\n".join(new)
            n += 1
            if n >= limit:
                return


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
            protos_h: Path | None, ctx_label: str, game: eematch.Game, timeout: int, hilo: set[str]) -> Attempt:
    at = Attempt(ctx_label, protos_h=protos_h, ctx=ctx)
    rc, draft, err = run_m2c(m2c_dir, a_out, ctx, timeout)
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
    if at.n_err:
        at.notes.append(f"{at.n_err} M2C_ERROR")
    at.files["c"] = c_text
    # compile under a per-attempt name: the variants of one function share the folder
    c_path = out / "obj" / f"{name}.{re.sub(r'[^a-z]+', '_', ctx_label)}.c"
    res, msg = compile_compare(name, addr, c_text, c_path, out, game)
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
    if not res.match and res.game_size == res.our_size:
        for k, alt in enumerate(store_orders(c_text)):
            res2, _msg2 = compile_compare(name, addr, alt, c_path.with_suffix(f".ord{k}.c"), out, game)
            if res2 and res2.match:
                res, c_text = res2, alt
                at.files["c"] = alt
                at.notes.append("store order permuted")
                break
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
                      game: eematch.Game, timeout: int, hilo: set[str]) -> Attempt | None:
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
    tag = "pt"
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
        tried.append(attempt(name, addr, a_out, out, m2c_dir, ctx, protos_h, ctx_label, game, timeout,
                             info.hilo_syms))
        if tried[-1].match:
            break
    best = min(tried, key=Attempt.score)
    if passthrough and not best.match and best.compiles:
        pt = passthrough_retry(name, addr, best, a_out, out, m2c_dir, game, timeout, info.hilo_syms)
        if pt is not None:
            tried.append(pt)
            if pt.match:
                best = pt
    KEPT[name] = (best.files.get("c", ""), best.protos_h)
    WIDENED[name] = dict(best.widened)
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
    notes += [n for at in tried if at is not best for n in at.notes if n.startswith(TIMEOUT_TAG)]
    if any(at.widened for at in tried) and not best.widened:
        notes.append("pass-through retry tried, no match")
    if len(tried) > 1:
        notes.append(f"ctx {best.ctx_label} ({len(tried)} tried)")
    row.notes = "; ".join(notes)
    row.seconds = round(time.time() - t0, 2)
    return row


# --- plain C (--plain, ER-054) ------------------------------------------------------------------------------

KEPT: dict[str, tuple[str, Path | None]] = {}     # function -> (kept draft text, its prototypes header)
WIDENED: dict[str, dict[str, str]] = {}           # function -> {callee: prototype widened by the pass-through retry}
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
    m = NAME_RE.findall(line)
    return m[0] if m else None


def file_decls(src_file: str) -> dict[str, str]:
    """Existing declarations of a src/ file: extern lines (first wins) and prototypes made from C definitions."""
    p = ROOT / "src" / f"{src_file}.c"
    out: dict[str, str] = {}
    if not p.exists():
        return out
    text = p.read_text(encoding="utf-8")
    for line in text.splitlines():
        s = line.strip()
        if s.startswith("extern ") and s.endswith(";") and (n := decl_name(s)):
            out.setdefault(n, s)
    for m in SIG_RE.finditer(text):
        if not m[1].strip().startswith("static"):
            out.setdefault(m[2], f"{' '.join(m[1].split())} {m[2]}({m[3].strip() or 'void'});")
    return out


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
    return decls, func, hilo


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
        head, func, _hilo = p
        fdecls = fdecls_cache.setdefault(r.file, file_decls(r.file))
        protos = proto_lines(KEPT[r.func][1])
        widened = {n: "extern " + plain_types(v) for n, v in WIDENED.get(r.func, {}).items()}
        must_widen = [n for n, v in widened.items()
                      if n in fdecls and " ".join(fdecls[n].split()) != " ".join(v.split())]
        lines = []
        for n in dict.fromkeys(NAME_RE.findall(func)):
            if n == r.func:
                if n in fdecls and fdecls[n].startswith("extern"):
                    lines.append(fdecls[n])      # the file's own extern for it must agree with the definition
                continue
            for src in (widened, fdecls, run_sigs, head, protos):
                if n in src:
                    lines.append(src[n])
                    break
            else:
                lines.append(f"/* m2c_bulk: no declaration for {n} */")
        text = '#include "common.h"\n\n' + "".join(ln + "\n" for ln in lines) + ("\n" if lines else "") + \
            PLAIN_COMMENT.format(tag=f"{TAG}: " if TAG else "") + "\n" + func.strip() + "\n"
        res, msg = compile_compare(r.func, game.addr(r.func), text, out / "obj" / f"{r.func}.plain.c", out, game)
        (out / "c" / f"{r.func}.plain.c").write_text(text, encoding="utf-8", newline="\n")
        if res is None:
            r.plain = "compile error: " + gcc_error_class(msg)
            own = fdecls.get(r.func, "")
            dm = SIG_RE.search(func)
            if own.startswith("extern") and dm and \
                    " ".join(own.split()) != f"extern {' '.join(dm[1].split())} {r.func}({dm[3].strip() or 'void'});":
                r.plain += f" (the file's own extern is `{own}`; align it with the definition, recipes 2.9)"
            (out / "c" / f"{r.func}.plain.err.txt").write_text(msg, encoding="utf-8", newline="\n")
        elif res is False:
            r.plain = "compare failed: " + msg[:80]
        else:
            (out / "c" / f"{r.func}.plain.diff.txt").write_text(eematch.report(res), encoding="utf-8", newline="\n")
            r.plain = "MATCH" if res.match else "diff: " + diff_class(res)
        if must_widen:
            r.plain += " (file declares " + ", ".join(must_widen) + " narrower; widen it to the plain file's line " \
                       "before placing)"
    # one name declared two ways by drafts of the same file would clash when both are placed
    by_file: dict[str, dict[str, set[str]]] = collections.defaultdict(lambda: collections.defaultdict(set))
    for r in rows:
        if r.plain == "MATCH":
            for ln in (out / "c" / f"{r.func}.plain.c").read_text(encoding="utf-8").splitlines():
                if ln.startswith("extern ") and (n := decl_name(ln)):
                    by_file[r.file][n].add(" ".join(ln.split()))
    for f, names in sorted(by_file.items()):
        for n, ds in sorted(names.items()):
            if len(ds) > 1:
                print(f"m2c_bulk: plain: {f}: {n} declared {len(ds)} ways by the drafts: " + " | ".join(sorted(ds)))


def run_batch(names: list[str], out: Path, args, m2c_dir: Path, idx: dict[str, Path], game: eematch.Game):
    """One run over `names` into `out` (asm/, c/, obj/, harvest/ cleared first): context (with its own harvest),
    the per-function jobs, --plain. Returns (rows, harvested signature count)."""
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
    if args.ctx != "none":
        protos, data = src_context()
        src_protos, src_data = dict(protos), dict(data)
        if args.ctx == "harvest":
            sigs = harvest(names, idx, protos, data, out, m2c_dir, args)
            protos.update(sigs)
            harvested = len(sigs)
            void_ret = {n for n, sig in sigs.items() if sig.startswith("void ")}
        protos_path = out / "protos.h"
        ctx_path = out / "m2c_ctx.h"
        body = ctx_body(protos, data)
        ctx_path.write_text(CTX_TYPEDEFS + "\n" + body, encoding="utf-8", newline="\n")
        protos_path.write_text(body, encoding="utf-8", newline="\n")
    print(f"m2c_bulk: {len(names)} function(s), ctx {args.ctx}"
          + (f" ({harvested} harvested signatures)" if args.ctx == "harvest" else "") + f", output {out}")

    def variant(n, tag, drop: set[str], src_only: bool):
        """Context files for function n: without its own prototype, minus `drop`, or src/ only."""
        c = out / "obj" / f"{n}.{tag}.ctx.h"
        ph = out / "obj" / f"{n}.{tag}.protos.h"
        body = ctx_body(src_protos, src_data) if src_only else drop_names(protos_path.read_text(encoding="utf-8"), drop)
        c.write_text(without_own(CTX_TYPEDEFS + "\n" + body, n), encoding="utf-8", newline="\n")
        ph.write_text(without_own(body, n), encoding="utf-8", newline="\n")
        return c, ph

    def run_one(n):
        # each draft's headers leave out its own prototype (m2c prints the definition itself)
        ctxs = []
        if args.ctx == "harvest":
            ctxs.append((*variant(n, "h", set(), False), "harvest"))
            if void_ret:
                ctxs.append((*variant(n, "hnv", void_ret, False), "harvest minus void returns"))
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
    global GCC_TIMEOUT, TAG
    GCC_TIMEOUT = args.gcc_timeout
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
    game = eematch.Game()
    if not game.have_syms:
        print("m2c_bulk: build/SLUS_200.15.syms.elf missing (run make regress first)", file=sys.stderr)
        return 2

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
