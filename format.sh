#!/bin/sh

find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i
find tst/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i
