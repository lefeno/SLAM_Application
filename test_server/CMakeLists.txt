cmake_minimum_required (VERSION 3.7.2)

# Define project name
project(rplidar CXX C)

set(SUPPRESSED_WARNING "-Wno-conversion-null -Wno-write-strings")

# Compiling flags
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SUPPRESSED_WARNING}")

add_subdirectory(src)
