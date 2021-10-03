#pragma once

#include <stdexcept>
#include <string>

namespace Archiver {
class ArchiverException : public std::runtime_error {
public:
    ArchiverException(const std::string& what_arg) : std::runtime_error(what_arg) { }
};

class FileNotFoundException : public ArchiverException {
public:
    FileNotFoundException(const std::string& filename)
        : ArchiverException("File with name \"" + filename + "\" doesn't exist.") { }
};
}  // namespace Archiver
