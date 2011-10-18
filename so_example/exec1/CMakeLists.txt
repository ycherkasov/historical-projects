cmake_minimum_required(VERSION 2.8)
project(exec1 CXX)
set(TARGET exec1)

cmake_policy(SET CMP0015 NEW)

########################################################
# source files
file(GLOB Files_src *.cpp)
set(SOURCES ${SOURCES} ${Files_src})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} .. )
include_directories(${INCLUDE_DIRS})

########################################################
link_directories(
  ../shared1
)


add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} shared1)
