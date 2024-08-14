#!/bin/bash

# Source the configuration file to get the container name and user
source ../image/config.sh

USER="root"

# Connect to the Docker container
docker exec -it --user "$USER" "$CONTAINER_NAME" /bin/bash