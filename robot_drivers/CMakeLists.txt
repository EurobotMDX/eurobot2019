cmake_minimum_required(VERSION 2.8.3)
project(robot_drivers)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
)

catkin_package(
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  libraries/EPOS_Linux_Library/include
  includes
)

## Declare a C++ library
add_library(${PROJECT_NAME}_LIB
  src/drive_train_manager.cpp
  src/epos_drive_manager.cpp
  src/tests/drive_train_manager_test.cpp
  src/tests/epos_drive_manager_test.cpp
)

target_link_libraries(${PROJECT_NAME}_LIB
  ${catkin_LIBRARIES}
  EposCmd
)

# add_executable(drive_train_manager_node src/drive_train_manager.cpp)
# target_link_libraries(drive_train_manager_node
#   ${catkin_LIBRARIES}
#   EposCmd
# )
# add_dependencies(drive_train_manager_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


# add_executable(epos_drive_manager_node src/epos_drive_manager.cpp)
# target_link_libraries(epos_drive_manager_node
#   ${catkin_LIBRARIES}
#   EposCmd
# )
# add_dependencies(epos_drive_manager_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


# add_executable(drive_train_manager_test_node src/tests/drive_train_manager_test.cpp)
# target_link_libraries(${PROJECT_NAME}_LIB
#   ${catkin_LIBRARIES}
#   EposCmd
# )
# add_dependencies(drive_train_manager_test_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


# add_executable(${PROJECT_NAME}_node src/tests/epos_drive_manager_test.cpp)
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
#   EposCmd
# )
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


# set (dependencies apple bananas carrots)
# foreach(dependency $(dependencies))
# 	message("$(dependencies) has item $(dependency)")
# endforeach()