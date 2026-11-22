#!/bin/bash
# ER-011: build the EE binutils (gas, ld, objdump, objcopy, ...) 2.9-ee-991111 from the same GPL source as
# ER-002's compiler (SSXModding/ps2-ee-toolchain commit b595ded), natively on Windows under MSYS2,
# host x86_64-pc-msys (LP64), target "ee" = mips64r5900-sky-elf.
#
# Needs the LF clone /c/rs/toolchain/src-lf with ER-002's config.sub patch (build_ee_gcc.sh step 1).
# ER-026: first applies our gas patches from patches/ beside this script, once (docs/toolchain.md 4.2, 4.4):
#   ee-as_r5900_fpcc_reorder.patch    no c.cond / bc1x hazard nop in reorder mode (ER-014; ruling 3.4)
#   ee-as_r5900_copmove_reorder.patch no mtc1 -> use hazard nop in reorder mode (ER-035; ruling 3.5)
#   ee-as_r5900_mfc1_reorder.patch    no mfc1 -> GPR use load-delay nop in reorder mode (ER-034; ruling 3.8)
#   ee-as_r5900_delay_swap.patch      reorder-mode delay-slot swap works (sync test typo), never a div.s / sqrt.s
#                                     (ER-070; ruling 3.12)
# Run:  MSYSTEM=MSYS C:\msys64\usr\bin\bash.exe -lc "'<repo>/tools/toolchain/build_ee_binutils.sh'"
# Output: /c/rs/toolchain/ee-binutils-2.9-991111-r4/ee-{as,ld,objdump,objcopy,...}.exe (+ msys-2.0.dll)
#   (r4 = with all four patches, ER-070; r3 = ER-034's build with the first three, r2 = ER-035's with the first two,
#   r1 = ER-026's with the first only and the unpatched ER-011 build in ee-binutils-2.9-991111 stay as fallbacks)
set -e
export PATH="/usr/bin:$PATH"
unset CC CXX

HERE=$(cd "$(dirname "$0")" && pwd)
PATCHES="$HERE/patches"
ROOT=/c/rs/toolchain
SRC=$ROOT/src-lf/ee
BUILD=$ROOT/b-binutils-r4
OUT=$ROOT/ee-binutils-2.9-991111-r4
LOG=$ROOT/build_ee_binutils-r4.log

CFLAGS="-O0 -g0 -std=gnu89 -w -fcommon -Wno-int-conversion -Wno-implicit-function-declaration -Wno-implicit-int -Wno-return-mismatch -Wno-incompatible-pointer-types"
export CFLAGS

grep -q 'ER-002 x86_64' "$SRC/config.sub"

# ER-026: apply each patch once (a reverse dry run that succeeds means it is already in the tree).
# ER-034: only the gas patches (ee-as_*); the compiler's (ee-gcc_*) are build_ee_gcc.sh's.
for p in "$PATCHES"/ee-as_*.patch; do
    if patch -p1 -d "$SRC" -R --dry-run -s -f < "$p" > /dev/null 2>&1; then
        echo "already applied: $(basename "$p")"
    else
        patch -p1 -d "$SRC" -N -s < "$p"
        echo "applied: $(basename "$p")"
    fi
done
grep -q 'Ringsmith ER-014/ER-026' "$SRC/gas/config/tc-mips.c"
grep -q 'Ringsmith ER-035' "$SRC/gas/config/tc-mips.c"
grep -q 'Ringsmith ER-034' "$SRC/gas/config/tc-mips.c"
grep -q 'Ringsmith ER-070' "$SRC/gas/config/tc-mips.c"

rm -rf "$BUILD"; mkdir -p "$BUILD"; cd "$BUILD"
"$SRC/configure" --target=ee --host=x86_64-pc-msys --build=x86_64-pc-msys \
    --prefix=$ROOT/ee-binutils-prefix --disable-nls > "$LOG" 2>&1
make -j4 CFLAGS="$CFLAGS" all-gas all-ld all-binutils >> "$LOG" 2>&1

mkdir -p "$OUT"
cp gas/as-new.exe "$OUT/ee-as.exe"
cp ld/ld-new.exe "$OUT/ee-ld.exe"
for p in objdump objcopy nm-new size strings readelf ar ranlib; do
    if [ -f binutils/$p.exe ]; then cp binutils/$p.exe "$OUT/ee-${p%-new}.exe"; fi
done
cp /usr/bin/msys-2.0.dll "$OUT/"
echo "Done: $OUT"
