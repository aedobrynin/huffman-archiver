#pragma once

#include <array>
#include <vector>

#include "BinaryTree.hpp"
#include "InputBitStream.hpp"

namespace Archiver {
enum class ControlCharacters {
    FILENAME_END = 256,
    ONE_MORE_FILE = 257,
    ARCHIVE_END = 258
};

using FrequencyList = std::array<size_t, 1 << 9>;
FrequencyList GetFrequencyList(const std::string& filename,
                               InputBitStream& ibitstream,
                               const std::vector<ControlCharacters>& additional_characters);

BinaryTree* GetBinaryTree(const FrequencyList& frequency_list);

struct CodeWord {
    unsigned short symbol = 0;
    size_t bit_count = 0;

    bool operator==(const CodeWord& other) const {
        return symbol == other.symbol && bit_count == other.bit_count;
    }
};
using Codebook = std::vector<CodeWord>;

Codebook GetCodebook(const BinaryTree* tree_root);
}  // namespace Archiver
