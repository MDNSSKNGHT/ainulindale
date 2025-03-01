# mettle's' libreflect (from Rapid7)
file (GLOB LIBREFLECT_SRC mettle_libreflect/src/*.c)
add_library(reflect STATIC ${LIBREFLECT_SRC})
target_compile_options(reflect PUBLIC -Wall -Werror -pedantic)
target_include_directories(reflect PUBLIC
    mettle_libreflect/include
    mettle_libreflect/arch/linux/aarch64)
target_link_libraries(reflect log)
