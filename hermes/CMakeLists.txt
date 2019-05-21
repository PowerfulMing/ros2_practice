cmake_minimum_required(VERSION 3.5)

project(hermes)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rosidl_actions REQUIRED)
find_package(rmw REQUIRED)
find_package(rmw_implementation REQUIRED)
find_package(std_msgs REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(universal_interface REQUIRED)

# find ROS 1 packages
set(cmake_extras_files cmake/find_ros1_package.cmake cmake/find_ros1_interface_packages.cmake)
include(cmake/find_ros1_package.cmake)

find_package(PkgConfig)
if(NOT PKG_CONFIG_FOUND)
  message(WARNING "Failed to find PkgConfig, skipping...")
  # call ament_package() to avoid ament_tools treating this as a plain CMake pkg
  ament_package()
  return()
endif()

find_ros1_package(roscpp)
if(NOT ros1_roscpp_FOUND)
  message(WARNING "Failed to find ROS 1 roscpp, skipping...")
  # call ament_package() to avoid ament_tools treating this as a plain CMake pkg
  ament_package(
    CONFIG_EXTRAS ${cmake_extras_files}
  )
  return()
endif()


# find ROS 1 packages with messages / services
include(cmake/find_ros1_interface_packages.cmake)
find_ros1_interface_packages(ros1_message_packages)
find_ros1_package(${ros1_message_package} REQUIRED)
find_ros1_package(actionlib)
find_ros1_package(actionlib_msgs)
find_ros1_package(message_generation)
find_ros1_package(genmsg)
find_ros1_package(actionlib_tutorials)
find_ros1_package(move_base_msgs)
find_ros1_package(geometry_msgs)

include_directories(include
 ${rclcpp_INCLUDE_DIRS}
 ${rclcpp_action_INCLUDE_DIRS}
 ${rosidl_actions_INCLUDE_DIRS}
 ${catkin_INCLUDE_DIRS}
 ${std_msgs_INCLUDE_DIRS}
 ${Qt5Core_INCLUDE_DIRS}
 ${Qt5Quick_INCLUDE_DIRS}
 )

 add_library(${PROJECT_NAME}_lib src/universal_publisher.cpp src/universal_actionlib_server.cpp)


ament_target_dependencies(${PROJECT_NAME}_lib
 rclcpp
 rclcpp_action
 rosidl_actions
 std_msgs
 rmw_implementation
 ros1_roscpp
 ros1_std_msgs
 ros1_actionlib
 ros1_actionlib_msgs
 ros1_actionlib_tutorials
 ros1_move_base_msgs
 ros1_geometry_msgs
 universal_interface
)

file(GLOB SOURCE_FILES
 "src/main.cpp"
)

add_executable(${PROJECT_NAME}_node ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}_node
	${PROJECT_NAME}_lib
 	${catkin_LIBRARIES}
 	Qt5::Core
 	${rclcpp_LIBRARIES}
        ${rclcpp_action_LIBRARIES}
        ${rmw_implementation_LIBRARIES}
 )

install(TARGETS
  ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME}
)

# ament_export_dependencies(rosidl_default_runtime)

ament_package()
