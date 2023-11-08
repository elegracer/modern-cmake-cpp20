#!/bin/bash

PROJ_DIR=$(
    cd "$(dirname "$0")"
    pwd
)
echo $PROJ_DIR

export CC=$(which clang)
export CXX=$(which clang++)

echo "Warning: 1. Ensure CC and CXX are set correctly."
echo "    CC =$CC"
echo "    CXX=$CXX"

cmake -S . -B build/clang \
    -D CMAKE_BUILD_TYPE=Release \
    -G Ninja

[ $? -eq 0 ] && cmake --build build/clang --config Release
