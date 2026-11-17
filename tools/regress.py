#!/usr/bin/env python3
"""ER-003: regression gate for the SLUS_200.15 split (`make regress`, and the pre-commit hook).

Steps (each must pass; exit 1 on the first failure):
  1. the original ELF still has its pinned SHA-1 and size (tools/regress_baseline.txt)
  2. clean split: remove asm/, linker/, baserom/, assets/, build/ and run `make split`   (skip: --no-split)
  3. clean build: `make -j4` of the stripped ELF and the unstripped one (clean of build/ always)
  4. SHA-1 of build/SLUS_200.15.elf equals the original's (the whole file, headers included)
  5. per-function byte check (tools/func_check.py): every function identical, no address-named symbol moved

--staged (commit gate): looks at the staged paths (follows GIT_INDEX_FILE); if none can change the build
(src/, include/, asm/, the yaml, symbol files, Makefile, build tools), it exits 0 at once. The gate
builds the working tree, not the staged snapshot, so commit whole files.

Usage: python -I tools/regress.py [--no-split] [--staged] [-j N]
"""
import argparse
import hashlib
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MSYS = Path(r"C:\msys64\usr\bin")
BASEELF = ROOT / "disc" / "extract" / "SLUS_200.15"
TARGET = ROOT / "build" / "SLUS_200.15.elf"
FULL = ROOT / "build" / "SLUS_200.15.syms.elf"
BASELINE = ROOT / "tools" / "regress_baseline.txt"
SPLIT_DIRS = ("asm", "linker", "baserom", "assets", "build")

BUILD_PATHS = ("src/", "include/", "asm/", "slus_200.15.yaml", "symbol_addrs.txt", "reloc_addrs.txt",
               "Makefile", "requirements.txt",
               "tools/split.py", "tools/gen_ld.py", "tools/gen_gp_externs.py", "tools/gen_elf_extras.py",
               "tools/extract_rom.py", "tools/elfutil.py", "tools/func_check.py", "tools/regress.py",
               "tools/regress_baseline.txt", "tools/hooks/")


def baseline(kind: str) -> tuple[str, int]:
    for line in BASELINE.read_text(encoding="utf-8").splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[0] == kind:
            return parts[1].lower(), int(parts[2])
    raise SystemExit(f"regress: no '{kind}' line in {BASELINE}")


def sha1_size(path: Path) -> tuple[str, int]:
    data = path.read_bytes()
    return hashlib.sha1(data).hexdigest(), len(data)


def clean_env() -> dict[str, str]:
    """MSYS2's usr/bin first; Git for Windows' own msys tools removed (they misbehave inside MSYS2 make)."""
    env = dict(os.environ)
    git_dirs = (r"git\usr\bin", r"git\mingw64\bin", r"git\bin")
    parts = [p for p in env.get("PATH", "").split(os.pathsep)
             if p and not p.lower().replace("/", "\\").rstrip("\\").endswith(git_dirs)]
    env["PATH"] = os.pathsep.join([str(MSYS)] + parts)
    for k in ("MSYSTEM", "MSYS", "CHERE_INVOKING", "SHELL", "MAKEFLAGS", "MFLAGS", "MAKELEVEL"):
        env.pop(k, None)
    return env


def make(args: list[str], log: Path) -> bool:
    cmd = [str(MSYS / "make.exe")] + args
    with log.open("a", encoding="utf-8") as fh:
        fh.write(f"$ {' '.join(cmd)}\n")
        fh.flush()
        r = subprocess.run(cmd, cwd=ROOT, env=clean_env(), stdout=fh, stderr=subprocess.STDOUT,
                           stdin=subprocess.DEVNULL)
    return r.returncode == 0


def staged_paths() -> list[str]:
    r = subprocess.run(["git", "diff", "--cached", "--name-only", "--no-renames"], cwd=ROOT,
                       capture_output=True, text=True, check=True)
    return [p for p in r.stdout.splitlines() if p]


def tail(log: Path, n: int = 25) -> str:
    lines = log.read_text(encoding="utf-8", errors="replace").splitlines()
    errs = [l for l in lines if any(k in l.lower() for k in ("error", "fail", "undefined", "warning:"))]
    return "\n".join((errs or lines)[-n:])


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--no-split", action="store_true", help="keep the current asm/ (still a clean build)")
    ap.add_argument("--staged", action="store_true", help="commit gate: skip if no staged path affects the build")
    ap.add_argument("-j", type=int, default=4)
    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)
    t0 = time.time()

    if args.staged:
        paths = staged_paths()
        hits = [p for p in paths if p.startswith(BUILD_PATHS)]
        if not hits:
            print(f"regress: {len(paths)} staged path(s), none affects the build: nothing to check")
            return 0
        print(f"regress: staged paths affect the build ({', '.join(hits[:5])}{' ...' if len(hits) > 5 else ''})")

    # 1. pinned original
    if not BASEELF.exists():
        print(f"regress: FAIL original missing: {BASEELF}")
        return 1
    want = baseline("elf")
    got = sha1_size(BASEELF)
    if got != want:
        print(f"regress: FAIL original changed: {BASEELF} is {got}, pinned {want}")
        return 1
    print(f"[1/5] original pinned: OK ({want[0]})")

    # 2. clean split
    logdir = ROOT / "build_logs"
    logdir.mkdir(exist_ok=True)
    log = logdir / "regress.log"
    log.write_text("", encoding="utf-8")
    if args.no_split:
        if not (ROOT / "asm").is_dir():
            print("regress: FAIL --no-split but there is no asm/: run without --no-split")
            return 1
        shutil.rmtree(ROOT / "build", ignore_errors=True)
        print("[2/5] split: kept (--no-split); build/ removed")
    else:
        for d in SPLIT_DIRS:
            shutil.rmtree(ROOT / d, ignore_errors=True)
        if not make(["split"], log):
            print(f"regress: FAIL make split\n{tail(log)}")
            return 1
        print(f"[2/5] clean split: OK ({time.time() - t0:.0f} s)")

    # 3. clean build (a second make invocation: the object list is read from asm/ at parse time)
    for p in (TARGET, FULL):
        p.unlink(missing_ok=True)
    if not make([f"-j{args.j}", "build/SLUS_200.15.elf", "build/SLUS_200.15.syms.elf"], log) \
            or not TARGET.exists() or not FULL.exists():
        print(f"regress: FAIL build (log: {log.relative_to(ROOT)})\n{tail(log)}")
        return 1
    print(f"[3/5] clean build: OK ({time.time() - t0:.0f} s)")

    # 4. whole-file SHA-1
    got = sha1_size(TARGET)
    if got != want:
        print(f"regress: FAIL {TARGET.relative_to(ROOT)} SHA-1/size {got}, original {want}")
        subprocess.run([sys.executable, "-I", str(ROOT / "tools" / "func_check.py"), str(FULL), str(BASEELF)],
                       cwd=ROOT)
        return 1
    print(f"[4/5] SHA-1 build/SLUS_200.15.elf: OK ({got[0]})")

    # 5. per-function check
    r = subprocess.run([sys.executable, "-I", str(ROOT / "tools" / "func_check.py"), str(FULL), str(BASEELF)],
                       cwd=ROOT, capture_output=True, text=True)
    print(f"[5/5] {r.stdout.strip()}")
    if r.returncode != 0:
        print("regress: FAIL per-function check")
        return 1
    print(f"regress: PASS ({time.time() - t0:.0f} s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
