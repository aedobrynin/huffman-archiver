#pragma once

#include <memory>
#include <string>

#include "BinaryTree.hpp"
#include "InputBitStream.hpp"
#include "InternalStructures.hpp"
#include "OutputBitStream.hpp"

namespace Archiver {
void Decompress(const std::string& archive_name);

std::string DecompressFile(InputBitStream& in);  // Returns decompressed filename.
CanonicalCodebook DecompressCanonicalCodebook(InputBitStream& in, size_t character_count);
std::unique_ptr<BinaryTree> RestoreBinaryTree(const CanonicalCodebook& canonical_codebook);
}  // namespace Archiver
