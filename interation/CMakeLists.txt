cmake_minimum_required(VERSION 2.8)
project(integration CXX)
set(TARGET integration)

include("../cmake/BoostDependency.cmake")
include("../cmake/FindBoost.cmake")

cmake_policy(SET CMP0015 NEW)
# if unix
#SET(CMAKE_CXX_FLAGS "-ggdb -O0")

# TODO: INIT BOOST CORRECT WAY!

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/boost/boost_1_50_0/boost")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/boost/boost_1_50_0/stage")



########################################################
# source files
file(GLOB Files_src src/*.cpp)

# headers
file(GLOB Files_h include/*.h)

########################################################
# test source files
#set(Files_tests  
#  test/test1.cpp
#  )

########################################################
# source files
set(SOURCES ${SOURCES} ${Files_src})
set(HEADERS ${HEADERS} ${Files_h})
#set(TEST_SOURCES ${TEST_SOURCES} ${Files_tests})

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})


########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} .. . include "C:/boost/boost_1_50_0/boost")
include_directories(${INCLUDE_DIRS})

set(LINK_DIRS ${LINK_DIRS} .. . "C:/boost/boost_1_50_0/stage/lib")
link_directories(${LINK_DIRS})

########################################################
#add_library(${TARGET} ${SOURCES})
add_executable(${TARGET} ${SOURCES} ${HEADERS})
