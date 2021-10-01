cmake_minimum_required(VERSION 3.18)
project(Tests)

SET(CMAKE_CXX_STANDARD 20)

SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(../googletest build)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_library(ArchiverLibrary ../lib)

add_executable(Run_tests
               InputBitStreamTests.cpp
               OutputBitStreamTests.cpp
               FrequencyListTests.cpp
               BinaryTreeTests.cpp
               CodebookTests.cpp
               )
target_link_libraries(Run_tests ArchiverLibrary gtest gtest_main)
