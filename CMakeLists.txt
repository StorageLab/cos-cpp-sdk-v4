cmake_minimum_required(VERSION 2.8)
CMAKE_policy(SET CMP0015 NEW)

project(cos-cpp-sdk)
SET(BOOST_HEADER_DIR "/root/boost_1_61_0")  

include_directories(./include)
include_directories(${BOOST_HEADER_DIR})

link_directories(./lib)

option(CMAKE_BUILD_TYPE "Release")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("debug mode")
    set(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -O0 -Wall -g")
    set(CMAKE_CXX_FLAGS_DEUBG "$ENV{CMAKE_CXX_FLAGS_DEUBG} -O0 -Wall -g")
else()
    message("release mode")
endif()

add_subdirectory (./src)
add_executable(cos_demo cos_demo.cpp)
if (__USE_L5)
    MESSAGE("use l5")
    add_definitions(-D__USE_L5)
    target_link_libraries(cos_demo cossdk curl ssl crypto rt jsoncpp qos_client stdc++ pthread z boost_system boost_thread)
ELSE()
    target_link_libraries(cos_demo cossdk curl ssl crypto rt jsoncpp stdc++ pthread z boost_system boost_thread)
ENDIF()