#pragma once

#include <memory>
#include <string>

#include "BinaryTree.hpp"
#include "InputBitStream.hpp"
#include "InternalStructures.hpp"
#include "OutputBitStream.hpp"

namespace Archiver {
void Decompress(const std::string& archive_name);

std::pair<std::string, ControlCharacters> DecompressFile(InputBitStream& in);  // Returns decompressed filename.
CodebookData DecompressCodebookData(InputBitStream& in, size_t character_count);
std::unique_ptr<BinaryTree> RestoreBinaryTree(const CodebookData& codebook_data);

// Extracts data till character from ControlCharacters.
// Returns found ControlCharacter.
// Throws if EOF and ControlCharacter wasn't found.
ControlCharacters Extract(InputBitStream& in, OutputBitStream& out, const BinaryTree* binary_tree);
}  // namespace Archiver
