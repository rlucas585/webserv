file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${MODULE_NAME}.test ${TEST_SOURCES})

add_test(NAME ${MODULE_NAME}.test COMMAND ${MODULE_NAME}.test)

target_link_libraries(${MODULE_NAME}.test
	PUBLIC gtest
	PUBLIC ${MODULE_NAME}_lib
	)
