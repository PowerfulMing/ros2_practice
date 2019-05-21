cmake_minimum_required(VERSION 3.5)
project(universal_actionlib)

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
find_package(rclcpp_action REQUIRED)
# boost components
find_package(Boost REQUIRED system filesystem thread REQUIRED)

include_directories(include
            ${rclcpp_INCLUDE_DIRS}
            ${rclcpp_action_INCLUDE_DIRS}
            ${rmw_implementation_INCLUDE_DIRS}
            ${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}_lib src/universal_actionlib_server.cpp src/universal_actionlib_client.cpp)

ament_target_dependencies(${PROJECT_NAME}_lib
  rclcpp
  rclcpp_action
  universal_interface
)

add_executable(action_server_node src/action_server_main.cpp)
target_link_libraries(action_server_node
                      ${PROJECT_NAME}_lib
                      ${rclcpp_LIBRARIES}
                      ${rclcpp_action_LIBRARIES}
                      ${rmw_implementation_LIBRARIES}
                      optimized "${progdll_opt_libs}"
                      boost_thread
                      boost_system
)

add_executable(action_client_node src/action_client_main.cpp)
target_link_libraries(action_client_node
                      ${PROJECT_NAME}_lib
                      ${rclcpp_LIBRARIES}
                      ${rclcpp_action_LIBRARIES}
                      ${rmw_implementation_LIBRARIES}
                      optimized "${progdll_opt_libs}"
                      boost_thread
                      boost_system
)

ament_export_dependencies(rosidl_default_runtime)

install(TARGETS
  # action_client_not_composable
  # action_client_not_composable_with_cancel
  action_server_node
  action_client_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
