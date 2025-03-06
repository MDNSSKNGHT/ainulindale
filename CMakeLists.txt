cmake_minimum_required(VERSION 3.10.2)

project(ainulindale)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Defines all the dependencies as targets
include(external/dependencyTarget.cmake)

# Tree-wide
include_directories(include)

# audioserver proxy source code
add_subdirectory(source)
