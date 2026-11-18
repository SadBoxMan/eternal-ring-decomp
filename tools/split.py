#!/usr/bin/env python3
"""ER-003 / ER-024: run splat with the output fixes this build needs.

1. Denormal floats (ER-003). ee-as 2.9 (GNU as 2.9-ee-991111) parses denormal floats wrongly: `.float
   5.605193857e-45` (word 0x00000004) assembles to 0x00000003. The first byte-for-byte link differed in exactly that
   one word (.data, 0x0032FD14). spimdisasm has no option for this, so this wrapper makes it write denormal floats
   and doubles as plain `.word`s (the bytes are what the disassembler read; only the spelling changes).

2. Per-file .rodata (ER-024, docs/build.md 3.2). A C file that owns a `.rodata` subsegment (`[addr, .rodata, file]`
   in the yaml) gets splat's rodata migration: each function's own .rodata goes into its INCLUDE_ASM file. splat
   writes the other symbols of the range (used by several functions, only from data, or not at all) only when the
   C file already names them in INCLUDE_RODATA lines. This wrapper writes all of them, and checks that the C file's
   INCLUDE_ASM / INCLUDE_RODATA lines and C function definitions come in splat's order (the order the bytes must
   have in the object). `--fix-includes` inserts missing INCLUDE_RODATA lines into the C files (run once when a
   file first owns its .rodata, then commit the C files); a wrong order is always an error.

3. Per-file .lit4 (ER-024). gas, not gcc, makes .lit4: `li.s $f, c` (c with non-zero low 16 bits) becomes `lwc1
   $f, off($gp)` plus a word in the object's own .lit4, in source order and never merged (docs/decomp_recipes.md 2.2).
   For a C file that owns a `.lit4` subsegment, every INCLUDE_ASM `lwc1 $f, (D_x)` of an owned literal is written
   back as `li.s $f, <value>`, so the asm functions' literals and those of C functions land in the object in
   function order, as the original compile left them. (A labelled word in .lit4 cannot be used instead: ee-as
   asserts when a gp-relative macro names a symbol defined in .lit4, tc-mips.c nopic_need_relax.) Checks: each
   owned word is used by exactly one instruction of one function of that file; its value is a normal float with
   non-zero low 16 bits (else gas would not use .lit4).

4. Per-file .sdata / .data (ER-024). splat migrates neither; for a C file that owns a `.sdata` / `.data` subsegment,
   each symbol of the range is written to asm/nonmatchings/<file>/<sym>.s and the C file must include all of them in
   address order (INCLUDE_SDATA / INCLUDE_DATA, include/include_asm.h; `--fix-includes` writes the block after the
   first #include when the file has none). gas makes gp-relative loads of .sdata symbols defined in the same file.

Everything else is splat 0.50.1 as pinned in requirements.txt.

Usage: python -I tools/split.py <config.yaml> [--fix-includes]   (other arguments as `python -m splat split`)
"""
import math
import re
import struct
import sys
from pathlib import Path

from spimdisasm.mips.symbols import MipsSymbolBase

_orig_is_float = MipsSymbolBase.SymbolBase.isFloat
_orig_is_double = MipsSymbolBase.SymbolBase.isDouble


def _denormal32(word: int) -> bool:
    return (word & 0x7F800000) == 0 and (word & 0x007FFFFF) != 0


def _denormal64(value: int) -> bool:
    return (value & 0x7FF0000000000000) == 0 and (value & 0x000FFFFFFFFFFFFF) != 0


def is_float(self, index: int) -> bool:
    return _orig_is_float(self, index) and not _denormal32(self.words[index])


def is_double(self, index: int) -> bool:
    if not _orig_is_double(self, index):
        return False
    w0, w1 = self.words[index], self.words[index + 1]
    # either word order: refuse if the value is denormal read either way
    return not (_denormal64((w0 << 32) | w1) or _denormal64((w1 << 32) | w0))


MipsSymbolBase.SymbolBase.isFloat = is_float
MipsSymbolBase.SymbolBase.isDouble = is_double

# ---- ER-024: per-file .rodata and .lit4 ------------------------------------------------------------------------

FIX_INCLUDES = False
ERRORS: list[str] = []
SUMMARY = {"lit4_files": 0, "lit4_asm": 0, "lit4_c": 0, "ro_files": 0, "ro_standalone": 0, "ro_inserted": 0,
           "align4": 0, "align_skipped": [], "data_files": [], "data_syms": 0}

INC_RE = re.compile(r"\b(INCLUDE_ASM|INCLUDE_RODATA)\s*\(\s*\"([^\"]*)\"\s*,\s*(\w+)\s*\)\s*;")
LWC1_RE = re.compile(r"^(\s*/\*[^*]*\*/\s*)lwc1(\s+)(\$f\d+), \((\w+)\)(?:\s*/\* gp_rel: \(\w+\) \*/)?\s*$")
EXTERN_RE = re.compile(r"^\.extern (\w+), \d+\s*$")


def _c_sequence(c_path: Path, strip, c_func_re):
    """(kind, name, line index) for each INCLUDE_ASM / INCLUDE_RODATA line and C function definition, in order."""
    raw = c_path.read_text(encoding="utf-8")
    text = strip(raw)
    items = []
    for m in INC_RE.finditer(text):
        items.append((m.start(), "asm" if m[1] == "INCLUDE_ASM" else "rodata", m[3]))
    for m in c_func_re.finditer(text):
        items.append((m.start(), "c", m[1]))
    items.sort()
    out = []
    for pos, kind, name in items:
        out.append((kind, name, text.count("\n", 0, pos)))
    return out


def _check_rodata_order(seg, entries, c_path: Path):
    from splat.segtypes.common.c import CommonSegC, C_FUNC_RE

    expected = []  # (kind, name) with kind "func" or "rodata"
    for e in entries:
        if e.function is not None:
            expected.append(("func", e.function.getNameUnquoted()))
        else:
            for s in e.rodataSyms:
                expected.append(("rodata", s.getNameUnquoted()))
    actual = _c_sequence(c_path, CommonSegC.strip_c_comments, C_FUNC_RE)
    exp_funcs = {n for k, n in expected if k == "func"}
    # C definitions of names that are not functions of this text range (none expected) are left out of the check
    act = [(("func" if k in ("asm", "c") else "rodata"), n, line) for k, n, line in actual
           if k == "rodata" or n in exp_funcs]
    present_ro = {n for k, n, _ in act if k == "rodata"}
    missing = [n for k, n in expected if k == "rodata" and n not in present_ro]
    if missing and FIX_INCLUDES:
        _insert_rodata_includes(seg, expected, act, c_path, set(missing))
        actual = _c_sequence(c_path, CommonSegC.strip_c_comments, C_FUNC_RE)
        act = [(("func" if k in ("asm", "c") else "rodata"), n, line) for k, n, line in actual
               if k == "rodata" or n in exp_funcs]
        SUMMARY["ro_inserted"] += len(missing)
    got = [(k, n) for k, n, _ in act]
    if got != expected:
        for i, (a, b) in enumerate(zip(got, expected)):
            if a != b:
                break
        else:
            i = min(len(got), len(expected))
        ERRORS.append(f"{c_path.as_posix()}: INCLUDE / function order differs from splat's at item {i}: "
                      f"file has {got[i:i + 3]}, splat expects {expected[i:i + 3]}"
                      + (f" ({len(missing)} INCLUDE_RODATA lines missing: run tools/split.py with --fix-includes)"
                         if missing and not FIX_INCLUDES else ""))


def _insert_rodata_includes(seg, expected, act, c_path: Path, missing: set):
    raw = c_path.read_text(encoding="utf-8")
    m = INC_RE.search(raw)
    folder = m[2] if m else f"asm/nonmatchings/{seg.name}"
    lines = raw.split("\n")
    func_line = {n: line for k, n, line in act if k == "func"}
    inserts: dict[int, list[str]] = {}
    for idx, (k, n) in enumerate(expected):
        if k != "rodata" or n not in missing:
            continue
        nxt = next((m for kk, m in expected[idx + 1:] if kk == "func"), None)
        at = func_line[nxt] if nxt is not None else len(lines)
        if nxt is None:
            while at > 0 and lines[at - 1] == "":
                at -= 1
        inserts.setdefault(at, []).append(f'INCLUDE_RODATA("{folder}", {n});')
    for at in sorted(inserts, reverse=True):
        block = inserts[at]
        if at < len(lines) and lines[at].startswith("INCLUDE_ASM"):
            lines[at:at] = block
        elif at >= len(lines) or (at > 0 and lines[at - 1] != "" and lines[at] == ""):
            lines[at:at] = [""] + block  # after a C definition at the end of the file
        else:  # before a C definition: a blank line after the block
            lines[at:at] = block + [""]
    c_path.write_text("\n".join(lines), encoding="utf-8", newline="\n")


DATA_MACROS = {".sdata": "INCLUDE_SDATA", ".data": "INCLUDE_DATA"}


def _owned_data(seg, sect: str, data_seg):
    """A C file owns a .sdata / .data range: write each symbol of it to asm/nonmatchings/<file>/<sym>.s and check
    the C file includes them all, in address order (INCLUDE_SDATA / INCLUDE_DATA). splat migrates no .sdata /
    .data, and where these lines sit among the functions does not matter (another section); only their order
    among themselves and among the file's own C data definitions does."""
    from splat.util import options

    macro = DATA_MACROS[sect]
    asm_dir = options.opts.nonmatchings_path / seg.dir / seg.name
    asm_dir.mkdir(parents=True, exist_ok=True)
    names = []
    for s in data_seg.spim_section.get_section().symbolList:
        name = s.getNameUnquoted()
        names.append(name)
        with (asm_dir / f"{name}.s").open("w", encoding="utf-8", newline="\n") as f:
            f.write(f".section {sect}\n\n")
            f.write(s.disassemble(useGlobalLabel=True, isSplittedSymbol=True))
    SUMMARY["data_syms"] += len(names)
    c_path = seg.out_path()
    raw = c_path.read_text(encoding="utf-8")
    inc = re.compile(rf"\b{macro}\s*\(\s*\"([^\"]*)\"\s*,\s*(\w+)\s*\)\s*;")
    from splat.segtypes.common.c import CommonSegC
    got = [m[2] for m in inc.finditer(CommonSegC.strip_c_comments(raw))]
    if not got and FIX_INCLUDES:
        m = INC_RE.search(raw)
        folder = m[2] if m else f"asm/nonmatchings/{seg.name}"
        lines = raw.split("\n")
        at = next(i for i, l in enumerate(lines) if l.startswith("#include")) + 1
        block = ["", f"/* ER-024: this file's {sect} (docs/build.md 3.2) */"]
        block += [f'{macro}("{folder}", {n});' for n in names]
        lines[at:at] = block
        c_path.write_text("\n".join(lines), encoding="utf-8", newline="\n")
        got = names
        SUMMARY["ro_inserted"] += len(names)
    if got != names:
        ERRORS.append(f"{c_path.as_posix()}: {macro} lines {got[:3]}... differ from the owned {sect} symbols "
                      f"{names[:3]}... (all of them, in address order; --fix-includes writes them if none exist)")


ALIGN_RE = re.compile(r"^\.align (\d+)\s*$")


def _rodata_align(seg, ro, entries, rom_bytes: bytes):
    """Give 16-byte alignment (`.align 4`) to the .rodata items that had it in the original object.

    spimdisasm writes `.align 3` before jump tables; ee-gcc 2.9 emits them with `.align 4` (test compile,
    docs/game_seams.md 3) and every game jump table is 16-aligned. Other items: a label on a 16-byte boundary with
    8 or more zero bytes before it (inside the file's range) was 16-aligned (8-byte alignment cannot explain the
    gap; the same evidence R as docs/game_seams.md 4.4). While the item before stays asm the zeros are in its
    bytes and the directive changes nothing; once that item comes from C, the directive makes the padding.
    Only for files whose range starts on a 16-byte boundary: in another file a 16-aligned item would move the
    whole object (text_001343E8, docs/build.md 3.2)."""
    from splat.util import options

    lo = ro.vram_start
    if lo % 16:
        SUMMARY["align_skipped"].append(seg.name)
        return
    rom0 = ro.rom_start
    files: dict[str, set] = {}
    for e in entries:
        for s in e.rodataSyms:
            v = s.vram
            is_jtbl = s.getNameUnquoted().startswith("jtbl_") or s.contextSym.isJumpTable()
            padded = (v % 16 == 0 and v - 8 >= lo
                      and rom_bytes[v - 8 - lo + rom0:v - lo + rom0] == bytes(8))
            if not (is_jtbl and v % 16 == 0) and not padded:
                continue
            owner = e.function.getNameUnquoted() if e.function is not None else s.getNameUnquoted()
            files.setdefault(owner, set()).add(s.getNameUnquoted())
    asm_dir = options.opts.nonmatchings_path / seg.dir / seg.name
    for owner, names in files.items():
        path = asm_dir / f"{owner}.s"
        if not path.exists():
            continue  # a C function's own rodata: gcc writes it
        lines = path.read_text(encoding="utf-8").split("\n")
        out = []
        for line in lines:
            parts = line.split()
            if len(parts) == 2 and parts[0] == "nonmatching" and parts[1] in names:
                j = len(out) - 1
                while j >= 0 and out[j].strip() == "":
                    j -= 1
                if j >= 0 and ALIGN_RE.match(out[j]):
                    out[j] = ".align 4"
                else:
                    out.append(".align 4")
                SUMMARY["align4"] += 1
            out.append(line)
        path.write_text("\n".join(out), encoding="utf-8", newline="\n")


def _float_operand(word: int) -> str:
    exp = (word >> 23) & 0xFF
    if exp == 0 or exp == 0xFF or (word & 0xFFFF) == 0:
        raise ValueError(f"word 0x{word:08X} is not a normal float with non-zero low half")
    value = struct.unpack("<f", struct.pack("<I", word))[0]
    text = f"{value:.20e}"  # the form ee-gcc writes for li.s operands
    assert struct.unpack("<I", struct.pack("<f", float(text)))[0] == word and math.isfinite(value)
    return text


def _lit4_rewrite(seg, lit4_seg, rom_bytes: bytes):
    from splat.util import options

    lo, hi = lit4_seg.vram_start, lit4_seg.vram_end
    rom0 = lit4_seg.rom_start
    names = {}
    for sym in lit4_seg.spim_section.get_section().symbolList:
        names[sym.getNameUnquoted()] = sym.vram
    owners: dict[int, list] = {}
    for func in seg.spim_section.get_section().symbolList:
        for _off, target in func.instrAnalyzer.referencedVramsInstrOffset.items():
            if lo <= target < hi:
                owners.setdefault(target, []).append(func)
    SUMMARY["lit4_files"] += 1
    asm_dir = options.opts.nonmatchings_path / seg.dir / seg.name
    todo: dict[str, dict[int, str]] = {}  # func name -> {vram: operand}
    for vram in range(lo, hi, 4):
        fs = owners.get(vram, [])
        if len(fs) != 1:
            ERRORS.append(f"{seg.name}: .lit4 word 0x{vram:08X} used by {len(fs)} instructions "
                          f"({sorted({f.getNameUnquoted() for f in fs})}); expected exactly 1")
            continue
        fname = fs[0].getNameUnquoted()
        if fname not in seg.global_asm_funcs:
            SUMMARY["lit4_c"] += 1  # a C function: its li.s gives this word
            continue
        off = vram - lo + rom0
        word = int.from_bytes(rom_bytes[off:off + 4], "little")
        try:
            todo.setdefault(fname, {})[vram] = _float_operand(word)
        except ValueError as e:
            ERRORS.append(f"{seg.name}: .lit4 0x{vram:08X} ({fname}): {e}")
    for fname, words in todo.items():
        path = asm_dir / f"{fname}.s"
        owned = re.compile(r"\b(" + "|".join(n for n, v in names.items() if v in words) + r")\b")
        out, done = [], set()
        for line in path.read_text(encoding="utf-8").split("\n"):
            m = LWC1_RE.match(line)
            if m and names.get(m[4]) in words:
                vram = names[m[4]]
                if vram in done:
                    ERRORS.append(f"{path.as_posix()}: .lit4 0x{vram:08X} loaded twice")
                done.add(vram)
                out.append(f"{m[1]}li.s{m[2]}{m[3]}, {words[vram]}")
                continue
            e = EXTERN_RE.match(line)
            if e and names.get(e[1]) in words:
                continue  # the literal has no label any more
            if owned.search(line):
                ERRORS.append(f"{path.as_posix()}: unexpected use of an owned .lit4 label: {line.strip()}")
            out.append(line)
        for vram in set(words) - done:
            ERRORS.append(f"{path.as_posix()}: no `lwc1 $f, (label)` found for .lit4 0x{vram:08X}")
        path.write_text("\n".join(out), encoding="utf-8", newline="\n")
        SUMMARY["lit4_asm"] += len(done)


def _install_hooks():
    import spimdisasm
    from splat.segtypes.common.c import CommonSegC
    from splat.util import options

    orig_split = CommonSegC.split

    def split(self, rom_bytes: bytes):
        orig_split(self, rom_bytes)
        if self.is_auto_segment or self.rom_start == self.rom_end:
            return
        ro = self.siblings.get(".rodata")
        if ro is not None and not ro.is_generated and options.opts.migrate_rodata_to_functions:
            entries = spimdisasm.mips.FunctionRodataEntry.getAllEntriesFromSections(
                self.spim_section.get_section(), ro.spim_section.get_section())
            asm_out_dir = options.opts.nonmatchings_path / self.dir
            for e in entries:
                if e.function is None:
                    for s in e.rodataSyms:
                        sym = self.get_symbol(s.vram, in_segment=True, local_only=True)
                        self.create_unmigrated_rodata_file(s, asm_out_dir, sym)
                        SUMMARY["ro_standalone"] += 1
            SUMMARY["ro_files"] += 1
            _check_rodata_order(self, entries, self.out_path())
            _rodata_align(self, ro, entries, rom_bytes)
        lit4 = self.siblings.get(".lit4")
        if lit4 is not None and not lit4.is_generated:
            _lit4_rewrite(self, lit4, rom_bytes)
        for sect in (".data", ".sdata"):
            d = self.siblings.get(sect)
            if d is not None and not d.is_generated:
                SUMMARY["data_files"].append(f"{self.name}{sect}")
                _owned_data(self, sect, d)

    CommonSegC.split = split


if __name__ == "__main__":
    from splat.__main__ import splat_main

    args = sys.argv[1:]
    if "--fix-includes" in args:
        args.remove("--fix-includes")
        FIX_INCLUDES = True
    _install_hooks()
    sys.argv = ["splat", "split"] + args
    splat_main()
    s = SUMMARY
    print(f"split.py: .rodata owned by {s['ro_files']} C files ({s['ro_standalone']} INCLUDE_RODATA symbols)"
          f"; .lit4 owned by {s['lit4_files']}"
          f" C files ({s['lit4_asm']} asm loads written as li.s, {s['lit4_c']} literals from C); .align 4 on"
          f" {s['align4']} .rodata items (not in {', '.join(s['align_skipped']) or '-'}: range not 16-aligned);"
          f" owned .data / .sdata: {len(s['data_files'])} ranges, {s['data_syms']} symbols"
          f"{'; %d INCLUDE lines inserted in C files' % s['ro_inserted'] if s['ro_inserted'] else ''}")
    if ERRORS:
        for e in ERRORS:
            print("split.py: ERROR:", e, file=sys.stderr)
        sys.exit(1)
