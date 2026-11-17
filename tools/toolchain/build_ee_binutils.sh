#!/bin/bash
# ER-011: build the EE binutils (gas, ld, objdump, objcopy, ...) 2.9-ee-991111 from the same GPL source as
# ER-002's compiler (SSXModding/ps2-ee-toolchain commit b595ded), natively on Windows under MSYS2,
# host x86_64-pc-msys (LP64), target "ee" = mips64r5900-sky-elf.
#
# Needs the LF clone /c/rs/toolchain/src-lf with ER-002's config.sub patch (build_ee_gcc.sh step 1).
# Run:  MSYSTEM=MSYS C:\msys64\usr\bin\bash.exe -lc /c/rs/toolchain/build_ee_binutils.sh
# Output: /c/rs/toolchain/ee-binutils-2.9-991111/ee-{as,ld,objdump,objcopy,...}.exe (+ msys-2.0.dll)
set -e
export PATH="/usr/bin:$PATH"
unset CC CXX

ROOT=/c/rs/toolchain
SRC=$ROOT/src-lf/ee
BUILD=$ROOT/b-binutils
OUT=$ROOT/ee-binutils-2.9-991111
LOG=$ROOT/build_ee_binutils.log

CFLAGS="-O0 -g0 -std=gnu89 -w -fcommon -Wno-int-conversion -Wno-implicit-function-declaration -Wno-implicit-int -Wno-return-mismatch -Wno-incompatible-pointer-types"
export CFLAGS

grep -q 'ER-002 x86_64' "$SRC/config.sub"

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
