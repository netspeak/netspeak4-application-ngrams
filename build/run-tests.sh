#!/bin/bash

set -e

# go to the dir of this script
cd "$(dirname "$0")"

echo "Rebuilding..."
cd ..
make


# extract test files
if [ ! -d ./test/data ]; then

    if ! [ -x "$(command -v unzip)" ]; then
        echo "Installing unzip..."
        if (( $EUID != 0 )); then
            sudo apt-get install -y unzip
        else
            apt-get install -y unzip
        fi
    fi

    echo "Extracting test data"
    unzip -q test/data.zip -d test/

fi


echo "Running test..."
./netspeak4-test --build_info=yes --detect_memory_leak=1 --log_level=test_suite
