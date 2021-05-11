set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES main.cpp)

add_subdirectory(Utils)
add_subdirectory(Slice)
add_subdirectory(Sys)
add_subdirectory(Net)
add_subdirectory(Option)
add_subdirectory(Result)
add_subdirectory(Traits)
add_subdirectory(ASyncServer)
add_subdirectory(HTTP)
add_subdirectory(Config)
add_subdirectory(VirtualServer)

# Only for testing
add_subdirectory(Examples)

include_directories(Utils)
include_directories(Slice)
include_directories(Sys)
include_directories(Net)
include_directories(Option)
include_directories(Result)
include_directories(Traits)
include_directories(ASyncServer)
include_directories(HTTP)
include_directories(Config)
include_directories(VirtualServer)

add_executable(${BINARY}.run ${SOURCES})

target_link_libraries(${BINARY}.run
    PUBLIC Slice_lib
	)
