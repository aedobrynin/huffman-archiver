#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "Compress.hpp"
#include "Decompress.hpp"

using namespace Archiver;

struct TestData {
    std::string filename;
    std::string content;
    std::string description = "";
};

std::string GenerateRandomString(size_t length) {
    std::string result;
    result.resize(length);

    std::mt19937 rng(200);
    std::uniform_int_distribution<> distr(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());
    std::generate_n(result.begin(), length, [&distr, &rng]() { return distr(rng); });

    return result;
}

TEST(CompressDecompressTests, CorrectCompressDecompressStream) {
    std::vector<TestData> tests{
        {.filename = "", .content = "", .description = "empty filename and empty content"},
        {.filename = "test123", .content = "", .description = "not empty filename and empty content"},
        {.filename = "", .content = "test123", .description = "empty filename and not empty content"},
        {.filename = GenerateRandomString(15),
         .content = GenerateRandomString(15),
         .description = "Random filename and content"},
        {.filename = GenerateRandomString(1),
         .content = GenerateRandomString(1000),
         .description = "Random filename and random long content"},
        {.filename = GenerateRandomString(1000),
         .content = GenerateRandomString(1000),
         .description = "Random long filename and random long content"},
        {.filename = GenerateRandomString(1000),
         .content = GenerateRandomString(1000),
         .description = "Random long filename and random long content"},
        {.filename = GenerateRandomString(3000),
         .content = GenerateRandomString(10000),
         .description = "Random long filename and random long content"},
        {.filename = GenerateRandomString(5000),
         .content = GenerateRandomString(30000),
         .description = "Random long filename and random long content"},
    };

    for (const auto& [filename, content, description] : tests) {
        std::stringstream content_stream{content};
        InputStreamData input_stream_data{.stream = content_stream, .name = filename};

        std::stringstream compressed;
        Archiver::Compress({input_stream_data}, compressed);

        InputBitStream ibitstream(compressed);
        auto stream_meta = Archiver::DecompressStreamMeta(ibitstream);
        std::stringstream decompressed_content;
        OutputBitStream obitstream(decompressed_content);
        Archiver::DecompressStreamData(ibitstream, obitstream, stream_meta.binary_tree.get());
        obitstream.Flush();

        ASSERT_EQ(filename, stream_meta.name) << description;
        ASSERT_EQ(content, decompressed_content.str()) << description;
    }
}
