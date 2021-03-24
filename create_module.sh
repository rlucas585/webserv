#!/bin/sh

# Creates a new module using default CMakeLists.txt templates

if [ "$#" -ne 1 ]; then
    echo "Please supply a name for the new module"
    exit
fi
if [ -d "$1" ]; then
    echo "Module '$1' is already present in the current directory"
    exit
fi

MODULE_ROOT_CMAKE_LIST="set(MODULE_NAME $1)\n\n\
include_directories(src)\n\n\
add_subdirectory(src)\n\
add_subdirectory(tst)"

MODULE_SRC_CMAKE_LIST="file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)\n\n\
set(SOURCES \${SOURCES})\n\n\
add_library(\${MODULE_NAME}_lib STATIC \${SOURCES})"

MODULE_TST_CMAKE_LIST="file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.hpp *.cpp)\n\n\
set(SOURCES \${TEST_SOURCES})\n\n\
set(CMAKE_CXX_STANDARD 11)\n\
set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n\
add_executable(\${MODULE_NAME}.test \${TEST_SOURCES})\n\n\
add_test(NAME \${MODULE_NAME}.test COMMAND \${MODULE_NAME}.test)\n\n\
target_link_libraries(\${MODULE_NAME}.test\n\
	PUBLIC gtest\n\
	PUBLIC \${MODULE_NAME}_lib\n\
	)"

TST_MAIN_DEFAULT="#include \"gtest/gtest.h\"\n\n\
int main(int argc, char** argv) {\n\
    ::testing::InitGoogleTest(&argc, argv);\n\
    return RUN_ALL_TESTS();\n\
}"

mkdir $1
mkdir $1/src
mkdir $1/tst
echo $MODULE_ROOT_CMAKE_LIST > $1/CMakeLists.txt
echo $MODULE_SRC_CMAKE_LIST > $1/src/CMakeLists.txt
echo $MODULE_TST_CMAKE_LIST > $1/tst/CMakeLists.txt
echo $TST_MAIN_DEFAULT > $1/tst/main.cpp

