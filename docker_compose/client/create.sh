#!/bin/bash

pull_images() {
	echo "Pulling images.."
	docker-compose pull
}

build_app() {
	echo "Build image of the app..."
	docker-compose build --force-rm --no-cache -f ../../docker-compose.yaml
}

pull_images
build_app