file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Utils)
include_directories(Result)

add_library(${MODULE_NAME}_lib STATIC ${SOURCES})

target_link_libraries(${MODULE_NAME}_lib
    PUBLIC Utils_lib
    )
