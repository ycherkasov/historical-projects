cmake_minimum_required(VERSION 2.8)
project(stlperf CXX)
set(TARGET stlperf)

cmake_policy(SET CMP0015 NEW)
SET(CMAKE_CXX_FLAGS "-ggdb -O0")


########################################################
# source files
file(GLOB Files_src *.cpp)


########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})

########################################################
# include directories
include_directories(.)

########################################################
add_executable(${TARGET} ${SOURCES})
