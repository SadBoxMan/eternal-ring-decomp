#!/usr/bin/env python3
"""ER-003: write the load image ("rom") of SLUS_200.15 for splat.

The main EE ELF has one PT_LOAD segment (file offset 0x1000, vaddr 0x00100000). splat splits that
segment's file bytes, not the ELF. This writes exactly p_filesz bytes from p_offset, after checking the
ELF's SHA-1 against the pinned value in tools/regress_baseline.txt. Game data: the output stays in a
gitignored folder.

Usage: python -I tools/extract_rom.py <SLUS_200.15> <out.rom>
"""
import hashlib
import struct
import sys
from pathlib import Path


def load_segment(elf: bytes) -> tuple[int, int, int]:
    if elf[:4] != b"\x7fELF" or elf[4] != 1 or elf[5] != 1:
        sys.exit("not a 32-bit little-endian ELF")
    e_phoff, = struct.unpack_from("<I", elf, 0x1C)
    e_phentsize, e_phnum = struct.unpack_from("<HH", elf, 0x2A)
    loads = []
    for i in range(e_phnum):
        p_type, p_offset, p_vaddr, _p_paddr, p_filesz = struct.unpack_from("<5I", elf, e_phoff + i * e_phentsize)
        if p_type == 1:
            loads.append((p_offset, p_vaddr, p_filesz))
    if len(loads) != 1:
        sys.exit(f"expected one PT_LOAD, found {len(loads)}")
    return loads[0]


def main() -> None:
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    src, out = Path(sys.argv[1]), Path(sys.argv[2])
    elf = src.read_bytes()
    baseline = Path(__file__).with_name("regress_baseline.txt")
    want = None
    for line in baseline.read_text(encoding="utf-8").splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[0] == "elf":
            want = (parts[1].lower(), int(parts[2]))
    got = (hashlib.sha1(elf).hexdigest(), len(elf))
    if want is None or got != want:
        sys.exit(f"{src}: SHA-1/size {got} does not match the pinned original {want}")
    off, vaddr, size = load_segment(elf)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(elf[off:off + size])
    print(f"{out}: {size:#x} bytes from file offset {off:#x} (vaddr {vaddr:#010x})")


if __name__ == "__main__":
    main()
