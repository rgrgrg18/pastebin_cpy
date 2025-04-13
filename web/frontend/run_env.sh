#!/bin/bash

# Check if virtual environment exists
if [ ! -d "env" ]; then
    echo "Creating virtual environment..."
    python3 -m venv env
fi

# Activate virtual environment
echo "Activating virtual environment..."
source env/bin/activate

# Install requirements
pip install --upgrade pip
echo "Installing requirements..."
pip3 install -r requirements.txt

echo "Setup complete!"