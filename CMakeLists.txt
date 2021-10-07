cmake_minimum_required(VERSION 3.18)
project(Archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic")

add_subdirectory(tests)
add_subdirectory(lib)

find_library(ArchiverLibrary lib)

set(CMAKE_CXX_STANDARD 20)

add_executable(archiver main.cpp)
target_link_libraries(archiver ArchiverLibrary)
