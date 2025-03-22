cmake_minimum_required(VERSION 3.10)
project(HuffmanCompression)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/FileHandler.cpp
    src/HuffmanTree.cpp
    src/HuffmanNode.cpp
)

# Add header files
set(HEADERS
    include/FileHandler.h
    include/HuffmanTree.h
    include/HuffmanNode.h
)

# Create executable
add_executable(huffman ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(huffman PRIVATE include)

# Enable warnings
if(MSVC)
    target_compile_options(huffman PRIVATE /W4)
else()
    target_compile_options(huffman PRIVATE -Wall -Wextra)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 