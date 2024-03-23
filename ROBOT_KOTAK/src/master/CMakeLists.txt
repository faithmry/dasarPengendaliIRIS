cmake_minimum_required(VERSION 3.0.2)
project(master)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
# )

# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )

## INCLUDE_DIRS: uncomment this if your package contains header files
catkin_package(
 INCLUDE_DIRS include
)

## Specify additional locations of header files
include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(master src/master.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(master ${catkin_LIBRARIES})

