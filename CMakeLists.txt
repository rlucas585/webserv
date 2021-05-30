cmake_minimum_required(VERSION 3.10.2)

project(WebServ VERSION 1.0)

enable_testing()

# Set Project Scoped definitions

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-noexcept-type")
# -Wno-noexcept-type: Warning is only relevant if project was compiled with another project
# using C++17 compilation
# Link: https://stackoverflow.com/questions/46798456/handling-gccs-noexcept-type-warning

# DOWNLOAD ALL GIT SUBMODULES
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if (GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if (NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init --recursive"
			"failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

# CHECK ALL SUBMODULES
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/external/googletest/CMakeLists.txt")
	message(FATAL_ERROR "The googletest submodules was not downloaded!"
		" GIT_SUBMODULE was turned off or failed.")
endif()

add_subdirectory(external/googletest)

add_subdirectory(src)
add_subdirectory(tst)

include_directories(src)
include_directories(external/googletest)

# Compile the WebServ.run executable
set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES src/main.cpp src/WebServ.cpp)

add_executable(${BINARY}.run ${SOURCES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/default.conf
  ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

target_link_libraries(${BINARY}.run
  PUBLIC Slice_lib
  PUBLIC ASyncServer_lib
  PUBLIC VirtualServer_lib
  PUBLIC Config_lib
  PUBLIC ServerLogic_lib
  PUBLIC HTTP_lib
  PUBLIC Debugging_lib
)
