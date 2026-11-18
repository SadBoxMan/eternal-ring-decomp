# ER-008: write a PS2Recomp "Ghidra map" CSV (name,start,end,size; end exclusive)
# from the FUNC symbols of our split's symbol ELF. Output stays outside the repo.
import struct, sys
src, dst = sys.argv[1], sys.argv[2]
d = open(src, 'rb').read()
assert d[:4] == b'\x7fELF' and d[4] == 1 and d[5] == 1
shoff, = struct.unpack_from('<I', d, 0x20)
shentsize, shnum = struct.unpack_from('<HH', d, 0x2E)
secs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
rows = []
for s in secs:
    if s[1] != 2:  # SHT_SYMTAB
        continue
    strtab = secs[s[6]]
    for off in range(s[4], s[4] + s[5], 16):
        name_off, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, off)
        if info & 0xF != 2 or size == 0:  # STT_FUNC with a size
            continue
        so = strtab[4] + name_off
        name = d[so:d.index(b'\0', so)].decode()
        rows.append((value, name, size))
rows.sort()
with open(dst, 'w', newline='\n') as f:
    f.write('name,start,end,size\n')
    for v, n, sz in rows:
        f.write(f'{n},0x{v:08X},0x{v+sz:08X},{sz}\n')
print(len(rows), 'functions')
