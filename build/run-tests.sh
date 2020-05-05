#!/bin/bash

echo "asd"

set -e

# go to the dir of this script
cd "$(dirname "$0")"

cd ..
make

cd build/release
./netspeak4-test