# Huffman Archiver

Huffman Archiver is an archiver that uses [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) to compress data.

It's written in C++20. Build is successful with Clang 12.

## Build
You need `cmake` and `make` installed if you want to build the project.
It compiles well
```
mkdir build
cd build
cmake ..
make
```

## Run tests
The project is covered with tests that are located at [tests/](tests/)
To run them do
```
cd build
./tests/Run_tests
```

## Usage
The compiled archiver is located in your build directory. It's filename is `archiver`.

* Use `./archiver -c archive_name file1 [file2 ...]` to compress files.
* Use `./archiver -d archive_name` to decompress an archive.
