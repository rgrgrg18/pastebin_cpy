#!/bin/bash

BUILD_DIR="../build"

if [ ! -d "$BUILD_DIR" ]; then
    ./build.sh
else
    echo "Directory $BUILD_DIR already exists. Skipping build."
fi

cd "$BUILD_DIR"
ctest
if [ $? -ne 0 ]; then
    echo "Test script failed. Exiting."
    exit 1
fi