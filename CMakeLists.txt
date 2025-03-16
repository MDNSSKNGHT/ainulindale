cmake_minimum_required(VERSION 3.10.2)

project(ainulindale)

include(cmake/${ANDROID_ABI}/ThirdParty.cmake)

include_directories(include)

add_library(norgothrond SHARED
    src/libnorgothrond/OnLoad.cpp
    src/libnorgothrond/Hooks.cpp)
target_link_libraries(norgothrond log shadowhook)

add_library(injector STATIC
    src/injector/RemoteProcess.cpp
    src/injector/Injector.cpp)
target_include_directories(injector PUBLIC src/injector)
target_link_libraries(injector log)

add_executable(audioserver src/audioserver.cpp)
target_link_libraries(audioserver reflect injector)
