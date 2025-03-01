cmake_minimum_required(VERSION 3.10.2)

project(audioserver)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(audioserver src/main.cpp)
