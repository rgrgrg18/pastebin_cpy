#!/bin/bash

source ../image/config.sh

LOCAL_LOAD_TESTING_DIR="../load_testing/"
CONTAINER_LOAD_TESTING_DIR="/app"

LOCAL_ACCEPTANCE_TESTING_DIR="../acceptance_testing/"
CONTAINER_ACCEPTANCE_TESTING_DIR="/app"

LOCAL_SCRIPT_DIR="../scripts/run.sh"
CONTAINER_SCRIPT_DIR="/app"

# Check if the container is running
if [ "$(docker ps -q -f name=$CONTAINER_NAME)" ]; then
    echo "Container $CONTAINER_NAME is running."
else
    echo "Container $CONTAINER_NAME is not running. Please start it first."
    exit 1
fi

echo "Copying $LOCAL_LOAD_TESTING_DIR to $CONTAINER_LOAD_TESTING_DIR in container $CONTAINER_NAME..."
docker cp "$LOCAL_LOAD_TESTING_DIR" "$CONTAINER_NAME:$CONTAINER_LOAD_TESTING_DIR"

echo "Copying $LOCAL_ACCEPTANCE_TESTING_DIR to $CONTAINER_ACCEPTANCE_TESTING_DIR in container $CONTAINER_NAME..."
docker cp "$LOCAL_ACCEPTANCE_TESTING_DIR" "$CONTAINER_NAME:$CONTAINER_ACCEPTANCE_TESTING_DIR"

echo "Copying $LOCAL_SCRIPT_DIR to $CONTAINER_SCRIPT_DIR in container $CONTAINER_NAME..."
docker cp "$LOCAL_SCRIPT_DIR" "$CONTAINER_NAME:$CONTAINER_SCRIPT_DIR"

if [ $? -eq 0 ]; then
    echo "Files successfully copied to $CONTAINER_SRC_DIR in container $CONTAINER_NAME."
else
    echo "Error: Failed to copy files to the container."
    exit 1
fi