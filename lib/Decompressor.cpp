#include "Decompressor.hpp"

using namespace Archiver;

StreamMeta Archiver::Decompressor::DecompressStreamMeta(InputBitStream& in) {
    return decompressor_impl_.DecompressStreamMeta(in);
}

ControlCharacters Archiver::Decompressor::DecompressStreamData(InputBitStream& in,
                                                               OutputBitStream& out,
                                                               const BinaryTree* binary_tree) {
    return decompressor_impl_.DecompressStreamData(in, out, binary_tree);
}
