# Disc layout (ER-001)

Written Tue 17 Nov 2026 by ER-001. Eternal Ring, US release, SLUS_200.15, v1.00, NTSC. Hashes, sizes and names only; no game data.

## Dump method

- Source: LuciDin's physical disc in drive D: (TSSTcorp SN-208FB USB DVD writer), the whole volume, sector 0 to the end.
- Tool: a small .NET reader (PowerShell `Add-Type`) reading the raw device `\\.\D:` in 2048-byte sectors, 32 sectors per read, 3 retries per chunk and 5 per sector, every error logged with its sector number. Chosen because no dump tool was installed and only native Windows methods are allowed. It reads user data only (no subchannel or other sector data).
- Output: `disc/eternal_ring_us.iso` (gitignored), 4,637,687,808 bytes = 2,264,496 sectors. Run took about 10 minutes.
- **Error log: 0 read errors, 0 retries, 0 unrecovered sectors** (`disc/dump_errors.log`).
- Primary volume descriptor (sector 16): `CD001`, system id `PLAYSTATION`, volume id empty, volume space size 2,264,496 blocks of 2048 = 4,637,687,808 bytes. **ISO size matches the PVD.**

## ISO hashes

| Hash | Value |
|---|---|
| CRC32 | BB98D81A |
| MD5 | 3F87FAD4395FE2AEA49D00CD3C921DAB |
| SHA-1 | 11DFDDCBD57F0424304CF821D627D8B7BC9A4F15 |

**redump.org comparison: NOT DONE.** curl and the in-app browser could not reach redump.org, so its robots.txt and terms were not checked and no entry was compared. Waiting on a saved page for SLUS-20015 from LuciDin. No match or mismatch is claimed.

## SYSTEM.CNF (58 bytes, CRLF line endings, ends with 0x1A)

```
BOOT2 = cdrom0:\SLUS_200.15;1
VER = 1.00
VMODE = NTSC
```

## Files (ISO 9660, extracted to gitignored `disc/extract/`, paths as on disc)

All files are in the filesystem from LBA 1,100,000 on; the sum of file sizes is 2,384,851,540 bytes of the 4,637,687,808. Root directory record: LBA 261, 852 bytes. Version suffix `;1` omitted from the names.

| Path | Type | Size | LBA | CRC32 | MD5 | SHA-1 |
|---|---|---:|---:|---|---|---|
| `SYSTEM.CNF` | text | 58 | 1100000 | B2908C17 | EE45C408F516B47458593675A26CF6BD | F27B56EBFFA8DFC18C669493661B172FB488B940 |
| `SLUS_200.15` | ELF (EE, main) | 2,485,924 | 1100001 | A91D8447 | C4C69C9459D06A770BDB542E6A063112 | 6087A8CADB7538A849A4F5760502A39DAA6A12AF |
| `IOPRP165.IMG` | IOP replacement image | 98,901 | 1101215 | 7E09B375 | B326B2C91930A524F568A50489435E1D | D8D2BC74AB39EC8D977742517FA803164B7F8512 |
| `IOP/FSIOPSND.IRX` | IRX module (IOP) | 47,553 | 1101264 | DE0DC4D1 | 2CAD2BB1A3F29101497DDB2FF73D08EE | 83BEAD410EC74BC6D6E4B7B2BCEC77255CFEF089 |
| `IOP/LIBSD.IRX` | IRX module (IOP) | 24,909 | 1101288 | 077844F2 | 470F175E07189FF1C5875A34BCE4170D | B11D9541D666945877D55DBFAEF538B2B3AE4246 |
| `IOP/MCMAN.IRX` | IRX module (IOP) | 82,021 | 1101301 | 8BBDDFBE | 741AC625D042A1384EE0369FB86D90E6 | FCE9D47E39352AC00EAB402A8EBDCAD4EA3F771D |
| `IOP/MCSERV.IRX` | IRX module (IOP) | 5,865 | 1101342 | 0EFB0A8E | 6F448BF6C6F72A70D23BB0F085003F7A | 324405FA23A5BB057A936040024659A94270CC45 |
| `IOP/MODHSYN.IRX` | IRX module (IOP) | 36,997 | 1101345 | A02896AF | ADFC4977A9A6C03561C6F8820B09FDDF | 9FE4ECB91461F9D8DC47E284DB8FBE332A68235A |
| `IOP/MODMIDI.IRX` | IRX module (IOP) | 19,557 | 1101364 | 11335F02 | 274108838AF533BCA44D533F15427CB5 | 01FADA2969F04C9478C746F88625D60574FFD817 |
| `IOP/PADMAN.IRX` | IRX module (IOP) | 42,613 | 1101374 | 03FD27F8 | 348D891962042B86B8FB378801C64AFA | F119BB9045F1194874C9027448C7D7FDD1FA7D22 |
| `IOP/SDRDRV.IRX` | IRX module (IOP) | 6,865 | 1101395 | A616C4EB | F3DF10C814396A6224F06BADC6F56D0B | 8B6DD4ECDBAE68802088F9F95F71F9FC690524CF |
| `IOP/SIO2MAN.IRX` | IRX module (IOP) | 8,317 | 1101399 | AD12FC85 | 802F93CDF20D5869ECAB43C43D84ED61 | 972FB46F9042C7E9F392CFE4C5E9574528F19930 |
| `ERDATA.BIN` | data archive | 432,832,512 | 1101404 | 8F3DC155 | EAF52DEC5CE6D78BF3CEF56148F5FB4A | 7910AC973D00F82947A8146495CF9352D2A004C0 |
| `VOICE/VOICEALL.ADX` | data archive (audio) | 467,734,528 | 1312748 | 97166425 | 4C2DFD648F362B689EFC92174F738C1D | 3712F98AC2441B776758074B10FE0C002D0BF670 |
| `AGETEC.PSS` | PSS movie | 6,766,596 | 1541134 | 78CEA46E | B92E90F8FFD42A99D884589278C1E6A9 | 5654EDAFB5B3D5E63A5D017279A80293BB981F4A |
| `EROPEN.PSS` | PSS movie | 60,620,804 | 1544439 | 46ECF8C7 | 0F6D8284345BDD411ED1F55453BDA499 | D560DDA39EFDE2661F721C04982FDA9A7D6B0F75 |
| `ERDEMO.PSS` | PSS movie | 55,721,988 | 1574040 | B33EAD56 | ECF0E84B0F2EA3A91B3C2AACDBBDBD2D | AC5B883D6937BC8C1084A21279A2953DBAA8EF86 |
| `TEXT0.PSS` | PSS movie | 110,051,332 | 1601249 | BEAE9608 | A8C252390A108AD1EF5ECB3FC30DCBEF | 23AB8A891488C3BE8C4C4D47F179D37A1AE55E89 |
| `TEXT1.PSS` | PSS movie | 88,752,132 | 1654986 | EB1B7615 | D5E23472FD25CE37DC9E1FCB2B5D34B7 | 0539525210677A1FA8B7D409DD038754C0B439D8 |
| `STAFF.PSS` | PSS movie | 85,770,244 | 1698323 | E402002D | C51F841728EF5EC83C919DE1084D0466 | 4B3249E4348237FD1005665AEC179976E209DA92 |
| `DUMMY.OUT` | padding | 1,073,741,824 | 1740204 | FD52A248 | ED3DD425C31EA00CD8DC8761170F7FA3 | 9CE5AD7D402B69B1CE7ED070FDCD9833CD82662B |

### Inside IOPRP165.IMG

Entry table parsed (16 bytes per entry: 10-byte name, u16 extinfo size, u32 size; Unconfirmed format reading, but the offsets agree with the four ELF headers found by a magic scan): RESET (0), ROMDIR (128), EXTINFO (212), SIFCMD (9,441 bytes, offset 352), FILEIO (9,245, offset 9,808), CDVDMAN (44,741, offset 19,056), CDVDFSV (35,093, offset 63,808). Not extracted or hashed individually.

### Notes on each file (all readings Unconfirmed)

- `SYSTEM.CNF`: Boot config (read in full below).
- `SLUS_200.15`: Main EE executable named by BOOT2. ELF header: 32-bit little-endian MIPS EXEC, entry 0x00100008. Unconfirmed: the game engine and SDK libraries are linked into it.
- `IOPRP165.IMG`: ROMDIR-format image (header table: RESET, ROMDIR, EXTINFO, SIFCMD, FILEIO, CDVDMAN, CDVDFSV). Unconfirmed: the name suggests an IOP reset image built for IOP library revision 1.65; the layout reading is from the entry table, not a spec.
- `IOP/FSIOPSND.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/LIBSD.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/MCMAN.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/MCSERV.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/MODHSYN.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/MODMIDI.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/PADMAN.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/SDRDRV.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `IOP/SIO2MAN.IRX`: Unconfirmed: IOP driver module, ELF header present; role guessed from the name only.
- `ERDATA.BIN`: Unconfirmed: main game data archive (432,832,512 bytes); format not examined (starts with zero bytes).
- `VOICE/VOICEALL.ADX`: Unconfirmed: file name and "SShd" magic suggest CRI ADX-style audio; probably all voice lines in one file. Format not examined.
- `AGETEC.PSS`: Unconfirmed: PS2 program-stream movie (starts with MPEG pack header 00 00 01 BA).
- `EROPEN.PSS`: Unconfirmed: PS2 program-stream movie (starts with MPEG pack header 00 00 01 BA).
- `ERDEMO.PSS`: Unconfirmed: PS2 program-stream movie (starts with MPEG pack header 00 00 01 BA).
- `TEXT0.PSS`: Unconfirmed: PS2 program-stream movie (starts with MPEG pack header 00 00 01 BA).
- `TEXT1.PSS`: Unconfirmed: PS2 program-stream movie (starts with MPEG pack header 00 00 01 BA).
- `STAFF.PSS`: Unconfirmed: PS2 program-stream movie (starts with MPEG pack header 00 00 01 BA).
- `DUMMY.OUT`: Unconfirmed: 1 GiB filler; begins with the text "(C) 2000 From..." (a copyright string), 32 distinct byte values in the first MiB; likely layout padding.
- `IOP/`: nine IOP modules (sound, memory card, pad, SIO2 drivers by name only).
