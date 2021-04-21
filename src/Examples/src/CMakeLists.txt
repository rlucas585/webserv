# Add new executable targets here to serve as examples

add_executable(Select.run Select.cpp)

target_link_libraries(Select.run
    PUBLIC Str_lib
    PUBLIC Net_lib
	)
