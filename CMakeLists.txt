cmake_minimum_required(VERSION 3.10.2)

project(ainulindale)

# For the LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ARCH aarch64)
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ARCH armv7a)
endif()

file(GLOB SRC third_party/libreflect/src/*.c)
add_library(reflect STATIC ${SRC})
set_target_properties(reflect PROPERTIES C_STANDARD 99)
target_compile_options(reflect PUBLIC -Wall -Werror -pedantic)
target_include_directories(reflect PUBLIC
    third_party/libreflect/include
    third_party/libreflect/arch/linux/${ARCH})

add_library(norgothrond SHARED src/libnorgothrond/OnLoad.cpp)
target_link_libraries(norgothrond log)

add_library(injector STATIC
    src/injector/RemoteProcess.cpp
    src/injector/Injector.cpp)
target_include_directories(injector PUBLIC src/injector)

add_executable(audioserver src/audioserver.cpp)
target_link_libraries(audioserver reflect)
