# Huffman Compression

A C++ implementation of the Huffman coding compression algorithm that provides efficient file compression and decompression using variable-length prefix codes.

## Overview

Huffman coding is a lossless data compression algorithm that assigns variable-length codes to characters based on their frequency of occurrence. More frequent characters get shorter codes, while less frequent characters get longer codes. This implementation provides both compression and decompression functionality through a command-line interface.

## Features

- File compression using Huffman coding
- File decompression
- Command-line interface
- Support for any text file format
- Efficient binary encoding
- Progress tracking during compression/decompression

## Requirements

- C++ compiler (C++17 or later)
- CMake (3.10 or later)

## Building the Project

1. Clone this repository:
```bash
git clone https://github.com/yourusername/HuffmanCompressionH.git
cd HuffmanCompressionH
```

2. Create a build directory and navigate to it:
```bash
mkdir build
cd build
```

3. Generate build files:
```bash
cmake ..
```

4. Build the project:
```bash
cmake --build .
```

The executable will be created in the `build/bin` directory as `huffman.exe`.

## Usage

The program is used from the command line with the following syntax:

```bash
./huffman <mode> <input_file> <output_file>
```

### Parameters:
- `mode`: 'c' for compression or 'd' for decompression
- `input_file`: Path to the input file
- `output_file`: Path to the output file

### Examples:

Compress a file:
```bash
./huffman c input.txt compressed.bin
```

Decompress a file:
```bash
./huffman d compressed.bin decompressed.txt
```

## How It Works

1. **Compression Process**:
   - Count frequency of each character in the input file
   - Build Huffman tree based on frequencies
   - Generate Huffman codes for each character
   - Convert input file to binary using Huffman codes
   - Save compressed data along with Huffman tree

2. **Decompression Process**:
   - Read Huffman tree from compressed file
   - Reconstruct original text using Huffman codes
   - Write decompressed data to output file

## Project Structure

- `src/main.cpp`: Main program entry point
- `src/FileHandler.h`: File handling class declaration
- `src/FileHandler.cpp`: File handling implementation
- `src/HuffmanTree.h`: Huffman tree class declaration
- `src/HuffmanTree.cpp`: Huffman tree implementation
- `src/Node.h`: Huffman tree node class declaration
- `src/Node.cpp`: Huffman tree node implementation
- `CMakeLists.txt`: CMake build configuration

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Author

Your Name
