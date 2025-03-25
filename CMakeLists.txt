cmake_minimum_required(VERSION 3.10)
project(HuffmanCompressor VERSION 1.0)

# Imposta lo standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Aggiungi la directory include
include_directories(include)

# Crea la libreria principale
add_library(huffman_lib 
    src/FileHandler.cpp
    src/HuffmanNode.cpp
    src/HuffmanTree.cpp
)

# Crea l'eseguibile principale
add_executable(huffman_compressor src/main.cpp)
target_link_libraries(huffman_compressor huffman_lib)

# Abilita i test (vedremo questa parte dopo)
enable_testing()
add_subdirectory(tests)
