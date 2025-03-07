cmake_minimum_required(VERSION 3.10.2)

project(ainulindale)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ARCH aarch64)
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ARCH armv7a)
endif()

file(GLOB SRC third_party/libreflect/src/*.c)
add_library(reflect STATIC ${SRC})
target_compile_options(reflect PUBLIC -Wall -Werror -pedantic -std=gnu99)
target_include_directories(reflect PUBLIC
    third_party/libreflect/include
    third_party/libreflect/arch/linux/${ARCH})

add_executable(android-ndk src/main.cpp)
