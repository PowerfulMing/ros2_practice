cmake_minimum_required(VERSION 3.5)
project(path_planning_server)

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
	    include/graph_lib
	    include/path_planning_lib
	    include/commu_lib
	    include/common_lib
            ${rclcpp_INCLUDE_DIRS}
            ${rmw_implementation_INCLUDE_DIRS}
            ${Boost_INCLUDE_DIRS}
)

file(GLOB GRAPH_LIB_SOURCES "src/graph_lib/*.cpp" )
file(GLOB GRAPH_LIB_HEADER "include/graph_lib/*.h" )
file(GLOB PATHPLANNING_LIB_SOURCES "src/path_planning_lib/*.cpp" )
file(GLOB PATHPLANNING_LIB_HEADER "include/path_planning_lib/*.h" )
file(GLOB COMMU_LIB_SOURCES "src/commu_lib/*.cpp" )
file(GLOB COMMU_LIB_HEADER "include/commu_lib/*.h" )


add_library(graph_lib ${GRAPH_LIB_SOURCES} ${GRAPH_LIB_HEADER})
add_library(pathplanning_lib ${PATHPLANNING_LIB_HEADER} ${PATHPLANNING_LIB_SOURCES})
add_library(commu_lib ${COMMU_LIB_HEADER} ${COMMU_LIB_SOURCES})
#add_library(common_lib ${COMMON_LIB_HEADER})

#set(GRAPH_LIB_SOURCE src/BasicElement.cpp src/Edge.cpp src/Graph.cpp src/Location.cpp src/Vertex.cpp)
#add_library(graph_lib ${GRAPH_LIB_SOURCE})

ament_target_dependencies(commu_lib
  rclcpp
  universal_interface
)

# JSON parser
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

target_link_libraries(pathplanning_lib
		graph_lib
		commu_lib
  		"${rclcpp_LIBRARIES}"
  		"${rclcpp_action_LIBRARIES}"
  		"${rmw_implementation_LIBRARIES}"
  		"${JSONCPP_LIBRARIES}"
  		"${boost_LIBRARIES}"
  		debug "${progdll_dbg_libs}"
  		optimized "${progdll_opt_libs}"
  		boost_thread
  		boost_system
)
add_executable(path_planning_server_node src/main.cpp)
target_link_libraries(path_planning_server_node
                      pathplanning_lib
)

install(TARGETS
  path_planning_server_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
