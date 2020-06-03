#!/bin/bash

set -e

# go to the dir of this script
cd "$(dirname "$0")"

# check protoc version
protocVersion=$(protoc --version)
if [ "$protocVersion" != "libprotoc 3.0.0" ]; then
    echo "protoc v3.0.0 is required! Your version is $protocVersion";
    exit 1;
fi

cd ../conf
mkdir -p ../src/netspeak/generated
rm -rf ../src/netspeak/generated/*
protoc --cpp_out=../src/netspeak/generated ./Netspeak_old.proto
