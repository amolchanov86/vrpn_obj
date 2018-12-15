cmake_minimum_required(VERSION 2.8.3)
project(vrpn_obj)

set(Eigen_INCLUDE_DIRS /opt/eigen/include/eigen3/Eigen )

# Specifying VRPN_ROOT
set(VRPN_ROOT vrpn)
set(ARTLIB_ROOT art_lib)

include_directories(
  include
  ${VRPN_ROOT}
  ${ARTLIB_ROOT}
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
  ${PROJECT_NAME}_lib	
)
