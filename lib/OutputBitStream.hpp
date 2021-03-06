#pragma once

#include <ostream>
#include <vector>

namespace Archiver {
class OutputBitStream {
public:
    explicit OutputBitStream(std::ostream& out);
    OutputBitStream(const OutputBitStream& other) = delete;
    OutputBitStream& operator=(const OutputBitStream& rhs) = delete;

    void WriteBits(unsigned short bits, size_t width);
    void WriteBits(const std::vector<bool>& bits);
    void Flush();

    ~OutputBitStream();

private:
    std::ostream& out_;
    unsigned char buffer_ = 0;
    size_t buffer_pos_ = 0;
};
}  // namespace Archiver
