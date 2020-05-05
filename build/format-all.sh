#!/bin/bash

cd "$(dirname "$0")"
cd ..

clang-format -i ./src/**/*.[ch]pp
clang-format -i ./test/**/*.[ch]pp
