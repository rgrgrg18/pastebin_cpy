#!/bin/bash

# Source the configuration file to get the container name
source ../image/config.sh

# Start the Docker container
docker start "$CONTAINER_NAME"