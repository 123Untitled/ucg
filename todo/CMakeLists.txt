
# minimal version
cmake_minimum_required(VERSION 2.8...3.14)

# exit if windows
if(WIN32)
	message(FATAL_ERROR "Windows is not supported")
endif()

# project name
project(ucg VERSION 0.1.0 LANGUAGES CXX)


include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

# toml++ dependency
FetchContent_Declare(
	tomlplusplus
	GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
	GIT_TAG        v3.4.0
	GIT_PROGRESS ON
)

# yaml-cpp dependency
FetchContent_Declare(
	yaml-cpp
	GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
	GIT_TAG        0.8.0
	GIT_PROGRESS ON
)

# fetch the dependencies
FetchContent_MakeAvailable(tomlplusplus yaml-cpp)



# set ninja as the default build system
#set(CMAKE_GENERATOR "Ninja")

# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror \
										-Wpedantic -Weffc++ \
										-Wno-unused -Wno-unused-variable \
										-Wno-unused-parameter -Wno-unused-private-field \
										-Wno-unused-local-typedef -Wno-unused-function \
										-Winline \
										-Wconversion -Wsign-conversion -Wfloat-conversion \
										-Wnarrowing")

# set the build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()



# source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "sources/*.cpp")

# include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)
include_directories(${tomlplusplus_SOURCE_DIR}/include)
include_directories(${yaml-cpp_SOURCE_DIR}/include)

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CONFIG:Debug>:-Og -g3 -DDEBUG>
	$<$<CONFIG:Release>:-O3 -g0>
)

# install path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)
endif()

# install the executable
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

# clangd database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
