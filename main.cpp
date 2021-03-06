#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "Compressor.hpp"
#include "Decompressor.hpp"
#include "Exceptions.hpp"

const char* help_message =
    "Compress/decompress files using Huffman coding.\n"
    "To compress: \"archiver -c archive_name file1 [file2 ...]\".\n"
    "To decompress: \"archiver -d archive_name\".\n";

int compress(int argc, char* argv[]) {
    if (argc == 2) {
        std::cout << "Provide archive name and at least one filename for compression.\n";
        return 1;
    }
    std::string archive_name = argv[2];
    std::vector<std::ifstream> file_streams;
    file_streams.resize(static_cast<size_t>(argc) - 3);
    std::vector<Archiver::InputStreamData> files_to_archive;
    for (int i = 3; i < argc; ++i) {
        std::string filename{argv[i]};
        if (!std::filesystem::exists(filename)) {
            std::cout << "File " << filename << " doesn't exist";
            return 1;
        }
        file_streams.emplace_back(filename);
        files_to_archive.push_back({.stream = dynamic_cast<std::istream&>(file_streams.back()),
                                    .name = std::move(filename)});
    }
    if (files_to_archive.empty()) {
        std::cout << "Provide at least one file to archive.\n";
        return 1;
    }
    try {
        std::ofstream out(archive_name);
        Archiver::Compressor compressor;
        compressor.Compress(files_to_archive, out);
        std::cout << "Successful compress\n";
        return 0;
    } catch (const Archiver::ArchiverException& e) {
        std::cout << e.what() << '\n';
        return 1;
    }
}

int decompress(int argc, char* argv[]) {
    if (argc == 2) {
        std::cout << "Provide archive name to decompress.\n";
        return 1;
    }
    if (argc > 3) {
        std::cout << "Too many arguments for decompression.\n";
        return 1;
    }
    std::string path(argv[2]);
    std::vector<std::string> filenames;
    try {
        Archiver::ControlCharacters control_character;
        std::ifstream fin(path);
        Archiver::InputBitStream ibitstream(fin);
        Archiver::Decompressor decompressor;

        do {
            auto stream_meta = decompressor.DecompressStreamMeta(ibitstream);
            filenames.push_back(stream_meta.name);
            std::ofstream fout(stream_meta.name);
            Archiver::OutputBitStream obitstream(fout);
            control_character = decompressor.DecompressStreamData(ibitstream,
                                                                  obitstream,
                                                                  stream_meta.binary_tree.get());
        } while (control_character != Archiver::ControlCharacters::ARCHIVE_END);

        std::cout << "Successful decompress\n";
        return 0;
    } catch (const Archiver::ArchiverException& e) {
        std::cout << e.what() << '\n';
        for (const auto& filename : filenames) {
            std::filesystem::remove(filename);
        }
        return 1;
    }
}


int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << help_message;
        return 0;
    }

    if (std::strcmp(argv[1], "-c") == 0) {
        return compress(argc, argv);
    }
    if (std::strcmp(argv[1], "-d") == 0) {
        return decompress(argc, argv);
    }
    std::cout << help_message;
    return 0;
}
