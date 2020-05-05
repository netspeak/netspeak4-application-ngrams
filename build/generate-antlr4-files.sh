#!/bin/bash

# go to the dir of this script
cd "$(dirname "$0")"

ANTLR4_JAR_PATH="$( pwd )/../antlr4/antlr-4.7.1-complete.jar"
CPP_OUT=./cpp-out
ANTLR4_DIR=../../src/antlr4/generated

cd ../conf/grammar/

# generate C++ files in CPP_OUT
rm -rf $CPP_OUT
java -Xmx500M -cp "$ANTLR4_JAR_PATH" org.antlr.v4.Tool -Dlanguage=Cpp Query.g4 -o $CPP_OUT -package "antlr4" -visitor

# copy C++ files
mkdir -p $ANTLR4_DIR
cp $CPP_OUT/*.h $ANTLR4_DIR
cp $CPP_OUT/*.cpp $ANTLR4_DIR
