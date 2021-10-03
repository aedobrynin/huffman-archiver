#pragma once

#include <stdexcept>
#include <string>

namespace Archiver {
class ArchiverException : public std::runtime_error {
public:
    explicit ArchiverException(const std::string& what_arg) : std::runtime_error(what_arg) { }
};

class FileNotFoundException : public ArchiverException {
public:
    explicit FileNotFoundException(const std::string& filename)
        : ArchiverException("File with name \"" + filename + "\" doesn't exist.") { }
};

class BadFileStructureException : public ArchiverException {
public:
    explicit BadFileStructureException(const std::string& what_arg) : ArchiverException(what_arg) { }
};
}  // namespace Archiver
