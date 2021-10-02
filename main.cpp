#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>

#include "Archiver.hpp"

const char* help_message =
    "Compress/decompress files using Huffman coding.\n"
    "To compress: \"archiver -c archive_name file1 [file2 ...]\".\n"
    "To decompress: \"archiver -d archive_name\".\n";

bool FileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

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
            std::string path(argv[i]);
            if (!FileExists(path)) {
                std::cout << "file " << path << " doesn't exist.\n";
                return 1;
            }
            files_to_archive.push_back(std::move(path));
        }
        if (files_to_archive.empty()) {
            std::cout << "Provide at least one file to archive.\n";
            return 1;
        }
        Archiver::Compress(archive_name, files_to_archive);
        return 0;
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
        if (!FileExists(path)) {
            std::cout << "file " << path << " doesn't exist.\n";
            return 1;
        }
        return 0;
    }
    std::cout << help_message;
    return 0;
}
