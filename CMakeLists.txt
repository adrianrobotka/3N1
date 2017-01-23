cmake_minimum_required(VERSION 2.8)

project(collatz_tester)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -lrt")

message("### Build in ${CMAKE_BUILD_TYPE} mode ###")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

find_package(Threads REQUIRED)

add_executable(collatz_tester ${SOURCES})

target_link_libraries(collatz_tester ${CMAKE_THREAD_LIBS_INIT})