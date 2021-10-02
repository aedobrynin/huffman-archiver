#include <gtest/gtest.h>

#include <sstream>

#include "Archiver.hpp"

using namespace Archiver;

TEST(EncodeTests, CorrectEncode) {
    EncodingTable encoding_table{{0b00000000, {1, 1, 0}},       // 0
                                 {0b10000000, {0, 1}},          // 1
                                 {0b01000000, {1, 0}},          // 2
                                 {0b000000001, {1, 1, 1, 1}}};  // ControlCharacters::FILENAME_END

    std::stringstream sstream_in;
    sstream_in << static_cast<unsigned char>(0);
    sstream_in << static_cast<unsigned char>(1);
    sstream_in << static_cast<unsigned char>(1);
    sstream_in << static_cast<unsigned char>(2);
    sstream_in << static_cast<unsigned char>(0);
    sstream_in << static_cast<unsigned char>(1);
    InputBitStream in(sstream_in);

    std::stringstream sstream_out;
    OutputBitStream out(sstream_out);
    Encode(in, encoding_table, out, ControlCharacters::FILENAME_END);
    out.Flush();

    std::vector<bool> expected{1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1};
    std::vector<bool> bits;
    InputBitStream in_check(sstream_out);
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], in_check.ReadBits(1));
    }
}
