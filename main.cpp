#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>

#include "Compress.hpp"
#include "Decompress.hpp"
#include "Exceptions.hpp"

const char* help_message =
    "Compress/decompress files using Huffman coding.\n"
    "To compress: \"archiver -c archive_name file1 [file2 ...]\".\n"
    "To decompress: \"archiver -d archive_name\".\n";

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << help_message;
        return 0;
    }

    if (std::strcmp(argv[1], "-c") == 0) {
        if (argc == 2) {
            std::cout << "Provide archive name and at least one filename for compression.\n";
            return 1;
        }
        std::string archive_name = argv[2];
        std::vector<std::string> files_to_archive;
        for (int i = 3; i < argc; ++i) {
            files_to_archive.emplace_back(argv[i]);
        }
        if (files_to_archive.empty()) {
            std::cout << "Provide at least one file to archive.\n";
            return 1;
        }
        try {
            Archiver::Compress(archive_name, files_to_archive);
            std::cout << "Successful compress\n";
            return 0;
        } catch (const Archiver::ArchiverException& e) {
            std::cout << e.what() << '\n';
            return 1;
        }
    }
    if (std::strcmp(argv[1], "-d") == 0) {
        if (argc == 2) {
            std::cout << "Provide archive name to decompress.\n";
            return 1;
        }
        if (argc > 3) {
            std::cout << "Too many arguments for decompression.\n";
            return 1;
        }
        std::string path(argv[2]);
        try {
            Archiver::Decompress(path);
            std::cout << "Successful decompress\n";
            return 0;
        } catch (const Archiver::ArchiverException& e) {
            std::cout << e.what() << '\n';
            return 1;
        }
    }
    std::cout << help_message;
    return 0;
}
