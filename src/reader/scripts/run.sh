#!/bin/bash

source config.sh

./test.sh

if [ $? -ne 0 ]; 
then
    echo "Test script failed. Exiting."
    exit 1
fi

cd "$APP_DIR"
./reader_server
