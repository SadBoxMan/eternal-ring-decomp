#!/usr/bin/env python3
"""ER-015: placement helper: swap a function's INCLUDE_ASM line in src/ for C, and back.

  python -I tools/place.py c   func_0014D8F8 scratch.c   put the C from scratch.c where func_0014D8F8's INCLUDE_ASM is
  python -I tools/place.py asm func_0014D8F8             put the INCLUDE_ASM line back in place of the C definition
  python -I tools/place.py extract func_0014D918 [-o f.c] write a placed C function (and the extern lines of its file
                                                        that it names) to a scratch file (default build/scratch_in/)
  python -I tools/place.py where func_0014D8F8           print the src/ file and line and whether it is C or asm
  --dry-run on c / asm prints the change without writing.

`c`: the scratch file is the function's C as cmpfunc.py compiled it: `#include` lines first (dropped), then any
declarations it needs (extern lines, prototypes, a comment), then the definition. Lines already present in the target
file are not copied again; the block replaces the INCLUDE_ASM line. The scratch file must define the named function and
no other function (place them one at a time). `asm`: removes the definition and the comment directly above it (no blank
line between); extern lines stay (other functions may use them) and are listed. Files are written with LF.

The tool only edits text: run `make regress` (or tools/cmpfunc.py on the src/ file) afterwards. Usage notes:
docs/matching_tools.md.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
INC_RE = r'^INCLUDE_ASM\("([^"]+)",\s*{name}\);\s*$'


def src_files() -> list[Path]:
    return sorted(SRC.rglob("*.c"))


def read(p: Path) -> list[str]:
    return p.read_text(encoding="utf-8").split("\n")


def write(p: Path, lines: list[str]) -> None:
    p.write_bytes("\n".join(lines).encode("utf-8"))


def strip_code(text: str) -> str:
    """Comments, strings and char literals blanked out (same length), so braces can be counted."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out.append(c + " " * (j - i - 2) + c if j - i >= 2 else c)
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def find_definition(lines: list[str], name: str) -> tuple[int, int] | None:
    """(first line, last line) of the C definition of `name` (signature through its closing brace), or None."""
    code = strip_code("\n".join(lines)).split("\n")
    sig = re.compile(r"^[A-Za-z_].*\b" + re.escape(name) + r"\s*\(")
    for i, line in enumerate(code):
        if not sig.match(line) or line.lstrip().startswith(("extern", "INCLUDE_", "#")):
            continue
        depth, started = 0, False
        for j in range(i, len(code)):
            seg = code[j]
            if not started and ";" in seg and "{" not in seg:
                break  # a prototype
            for ch in seg:
                if ch == "{":
                    depth += 1
                    started = True
                elif ch == "}":
                    depth -= 1
            if started and depth == 0:
                return i, j
    return None


def comment_above(lines: list[str], first: int) -> int:
    """Index of the first line of a /* */ comment that ends directly above line `first` (or `first`)."""
    k = first - 1
    if k < 0 or not lines[k].rstrip().endswith("*/"):
        return first
    while k >= 0 and "/*" not in lines[k]:
        k -= 1
    return max(k, 0)


def locate(name: str):
    """('asm', file, line, folder) or ('c', file, (first, last)) or None."""
    pat = re.compile(INC_RE.format(name=re.escape(name)))
    for f in src_files():
        lines = read(f)
        for i, l in enumerate(lines):
            m = pat.match(l)
            if m:
                return "asm", f, i, m[1]
        d = find_definition(lines, name)
        if d:
            return "c", f, d
    return None


def defined_functions(lines: list[str]) -> list[str]:
    code = strip_code("\n".join(lines))
    return re.findall(r"^[A-Za-z_][^;{}()]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", code, re.M)


def cmd_c(name: str, scratch: Path, dry: bool) -> int:
    loc = locate(name)
    if loc is None:
        print(f"place: {name} not found in src/")
        return 1
    if loc[0] == "c":
        print(f"place: {name} is already C in {loc[1].relative_to(ROOT).as_posix()} (line {loc[2][0] + 1})")
        return 1
    _, target, at, _folder = loc
    s_lines = read(scratch)
    while s_lines and s_lines[-1].strip() == "":
        s_lines.pop()
    d = find_definition(s_lines, name)
    if d is None:
        print(f"place: {scratch} does not define {name}")
        return 1
    others = [f for f in defined_functions(s_lines) if f != name]
    if others:
        print(f"place: {scratch} also defines {', '.join(others)}: place one function at a time")
        return 1
    t_lines = read(target)
    above = {l.strip() for l in t_lines[:at] if l.strip()}
    below = {}  # declaration lines that only exist below the insertion point: index of the first copy
    for k in range(at + 1, len(t_lines)):
        below.setdefault(t_lines[k].strip(), k)
    # only a whole declaration at brace depth 0 (extern / prototype / typedef ending in ';') is deduplicated: never a
    # comment line (a `//` line, a line inside a block comment), a blank, or a line of a struct / initialiser body
    pre = s_lines[:d[0]]
    code = strip_code("\n".join(pre)).split("\n")
    depth, block, moved = 0, [], []
    for l, c in zip(pre, code):
        st = l.strip()
        top = depth
        depth += c.count("{") - c.count("}")
        if st.startswith("#include"):
            continue
        if top == 0 and depth == 0 and c.strip().endswith(";") and st == c.strip():
            if st in above:
                continue
            if st in below:  # the file's copy is below: move it up, so this function (and any between) sees it
                moved.append(below[st])
        block.append(l)
    for k in sorted(set(moved), reverse=True):
        del t_lines[k]
    while block and block[0].strip() == "":
        block.pop(0)
    block += s_lines[d[0]:d[1] + 1]
    new = t_lines[:at] + block + t_lines[at + 1:]
    print(f"place: {name}: INCLUDE_ASM at {target.relative_to(ROOT).as_posix()}:{at + 1} -> {len(block)} line(s) of C")
    if dry:
        print("\n".join(block))
        return 0
    write(target, new)
    return 0


def cmd_asm(name: str, dry: bool) -> int:
    loc = locate(name)
    if loc is None:
        print(f"place: {name} not found in src/")
        return 1
    if loc[0] == "asm":
        print(f"place: {name} is already INCLUDE_ASM in {loc[1].relative_to(ROOT).as_posix()} (line {loc[2] + 1})")
        return 1
    _, target, (first, last) = loc
    lines = read(target)
    folders = [m[1] for l in lines if (m := re.match(r'^INCLUDE_ASM\("([^"]+)"', l))]
    if folders:
        folder = folders[0]
    else:
        rel = target.relative_to(SRC).with_suffix("").as_posix()
        folder = f"asm/nonmatchings/{rel}"
    start = comment_above(lines, first)
    inc = f'INCLUDE_ASM("{folder}", {name});'
    removed = lines[start:last + 1]
    externs = [l for l in lines[max(0, start - 12):start] if l.startswith("extern")]
    print(f"place: {name}: {len(removed)} line(s) of C at {target.relative_to(ROOT).as_posix()}:{start + 1} -> {inc}")
    if externs:
        print("place: extern lines above it stay (remove by hand if nothing else uses them):\n  " + "\n  ".join(externs))
    if dry:
        return 0
    write(target, lines[:start] + [inc] + lines[last + 1:])
    return 0


def cmd_extract(name: str, out: Path | None) -> int:
    loc = locate(name)
    if loc is None or loc[0] != "c":
        print(f"place: {name} is not C in src/")
        return 1
    _, f, (first, last) = loc
    lines = read(f)
    body = lines[comment_above(lines, first):last + 1]
    text = "\n".join(body)
    ext = []
    for l in lines[:first]:
        m = re.match(r"^extern\b.*?\b(\w+)\s*(?:\(|\[|;)", l)
        if m and re.search(r"\b" + re.escape(m[1]) + r"\b", text) and l not in ext:
            ext.append(l)
    out = out or (ROOT / "build" / "scratch_in" / f"{name}.c")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(("\n".join(['#include "common.h"', ""] + ext + ([""] if ext else []) + body) + "\n").encode())
    print(f"place: {name} -> {out}")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("c")
    p.add_argument("name")
    p.add_argument("scratch", type=Path)
    p.add_argument("--dry-run", action="store_true")
    p = sub.add_parser("asm")
    p.add_argument("name")
    p.add_argument("--dry-run", action="store_true")
    p = sub.add_parser("extract")
    p.add_argument("name")
    p.add_argument("-o", type=Path)
    p = sub.add_parser("where")
    p.add_argument("name")
    a = ap.parse_args()
    if a.cmd == "c":
        return cmd_c(a.name, a.scratch, a.dry_run)
    if a.cmd == "asm":
        return cmd_asm(a.name, a.dry_run)
    if a.cmd == "extract":
        return cmd_extract(a.name, a.o)
    loc = locate(a.name)
    if loc is None:
        print(f"{a.name}: not in src/")
        return 1
    line = loc[2] + 1 if loc[0] == "asm" else loc[2][0] + 1
    print(f"{a.name}: {loc[0]} in {loc[1].relative_to(ROOT).as_posix()}:{line}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
