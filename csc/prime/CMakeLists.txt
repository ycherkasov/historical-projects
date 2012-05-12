cmake_minimum_required(VERSION 2.8)
project(prime CXX)
set(TARGET prime)

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
set(INCLUDE_DIRS ${INCLUDE_DIRS} . )
include_directories(${INCLUDE_DIRS})
link_libraries( ${TARGET} ntl)

########################################################
add_executable(${TARGET} ${SOURCES})
