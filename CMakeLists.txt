cmake_minimum_required(VERSION 3.10.2)

project(audioserver)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Strip binaries (otherwise audioserver will be a large binary)
# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

include_directories(include)

# Custom shared library injector
add_library(library_injector
    STATIC
    source/library_injector/Injector.cpp
    source/library_injector/RemoteProcess.cpp)
target_include_directories(library_injector PUBLIC source/library_injector)

# mettle's libreflect (from Rapid7)
set(LIBREFLECT_PATH source_deps/mettle_libreflect)
file (GLOB LIBREFLECT_SRC ${LIBREFLECT_PATH}/src/*.c)
add_library(libreflect STATIC ${LIBREFLECT_SRC})
target_compile_options(libreflect PUBLIC -Wall -Werror -pedantic -std=gnu99)
target_include_directories(libreflect PUBLIC
    ${LIBREFLECT_PATH}/include
    ${LIBREFLECT_PATH}/arch/linux/aarch64)
target_link_libraries(libreflect log)

# audioserver proxy binary
add_executable(audioserver source/audioserver.cpp)
target_link_libraries(audioserver library_injector log)
