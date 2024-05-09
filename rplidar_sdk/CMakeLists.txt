file(GLOB_RECURSE SOURCES
        "src/*.cpp"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(rplidar_sdk STATIC ${SOURCES})

target_include_directories(rplidar_sdk
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dataunpacker
        ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/linux  # Adjust for other platforms
)