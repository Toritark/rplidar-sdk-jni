file(GLOB_RECURSE SOURCES
        "src/*.cpp"
)

add_library(rplidar_sdk SHARED ${SOURCES})

target_include_directories(rplidar_sdk
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dataunpacker
        ${CMAKE_CURRENT_SOURCE_DIR}/src/arch/linux  # Adjust for other platforms
)