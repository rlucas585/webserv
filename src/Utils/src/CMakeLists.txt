file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${MODULE_NAME}_lib STATIC ${SOURCES})

target_include_directories(${MODULE_NAME}_lib
    PUBLIC ../../Option/src/
    PUBLIC ../../Result/src/
    PUBLIC ../../Slice/src/
    )

target_link_libraries(${MODULE_NAME}_lib
  PUBLIC Slice_lib
  )
