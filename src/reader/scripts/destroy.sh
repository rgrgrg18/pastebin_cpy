#!/bin/bash

source config.sh

if [ -d "$BUILD_DIR" ]; 
then
    cd ..
    rm -rf build
else
    echo "Directory $BUILD_DIR does not exist. Skipping destroy."
fi
