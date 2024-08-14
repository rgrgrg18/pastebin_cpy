#!/bin/bash

CONFIG_FILE="../image/config.sh"

if [ ! -f "$CONFIG_FILE" ]; then
    echo "Configuration file not found!"
    exit 1
fi

source "$CONFIG_FILE"

if [ -z "$CONTAINER_NAME" ]; then
    echo "Container name not set in $CONFIG_FILE"
    exit 1
fi

IMAGE_NAME="pastebin_cpy_image"
CONTAINER="$CONTAINER_NAME"

# Build the Docker image
build_image() {
    echo "Building Docker image $IMAGE_NAME..."
    docker build -t "$IMAGE_NAME" -f ../image/Dockerfile ../../
}

# Run the Docker container
run_container() {
    echo "Starting Docker container $CONTAINER from image $IMAGE_NAME..."
    docker run -p 9999:9999 -p 2222:22 -d --name "$CONTAINER" "$IMAGE_NAME"
}

build_image
run_container
