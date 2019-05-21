cmake_minimum_required (VERSION 3.7.2)

# Define project name
project(slam)

#Search for Boost Libraries - STATIC only
set(Boost_USE_STATIC_LIBS ON)

if(CMAKE_TOOLCHAIN_FILE)
	find_package(Boost 1.63.0 REQUIRED program_options thread)
	#set(OpenCV_DIR ~/rpi//include/opencv2)
	#find_package (OpenCV REQUIRED)

	#set(LIBRARIES ${Boost_LIBRARIES} ies_lib ${CMAKE_SOURCE_DIR}/app/lib/rasp_rplidar_sdk.a m rt pthread gcov stdc++ Eigen3::Eigen ${OpenCV_LIBS})
	set(LIBRARIES ${Boost_LIBRARIES} ies_lib ${CMAKE_SOURCE_DIR}/app/lib/rasp_rplidar_sdk.a m rt pthread gcov Eigen3::Eigen)
	find_package (Eigen3 3.3 REQUIRED NO_MODULE)

	#include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/app/inc ${Boost_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
	include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
	link_directories(${CMAKE_SOURCE_DIR}/app/lib ${Boost_LIBRARY_DIR})
else()
	# Test opencv on ubuntu
	set(OpenCV_DIR /usr/local/include/opencv2)
	find_package (OpenCV REQUIRED)
	set(LIBRARIES ${CMAKE_SOURCE_DIR}/app/lib/ubuntu_rplidar_sdk.a m rt pthread gcov stdc++ Eigen3::Eigen ${OpenCV_LIBS})		
	find_package (Eigen3 3.3 REQUIRED NO_MODULE)
	include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/app/inc)
	link_directories(${CMAKE_SOURCE_DIR}/app/lib)
endif(CMAKE_TOOLCHAIN_FILE)

set ( Project_SRCS
	test.cpp
	#main.cpp
	RPLidarController.cpp
	OccuGridMap.cpp
	MeasurementModel.cpp
	MotionModel.cpp
	IncrementalMLMapping.cpp
	jsoncpp.cpp
	utils.cpp
	ServoController.cpp
	Mediator.cpp
	Employee.cpp
)

add_executable(main ${Project_SRCS})
target_link_libraries(main ${LIBRARIES})
