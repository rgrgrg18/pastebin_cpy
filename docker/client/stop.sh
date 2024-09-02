#!/bin/bash

CONFIG_FILE="../image/config.sh"

if [ ! -f "$CONFIG_FILE" ]; then
    echo "Configuration file not found!"
    exit 1
fi

source "$CONFIG_FILE"

if [ -z "$CONTAINER_NAME1" ] || [ -z "$CONTAINER_NAME2" ] || [ -z "$CONTAINER_NAME3" ]; 
then
    echo "Container names not set in $CONFIG_FILE"
    exit 1
fi

service=$1

if 		[ "$service" == "postgres" ] || 
    	[ "$service" == "redis" ] || 
   		[ "$service" == "pastebin" ]; 
then
# start specific service
	docker-compose stop $service
elif [ $# -eq 0 ] 
then
# start all services in docker-compose
	docker-compose stop
else
	echo "Error: Write nothing or one of the following services: $CONTAINER_NAME1, $CONTAINER_NAME2, $CONTAINER_NAME3 after script."
fi