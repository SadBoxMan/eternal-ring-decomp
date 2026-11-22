# ER-008: write a PS2Recomp "Ghidra map" CSV (name,start,end,size; end exclusive)
# from the FUNC symbols of our split's symbol ELF. Output stays outside the repo.
#
# ER-074: also take the code blocks the split labels as data. spimdisasm writes a whole function as a `dlabel`
# block (an STT_OBJECT symbol, still named func_XXXXXXXX) when one of its words decodes as an invalid
# instruction (docs/toolchain.md 6.1: func_001D73D0, func_001D7658, func_001DECA0, each a VU0 `vsqrt` with a
# stray field, or a constant that doubles as a no-op). Left out, PS2Recomp emits no body for them and every call
# ends at `[guest-branch:missing-target]` (NEWGAME: JAL 0x001D73D0 from 0x00123CAC). Only OBJECT symbols named
# func_* with a size, in an executable section, are taken.
# ER-074: every row is written with its address name (func_XXXXXXXX), not the split's current name. At ER-008
# all names were address names; since then the decomp has named some (sqrtf, dpadd, ...). PS2Recomp only avoids
# C++ keywords, so a generated function called `sqrtf` could clash with the C runtime, and each rename would
# rename generated files. The recomp output thus depends only on the function boundaries.
import struct, sys
src, dst = sys.argv[1], sys.argv[2]
d = open(src, 'rb').read()
assert d[:4] == b'\x7fELF' and d[4] == 1 and d[5] == 1
shoff, = struct.unpack_from('<I', d, 0x20)
shentsize, shnum = struct.unpack_from('<HH', d, 0x2E)
secs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
SHF_EXECINSTR = 0x4
rows, data_blocks = [], []
for s in secs:
    if s[1] != 2:  # SHT_SYMTAB
        continue
    strtab = secs[s[6]]
    for off in range(s[4], s[4] + s[5], 16):
        name_off, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, off)
        kind = info & 0xF
        if size == 0 or kind not in (1, 2):  # STT_OBJECT / STT_FUNC with a size
            continue
        so = strtab[4] + name_off
        name = d[so:d.index(b'\0', so)].decode()
        if kind == 1:
            if not name.startswith('func_') or shndx >= len(secs) or not secs[shndx][2] & SHF_EXECINSTR:
                continue
            data_blocks.append(name)
        rows.append((value, name, size))
rows.sort()
for (v1, n1, s1), (v2, n2, s2) in zip(rows, rows[1:]):
    assert v1 + s1 <= v2, f'overlap: {n1} and {n2}'
with open(dst, 'w', newline='\n') as f:
    f.write('name,start,end,size\n')
    for v, n, sz in rows:
        f.write(f'func_{v:08X},0x{v:08X},0x{v+sz:08X},{sz}\n')
print(len(rows), 'functions, of which', len(data_blocks), 'data-labelled code blocks:', ', '.join(sorted(data_blocks)))
