cmake_minimum_required(VERSION 2.8)
project(part_03_expressions CXX)
set(TARGET part_03_expressions)

########################################################
# source files
# source files
file(GLOB Files_src *.cpp)

# headers
file(GLOB Files_h *.h *.txt)


########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
set(HEADERS ${HEADERS} ${Files_h})

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . )
include_directories(${INCLUDE_DIRS})

########################################################
add_executable(${TARGET} ${SOURCES} ${HEADERS})
