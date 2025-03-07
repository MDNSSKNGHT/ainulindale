#!/usr/bin/env python

import subprocess
import os

# Export the Android NDK path to ANDROID_NDK env variable
android_ndk = os.environ.get("ANDROID_NDK")

for abi in ["armeabi-v7a", "arm64-v8a"]:
    subprocess.run([
        "cmake",
        "-H.",
        f"-Bbuild/{abi}",
        f"-DANDROID_ABI={abi}",
        "-DANDROID_PLATFORM=latest",
        f"-DANDROID_NDK={android_ndk}",
        f"-DCMAKE_TOOLCHAIN_FILE={android_ndk}/build/cmake/android.toolchain.cmake",
        "-GNinja"])
    subprocess.run(["cmake", "--build", "build/" + abi])
