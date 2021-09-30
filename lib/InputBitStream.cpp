#include "InputBitStream.hpp"

using namespace Archiver;

InputBitStream::InputBitStream(std::istream& in) : in_(in) { }

unsigned short InputBitStream::ReadBits(size_t n) {
    unsigned short bits = 0;

    while (n) {
        if (buffer_pos_ == CHAR_BIT) {
            if (in_ >> buffer_) {
                buffer_pos_ = 0;
            } else {
                break;
            }
        }
        bool bit = (buffer_ >> (CHAR_BIT - buffer_pos_ - 1)) & 1;
        bits |= (bit << (n - 1));
        ++buffer_pos_;
        --n;
    }

    return bits;
}
