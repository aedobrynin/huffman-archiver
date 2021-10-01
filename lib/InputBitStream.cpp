#include "InputBitStream.hpp"

using namespace Archiver;

InputBitStream::InputBitStream(std::istream& in) : in_(in) { }

unsigned short InputBitStream::ReadBits(size_t n) {
    unsigned short bits = 0;

    for (size_t i = 0; i < n; ++i) {
        if (buffer_pos_ == CHAR_BIT) {
            if (in_.peek() != EOF) {
                char tmp;
                in_.read(&tmp, 1);
                buffer_ = static_cast<unsigned char>(tmp);
                buffer_pos_ = 0;
            } else {
                break;
            }
        }
        bool bit = (buffer_ >> (buffer_pos_)) & 1;
        bits |= bit << (n - i - 1);
        ++buffer_pos_;
    }

    return bits;
}

bool InputBitStream::good() {
    return buffer_pos_ != CHAR_BIT || in_.peek() != EOF;
}
