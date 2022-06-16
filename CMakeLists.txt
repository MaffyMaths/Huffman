cmake_minimum_required(VERSION 3.20)
project(Huffman C)

set(CMAKE_C_STANDARD 11)

add_executable(Huffman main.c compress.c compress.h decompress.c decompress.h)
