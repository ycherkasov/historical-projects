cmake_minimum_required(VERSION 2.8)
project(shared1 CXX)
set(TARGET shared1)


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
  ../static1
  ../static2
)


add_library(${TARGET} SHARED ${SOURCES})
target_link_libraries(${TARGET} static1 static2)
