cmake_minimum_required(VERSION 2.8)
CMAKE_policy(SET CMP0015 NEW)

add_definitions(-std=c++0x)
project(cos-cpp-sdk)
#SET(BOOST_HEADER_DIR "/root/boost_1_61_0")
SET(CMAKE_VERBOSE_MAKEFILE  ON)
# find_package自动查找，或者通过SET(OPENSSL_VERSION 1.0.0)人工设置
find_package(OpenSSL REQUIRED)
#SET(OPENSSL_VERSION 1.0.1e)
message(${OPENSSL_VERSION})

# 单元测试与覆盖率统计
OPTION (ENABLE_UNITTEST "Unittest" ON)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0 -g3)

include_directories(./include)
include_directories(${BOOST_HEADER_DIR})

#link_directories(./lib /root/down/c-ares/lib)
link_directories(./lib)

option(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("debug mode")
    # coverage option
    IF(ENABLE_UNITTEST)
        MESSAGE(STATUS ENABLE_UNITTEST=${ENABLE_UNITTEST})
        SET(CMAKE_CXX_FLAGS "ENV${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -O0 -Wall -g")
        set(CMAKE_CXX_FLAGS_DEUBG "$ENV{CMAKE_CXX_FLAGS_DEUBG} -fprofile-arcs -ftest-coverage -O0 -Wall -g")
    ELSE()
        set(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -O0 -Wall -g")
        set(CMAKE_CXX_FLAGS_DEUBG "$ENV{CMAKE_CXX_FLAGS_DEUBG} -O0 -Wall -g")
    ENDIF()
else()
    message("release mode")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

IF(ENABLE_UNITTEST)
    FILE(COPY config.json DESTINATION ${EXECUTABLE_OUTPUT_PATH}/)
    FILE(COPY testdata DESTINATION ${EXECUTABLE_OUTPUT_PATH}/)
    FILE(COPY unittest/merge_xml_results.py DESTINATION ${EXECUTABLE_OUTPUT_PATH}/)
    # INCLUDE_DIRECTORIES(/usr/src/googletest-release-1.8.1/googletest/include/gtest/)
    INCLUDE_DIRECTORIES(/root/down/googletest-release-1.6.0/include/)
    ADD_SUBDIRECTORY(unittest)
ENDIF()

add_subdirectory (./src)
add_executable(cos_demo cos_demo.cpp)
if (__USE_L5)
    MESSAGE("use l5")
    add_definitions(-D__USE_L5)
#cares
    target_link_libraries(cos_demo cossdk curl ssl crypto rt jsoncpp qos_client stdc++ pthread z boost_system boost_thread)
ELSE()
    target_link_libraries(cos_demo cossdk curl ssl crypto rt jsoncpp stdc++ pthread z boost_system boost_thread)
ENDIF()
