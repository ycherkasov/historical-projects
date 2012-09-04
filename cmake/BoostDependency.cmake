<<<<<<< HEAD
macro(BoostDependency REQUIRED_COMPONENTS)
  set(Boost_USE_STATIC_LIBS ON)
=======


macro(BoostDependency REQUIRED_COMPONENTS)
  set(Boost_USE_STATIC_LIBS   ON)
>>>>>>> 41b4fbbf754e882c361d70ddb09b868828cb78b0
  set(Boost_USE_MULTITHREADED ON)
  
  add_definitions(-DBOOST_ALL_NO_LIB)

<<<<<<< HEAD
  find_package(Boost 1.50 REQUIRED COMPONENTS ${REQUIRED_COMPONENTS})
=======
  find_package(Boost 1.41 REQUIRED COMPONENTS ${REQUIRED_COMPONENTS})
>>>>>>> 41b4fbbf754e882c361d70ddb09b868828cb78b0

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
  endif()
endmacro(BoostDependency)


macro(BoostHeaders)
<<<<<<< HEAD
  set(Boost_USE_STATIC_LIBS ON)
=======
  set(Boost_USE_STATIC_LIBS   ON)
>>>>>>> 41b4fbbf754e882c361d70ddb09b868828cb78b0
  set(Boost_USE_MULTITHREADED ON)

  add_definitions(-DBOOST_ALL_NO_LIB)

<<<<<<< HEAD
  find_package(Boost 1.50 REQUIRED )
=======
  find_package(Boost 1.41 REQUIRED )
>>>>>>> 41b4fbbf754e882c361d70ddb09b868828cb78b0

  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
  endif()
endmacro(BoostHeaders)

function(BoostDependencyLink TARGET)
<<<<<<< HEAD
# message("Boost_LIBRARIES=${Boost_LIBRARIES}")
=======
#  message("Boost_LIBRARIES=${Boost_LIBRARIES}")
>>>>>>> 41b4fbbf754e882c361d70ddb09b868828cb78b0
  target_link_libraries(${TARGET} ${Boost_LIBRARIES})
endfunction(BoostDependencyLink)