#!/bin/bash
# Build script for rs

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

cmake .. \
    -DSTREAMPU_ROOT="/net/netud/e/nhitar/Bureau/hulotte/streampu" \
    -DAFF3CT_ROOT="/net/netud/e/nhitar/Bureau/hulotte/aff3ct" \
    -DCMAKE_BUILD_TYPE=Release

make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo "Run: ./build/rs"
else
    echo "Build failed"
    exit 1
fi
