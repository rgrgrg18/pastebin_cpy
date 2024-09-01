#!/bin/bash

# Source the configuration file to get the container name
source ../image/config.sh

# Stop the Docker container
docker stop "$CONTAINER_NAME"