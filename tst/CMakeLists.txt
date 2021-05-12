include_directories(../src/Utils)
include_directories(../src/Slice)
include_directories(../src/Sys)
include_directories(../src/Net)
include_directories(../src/Option)
include_directories(../src/Result)
include_directories(../src/Traits)
include_directories(../src/ASyncServer)
include_directories(../src/HTTP)
include_directories(../src/Config)
include_directories(../src/VirtualServer)

set(BINARY HttpParsing)

add_executable(${BINARY}.test ${BINARY}.cpp)

add_test(NAME ${BINARY}.test COMMAND ${BINARY}.test)

target_link_libraries(${BINARY}.test
  PUBLIC gtest
  PUBLIC ASyncServer_lib
  PUBLIC HTTP_lib
  )

set(BINARY SetUp)

add_executable(${BINARY}.test ${BINARY}.cpp)

add_test(NAME ${BINARY}.test COMMAND ${BINARY}.test)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.txt
  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

target_link_libraries(${BINARY}.test
  PUBLIC gtest
  PUBLIC ASyncServer_lib
  PUBLIC HTTP_lib
  PUBLIC Config_lib
  )
