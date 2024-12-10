#!/bin/bash

source config.sh

if [ ! -d "$BUILD_DIR" ]; then
    ./build.sh
else
    echo "Directory $BUILD_DIR already exists. Skipping build."
fi

cd "$BUILD_DIR"

if [ "$1" = "reader" ];
then
    if [ "$2" = "ut" ] || [ $# -eq 1 ];
    then
        echo "Run reader unit tests..."
        cd "$UT_DIR"
        ./reader_ut
    elif [ "$2" = "bm" ];
    then
        echo "Run reader benchmark..."
        cd "$BM_DIR"
        ./reader_benchmark
    else 
        echo "Error: Write nothing or ut/bm."
    fi
elif [ "$1" = "server" ];
then
    if [ "$2" = "ut" ] || [ $# -eq 1 ];
    then
        echo "Run gRPC reader unit tests..."
        cd "$UT_DIR"
        ./server_ut
    else 
        echo "Error: Write nothing or ut."
    fi
elif [ $# -eq 0 ];
then
    echo "Run all tests..."
    cd "$BUILD_DIR"
    ctest
else 
    echo "Error: Write nothing or reader."
fi

if [ $? -ne 0 ]; 
then
    exit 1
fi