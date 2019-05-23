# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_path_planning_server_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED path_planning_server_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(path_planning_server_FOUND FALSE)
  elseif(NOT path_planning_server_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(path_planning_server_FOUND FALSE)
  endif()
  return()
endif()
set(_path_planning_server_CONFIG_INCLUDED TRUE)

# output package information
if(NOT path_planning_server_FIND_QUIETLY)
  message(STATUS "Found path_planning_server: 0.0.1 (${path_planning_server_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'path_planning_server' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  message(WARNING "${_msg}")
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(path_planning_server_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${path_planning_server_DIR}/${_extra}")
endforeach()
