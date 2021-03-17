set(MODULE_NAME test_module1)

include_directories(src)

message(STATUS "test_module1 CMakeLists.txt is run")

add_subdirectory(src)
add_subdirectory(tst)

