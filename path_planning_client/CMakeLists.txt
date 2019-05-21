cmake_minimum_required(VERSION 3.5)
project(path_planning_client)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# enable boost Boost_LIBRARY_DIRstatic flag
unset(Boost_LIBRARIES)
set(Boost_USE_STATIC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(progdll_dbg_libs
  "${Boost_SYSTEM_LIBRARY_DEBUG}"
  "${Boost_FILESYSTEM_LIBRARY_DEBUG}"
)
set(progdll_opt_libs
  "${Boost_SYSTEM_LIBRARY_RELEASE}"
  "${Boost_FILESYSTEM_LIBRARY_RELEASE}"
)


# find dependencies
find_package(ament_cmake REQUIRED)
find_package(universal_interface REQUIRED)
find_package(rclcpp REQUIRED)
# boost components
find_package(Boost REQUIRED system filesystem thread REQUIRED)

include_directories(include
            ${rclcpp_INCLUDE_DIRS}
            ${rmw_implementation_INCLUDE_DIRS}
            ${Boost_INCLUDE_DIRS}
)

set(COM_LIB_SOURCE src/UniversalPublisher.cpp src/UniversalClient.cpp src/Thumper.cpp src/Client.cpp)
add_library(communication_lib ${COM_LIB_SOURCE})

ament_target_dependencies(communication_lib
  rclcpp
  universal_interface
)

add_executable(path_planning_client_node src/main.cpp src/PathPlanningClient.cpp)
target_link_libraries(path_planning_client_node
                      communication_lib
                      ${rclcpp_LIBRARIES}
                      ${rmw_implementation_LIBRARIES}
                      optimized "${progdll_opt_libs}"
                      boost_thread
                      boost_system
                      jsoncpp
)

install(TARGETS
  path_planning_client_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
