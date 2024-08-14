#!/bin/bash

# Source the configuration file to get the container name
source ../image/config.sh

# Restart the Docker container
docker restart "$CONTAINER_NAME"