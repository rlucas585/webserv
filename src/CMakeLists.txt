set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES main.cpp)

add_subdirectory(Utils)
add_subdirectory(Str)
add_subdirectory(FileDesc)
add_subdirectory(Socket)
add_subdirectory(Option)
add_subdirectory(Result)
add_subdirectory(Traits)
add_subdirectory(AlignedStorage)

include_directories(Utils)
include_directories(Str)
include_directories(FileDesc)
include_directories(Socket)
include_directories(Option)
include_directories(Result)
include_directories(Traits)
include_directories(AlignedStorage)

add_executable(${BINARY}.run ${SOURCES})

target_link_libraries(${BINARY}.run
    PUBLIC Str_lib
	)
