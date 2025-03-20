cmake_minimum_required(VERSION 3.10)
project(project-name)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(HuffmanTreeTest test/HuffmanTreeTest.cpp)
target_link_libraries(HuffmanTreeTest gtest gtest_main)

add_test(NAME HuffmanTreeTest COMMAND HuffmanTreeTest)