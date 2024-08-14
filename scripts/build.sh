#!/bin/bash

BUILD_DIR="../build"

if [ ! -d "$BUILD_DIR" ]; then
    cd ..
    cmake . -B build
    cd build
    make
else
    echo "Directory $BUILD_DIR already exists. Skipping build."
fi