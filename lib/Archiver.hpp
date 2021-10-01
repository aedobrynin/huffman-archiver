#pragma once

#include <array>
#include <vector>

#include "BinaryTree.hpp"
#include "InputBitStream.hpp"
#include "OutputBitStream.hpp"

namespace Archiver {
enum class ControlCharacters {
    FILENAME_END = 256,
    ONE_MORE_FILE = 257,
    ARCHIVE_END = 258
};

void Compress(const std::string& archive_name, const std::vector<std::string>& files_to_archive);
void CompressFile(const std::string& file_name, InputBitStream& in, OutputBitStream& out, bool is_last);

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

struct CanonicalCodebook {
    std::vector<size_t> word_count_by_bit_count;
    std::vector<unsigned short> characters;

    bool operator==(const CanonicalCodebook& other) const {
        return word_count_by_bit_count == other.word_count_by_bit_count
            && characters == other.characters;
    }
};
CanonicalCodebook GetCanonicalCodebook(Codebook regular_codebook);
}  // namespace Archiver
