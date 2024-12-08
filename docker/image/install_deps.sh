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
    unzip \
    clang-tidy \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Download the AWS CLI v2 installation package
curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"

# For ARM64 architecture, use this link:
# curl "https://awscli.amazonaws.com/awscli-exe-linux-aarch64.zip" -o "awscliv2.zip"

# Install the AWS CLI
unzip awscliv2.zip && ./aws/install && rm -rf awscliv2.zip aws

# Install redis-plus-plus
git clone https://github.com/sewenew/redis-plus-plus.git \
    && cd redis-plus-plus \
    && mkdir -p build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
    && make -j4 \
    && make install \
    && cd ../../ \
    && rm -rf redis-plus-plus

# Install AWS SDK for C++
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git \
    && cd aws-sdk-cpp \
    && mkdir build \
    && cd build \
    && cmake .. -DBUILD_ONLY="s3;sts;core" -DCMAKE_BUILD_TYPE=Release -DENABLE_UNITY_BUILD=ON \
    && make -j4 \
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
    && make -j4 \
    && make install \
    && cd ../../../ \
    && rm -rf grpc

# Install Google Test (gtest)
git clone https://github.com/google/googletest.git \
    && cd googletest \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make -j4 \
    && make install \
    && cd ../../ \
    && rm -rf googletest

git clone https://github.com/google/benchmark.git \
    && cd benchmark \
    && cmake -E make_directory "build" \
    && cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../ \
    && cmake --build "build" --config Release -j4 \
    && cd build \
    && make install \
    && cd ../.. \
    && rm -rf benchmark


ln -s /usr/src/app/codestyle_checker/src/runner.py /usr/local/bin/style

