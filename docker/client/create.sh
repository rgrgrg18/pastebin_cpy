#!/bin/bash

pull_images() {
	echo "Pulling images.."
	docker-compose pull
}

build_pastebin() {
	echo "Build image of the app..."
	docker-compose -f ../../docker-compose.yaml	build --force-rm --no-cache pastebin
}

pull_images
build_pastebin