#!/bin/bash

source "../image/config.sh"

IMAGE_NAME="load_testing_image1"
CONTAINER="$CONTAINER_NAME"

# Build the Docker image
build_image() {
    echo "Building Docker image $IMAGE_NAME..."
    docker build -t "$IMAGE_NAME" -f ../image/Dockerfile ../../
}

# Run the Docker container
run_container() {
    echo "Starting Docker container $CONTAINER from image $IMAGE_NAME..."
    docker run --network="host" -p 2225:22 --name $CONTAINER $IMAGE_NAME
}

build_image
run_container