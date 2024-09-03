#!/bin/bash

CONFIG_FILE="../image/config.sh"

if [ ! -f "$CONFIG_FILE" ]; then
    echo "Configuration file not found!"
    exit 1
fi

source "$CONFIG_FILE"

if [ -z "$PROFILE_NAME1" ] || [ -z "$PROFILE_NAME2" ]; 
then
    echo "Profile names not set in $CONFIG_FILE"
    exit 1
fi

profile=$1

run_app() {
	echo "Staring Docker containers..."
	docker-compose --profile release up --detach
}

if [ "$profile" == "no-logs" ];
then 
	docker-compose --profile no-logs up --detach
elif [ "$profile" == "release" ] || [ $# -eq 0 ];
then 
	docker-compose --profile release up --detach
elif [ "$profile" == "postgres" ]
then
	docker-compose --profile postgres up --detach
elif [ "$profile" == "redis" ]
then
	docker-compose --profile redis up --detach
elif [ "$profile" == "app" ]
then
	docker-compose --profile app up --detach
else
	echo "Error: Write nothing or one of the following services: $PROFILE_NAME1, $PROFILE_NAME2, app, $CONTAINER_NAME2, $CONTAINER_NAME3 after script."
fi
