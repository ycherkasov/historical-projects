cmake_minimum_required(VERSION 2.8)
project(ntl CXX)
set(TARGET ntl)

cmake_policy(SET CMP0015 NEW)
#SET(CMAKE_CXX_FLAGS "-ggdb -O0")


########################################################
# source files
file(GLOB Files_src src/*.cpp)
file(GLOB Files_h include/*.cpp)

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
set(HEADERS ${HEADERS} ${Files_h})

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . include )
include_directories(${INCLUDE_DIRS})

########################################################
add_library(${TARGET} ${SOURCES})
