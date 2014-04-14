cmake_minimum_required(VERSION 2.8)
project(strauscpp3 CXX)
set(TARGET strauscpp3)

if(UNIX)
  message("Unix configuration")
  set(CMAKE_CXX_FLAGS "-std=c++11")

elseif("WIN32")
  message("Windows configuration")
  set(CMAKE_CXX_FLAGS "/EHsc")
  set(MY_BOOST_DIR ${WINDOWS_BOOST_DIR})

endif()


########################################################

# CMake policies
cmake_policy(SET CMP0015 NEW)

add_subdirectory(ch_03_basics)
add_subdirectory(ch_04_types)
add_subdirectory(ch_07_functions)
add_subdirectory(ch_08_namespaces)
add_subdirectory(ch_09_linkage)
add_subdirectory(ch_10_classes)
add_subdirectory(ch_11_operators)
add_subdirectory(ch_12_inheritance)
add_subdirectory(ch_13_templates)
add_subdirectory(ch_14_exceptions)
add_subdirectory(ch_15_inheritance2)
add_subdirectory(ch_16_containers)
add_subdirectory(ch_17_containers2)
add_subdirectory(ch_18_algorithms)
add_subdirectory(ch_19_iterators)
add_subdirectory(ch_20_string)
add_subdirectory(ch_21_iostream)
add_subdirectory(ch_22_numeric)
