cmake_minimum_required(VERSION 3.13)

project(test_inotify)

file(GLOB FILE_LISTEN "${CMAKE_SOURCE_DIR}/*.h"
                      "${CMAKE_SOURCE_DIR}/*.cpp"
                      "${CMAKE_SOURCE_DIR}/filelisten/*.h"
                      "${CMAKE_SOURCE_DIR}/filelisten/*.cpp")

set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)

add_executable(${PROJECT_NAME} ${FILE_LISTEN})
target_link_libraries(${PROJECT_NAME} Qt5::Core)