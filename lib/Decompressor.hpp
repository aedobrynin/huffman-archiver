#pragma once

#include "DecompressorImpl.hpp"

namespace Archiver {
class Decompressor {
public:
    StreamMeta DecompressStreamMeta(InputBitStream& in);
    ControlCharacters DecompressStreamData(InputBitStream& in, OutputBitStream& out, const BinaryTree* binary_tree);
private:
    DecompressorImpl decompressor_impl_;
};
}  // namespace Archiver
