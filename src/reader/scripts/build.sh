#!/bin/bash

source config.sh

if [ ! -d "$BUILD_DIR" ]; 
then
    cd ..
    cmake . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cd build
    make -j4
else
    echo "Directory $BUILD_DIR already exists. Skipping build."
fi