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
    bool Good();
    bool IsLastReadSuccessful();

private:
    std::istream& in_;
    unsigned char buffer_;
    size_t buffer_pos_ = CHAR_BIT;
    bool is_last_read_successful_ = true;
};
}  // namespace Archiver
