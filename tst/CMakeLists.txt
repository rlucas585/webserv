set(BINARY HttpParsing)

add_executable(${BINARY}.test ${BINARY}.cpp)

add_test(NAME ${BINARY}.test COMMAND ${BINARY}.test)

target_include_directories(${BINARY}.test
  PUBLIC ../src/HTTP/src
  PUBLIC ../src/ASyncServer/src
  )

target_link_libraries(${BINARY}.test
  PUBLIC gtest
  PUBLIC ASyncServer_lib
  PUBLIC HTTP_lib
  )
