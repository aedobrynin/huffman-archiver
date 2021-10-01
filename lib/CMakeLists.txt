project(ArchiverLib)

SET(CMAKE_CXX_STANDARD 20)

set(SRCS
    InputBitStream.hpp
    InputBitStream.cpp
    OutputBitStream.hpp
    OutputBitStream.cpp
    Archiver.hpp
    Archiver.cpp
    BinaryTree.hpp
    BinaryTree.cpp
)

add_library(ArchiverLibrary STATIC ${SRCS})
target_link_libraries(ArchiverLibrary)
target_include_directories(ArchiverLibrary PUBLIC ./)
