#!/bin/sh

find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i
