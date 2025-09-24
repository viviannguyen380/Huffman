cmake_minimum_required(VERSION 3.29)
project(Huffman)

set(CMAKE_CXX_STANDARD 20)

add_executable(Huffman main.cpp
        Node.h
        Storage.h
        Storage.cpp
        Huffman.h
        Huffman.cpp)
