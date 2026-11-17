"""ER-003: minimal ELF32 little-endian reader shared by the build tools (no third-party packages)."""
import struct
from dataclasses import dataclass


@dataclass
class Section:
    name: str
    type: int
    flags: int
    addr: int
    offset: int
    size: int
    link: int
    info: int
    align: int
    entsize: int


@dataclass
class Symbol:
    name: str
    value: int
    size: int
    type: int      # 0 NOTYPE, 1 OBJECT, 2 FUNC, 3 SECTION, 4 FILE
    bind: int      # 0 LOCAL, 1 GLOBAL, 2 WEAK
    shndx: int


class Elf:
    def __init__(self, data: bytes):
        if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
            raise ValueError("not an ELF32 little-endian file")
        self.data = data
        self.entry, self.phoff, self.shoff, self.flags = struct.unpack_from("<4I", data, 0x18)
        self.phentsize, self.phnum, self.shentsize, self.shnum, self.shstrndx = struct.unpack_from("<5H", data, 0x2A)
        raw = [struct.unpack_from("<10I", data, self.shoff + i * self.shentsize) for i in range(self.shnum)]
        stroff = raw[self.shstrndx][4] if self.shnum else 0
        self.sections = [Section(self._str(stroff, r[0]), *r[1:]) for r in raw]
        self.phdrs = [struct.unpack_from("<8I", data, self.phoff + i * self.phentsize) for i in range(self.phnum)]

    def _str(self, base: int, off: int) -> str:
        start = base + off
        return self.data[start:self.data.index(b"\0", start)].decode("ascii", "replace")

    def symbols(self) -> list[Symbol]:
        out = []
        for sec in self.sections:
            if sec.type != 2:  # SHT_SYMTAB
                continue
            strtab = self.sections[sec.link].offset
            for i in range(sec.size // 16):
                st_name, st_value, st_size, st_info, _other, st_shndx = struct.unpack_from(
                    "<IIIBBH", self.data, sec.offset + i * 16)
                out.append(Symbol(self._str(strtab, st_name), st_value, st_size, st_info & 0xF, st_info >> 4, st_shndx))
        return out

    def load_bytes(self, vaddr: int, size: int) -> bytes | None:
        """Bytes at a virtual address from the file image of a PT_LOAD segment (None if not file-backed)."""
        for p_type, p_offset, p_vaddr, _pa, p_filesz, _memsz, _fl, _al in self.phdrs:
            if p_type == 1 and p_vaddr <= vaddr and vaddr + size <= p_vaddr + p_filesz:
                off = p_offset + vaddr - p_vaddr
                return self.data[off:off + size]
        return None
