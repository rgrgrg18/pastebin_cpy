#!/bin/bash

apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    lsb-release \
    libssl-dev \
    libcurl4-openssl-dev \
    zlib1g-dev \
    libpqxx-dev \
    libboost-all-dev \
    pkg-config \
    libprotobuf-dev \
    protobuf-compiler \
    libgtest-dev \
    libhiredis-dev \
    redis-server \
    openssh-server \
    vim \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install redis-plus-plus
git clone https://github.com/sewenew/redis-plus-plus.git \
    && cd redis-plus-plus \
    && mkdir -p build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
    && make \
    && make install \
    && cd ../../ \
    && rm -rf redis-plus-plus

# Install AWS SDK for C++
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git \
    && cd aws-sdk-cpp \
    && mkdir build \
    && cd build \
    && cmake .. -DBUILD_ONLY="s3;sts;core" -DCMAKE_BUILD_TYPE=Release -DENABLE_UNITY_BUILD=ON \
    && make \
    && make install \
    && cd ../../ \
    && rm -rf aws-sdk-cpp

# Install gRPC
git clone -b v1.48.0 https://github.com/grpc/grpc \
    && cd grpc \
    && git submodule update --init \
    && mkdir -p cmake/build \
    && cd cmake/build \
    && cmake ../.. -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release \
    && make \
    && make install \
    && cd ../../../ \
    && rm -rf grpc

# Install Google Test (gtest)
git clone https://github.com/google/googletest.git \
    && cd googletest \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && make install \
    && cd ../../ \
    && rm -rf googletest

# Generate proto file for grpc
cd src/server/proto &&  \
    protoc -I ../proto --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../proto/server.proto && \
    protoc -I ../proto --cpp_out=. ../proto/server.proto