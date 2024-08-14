#!/bin/bash

./test.sh

if [ $? -ne 0 ]; then
    echo "Test script failed. Exiting."
    exit 1
fi

cd ../build/app
./pastebin_server