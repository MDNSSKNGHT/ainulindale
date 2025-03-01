cmake_minimum_required(VERSION 3.10.2)

project(audioserver)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

# Custom shared library injector
add_library(library_injector
    STATIC
    source/library_injector/Injector.cpp
    source/library_injector/RemoteProcess.cpp)
target_include_directories(library_injector PUBLIC
    source/library_injector/Injector.hpp
    source/library_injector/RemoteProcess.hpp)

add_executable(audioserver source/main.cpp)
target_link_libraries(audioserver library_injector log)
