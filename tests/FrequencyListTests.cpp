#include <gtest/gtest.h>

#include <sstream>
#include <vector>

#include "CompressorImpl.hpp"

using namespace Archiver;

TEST(FrequencyListTests, CorrectGetFrequencyList) {
    std::string filename = "test";
    std::stringstream sstream("123test");
    InputBitStream ibitstream(sstream);
    std::vector<ControlCharacters> additional_characters{ControlCharacters::FILENAME_END};

    FrequencyList expected;
    expected.fill(0);
    expected[0b001011100] = 4;   // 't'
    expected[0b101001100] = 2;   // 'e'
    expected[0b110011100] = 2;   // 's'
    expected[0b100011000] = 1;   // '1'
    expected[0b010011000] = 1;   // '2'
    expected[0b110011000] = 1;   // '3'
    expected[0b000000001] = 1;  // ControlCharacters::FILENAME_END

    CompressorImpl compressor_impl;
    ASSERT_EQ(expected, compressor_impl.GetFrequencyList(filename, ibitstream, additional_characters));
}
