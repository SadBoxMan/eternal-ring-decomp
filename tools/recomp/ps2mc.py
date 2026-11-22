#!/usr/bin/env python3
"""ER-099: list (and optionally extract) a PCSX2 PS2 memory card without changing it.

Reads either form PCSX2 uses:
  * a card image (`Mcd001.ps2`): the raw flash, 8 MB of data, normally with the 16-byte spare (ECC) area after
    every 512-byte page (8,650,752 bytes); an image without the spare area (8,388,608 bytes) is read too;
  * a folder card (a folder named `*.ps2` holding `_pcsx2_superblock`): plain host files, one folder per save, with
    PCSX2's bookkeeping files `_pcsx2_*` (skipped).

It is the cross-check for the runtime's import (`er099_mc_import.patch`): run it on a card and on the folder the
runtime made from it, with `--sha1`, and compare the two listings. The card file is only ever opened for reading.

Usage:
  python -I tools/recomp/ps2mc.py CARD [--sha1] [--extract NEW_DIR]
  python -I tools/recomp/ps2mc.py --plain DIR --sha1        (any host folder, e.g. the runtime's mc0_import)

The file system layout follows the public descriptions of the PS2 memory card format (superblock, indirect FAT,
512-byte directory entries); the ECC is the 3-byte Hamming code per 128-byte chunk used by mcman. Names and field
meanings here are those public ones, not read from Eternal Ring.
"""
import argparse
import datetime
import hashlib
import os
import struct
import sys

MAGIC = b"Sony PS2 Memory Card Format "

DF_READ = 0x0001
DF_WRITE = 0x0002
DF_EXECUTE = 0x0004
DF_PROTECTED = 0x0008
DF_FILE = 0x0010
DF_DIRECTORY = 0x0020
DF_EXISTS = 0x8000

FAT_END = 0xFFFFFFFF
FAT_ALLOCATED = 0x80000000

# ECC: column parity of each byte through seven masks, line parity of the byte indices with odd parity.
_PARITY = [bin(b).count("1") & 1 for b in range(256)]
_CP_MASKS = [0x55, 0x33, 0x0F, 0x00, 0xAA, 0xCC, 0xF0]
_COLUMN = [sum(_PARITY[b & m] << i for i, m in enumerate(_CP_MASKS)) for b in range(256)]


def ecc128(chunk):
    cp, lp0, lp1 = 0x77, 0x7F, 0x7F
    for i, b in enumerate(chunk):
        cp ^= _COLUMN[b]
        if _PARITY[b]:
            lp0 ^= ~i
            lp1 ^= i
    return bytes([cp & 0xFF, lp0 & 0x7F, lp1 & 0x7F])


def stamp(raw):
    """sceMcStDateTime: resv, sec, min, hour, day, month, year (u16), as stored on the card."""
    _, sec, mnt, hour, day, month, year = struct.unpack("<BBBBBBH", raw)
    return "%04d-%02d-%02d %02d:%02d:%02d" % (year, month, day, hour, mnt, sec)


class CardError(Exception):
    pass


class Entry:
    def __init__(self, raw):
        (self.mode, _, self.length, created, self.cluster, self.dir_entry, modified,
         self.attr) = struct.unpack_from("<HHI8sII8sI", raw, 0)
        self.created = stamp(created)
        self.modified = stamp(modified)
        self.name = raw[0x40:0x60].split(b"\0", 1)[0].decode("ascii", "replace")


class CardImage:
    def __init__(self, path):
        with open(path, "rb") as f:
            self.buf = f.read()
        sb = self.buf[:0x154]
        if not sb.startswith(MAGIC):
            raise CardError("not a PS2 memory card image (no superblock magic)")
        self.version = sb[0x1C:0x28].split(b"\0", 1)[0].decode("ascii", "replace")
        (self.page_len, self.pages_per_cluster, self.pages_per_block, _, self.clusters_per_card,
         self.alloc_offset, self.alloc_end, self.rootdir_cluster, self.backup1,
         self.backup2) = struct.unpack_from("<HHHHIIIIII", sb, 0x28)
        self.ifc_list = struct.unpack_from("<32I", sb, 0x50)
        self.card_type, self.card_flags = sb[0x150], sb[0x151]
        self.spare_len = self.page_len // 32
        pages = self.clusters_per_card * self.pages_per_cluster
        if len(self.buf) == pages * (self.page_len + self.spare_len):
            self.ecc = True
            self.raw_page = self.page_len + self.spare_len
        elif len(self.buf) == pages * self.page_len:
            self.ecc = False
            self.raw_page = self.page_len
        else:
            raise CardError("file size %d fits neither %d pages of %d nor of %d bytes"
                            % (len(self.buf), pages, self.page_len + self.spare_len, self.page_len))
        self.cluster_size = self.page_len * self.pages_per_cluster
        self.per_cluster = self.cluster_size // 4
        self.ecc_pages = 0
        self.ecc_bad = []
        self._checked = set()

    def page(self, n):
        off = n * self.raw_page
        data = self.buf[off:off + self.page_len]
        if self.ecc and n not in self._checked:
            self._checked.add(n)
            spare = self.buf[off + self.page_len:off + self.raw_page]
            self.ecc_pages += 1
            for k in range(self.page_len // 128):
                if ecc128(data[k * 128:(k + 1) * 128]) != spare[k * 3:k * 3 + 3]:
                    self.ecc_bad.append((n, k))
        return data

    def cluster(self, n):
        if not 0 <= n < self.clusters_per_card:
            raise CardError("cluster %d outside the card" % n)
        first = n * self.pages_per_cluster
        return b"".join(self.page(first + i) for i in range(self.pages_per_cluster))

    def fat(self, n):
        """FAT entry of allocatable cluster n (relative to alloc_offset)."""
        indirect_index, fat_offset = divmod(n, self.per_cluster)
        dbl_index, indirect_offset = divmod(indirect_index, self.per_cluster)
        if dbl_index >= len(self.ifc_list):
            raise CardError("FAT index %d outside the indirect list" % n)
        fat_cluster = struct.unpack_from("<I", self.cluster(self.ifc_list[dbl_index]), indirect_offset * 4)[0]
        return struct.unpack_from("<I", self.cluster(fat_cluster), fat_offset * 4)[0]

    def chain(self, start):
        n, seen = start, set()
        while True:
            if n in seen:
                raise CardError("FAT chain from %d loops at %d" % (start, n))
            seen.add(n)
            yield n
            entry = self.fat(n)
            if entry == FAT_END:
                return
            if not entry & FAT_ALLOCATED:
                raise CardError("FAT chain from %d reaches free cluster entry 0x%08X" % (start, entry))
            n = entry & 0x7FFFFFFF

    def read_chain(self, start, nbytes):
        out = bytearray()
        for n in self.chain(start):
            if len(out) >= nbytes:
                break
            out += self.cluster(n + self.alloc_offset)
        if len(out) < nbytes:
            raise CardError("chain from %d holds %d bytes, %d needed" % (start, len(out), nbytes))
        return bytes(out[:nbytes])

    def read_dir(self, start, count=None):
        """Entries of the directory at allocatable cluster `start`. Its entry count is the length field of the
        directory's own entry in its parent; only the root, which has no parent, keeps it in its '.' entry."""
        if count is None:
            count = Entry(self.cluster(start + self.alloc_offset)[:512]).length
        data = self.read_chain(start, count * 512)
        return [Entry(data[i * 512:(i + 1) * 512]) for i in range(count)]

    def walk(self, start=None, count=None, prefix=""):
        """Yields (path, Entry, data or None); skips '.', '..' and deleted entries."""
        start = self.rootdir_cluster if start is None else start
        for i, e in enumerate(self.read_dir(start, count)):
            if i < 2 or not e.mode & DF_EXISTS:
                continue
            path = prefix + "/" + e.name
            if e.mode & DF_DIRECTORY:
                yield path, e, None
                yield from self.walk(e.cluster, e.length, path)
            elif e.mode & DF_FILE:
                yield path, e, (self.read_chain(e.cluster, e.length) if e.length else b"")


def walk_folder(root, skip_pcsx2):
    for dirpath, dirnames, filenames in os.walk(root):
        dirnames.sort(key=str.lower)
        rel = os.path.relpath(dirpath, root).replace(os.sep, "/")
        rel = "" if rel == "." else "/" + rel
        for d in dirnames:
            yield rel + "/" + d, os.path.join(dirpath, d), None
        for fn in sorted(filenames, key=str.lower):
            if skip_pcsx2 and fn.startswith("_pcsx2_"):
                continue
            full = os.path.join(dirpath, fn)
            with open(full, "rb") as f:
                yield rel + "/" + fn, full, f.read()


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("card", help="card image file, PCSX2 folder card, or (with --plain) any folder")
    ap.add_argument("--plain", action="store_true", help="list a host folder as is (no superblock needed)")
    ap.add_argument("--sha1", action="store_true", help="print each file's SHA-1")
    ap.add_argument("--extract", metavar="NEW_DIR", help="write the card's files into a new (or empty) folder")
    args = ap.parse_args()

    rows = []
    if os.path.isdir(args.card):
        sbp = os.path.join(args.card, "_pcsx2_superblock")
        if not args.plain:
            if not os.path.isfile(sbp):
                sys.exit("%s: no _pcsx2_superblock (use --plain for any folder)" % args.card)
            with open(sbp, "rb") as f:
                if not f.read(len(MAGIC)) == MAGIC:
                    sys.exit("%s: _pcsx2_superblock has no PS2 card magic" % sbp)
            print("folder card %s" % args.card)
        for path, full, data in walk_folder(args.card, not args.plain):
            mt = datetime.datetime.fromtimestamp(os.path.getmtime(full)).strftime("%Y-%m-%d %H:%M:%S")
            rows.append((path, data, "host " + mt))
    else:
        try:
            card = CardImage(args.card)
            print("image %s: format %s, page %d, %d pages/cluster, %d clusters, alloc %d-%d, root %d, type %d, "
                  "flags 0x%02X, ECC %s" % (args.card, card.version, card.page_len, card.pages_per_cluster,
                                            card.clusters_per_card, card.alloc_offset, card.alloc_end,
                                            card.rootdir_cluster, card.card_type, card.card_flags,
                                            "yes" if card.ecc else "no"))
            for path, e, data in card.walk():
                rows.append((path, data, "mode 0x%04X mod %s cre %s" % (e.mode, e.modified, e.created)))
        except CardError as exc:
            sys.exit("%s: %s" % (args.card, exc))
        if card.ecc:
            print("ECC: %d pages read, %d bad 128-byte chunks%s" % (
                card.ecc_pages, len(card.ecc_bad), (" (first: page %d chunk %d)" % card.ecc_bad[0])
                if card.ecc_bad else ""))

    for path, data, info in rows:
        kind = "<dir>" if data is None else "%8d" % len(data)
        digest = (" " + hashlib.sha1(data).hexdigest()) if (args.sha1 and data is not None) else ""
        print("%-48s %8s  %s%s" % (path, kind, info, digest))
    print("%d directories, %d files" % (sum(r[1] is None for r in rows), sum(r[1] is not None for r in rows)))

    if args.extract:
        out = os.path.abspath(args.extract)
        if os.path.abspath(args.card) == out:
            sys.exit("refusing to extract onto the card itself")
        if os.path.exists(out) and os.listdir(out):
            sys.exit("%s exists and is not empty" % out)
        for path, data, _ in rows:
            target = os.path.join(out, *path.strip("/").split("/"))
            if data is None:
                os.makedirs(target, exist_ok=True)
            else:
                os.makedirs(os.path.dirname(target), exist_ok=True)
                with open(target, "wb") as f:
                    f.write(data)
        print("extracted to %s" % out)


if __name__ == "__main__":
    main()
