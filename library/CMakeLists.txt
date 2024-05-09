cmake_minimum_required(VERSION 3.22.1)
project(lidar_jni)

set(CMAKE_CXX_STANDARD 17)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

add_library(lidar_jni SHARED library.cpp
        RPLidar.cpp)
target_include_directories(lidar_jni PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lidar_jni PRIVATE rplidar_sdk)

set_target_properties(lidar_jni PROPERTIES OUTPUT_NAME "rplidar")