cmake_minimum_required(VERSION 3.5)
project(universal_publisher)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include
            ${rclcpp_INCLUDE_DIRS}
            ${rmw_implementation_INCLUDE_DIRS}
)
add_library(${PROJECT_NAME}_lib src/${PROJECT_NAME}.cpp)
ament_target_dependencies(${PROJECT_NAME}_lib 
  rclcpp
  std_msgs
)

set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME}_node ${SOURCES})

target_link_libraries(${PROJECT_NAME}_node 
                      ${PROJECT_NAME}_lib
                      ${rclcpp_LIBRARIES}
                      ${rmw_implementation_LIBRARIES}
)

install(TARGETS
  ${PROJECT_NAME}_node
  DESTINATION lib/${PROJECT_NAME}
)


ament_export_dependencies(rosidl_default_runtime)
ament_package()
