# mettle's' libreflect (from Rapid7)
file (GLOB LIBREFLECT_SRC mettle_libreflect/src/*.c)
add_library(reflect STATIC ${LIBREFLECT_SRC})
target_compile_options(reflect PUBLIC -Wall -Werror -pedantic)
target_include_directories(reflect PUBLIC
    mettle_libreflect/include
    mettle_libreflect/arch/linux/aarch64)
target_link_libraries(reflect log)

# shadowhook from bytedance
if(${ANDROID_ABI} STREQUAL "arm64-v8a")
    set(ARCH "arm64")
    set(ARCH_LINK_FLAGS "-Wl,-z,max-page-size=16384")
elseif(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set(ARCH "arm")
    set(ARCH_LINK_FLAGS "")
endif()

file(GLOB SHADOWHOOK_SRC
    shadowhook/*.c
    shadowhook/arch/${ARCH}/*.c
    shadowhook/common/*.c
    shadowhook/third_party/*/*.c)
add_library(shadowhook SHARED ${SHADOWHOOK_SRC})
target_compile_features(shadowhook PUBLIC c_std_17)
# Remove -Werror to build with any NDK version
target_compile_options(shadowhook PUBLIC -std=c17 -Weverything)
target_include_directories(shadowhook PUBLIC
    shadowhook/.
    shadowhook/include
    shadowhook/arch/${ARCH}
    shadowhook/common
    shadowhook/third_party/xdl
    shadowhook/third_party/bsd
    shadowhook/third_party/lss)
target_link_libraries(shadowhook log)

target_link_options(shadowhook PUBLIC ${ARCH_LINK_FLAGS})
if(USEASAN)
    target_compile_options(shadowhook PUBLIC -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(shadowhook PUBLIC -fsanitize=address)
else()
    target_compile_options(shadowhook PUBLIC -Oz -ffunction-sections -fdata-sections)
    target_link_options(shadowhook PUBLIC -Oz -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/shadowhook/shadowhook.map.txt)
endif()
