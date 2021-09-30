#include "Archiver.hpp"

#include <sstream>

using namespace Archiver;

FrequencyList Archiver::GetFrequencyList(const std::string& filename,
                                         InputBitStream& ibitstream,
                                         const std::vector<ControlCharacters>& additional_characters) {
    FrequencyList frequency_list;
    frequency_list.fill(0);

    std::stringstream filename_sstream(filename);
    InputBitStream filename_ibitstream(filename_sstream);
    while (filename_ibitstream.good()) {
        auto character = filename_ibitstream.ReadBits(8);
        ++frequency_list[character];
    }

    while (ibitstream.good()) {
        auto character = ibitstream.ReadBits(8);
        ++frequency_list[character];
    }

    for (auto additional_character : additional_characters) {
        unsigned short character = 0;
        for (size_t i = 0; i < 9; ++i) {
            character <<= 1;
            character |= (static_cast<unsigned short>(additional_character) >> i) & 1;
        }
        ++frequency_list[character];
    }

    return frequency_list;
}
