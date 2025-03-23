cmake_minimum_required(VERSION 3.10)
project(HuffmanCompression)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Download and configure Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Add source files for main executable
set(MAIN_SOURCES
    src/main.cpp
    src/FileHandler.cpp
    src/HuffmanTree.cpp
    src/HuffmanNode.cpp
)

# Add source files for tests
set(TEST_SOURCES
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

# Create main executable
add_executable(huffman ${MAIN_SOURCES} ${HEADERS})

# Add include directories for main executable
target_include_directories(huffman PRIVATE include)

# Enable warnings for main executable
if(MSVC)
    target_compile_options(huffman PRIVATE /W4)
else()
    target_compile_options(huffman PRIVATE -Wall -Wextra)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add test executable
add_executable(huffman_test
    tests/test_main.cpp
    tests/HuffmanNodeTest.cpp
    tests/HuffmanTreeTest.cpp
    tests/FileHandlerTest.cpp
    ${TEST_SOURCES}
)

# Link test executable with Google Test
target_link_libraries(huffman_test PRIVATE GTest::gtest_main GTest::gtest)

# Add include directories for test executable
target_include_directories(huffman_test PRIVATE include)

# Enable warnings for test executable
if(MSVC)
    target_compile_options(huffman_test PRIVATE /W4)
else()
    target_compile_options(huffman_test PRIVATE -Wall -Wextra)
endif()

# Add test to CTest
add_test(NAME huffman_test COMMAND huffman_test) 