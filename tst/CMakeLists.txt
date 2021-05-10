include_directories(../src/Utils)
include_directories(../src/Slice)
include_directories(../src/Sys)
include_directories(../src/Net)
include_directories(../src/Option)
include_directories(../src/Result)
include_directories(../src/Traits)
include_directories(../src/ASyncServer)
include_directories(../src/HTTP)

set(BINARY HttpParsing)

add_executable(${BINARY}.test ${BINARY}.cpp)

add_test(NAME ${BINARY}.test COMMAND ${BINARY}.test)

target_link_libraries(${BINARY}.test
  PUBLIC gtest
  PUBLIC ASyncServer_lib
  PUBLIC HTTP_lib
  )
