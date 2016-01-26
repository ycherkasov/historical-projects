cmake_minimum_required(VERSION 2.8)
project(amazon_tasks CXX)
set(TARGET amazon_tasks)

########################################################
# source files
file(GLOB Files_src *.cpp *.h)

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
set(TEST_SOURCES ${TEST_SOURCES} ${Files_tests})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} .. . )
include_directories(${INCLUDE_DIRS})

########################################################
add_executable(${TARGET} ${SOURCES})
