project(ArchiverLib)

SET(CMAKE_CXX_STANDARD 20)

set(SRCS
    InputBitStream.hpp
    InputBitStream.cpp
    OutputBitStream.hpp
    OutputBitStream.cpp
    BinaryTree.hpp
    BinaryTree.cpp
    Compress.hpp
    Compress.cpp
)

add_library(ArchiverLibrary STATIC ${SRCS})
target_link_libraries(ArchiverLibrary)
target_include_directories(ArchiverLibrary PUBLIC ./)
