cmake_minimum_required(VERSION 2.8.3)
project(vrpn_obj)

#find_package(
#  catkin REQUIRED COMPONENTS 
#  roscpp
#  geometry_msgs
#  std_msgs
#  tf
#  tf_conversions  roscpp 
#)

set(Eigen_INCLUDE_DIRS /opt/eigen/include/eigen3/Eigen )
#set(Eigen_DIR $ENV{Eigen_DIR})
#find_package(Eigen REQUIRED)

#Specifying VRPN_ROOT
#$ENV{VRPN_ROOT}
set(VRPN_ROOT vrpn)
set(ARTLIB_ROOT art_lib)


#catkin_package(
#  INCLUDE_DIRS ${VRPN_ROOT} include
#  CATKIN_DEPENDS 
#    roscpp
#    geometry_msgs
#    std_msgs
#    tf
#    tf_conversions
#  DEPENDS 
#    Eigen
#)

include_directories(
  include
  ${VRPN_ROOT}
  ${ARTLIB_ROOT}
#  ${catkin_INCLUDE_DIRS}
  ${Eigen_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}_lib
  src/${PROJECT_NAME}.cpp
  ${ARTLIB_ROOT}/art_timer.cpp
  ${ARTLIB_ROOT}/art_signals.cpp
  ${ARTLIB_ROOT}/art_time.cpp
  ${VRPN_ROOT}/vrpn_Connection.C
  ${VRPN_ROOT}/vrpn_Tracker.C
  ${VRPN_ROOT}/vrpn_Shared.C
  ${VRPN_ROOT}/vrpn_FileConnection.C
  ${VRPN_ROOT}/vrpn_BaseClass.C
  ${VRPN_ROOT}/vrpn_Serial.C
  ${VRPN_ROOT}/vrpn_EndpointContainer.C
  ${VRPN_ROOT}/vrpn_Thread.C
)

target_link_libraries(${PROJECT_NAME}_lib
  rt
  pthread
)

add_executable(${PROJECT_NAME}_test
  src/${PROJECT_NAME}_test.cpp
)

target_link_libraries(${PROJECT_NAME}_test 
#  ${catkin_LIBRARIES}
  ${PROJECT_NAME}_lib	
)
