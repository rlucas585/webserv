#!/bin/sh

# if ran with argument, that argument is the cmake build type
mkdir -p ./build && cd build && { [ $# -eq 0 ] && { cmake ..; exit $?; }  } || { cmake -DCMAKE_BUILD_TYPE="$1" ..; exit $?; }
