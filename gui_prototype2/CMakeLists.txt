cmake_minimum_required(VERSION 3.0)
project(windows_gui CXX)
set(TARGET windows_gui)

if("UNIX" )

  message( FATAL_ERROR "Unix configuration is not supported for that project")

elseif("WIN32")

  message("Windows configuration")
  set(CMAKE_CXX_FLAGS "/EHsc")

endif()


cmake_policy(SET CMP0015 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")


########################################################
# include directories
set(INCLUDE_DIRS ${INCLUDE_DIRS} .)
include_directories(${INCLUDE_DIRS})


########################################################
# source files
file(GLOB Files_src *.cpp *.h *.rc *.ico)
set(SOURCES ${SOURCES} ${Files_src})
source_group("Sources" FILES ${SOURCES})

########################################################
add_executable(${TARGET} WIN32 ${SOURCES})
