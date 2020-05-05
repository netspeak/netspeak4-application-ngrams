#!/bin/bash

set -e

# go to the dir of this script
cd "$(dirname "$0")"

# check protoc version
protocVersion=$(protoc --version)
if [ "$protocVersion" != "libprotoc 2.6.1" ]; then
    echo "protoc v2.6.1 is required! Your version is $protocVersion";
    # Netspeak might still work even with the wrong version.
    # exit 1;
fi

cd ../conf
mkdir -p ../src/netspeak/generated
rm -rf ../src/netspeak/generated/*
protoc --cpp_out=../src/netspeak/generated ./NetspeakMessages.proto
