#!/bin/bash

BUILD_DIR="../build"

if [ -d "$BUILD_DIR" ]; then
    cd ..
    rm -rf build
else
    echo "Directory $BUILD_DIR does not exist. Skipping destroy."
fi
