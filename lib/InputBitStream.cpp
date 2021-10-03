#include "InputBitStream.hpp"

using namespace Archiver;

InputBitStream::InputBitStream(std::istream& in) : in_(in) { }

unsigned short InputBitStream::ReadBits(size_t n) {
    unsigned short bits = 0;
    is_last_read_successful_ = false;
    for (size_t i = 0; i < n; ++i) {
        if (buffer_pos_ == CHAR_BIT) {
            if (in_.peek() != EOF) {
                char tmp;
                in_.read(&tmp, 1);
                buffer_ = static_cast<unsigned char>(tmp);
                buffer_pos_ = 0;
            } else {
                return bits;
            }
        }
        bool bit = (buffer_ >> (buffer_pos_)) & 1;
        bits |= bit << (n - i - 1);
        ++buffer_pos_;
    }
    is_last_read_successful_ = true;
    return bits;
}

void InputBitStream::Reset() {
    in_.clear();
    in_.seekg(std::ios_base::beg);
    buffer_pos_ = CHAR_BIT;
    buffer_ = 0;
}

bool InputBitStream::Good() {
    return buffer_pos_ != CHAR_BIT || in_.peek() != EOF;
}

bool InputBitStream::IsLastReadSuccessful() {
    return is_last_read_successful_;
}
