#!/bin/bash

set -e

# go to the dir of this script
cd "$(dirname "$0")"

# check protoc version
protocVersion=$(protoc --version)
if [[ ! $protocVersion = "libprotoc 3."* ]]; then
    echo "protoc v3.x.x is required! Your version is $protocVersion";
    exit 1;
fi

cd ../conf

mkdir -p ../src/netspeak/service
rm -rf ../src/netspeak/service/*

protoc --cpp_out=../src/netspeak/service ./NetspeakService.proto
protoc --grpc_out=../src/netspeak/service --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin) ./NetspeakService.proto
