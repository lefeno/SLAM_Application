cmake_minimum_required (VERSION 3.7.2)

# Define project name
project(slam)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED program_options thread)

set(LIBRARIES ${Boost_LIBRARIES} m rt pthread gcov Eigen3::Eigen)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/app/lib ${Boost_LIBRARY_DIR})
set ( Project_SRCS
	test.cpp
	jsoncpp.cpp
	utils.cpp
	Mediator.cpp
	Employee.cpp
)

add_executable(main ${Project_SRCS})
target_link_libraries(main ${LIBRARIES})