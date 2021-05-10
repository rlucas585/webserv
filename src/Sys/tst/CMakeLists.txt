file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${TEST_SOURCES})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${MODULE_NAME}.test ${TEST_SOURCES})

add_test(NAME ${MODULE_NAME}.test COMMAND ${MODULE_NAME}.test)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/buf_line_by_line.txt
  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

target_link_libraries(${MODULE_NAME}.test
	PUBLIC gtest
	PUBLIC ${MODULE_NAME}_lib
	)
