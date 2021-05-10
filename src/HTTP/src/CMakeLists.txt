file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${MODULE_NAME}_lib STATIC ${SOURCES})

target_link_libraries(${MODULE_NAME}_lib
<<<<<<< HEAD
  PUBLIC Slice_lib
=======
  PUBLIC Str_lib
>>>>>>> d1b0db0ed98f0fb136b3f096db37f2b9bfc279ae
  )
