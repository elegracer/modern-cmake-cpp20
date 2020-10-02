#!/bin/bash

PROJ_DIR=$(
    cd "$(dirname "$0")"
    pwd
)
echo $PROJ_DIR

export CC=$(which clang)
export CXX=$(which clang++)

echo "Warning: 1. Run this script only when you are in ubuntu."
echo "         2. Ensure CC and CXX are set correctly."
echo "    CC =$CC"
echo "    CXX=$CXX"

cmake -S . -B build/archlinux \
    -D CMAKE_BUILD_TYPE=Release \
    -G Ninja

[ $? -eq 0 ] && cmake --build build/archlinux --config Release
