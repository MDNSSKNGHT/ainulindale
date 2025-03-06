#!/usr/bin/env python

import os
import shutil
import subprocess

project_dir = os.path.dirname(os.path.realpath(__file__))

def build_dependency(dep, arch):
    android_ndk = os.environ.get("ANDROID_NDK_LEGACY")
    project_dir = os.path.dirname(os.path.realpath(__file__))
    subprocess.call([
        "cmake",
        "-H./source_deps/" + dep,
        "-DCMAKE_SYSTEM_NAME=Android",
        "-DCMAKE_SYSTEM_VERSION=latest",
        "-DANDROID_PLATFORM=latest",
        "-DANDROID_ABI=" + arch,
        "-DCMAKE_ANDROID_ARCH_ABI=" + arch,
        "-DANDROID_NDK=" + android_ndk,
        "-DCMAKE_TOOLCHAIN_FILE=" + android_ndk + "/build/cmake/android.toolchain.cmake",
        "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + project_dir + "/external/" + dep,
        "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=" + project_dir + "/external/" + dep,
        "-DCMAKE_BUILD_TYPE=Debug",
        "-B./source_deps/build/" + dep,
        "-GNinja",
        "-DANDROID_STL=none"])
    subprocess.call(["cmake", "--build", "./source_deps/build/" + dep])

def create_package_file(deps_dict):
    package_file = open("external/dependencyTarget.cmake", "w")

    for dep in deps_dict:
        package_file.write(
            'if (NOT TARGET {target})\n' \
            '\tadd_library({target} SHARED IMPORTED)\n' \
            '\tset_target_properties({target} PROPERTIES\n' \
            '\t\tIMPORTED_LOCATION "{lib_path}"\n' \
            '\t\tINTERFACE_INCLUDE_DIRECTORIES "{include_path}"\n' \
            '\t\tINTERFACE_LINK_LIBRARIES "{link_libraries}")\n' \
            'endif()\n'.format(
                target=deps_dict[dep][0],
                lib_path=project_dir + '/external/' + dep + '/' + deps_dict[dep][1],
                include_path=project_dir + '/external/' + dep + '/include',
                link_libraries=''
            )
        )

build_dependency('shadowhook', 'arm64-v8a')
build_dependency('mettle_libreflect', 'arm64-v8a')

shutil.copytree("source_deps/shadowhook/include", "external/shadowhook/include", dirs_exist_ok=True)
shutil.copytree("source_deps/mettle_libreflect/include", "external/mettle_libreflect/include", dirs_exist_ok=True)

create_package_file({
    'shadowhook': ['shadowhook', 'libshadowhook.so'],
    'mettle_libreflect': ['reflect', 'libreflect.so']
})

# create_package_file('shadowhook', 'shadowhook', 'libshadowhook.so')
# create_package_file('mettle_libreflect', 'reflect', 'libreflect.so')
