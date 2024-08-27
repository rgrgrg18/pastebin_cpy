#!/bin/bash

build_app() {
	echo "Build image of the app..."
	docker-compose build --force-rm --no-cache -f ../../docker-compose.yaml
}

run_app() {
	echo "Staring Docker containers..."
	docker-compose up --detach
}

build_app
run_app