#!/bin/bash

set -e

if (( $EUID != 0 )); then
    echo "Please run as root"
    exit 1
fi

# go to the dir of this script
cd "$(dirname "$0")"

apt-get update
apt-get install make cmake cmake-data pkg-config qt4-default -y
apt-get install build-essential libboost-filesystem-dev libboost-system-dev libboost-test-dev libboost-program-options-dev libboost-stacktrace-dev libcmph-dev libaio-dev libprotobuf-dev protobuf-compiler libboost-regex-dev -y


# checkout aitools dependencies
function ensure_git {
    if ! [ -x "$(command -v git)" ]; then
        echo "Installing git..."
        apt-get install git -y
    fi
}

mkdir -p ./dependencies
cd ./dependencies

if [ ! -d ./aitools3-aq-bighashmap-cpp ]; then
    ensure_git
    git clone https://gitlab+deploy-token-8:52JyyPqQEkGg13es2qtb@git.webis.de/aitools/aitools3-aq-bighashmap-cpp.git
else
    echo "aitools3-aq-bighashmap-cpp already present."
fi
if [ ! -d ./aitools3-aq-invertedindex3-cpp ]; then
    ensure_git
    git clone https://gitlab+deploy-token-9:hxvTpSBa-WHExN5RZzey@git.webis.de/aitools/aitools3-aq-invertedindex3-cpp.git
else
    echo "aitools3-aq-invertedindex3-cpp already present."
fi


# download, compile, and install Antlr4
# we detect the presence of Antlr using its lib*.so file
if [ ! -f /usr/lib/libantlr4-runtime.so.4.7.1 ]; then

    echo "Installing Antlr 4.7.1"

    echo "Clean up"

    rm -rf antlr4
    mkdir antlr4
    cd antlr4

    echo "Download release"

    apt-get install unzip wget -y
    wget -O antlr-4.7.1-complete.jar 'http://www.antlr.org/download/antlr-4.7.1-complete.jar'
    wget -O antlr-4.7.1.zip 'https://github.com/antlr/antlr4/archive/4.7.1.zip'
    unzip -q antlr-4.7.1.zip


    echo "Build Antlr4"


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

    echo "Install Antlr4 files"

    cp -r run/usr/local/include/antlr4-runtime /usr/include
    cp -r run/usr/local/lib/* /usr/lib/

    cd ../../.. # antlr4-4.7.1/runtime/Cpp

    cd .. # antlr4

    echo "Done installing Antlr 4.7.1"

else

    echo "Antlr 4.7.1 is already installed.
Remove the 'antlr4' directory and run this script again to reinstall it."

fi

# download, compile, and install gRPC
# we detect the presence of gRPC using its protoc plugin
if ! [ -x "$(command -v grpc_cpp_plugin)" ]; then

    if [ ! -d ./cmake-3.17.0 ]; then

        # the default version of CMake under most distributions is too low

        # This will install a local version of cmake.
        # This does not affect the cmake version installed on your system

        mkdir -p cmake-3.17.0
        cd cmake-3.17.0

        apt-get install -y wget
        wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0-Linux-x86_64.sh
        sh cmake-linux.sh -- --skip-license --prefix=./
        rm cmake-linux.sh

        cd ..

    fi

    # other build tools
    apt-get install -y build-essential autoconf libtool pkg-config

    # clone repo
    ensure_git
    git clone --recurse-submodules -b v1.28.1 https://github.com/grpc/grpc
    cd grpc

    mkdir -p cmake/build
    pushd cmake/build
    ./../../../cmake-3.17.0/bin/cmake -DgRPC_INSTALL=ON \
        -DgRPC_BUILD_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX=/usr \
        ../..
    make -j
    make install
    popd

    cd ..

else

    echo "gRPC is already installed.
Remove the 'grpc' directory and run this script again to reinstall it."

fi
