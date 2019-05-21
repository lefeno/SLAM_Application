cmake_minimum_required (VERSION 3.7.2)

# Define project name
project(slam)

#Search for Boost Libraries - STATIC only
#set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED program_options thread)

# Test opencv on ubuntu
set(OpenCV_DIR /usr/local/include/opencv2)
find_package (OpenCV REQUIRED)
set(LIBRARIES ${CMAKE_SOURCE_DIR}/app/lib/ubuntu_rplidar_sdk.a m rt pthread gcov stdc++ Eigen3::Eigen ${OpenCV_LIBS} ncurses boost_system boost_thread)		
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/app/inc)
link_directories(${CMAKE_SOURCE_DIR}/app/lib)

set ( Project_SRCS
	main.cpp
	jsoncpp.cpp
	utils.cpp
)

add_executable(main ${Project_SRCS})
target_link_libraries(main ${LIBRARIES})