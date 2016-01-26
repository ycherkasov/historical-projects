cmake_minimum_required(VERSION 2.8)
project(static2 CXX)
set(TARGET static2)

########################################################
# source files
file(GLOB Files_src *.cpp)
set(SOURCES ${SOURCES} ${Files_src})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} .. )
include_directories(${INCLUDE_DIRS})

########################################################
add_library(${TARGET} ${SOURCES})
