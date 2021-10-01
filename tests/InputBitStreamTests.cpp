#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <sstream>

#include "InputBitStream.hpp"

using namespace Archiver;

TEST(InputBitStreamTests, CorrectReadBits) {
    std::stringstream sstream("123abc()");
    InputBitStream ibitstream(sstream);

    std::vector<bool> sstream_bits;
    for (char symb : sstream.str()) {
        for (size_t i = 0; i < CHAR_BIT; ++i) {
            sstream_bits.push_back((symb >> i) & 1);
        }
    }

    std::vector<size_t> lengths{1, 3, 5, 7, 4, 8, 16, 4, 16};
    ASSERT_EQ(std::accumulate(lengths.begin(), lengths.end(), 0), sstream.str().size() * CHAR_BIT);
    size_t already_read_count = 0;
    for (size_t bit_count : lengths) {
        auto bits = ibitstream.ReadBits(bit_count);
        for (size_t i = 0; i < bit_count; ++i) {
            ASSERT_EQ(sstream_bits[already_read_count + i], (bits >> (bit_count - i - 1)) & 1);
        }
        already_read_count += bit_count;
    }
}
