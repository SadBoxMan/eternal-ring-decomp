#!/usr/bin/env python3
"""ER-003: run splat with one output fix for the 1999 EE assembler.

ee-as 2.9 (GNU as 2.9-ee-991111) parses denormal floats wrongly: `.float 5.605193857e-45` (word 0x00000004)
assembles to 0x00000003. The first byte-for-byte link differed in exactly that one word (.data, 0x0032FD14).
spimdisasm has no option for this, so this wrapper makes it write denormal floats and doubles as plain
`.word`s (the bytes are what the disassembler read; only the spelling changes). Everything else is splat
0.50.1 as pinned in requirements.txt.

Usage: python -I tools/split.py <config.yaml>   (same arguments as `python -m splat split`)
"""
import sys

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

if __name__ == "__main__":
    from splat.__main__ import splat_main

    sys.argv = ["splat", "split"] + sys.argv[1:]
    splat_main()
