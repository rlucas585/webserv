# Add new executable targets here to serve as examples

set(BINARY Select)

add_executable(${BINARY}.run ${BINARY}.cpp)

target_link_libraries(${BINARY}.run
    PUBLIC Slice_lib
    PUBLIC Net_lib
    PUBLIC ASyncServer_lib
	)

set(BINARY BufReader)

add_executable(${BINARY}.run ${BINARY}.cpp)

target_link_libraries(${BINARY}.run
    PUBLIC Sys_lib
	)

set(BINARY HttpRequest)

add_executable(${BINARY}.run ${BINARY}.cpp)

target_link_libraries(${BINARY}.run
  PUBLIC Net_lib
  PUBLIC HTTP_lib
	)

set(BINARY ClientParsing)

add_executable(${BINARY}.run ${BINARY}.cpp)

target_link_libraries(${BINARY}.run
  PUBLIC ASyncServer_lib
	)
