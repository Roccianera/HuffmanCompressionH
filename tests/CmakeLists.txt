# Scarica e configura Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Aggiungi i file di test
add_executable(
  huffman_tests
  HuffmanTests.cpp
)

# Collega Google Test e la tua libreria
target_link_libraries(
  huffman_tests
  gtest_main
  huffman_lib
)

# Registra il test
include(GoogleTest)
gtest_discover_tests(huffman_tests)
