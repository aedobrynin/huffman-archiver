#pragma once

#include <climits>
#include <istream>

namespace Archiver {
class InputBitStream {
public:
    explicit InputBitStream(std::istream& in);
    InputBitStream(const InputBitStream& other) = delete;
    InputBitStream& operator=(const InputBitStream& rhs) = delete;
    unsigned short ReadBits(size_t n);
    bool good();

private:
    std::istream& in_;
    unsigned char buffer_;
    size_t buffer_pos_ = CHAR_BIT;
};
}  // namespace Archiver
