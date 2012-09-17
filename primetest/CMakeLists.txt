cmake_minimum_required(VERSION 2.8)
project(prime CXX)
set(TARGET prime)


########################################################

# CMake policies
cmake_policy(SET CMP0015 NEW)

add_subdirectory(ntl)
add_subdirectory(prime)
