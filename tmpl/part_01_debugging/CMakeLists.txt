cmake_minimum_required(VERSION 2.8)
project(part_01_debugging CXX)
set(TARGET part_01_debugging)

cmake_policy(SET CMP0015 NEW)


########################################################
# source files
file(GLOB Files_src *.cpp)

# headers
file(GLOB Files_h *.h)

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src} ${Files_h})

source_group("Sources" FILES ${SOURCES})

########################################################
add_executable(${TARGET} ${SOURCES})
