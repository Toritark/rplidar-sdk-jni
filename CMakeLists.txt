cmake_minimum_required(VERSION 3.22.1)

project(lidar)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(rplidar_sdk)
add_subdirectory(library)
add_subdirectory(demo)
