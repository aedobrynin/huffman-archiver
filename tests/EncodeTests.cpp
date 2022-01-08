#include <gtest/gtest.h>

#include <sstream>

#include "CompressorImpl.hpp"

using namespace Archiver;

TEST(EncodeTests, CorrectEncode) {
    EncodingTable encoding_table{{0b000000000, {.code = 0b110, .code_size = 3}},      // 0
                                 {0b100000000, {.code = 0b01, .code_size = 2}},         // 1
                                 {0b010000000, {.code = 0b10, .code_size = 2}},         // 2
                                 {0b000000001, {.code = 0b1111, .code_size = 4}}};  // ControlCharacters::FILENAME_END

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
    CompressorImpl compressor_impl;
    compressor_impl.Encode(in, encoding_table, out, ControlCharacters::FILENAME_END);
    out.Flush();

    std::vector<bool> expected{1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1};
    std::vector<bool> bits;
    InputBitStream in_check(sstream_out);
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], in_check.ReadBits(1));
    }
}
