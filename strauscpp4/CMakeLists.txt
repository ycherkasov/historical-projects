cmake_minimum_required(VERSION 2.8)
project(strauscpp3 CXX)
set(TARGET strauscpp3)


########################################################

# CMake policies
cmake_policy(SET CMP0015 NEW)

add_subdirectory(ch01_intro)
add_subdirectory(ch02_basics)
