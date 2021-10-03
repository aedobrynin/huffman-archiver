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
            auto [filename, control_character] = DecompressFile(ibitstream);
            decompressed_filenames.push_back(filename);
            if (control_character == ControlCharacters::ARCHIVE_END) {
                break;
            }
        } catch (const ArchiverException& e) {
            for (auto filename : decompressed_filenames) {
                std::filesystem::remove(filename);
            }
            throw;
        }
    }
}

std::pair<std::string, ControlCharacters> Archiver::DecompressFile(InputBitStream& in) {
    auto character_count = in.ReadBits(9);
    if (!in.IsLastReadSuccessful()) {
        throw BadFileStructureException("Can't read character count.");
    }

    auto codebook_data = DecompressCodebookData(in, character_count);
    auto binary_tree{RestoreBinaryTree(codebook_data)};
    if (binary_tree.get() == nullptr) {
        throw BadFileStructureException("Codebook is empty.");
    }

    std::stringstream filename_sstream;
    OutputBitStream filename_obitstream{filename_sstream};
    auto control_character = Extract(in, filename_obitstream, binary_tree.get());

    if (control_character != ControlCharacters::FILENAME_END) {
        throw BadFileStructureException("Filename end control character was expected, but got another.");
    }

    filename_obitstream.Flush();
    std::string filename;
    filename_sstream >> filename;
    std::ofstream fout(filename);
    OutputBitStream file_obitstream(fout);

    control_character = Extract(in, file_obitstream, binary_tree.get());
    if (control_character != ControlCharacters::ONE_MORE_FILE
     && control_character != ControlCharacters::ARCHIVE_END) {
        throw BadFileStructureException("File/Archive end control character was expected, but got another.");
    }
    file_obitstream.Flush();
    fout.close();

    return {filename, control_character};
}

CodebookData Archiver::DecompressCodebookData(InputBitStream& in, size_t character_count) {
    CodebookData codebook_data;
    for (size_t i = 0; i < character_count; ++i) {
        auto character = in.ReadBits(9);
        if (!in.IsLastReadSuccessful()) {
            throw BadFileStructureException("Can't read character from encoded codebook.");
        }
        codebook_data.characters.push_back(character);
    }

    size_t characters_processed = 0;
    while (characters_processed != character_count) {
        auto current_word_count = in.ReadBits(9);
        if (!in.IsLastReadSuccessful()) {
            throw BadFileStructureException("Can't read word count from encoded codebook.");
        }
        codebook_data.word_count_by_bit_count.push_back(current_word_count);
        characters_processed += current_word_count;
    }
    return codebook_data;
}
#include<iostream>
std::unique_ptr<BinaryTree> Archiver::RestoreBinaryTree(const CodebookData& codebook_data) {
    std::unique_ptr<BinaryTree> binary_tree{new BinaryTree()};

    auto cur_bit_count_it = codebook_data.word_count_by_bit_count.begin();
    size_t cur_bit_count = 1;
    size_t cur_word_count = 0;

    std::deque<bool> cur_code;
    for (auto character : codebook_data.characters) {
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

ControlCharacters Archiver::Extract(InputBitStream& in, OutputBitStream& out, const BinaryTree* binary_tree) {
    while (in.Good()) {
        const BinaryTree* cur = binary_tree;
        while (cur->GetValue() == BinaryTree::NO_VALUE) {
            bool to_the_right = in.ReadBits(1);
            if (!in.IsLastReadSuccessful()) {
                throw BadFileStructureException("Stop character is expected, but EOF is reached.");
            }

            if (to_the_right) {
                cur = cur->GetRightSon();
            } else {
                cur = cur->GetLeftSon();
            }

            if (!cur) {
                throw BadFileStructureException("Character wasn't found in the binary tree.");
            }
        }
        auto value = cur->GetValue();
        if (IsControlCharacter(value)) {
            return static_cast<ControlCharacters>(value);
        }
        out.WriteBits(value, 8);
    }
    throw BadFileStructureException("Stop character is expected, but EOF is reached.");
}
