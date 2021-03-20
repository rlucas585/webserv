set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES main.cpp)

add_subdirectory(Utils)
add_subdirectory(Str)
add_subdirectory(FileDesc)

include_directories(Utils)
include_directories(Str)
include_directories(FileDesc)

add_executable(${BINARY}.run ${SOURCES})

target_link_libraries(${BINARY}.run
    PUBLIC Str_lib
	)
