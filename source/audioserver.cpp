#include <reflect.h>

#include "audioserver.hpp"

int main(int argc, char *argv[]) {
    reflect_execves(audioserver_embedded, argv + 1, NULL, (size_t *)argv - 1);
    return 0;
}
