project(ECE180)
set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8.4)
add_executable(ECE180 Command.cpp Command.hpp Options.cpp Options.hpp main.cpp AddFile.hpp AddFile.cpp Huffman.cpp Huffman.hpp huffman.h)
