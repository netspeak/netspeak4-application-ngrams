#!/bin/bash

set -e

if (( $EUID != 0 )); then
    echo "Please run as root"
    exit
fi

# go to the dir of this script
cd "$(dirname "$0")"

apt-get install make cmake cmake-data pkg-config qt4-default -y
apt-get install build-essential libboost-filesystem-dev libboost-system-dev libboost-test-dev libboost-program-options-dev libcmph-dev libaio-dev libprotobuf-dev protobuf-compiler libboost-regex-dev -y


if [ ! -d ../antlr4 ]; then

    echo "Installing Antlr 4.7.1"

    echo "Clean up"

    # go to the dir of this script
    cd ..
    rm -rf antlr4
    mkdir antlr4
    cd antlr4

    echo "Download release"

    apt-get install unzip wget -y
    wget -O antlr-4.7.1-complete.jar 'http://www.antlr.org/download/antlr-4.7.1-complete.jar'
    wget -O antlr-4.7.1.zip 'https://github.com/antlr/antlr4/archive/4.7.1.zip'
    unzip -q antlr-4.7.1.zip


    echo "Install and build Antlr4"


    # install Antlr4 dependency
    apt-get install uuid-dev cmake make -y

    # clean up an prepare build
    cd antlr4-4.7.1/runtime/Cpp
    rm -rf dist
    rm -rf build && mkdir build
    rm -rf run && mkdir run

    # build
    cd build
    cmake .. -DANTLR_JAR_LOCATION=../../../../antlr4.7.1-complete.jar
    make
    DESTDIR=../run make install
    cd ..

    echo "Copy Antlr4 files"

    cp -r run/usr/local/include/antlr4-runtime /usr/include
    cp -r run/usr/local/lib/* /usr/lib/

else

    echo "Antlr 4.7.1 is already installed.
Remove the 'antlr4' directory and run this script again to reinstall it."

fi

echo "Done installing Antlr 4.7.1"
