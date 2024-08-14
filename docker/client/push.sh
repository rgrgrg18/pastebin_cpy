#!/bin/bash

source ../image/config.sh

# Source and destination directories
LOCAL_SRC_DIR="../../src"
CONTAINER_SRC_DIR="/usr/src/app/"
LOCAL_CMAKE_PATH="../../CMakeLists.txt"
CONTAINER_CMAKE_PATH="/usr/src/app/"
LOCAL_SCRIPT_DIR="../../scripts"
CONTAINER_SCRIPT_DIR="/usr/src/app/"

# Check if the container is running
if [ "$(docker ps -q -f name=$CONTAINER_NAME)" ]; then
    echo "Container $CONTAINER_NAME is running."
else
    echo "Container $CONTAINER_NAME is not running. Please start it first."
    exit 1
fi

# Copy the local files to the container
echo "Copying $LOCAL_SRC_DIR to $CONTAINER_SRC_DIR in container $CONTAINER_NAME..."
docker cp "$LOCAL_SRC_DIR" "$CONTAINER_NAME:$CONTAINER_SRC_DIR"
echo "Copying $LOCAL_CMAKE_PATH to $CONTAINER_CMAKE_PATH in container $CONTAINER_NAME..."
docker cp "$LOCAL_CMAKE_PATH" "$CONTAINER_NAME:CONTAINER_CMAKE_PATH"
echo "Copying $LOCAL_SCRIPT_DIR to $CONTAINER_SCRIPT_DIR in container $CONTAINER_NAME..."
docker cp "$LOCAL_SCRIPT_DIR" "$CONTAINER_NAME:CONTAINER_SCRIPT_DIR"

if [ $? -eq 0 ]; then
    echo "Files successfully copied to $CONTAINER_SRC_DIR in container $CONTAINER_NAME."
else
    echo "Error: Failed to copy files to the container."
    exit 1
fi