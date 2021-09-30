cmake_minimum_required(VERSION 3.18)
project(Archiver)

add_subdirectory(tests)
add_subdirectory(lib)

find_library(ArchiverLibrary lib)

set(CMAKE_CXX_STANDARD 20)

add_executable(archiver main.cpp)
target_link_libraries(archiver ArchiverLibrary)
