# Custom shared library injector
add_library(injector
    STATIC
    injector/Injector.cpp
    injector/RemoteProcess.cpp)
target_include_directories(injector PUBLIC injector)

# libnorgothrond
add_library(norgothrond
    SHARED
    libnorgothrond/Hooks.cpp
    libnorgothrond/OnLoad.cpp)
target_link_libraries(norgothrond shadowhook log)

# audioserver proxy binary
add_executable(audioserver audioserver.cpp)
target_include_directories(audioserver PUBLIC)
target_link_libraries(audioserver injector reflect log)
