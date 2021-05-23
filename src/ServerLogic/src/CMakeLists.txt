file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${MODULE_NAME}_lib STATIC ${SOURCES})

target_include_directories(${MODULE_NAME}_lib
  PUBLIC ../../Result/src/
  PUBLIC ../../Utils/src
  PUBLIC ../../Slice/src/
  PUBLIC ../../Sys/src/
  PUBLIC ../../HTTP/src/
  PUBLIC ../../ASyncServer/src/
  PUBLIC ../../VirtualServer/src
  )

target_link_libraries(${MODULE_NAME}_lib
    PUBLIC Slice_lib
    PUBLIC Utils_lib
    PUBLIC Sys_lib
    PUBLIC HTTP_lib
    PUBLIC ASyncServer_lib
    PUBLIC VirtualServer_lib
    )
