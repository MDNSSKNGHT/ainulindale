#!/usr/bin/env python

import subprocess
import os
import os.path as path

# Export the Android NDK path to ANDROID_NDK env variable
android_ndk = os.environ.get("ANDROID_NDK")

# Export the Android NDK path to ANDROID_NDK 23.2.8568313 env variable
android_ndk_legacy = os.environ.get("ANDROID_NDK_LEGACY")

# Absolute path where this script resides in
project_dir = path.dirname(path.realpath(__file__))

def targets_cmake(parties_dict):
    with open(f'{project_dir}/cmake/ThirdParty.cmake.template', 'r') as file:
        template = file.read()

    for abi in ["armeabi-v7a", "arm64-v8a"]:
        os.makedirs(f'{project_dir}/cmake/{abi}', exist_ok=True)

        with open(f'cmake/{abi}/ThirdParty.cmake', 'w') as file:
            for party in parties_dict:
                file.write(template.format(
                    target = parties_dict[party][0],
                    linkType = parties_dict[party][1].upper(),
                    libPath = f'{project_dir}/third_party/build/{abi}/{party}/{parties_dict[party][2]}',
                    includePath = f'{project_dir}/third_party/{party}/include',
                    linkLibraries = ''))

def build_party(prj, ndk):
    for abi in ["armeabi-v7a", "arm64-v8a"]:
        subprocess.run([
            "cmake",
            f"-Hthird_party/{prj}",
            f"-Bthird_party/build/{abi}/{prj}",
            f"-DANDROID_ABI={abi}",
            "-DANDROID_PLATFORM=latest",
            f"-DANDROID_NDK={ndk}",
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
            f"-DCMAKE_TOOLCHAIN_FILE={ndk}/build/cmake/android.toolchain.cmake",
            "-GNinja"])
        subprocess.run(["cmake", "--build", f"third_party/build/{abi}/{prj}"])

def main():
    build_party('libreflect', android_ndk)
    build_party('shadowhook', android_ndk_legacy)

    targets_cmake({
        'libreflect': ['reflect', 'static', 'libreflect.a'],
        'shadowhook': ['shadowhook', 'shared', 'libshadowhook.so'], })

if __name__ == '__main__':
    main()
