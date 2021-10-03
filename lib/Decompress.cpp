#include "Decompress.hpp"

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

std::string Archiver::DecompressFile(InputBitStream& in) {
    auto character_count = in.ReadBits(9);
    if (!in.IsLastReadSuccessful()) {
        throw BadFileStructureException("Can't read character count.");
    }

    auto canonical_codebook = DecompressCanonicalCodebook(in, character_count);
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
