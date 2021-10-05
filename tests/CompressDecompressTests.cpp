#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "Compressor.hpp"
#include "Decompressor.hpp"

using namespace Archiver;

struct TestData {
    std::string filename;
    std::string content;
    std::string description = "";
};

std::string GenerateRandomString(size_t length,
                                 char min_char = std::numeric_limits<char>::min(),
                                 char max_char = std::numeric_limits<char>::max()) {
    std::string result;
    result.resize(length);

    std::mt19937 rng(200);
    std::uniform_int_distribution<> distr(min_char, max_char);
    std::generate_n(result.begin(), length, [&distr, &rng]() { return distr(rng); });

    return result;
}

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

TEST(CompressDecompressTests, CorrectCompressDecompressStream) {
    for (const auto& [filename, content, description] : tests) {
        std::stringstream content_sstream{content};
        InputStreamData input_stream_data{.stream = content_sstream, .name = filename};

        std::stringstream compressed;
        Compressor compressor;
        compressor.Compress({input_stream_data}, compressed);

        InputBitStream ibitstream(compressed);
        Decompressor decompressor;
        auto stream_meta = decompressor.DecompressStreamMeta(ibitstream);
        std::stringstream decompressed_content;
        OutputBitStream obitstream(decompressed_content);
        decompressor.DecompressStreamData(ibitstream, obitstream, stream_meta.binary_tree.get());
        obitstream.Flush();

        ASSERT_EQ(filename, stream_meta.name) << description;
        ASSERT_EQ(content, decompressed_content.str()) << description;
    }
}

TEST(CompressDecompressTests, CorrectCompressDecompressManyStreams) {
    std::vector<std::stringstream> content_streams;
    content_streams.resize(tests.size());
    std::vector<InputStreamData> streams_to_archive;
    for (const auto& [filename, content, description] : tests) {
        content_streams.emplace_back(content);
        InputStreamData input_stream_data{.stream = content_streams.back(), .name = filename};
        streams_to_archive.push_back(input_stream_data);
    }

    std::stringstream compressed;
    Compressor compressor;
    compressor.Compress(streams_to_archive, compressed);

    InputBitStream ibitstream(compressed);
    Decompressor decompressor;
    for (size_t i = 0; i < tests.size(); ++i) {
        auto stream_meta = decompressor.DecompressStreamMeta(ibitstream);
        std::stringstream decompressed_content;
        OutputBitStream obitstream(decompressed_content);
        decompressor.DecompressStreamData(ibitstream, obitstream, stream_meta.binary_tree.get());
        obitstream.Flush();

        ASSERT_EQ(tests[i].filename, stream_meta.name) << tests[i].description;
        ASSERT_EQ(tests[i].content, decompressed_content.str()) << tests[i].description;
    }
}

// Check compressed size on all ASCII characters.
TEST(CompressDecompressTests, CompressedSizeLessThanDecompressed) {
    for (auto content_size : {5000, 10000, 20000, 30000, 50000, 100000}) {
        std::string content = GenerateRandomString(static_cast<size_t>(content_size), 0, 127);
        std::stringstream content_sstream{content};
        InputStreamData input_stream_data{.stream = content_sstream, .name = ""};

        std::stringstream compressed;
        Compressor compressor;
        compressor.Compress({input_stream_data}, compressed);

        auto before_compress_size = content_size;
        auto after_compress_size = compressed.str().size();
        ASSERT_LE(after_compress_size, before_compress_size);
    }
}
