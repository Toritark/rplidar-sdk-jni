cmake_minimum_required(VERSION 3.28)
project(lidar_jni_demo)

set(CMAKE_CXX_STANDARD 23)

set(LIBRARY_SOURCES ../library)

add_executable(lidar_jni_demo demo.cpp)
target_link_libraries(lidar_jni_demo PRIVATE rplidar_sdk)
target_link_libraries(lidar_jni_demo PRIVATE lidar_jni)