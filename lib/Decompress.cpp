#include "Decompress.hpp"

#include <deque>
#include <filesystem>
#include <fstream>

#include "Exceptions.hpp"

using namespace Archiver;

void Archiver::Decompress(const std::string& archive_name) {
    if (!std::filesystem::exists(archive_name)) {
        throw FileNotFoundException(archive_name);
    }

    std::ifstream fin(archive_name);
    InputBitStream ibitstream(fin);

    std::vector <std::string> decompressed_filenames;
    while (ibitstream.Good()) {
        try {
            auto filename = DecompressFile(ibitstream);
            decompressed_filenames.push_back(filename);
        } catch (const ArchiverException& e) {
            for (auto filename : decompressed_filenames) {
                std::filesystem::remove(filename);
            }
            throw;
        }
        break;
    }
}

#include <iostream>
std::string Archiver::DecompressFile(InputBitStream& in) {
    auto character_count = in.ReadBits(9);
    if (!in.IsLastReadSuccessful()) {
        throw BadFileStructureException("Can't read character count.");
    }

    auto canonical_codebook = DecompressCanonicalCodebook(in, character_count);
    auto binary_tree{RestoreBinaryTree(canonical_codebook)};

    return {};
}

CanonicalCodebook Archiver::DecompressCanonicalCodebook(InputBitStream& in, size_t character_count) {
    CanonicalCodebook canonical_codebook;
    for (size_t i = 0; i < character_count; ++i) {
        auto character = in.ReadBits(9);
        if (!in.IsLastReadSuccessful()) {
            throw BadFileStructureException("Can't read character from encoded canonical codebook.");
        }
        canonical_codebook.characters.push_back(character);
    }

    size_t characters_processed = 0;
    while (characters_processed != character_count) {
        auto current_word_count = in.ReadBits(9);
        if (!in.IsLastReadSuccessful()) {
            throw BadFileStructureException("Can't read word count from encoded canonical codebook.");
        }
        canonical_codebook.word_count_by_bit_count.push_back(current_word_count);
        characters_processed += current_word_count;
    }
    return canonical_codebook;
}

std::unique_ptr<BinaryTree> Archiver::RestoreBinaryTree(const CanonicalCodebook& canonical_codebook) {
    std::unique_ptr<BinaryTree> binary_tree{new BinaryTree()};

    auto cur_bit_count_it = canonical_codebook.word_count_by_bit_count.begin();
    size_t cur_bit_count = 1;
    size_t cur_word_count = 0;

    std::deque<bool> cur_code;
    for (auto character : canonical_codebook.characters) {
        while (*cur_bit_count_it == 0 || *cur_bit_count_it == cur_word_count) {
            cur_word_count = 0;
            ++cur_bit_count_it;
            ++cur_bit_count;
        }

        if (!cur_code.empty()) {  // + 1 to cur_code.
            auto it = cur_code.rbegin();
            while (it != cur_code.rend() && *it == 1) {
                *it = 0;
                ++it;
            }
            if (it == cur_code.rend()) {
                cur_code.push_front(1);
            } else {
                *it = 1;
            }
        }

        size_t shift = cur_bit_count - cur_code.size();
        for (size_t i = 0; i < shift; ++i) {
            cur_code.push_back(0);
        }
        ++cur_word_count;
        binary_tree->Append(cur_code.begin(), cur_code.end(), character);
    }
    return binary_tree;
}
