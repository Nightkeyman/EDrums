cmake_minimum_required(VERSION 3.21)

# set the project name
project(EDrums)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY test)

# include directories
add_subdirectory(EDrumController)

message("CMake build type: " ${CMAKE_BUILD_TYPE})
