set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES main.cpp)

add_subdirectory(Utils)
add_subdirectory(Str)
add_subdirectory(FileDesc)
# add_subdirectory(Socket)

include_directories(Utils)
include_directories(Str)
include_directories(FileDesc)
# include_directories(Socket)

add_executable(${BINARY}.run ${SOURCES})

target_link_libraries(${BINARY}.run
    PUBLIC Str_lib
	)
