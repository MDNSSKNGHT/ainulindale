#include <reflect.h>

#include "audioserver.hpp"

int main(int argc, char *argv[]) {
    reflect_execves(audioserver, argv + 1, nullptr, (size_t *)argv - 1);
    return 0;
}
