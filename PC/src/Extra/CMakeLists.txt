cmake_minimum_required(VERSION 3.16)

project(mandelbrot-resources LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS -no-pie)

add_executable(ResourceCompiler ResourceCompiler.cpp ShaderReader.cpp ImageHexDump.cpp FontHexDump.cpp)