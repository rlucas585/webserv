set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES main.cpp)

add_subdirectory(Utils)
add_subdirectory(Str)
add_subdirectory(test_module1)

include_directories(Utils)
include_directories(Str)
include_directories(test_module1/src)

add_executable(${BINARY}.run ${SOURCES})

target_link_libraries(${BINARY}.run
	PUBLIC test_module1_lib
	)
