cmake_minimum_required(VERSION 2.8)
project(strauscpp CXX)
set(TARGET strauscpp)


########################################################

# CMake policies
cmake_policy(SET CMP0015 NEW)

add_subdirectory(ch01_intro)
add_subdirectory(ch02_basics)
