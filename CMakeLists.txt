cmake_minimum_required(VERSION 3.10.2)

project(ainulindale)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tree-wide
include_directories(include)

# Dependencies source code
add_subdirectory(source_deps)

# audioserver proxy source code
add_subdirectory(source)
