#pragma once

#include <string>

#include "InternalStructures.hpp"
#include "InputBitStream.hpp"
#include "OutputBitStream.hpp"

namespace Archiver {
void Decompress(const std::string& archive_name);
std::string DecompressFile(InputBitStream& in);  // Returns decompressed filename.
CanonicalCodebook DecompressCanonicalCodebook(InputBitStream& in, size_t character_count);
}  // namespace Archiver
