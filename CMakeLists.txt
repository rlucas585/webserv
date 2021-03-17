cmake_minimum_required(VERSION 3.10.2)

project(WebServ VERSION 1.0)

# Set Project Scoped definitions

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")

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
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/lib/googletest/CMakeLists.txt")
	message(FATAL_ERROR "The glfw submodules was not downloaded!"
		" GIT_SUBMODULE was turned off or failed.")
endif()

add_subdirectory(lib/googletest)

include_directories(src)
include_directories(tst)
include_directories(lib/googletest)
