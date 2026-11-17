"""ER-011: byte-compare functions in a relocatable EE object (or a linked ELF) with the game ELF.

Usage:
  python -I er011_compare.py obj  <file.o>   <game_elf>   # masked compare of the unlinked object
  python -I er011_compare.py elf  <linked>   <game_elf>   # exact compare of a linked ELF (symbols kept)

Function map: object symbol -> game address (the address names come from ER-002's reading of the ELF).
Game file offset = vaddr - 0x000FF000 (ER-002, docs/toolchain.md section 6).
Masking (obj mode): R_MIPS_26 compares the top 6 bits and checks that the game's jump target equals the address
in the callee's name (func_XXXXXXXX); R_MIPS_GPREL16 compares the top 16 bits (opcode, base $gp, rt) and reports
the game's gp offset, which cannot be checked without the variable's address. Nothing else is masked.
"""
import struct
import sys

FUNCS = {
    "wrap_12f7e0": 0x0012F7E0,
    "update_1257e8": 0x001257E8,
    "pack_12d3b8": 0x0012D3B8,
}
FILE_DELTA = 0x000FF000
R_MIPS_26, R_MIPS_GPREL16 = 4, 7


def read_elf(path):
    d = open(path, "rb").read()
    (e_shoff,) = struct.unpack_from("<I", d, 0x20)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from("<HHH", d, 0x2E)
    secs = []
    for i in range(e_shnum):
        secs.append(struct.unpack_from("<IIIIIIIIII", d, e_shoff + i * e_shentsize))
    shstr = secs[e_shstrndx]
    def name(off, tab):
        s = tab[4] + off
        return d[s:d.index(b"\0", s)].decode()
    named = {name(s[0], shstr): (i, s) for i, s in enumerate(secs)}
    return d, secs, named, name


def symbols(d, secs, named, name):
    _, sym = named[".symtab"]
    strtab = secs[sym[6]]
    out = []
    for k in range(sym[5] // 16):
        st_name, st_value, st_size, st_info, st_other, st_shndx = struct.unpack_from("<IIIBBH", d, sym[4] + k * 16)
        out.append((name(st_name, strtab), st_value, st_size, st_shndx))
    return out


def main():
    mode, path, game_path = sys.argv[1:4]
    game = open(game_path, "rb").read()
    d, secs, named, name = read_elf(path)
    tidx, text = named[".text"]
    syms = symbols(d, secs, named, name)
    relocs = {}
    if mode == "obj" and ".rel.text" in named:
        _, rel = named[".rel.text"]
        for k in range(rel[5] // 8):
            r_off, r_info = struct.unpack_from("<II", d, rel[4] + k * 8)
            relocs[r_off] = (r_info & 0xFF, syms[r_info >> 8][0])
    starts = sorted(v for (n, v, s, sh) in syms if sh == tidx and n in FUNCS)
    text_end = text[3] + text[5] if mode == "elf" else text[5]
    base = text[3] if mode == "elf" else 0
    all_ok = True
    for n, v, s, sh in syms:
        if sh != tidx or n not in FUNCS:
            continue
        nxt = [x for x in starts if x > v]
        end = nxt[0] if nxt else text_end
        mine = d[text[4] + (v - base): text[4] + (end - base)]
        # trailing alignment padding (zero words) is not part of the function
        while mine.endswith(b"\0\0\0\0"):
            mine = mine[:-4]
        gaddr = FUNCS[n]
        theirs = game[gaddr - FILE_DELTA: gaddr - FILE_DELTA + len(mine)]
        notes, first_diff = [], None
        for w in range(0, len(mine), 4):
            a, = struct.unpack_from("<I", mine, w)
            b, = struct.unpack_from("<I", theirs, w)
            rel = relocs.get(v + w)
            if rel and rel[0] == R_MIPS_26:
                ok = (a >> 26) == (b >> 26)
                target = (gaddr & 0xF0000000) | ((b & 0x03FFFFFF) << 2)
                want = int(rel[1].split("_")[1], 16) if rel[1].startswith("func_") else None
                ok = ok and target == want
                notes.append(f"+{w:#04x} R_MIPS_26 {rel[1]}: game jal target {target:#010x} {'ok' if target == want else 'MISMATCH'}")
            elif rel and rel[0] == R_MIPS_GPREL16:
                ok = (a >> 16) == (b >> 16)
                off = struct.unpack("<h", struct.pack("<H", b & 0xFFFF))[0]
                notes.append(f"+{w:#04x} R_MIPS_GPREL16 {rel[1]}: game gp offset {off:#x} (not checkable)")
            elif rel:
                ok = False
                notes.append(f"+{w:#04x} unhandled relocation type {rel[0]}")
            else:
                ok = a == b
            if not ok and first_diff is None:
                first_diff = (w, a, b)
        status = "MATCH" if first_diff is None else f"DIFF at +{first_diff[0]:#x}: ours {first_diff[1]:08x} game {first_diff[2]:08x}"
        all_ok &= first_diff is None
        print(f"{n} -> func_{gaddr:08X}: {len(mine)} bytes ({len(mine)//4} words): {status}")
        for x in notes:
            print("    " + x)
    sys.exit(0 if all_ok else 1)


if __name__ == "__main__":
    main()
