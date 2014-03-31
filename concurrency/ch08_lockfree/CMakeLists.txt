cmake_minimum_required(VERSION 2.8)
project(ch08_lockfree CXX)
set(TARGET ch08_lockfree)

cmake_policy(SET CMP0015 NEW)
SET(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(CMAKE_EXE_LINKER_FLAGS "-std=c++11 -pthread -Wl,--no-as-needed")


########################################################
# source files
file(GLOB Files_src *.cpp)

# headers
#file(GLOB Files_h include/*.h)

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
set(HEADERS ${HEADERS} ${Files_h})

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})


########################################################
# include directories
include_directories(${INCLUDE_DIRS})

set(LINK_DIRS ${LINK_DIRS} .. . )
link_directories(${LINK_DIRS})

########################################################
add_executable(${TARGET} ${SOURCES})
