#!/bin/bash

if [ -z "$CODESTYLE_CONFIG_DIR_PATH" ]; then
    echo "ERROR: CODESTYLE_CONFIG_DIR_PATH is not set."
    exit 1
fi

mkdir -p ~/bin
ln -sf "$CODESTYLE_CONFIG_DIR_PATH/src/runner.py" ~/bin/style

export PATH=$PATH:~/bin


cd ../
style -r