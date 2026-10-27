cmake_minimum_required(VERSION 3.10)
project(rs_decoder_tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

# Test the Hulotte/StreamPU copy and the self-contained SW/src copy separately.
add_executable(rs_decoder_tests_custom
    rs_decoder_tests.cpp
    ../src/custom/RS_Decoder.cpp
    ../src/custom/RS_tools.cpp)
target_include_directories(rs_decoder_tests_custom PRIVATE ../src/custom)
add_test(NAME rs_decoder_tests_custom COMMAND rs_decoder_tests_custom)

add_executable(rs_decoder_tests_standalone
    rs_decoder_tests.cpp
    ../../src/RS_Decoder.cpp
    ../../src/RS_tools.cpp)
target_include_directories(rs_decoder_tests_standalone PRIVATE ../../include)
add_test(NAME rs_decoder_tests_standalone COMMAND rs_decoder_tests_standalone)
