cmake_minimum_required(VERSION 2.8)
project(prime CXX)
set(TARGET prime)

cmake_policy(SET CMP0015 NEW)
#SET(CMAKE_CXX_FLAGS "-ggdb -O0")

include("../../cmake/BoostDependency.cmake")
include("../../cmake/FindBoost.cmake")


FIND_PACKAGE(Boost)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/boost/boost_1_50_0/boost")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/boost/boost_1_50_0/stage")

########################################################
# source files
set(SOURCES ${SOURCES} prime.cpp primetest_ntl.h)

########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} . ../ntl/include)
set(LINK_DIRS ${LINK_DIRS} . ntl/Debug)

include_directories(${INCLUDE_DIRS})
link_directories(${LINK_DIRS})
link_libraries( ${TARGET} ntl)

########################################################
add_executable(${TARGET} ${SOURCES})
