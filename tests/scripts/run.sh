#!/bin/bash

# Check if an argument is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <application_type>"
    echo "acceptance - Start Acceptance Testing"
    echo "load - Start Load Testing"
    exit 1
fi

# Get the application type from the argument
APP_TYPE=$1

case $APP_TYPE in
    acceptance)
        echo "Starting Acceptance Testing..."
        cd acceptance_testing
        python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. server.proto

        python test.py
        ;;
    load)
        echo "Starting Load Testing..."
        cd load_testing
        python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. server.proto

        python test.py
        ;;
    *)
        echo "Invalid application type. Use 'acceptance' or 'load'."
        exit 1
        ;;
esac
