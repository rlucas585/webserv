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

set(BINARY SetUp)

add_executable(${BINARY}.test ${BINARY}.cpp)

add_test(NAME ${BINARY}.test COMMAND ${BINARY}.test)

target_include_directories(${BINARY}.test
  PUBLIC ../src/HTTP/src
  PUBLIC ../src/ASyncServer/src
  PUBLIC ../src/Config/src
  PUBLIC ../src/VirtualServer/src
  )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.txt
  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

target_link_libraries(${BINARY}.test
  PUBLIC gtest
  PUBLIC ASyncServer_lib
  PUBLIC HTTP_lib
  PUBLIC Config_lib
  )
