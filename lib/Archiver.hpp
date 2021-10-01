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
    unsigned short character = 0;
    std::vector<bool> code;

    bool operator==(const CodeWord& other) const {
        return character == other.character && code == other.code;
    }
};

using Codebook = std::vector<CodeWord>;
Codebook GetCodebook(const BinaryTree* tree_root);

struct CannonicalCodebook {
    std::vector<size_t> word_count_by_bit_count;
    std::vector<unsigned short> characters;

    bool operator==(const CannonicalCodebook& other) const {
        return word_count_by_bit_count == other.word_count_by_bit_count
            && characters == other.characters;
    }
};
CannonicalCodebook GetCannonicalCodebook(Codebook regular_codebook);
}  // namespace Archiver
