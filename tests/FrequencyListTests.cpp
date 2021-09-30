#include <gtest/gtest.h>

#include <sstream>
#include <vector>

#include "Archiver.hpp"

using namespace Archiver;

TEST(FrequencyListTests, CorrectGetFrequencyList) {
    std::string filename = "test";
    std::stringstream sstream("123test");
    InputBitStream ibitstream(sstream);
    std::vector<ControlCharacters> additional_characters{ControlCharacters::FILENAME_END};

    FrequencyList expected;
    expected.fill(0);
    expected[0b00101110] = 4;  // 't'
    expected[0b10100110] = 2;  // 'e'
    expected[0b11001110] = 2;  // 's'
    expected[0b10001100] = 1;  // '1'
    expected[0b01001100] = 1;  // '2'
    expected[0b11001100] = 1;  // '3'
    expected[0b000000001] = 1;  // ControlCharacters::FILENAME_END

    ASSERT_EQ(expected, GetFrequencyList(filename, ibitstream, additional_characters));
}
