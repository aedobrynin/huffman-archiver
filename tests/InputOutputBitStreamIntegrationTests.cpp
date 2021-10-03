#include <gtest/gtest.h>

#include <numeric>
#include <sstream>
#include <vector>

#include "InputBitStream.hpp"
#include "OutputBitStream.hpp"

using namespace Archiver;

TEST(InputOutputBitStreamIntegrationTests, CorrectOneBitTransfer) {
    std::string data = "Hello, world!";
    std::stringstream sstream_in(data);
    InputBitStream ibitstream(sstream_in);

    std::stringstream sstream_out;
    OutputBitStream obitstream(sstream_out);

    while (ibitstream.Good()) {
        auto bits = ibitstream.ReadBits(1);
        obitstream.WriteBits(bits, 1);
    }
    ASSERT_EQ(data, sstream_out.str());
}

TEST(InputOutputBitStreamIntegrationTests, CorrectManyBitTransfer) {
    std::string data = "test    123\n\t__@@!@#";
    std::stringstream sstream_in(data);
    InputBitStream ibitstream(sstream_in);

    std::stringstream sstream_out;
    OutputBitStream obitstream(sstream_out);

    std::vector<size_t> lengths{1, 2, 3, 13, 14, 9, 13, 1, 1, 1, 16, 16, 16, 8, 7, 9, 15, 15};
    ASSERT_EQ(std::accumulate(lengths.begin(), lengths.end(), 0), data.size() * CHAR_BIT);

    for (auto length : lengths) {
        auto bits = ibitstream.ReadBits(length);
        obitstream.WriteBits(bits, length);
    }
    ASSERT_EQ(data, sstream_out.str());
}
