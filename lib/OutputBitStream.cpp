#include "OutputBitStream.hpp"

#include <climits>

using namespace Archiver;

OutputBitStream::OutputBitStream(std::ostream& out) : out_(out) { }

void OutputBitStream::WriteBits(unsigned short bits, size_t width) {
    for (size_t i = 0; i < width; ++i) {
        buffer_ |= (bits >> (width - i - 1) & 1) << buffer_pos_;
        ++buffer_pos_;
        if (buffer_pos_ == CHAR_BIT) {
            Flush();
        }
    }
}

void OutputBitStream::WriteBits(const std::vector<bool>& bits) {
    for (size_t i = 0; i < bits.size(); ++i) {
        buffer_ |= bits[i] << buffer_pos_;
        ++buffer_pos_;
        if (buffer_pos_ == CHAR_BIT) {
            Flush();
        }
    }
}

void OutputBitStream::Flush() {
    if (buffer_pos_) {
        char tmp = static_cast<char>(buffer_);
        out_.write(&tmp, 1);
        buffer_ = 0;
        buffer_pos_ = 0;
    }
}

OutputBitStream::~OutputBitStream() {
    Flush();
}
