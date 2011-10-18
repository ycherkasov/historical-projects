cmake_minimum_required(VERSION 2.8)
project(static1 CXX)
set(TARGET static1)

########################################################
# source files
file(GLOB Files_src *.cpp)

########################################################
# test source files
set(Files_tests  
  test1/test1.cpp
  )

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
set(TEST_SOURCES ${TEST_SOURCES} ${Files_tests})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} .. )
include_directories(${INCLUDE_DIRS})

########################################################
add_library(${TARGET} ${SOURCES})
