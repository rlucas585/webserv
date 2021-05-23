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

add_executable(${BINARY}.test ${BINARY}.cpp
    ../src/WebServ.cpp
    )

add_test(NAME ${BINARY}.test COMMAND ${BINARY}.test)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/error.conf
  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

target_include_directories(${BINARY}.test
  PUBLIC ../src/HTTP/src
  PUBLIC ../src/ASyncServer/src
  PUBLIC ../src/Config/src
  PUBLIC ../src/Net/src
  PUBLIC ../src/ServerLogic/src
  PUBLIC ../src
  )

target_link_libraries(${BINARY}.test
  PUBLIC gtest
  PUBLIC ASyncServer_lib
  PUBLIC HTTP_lib
  PUBLIC Config_lib
  PUBLIC ServerLogic_lib
  PUBLIC VirtualServer_lib
  )
