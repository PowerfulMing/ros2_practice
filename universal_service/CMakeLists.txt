cmake_minimum_required(VERSION 3.5)
project(universal_service)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

find_package(universal_interface REQUIRED)
if(NOT universal_interface_FOUND)
  message(WARNING "Failed to find UNIVERSAL_INTERFACE in Universal Service, skipping...")
  return()
endif()

find_package(example_interfaces REQUIRED)
if(NOT example_interfaces_FOUND)
  message(WARNING "Failed to find UNIVERSAL_INTERFACE in Universal Service, skipping...")
  return()
endif()

include_directories(include
            ${rclcpp_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}_lib src/${PROJECT_NAME}.cpp)
ament_target_dependencies(${PROJECT_NAME}_lib 
  rclcpp
  universal_interface
  example_interfaces
)

set(SOURCES src/main.cpp)
add_executable(${PROJECT_NAME}_node  ${SOURCES})
target_link_libraries(${PROJECT_NAME}_node 
                      ${PROJECT_NAME}_lib
                      ${rclcpp_LIBRARIES}
)

add_executable(map_phraser_node src/map_phraser.cpp)
target_link_libraries(map_phraser_node
                      jsoncpp
                      yaml-cpp
                      ${PROJECT_NAME}_lib
                      ${rclcpp_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}_node DESTINATION lib/${PROJECT_NAME})
install(TARGETS map_phraser_node DESTINATION lib/${PROJECT_NAME})

ament_package()
