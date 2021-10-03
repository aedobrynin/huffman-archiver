#include "InternalStructures.hpp"

using namespace Archiver;

bool Archiver::IsControlCharacter(unsigned short value) {
    return value == static_cast<unsigned short>(ControlCharacters::FILENAME_END)
        || value == static_cast<unsigned short>(ControlCharacters::ONE_MORE_FILE)
        || value == static_cast<unsigned short>(ControlCharacters::ARCHIVE_END);
}
