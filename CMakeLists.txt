# Top-level CMake for SoftProjector
# Prefer: cmake -S . -B build  (or open this file / src/CMakeLists.txt in Qt Creator)
cmake_minimum_required(VERSION 3.21)
project(SoftProjectorRoot LANGUAGES CXX)
add_subdirectory(src)
