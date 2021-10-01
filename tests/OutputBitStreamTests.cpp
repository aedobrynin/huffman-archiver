#include <gtest/gtest.h>

#include "OutputBitStream.hpp"

#include <vector>
#include <sstream>

using namespace Archiver;

TEST(OutputBitStreamTests, CorrectWriteBits) {
    struct TestData {
        std::vector<unsigned short> bits;
        std::vector<size_t> widths;
        std::string expected;
    };

    std::vector<TestData> tests;
    {
        TestData test_data{
            .bits = {0b0010, 0b1110, 0b1, 0b01, 0b001, 0b10, 0b110, 0b011,
                     0b10, 0b0010, 0b1110, 0b1000110001001100, 0b11001100},
            .widths = {4, 4, 1, 2, 3, 2, 3, 3, 2, 4, 4, 16, 8},
            .expected = "test123"
        };
        tests.push_back(test_data);
    }

    ASSERT_FALSE(tests.empty());

    for (const auto& [bits, widths, expected] : tests) {
        std::stringstream sstream;
        OutputBitStream obitstream(sstream);

        for (size_t i = 0; i < widths.size(); ++i) {
            obitstream.WriteBits(bits[i], widths[i]);
        }
        obitstream.Flush();
        ASSERT_EQ(sstream.str(), expected);
    }
}
