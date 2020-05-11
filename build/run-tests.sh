#!/bin/bash

set -e

# go to the dir of this script
cd "$(dirname "$0")"

echo "Rebuilding..."
cd ..
make

echo "Running test..."
cd build/release
./netspeak4-test
