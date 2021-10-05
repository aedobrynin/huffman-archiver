project(ArchiverLib)

SET(CMAKE_CXX_STANDARD 20)

set(SRCS
    InternalStructures.hpp
    InternalStructures.cpp
    InputBitStream.hpp
    InputBitStream.cpp
    OutputBitStream.hpp
    OutputBitStream.cpp
    BinaryTree.hpp
    BinaryTree.cpp
    CompressorImpl.hpp
    CompressorImpl.cpp
    DecompressorImpl.hpp
    DecompressorImpl.cpp
    Compressor.hpp
    Compressor.cpp
    Decompressor.hpp
    Decompressor.cpp
    Exceptions.hpp
)

add_library(ArchiverLibrary STATIC ${SRCS})
target_link_libraries(ArchiverLibrary)
target_include_directories(ArchiverLibrary PUBLIC ./)
