cmake_minimum_required(VERSION 2.8)
project(swm CXX)
set(TARGET swm)

########################################################
# source files
file(GLOB Files_src *.cpp)

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . )
include_directories(${INCLUDE_DIRS})

########################################################
add_executable(${TARGET} ${SOURCES})
