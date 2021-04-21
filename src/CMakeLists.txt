set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES main.cpp)

add_subdirectory(Utils)
add_subdirectory(Str)
add_subdirectory(Sys)
add_subdirectory(Net)
add_subdirectory(Option)
add_subdirectory(Result)
add_subdirectory(Traits)

# Only for testing
add_subdirectory(Examples)

include_directories(Utils)
include_directories(Str)
include_directories(Sys)
include_directories(Net)
include_directories(Option)
include_directories(Result)
include_directories(Traits)

add_executable(${BINARY}.run ${SOURCES})

target_link_libraries(${BINARY}.run
    PUBLIC Str_lib
	)
