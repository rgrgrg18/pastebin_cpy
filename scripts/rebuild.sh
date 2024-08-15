#!/bin/bash

cd ../src/server/proto
rm -rf server.grpc.pb.cc server.grpc.pb.h server.pb.cc server.pb.h
protoc -I ../proto --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../proto/server.proto
protoc -I ../proto --cpp_out=. ../proto/server.proto
cd ../../../scripts/

./destroy.sh
./build.sh
