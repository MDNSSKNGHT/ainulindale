cmake_minimum_required(VERSION 3.10.2)

project(libreflect)

if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ARCH aarch64)
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ARCH armv7a)
endif()

file(GLOB SRC src/*.c)
add_library(reflect STATIC ${SRC})
set_target_properties(reflect PROPERTIES C_STANDARD 99)
target_compile_options(reflect PUBLIC -Wall -Werror -pedantic)
target_include_directories(reflect PUBLIC include arch/linux/${ARCH})
