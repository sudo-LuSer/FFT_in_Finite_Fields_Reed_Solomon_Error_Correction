cmake_minimum_required(VERSION 3.10)
project(ECC_ReedSolomon)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(RS_RUN 
    src/main.cpp 
    src/RS_Encoder.cpp 
    src/RS_Decoder.cpp 
    src/RS_tools.cpp
)