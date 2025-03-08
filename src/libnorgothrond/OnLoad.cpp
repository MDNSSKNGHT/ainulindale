#define LOG_TAG "libnorgothrond"
#include "logging.hpp"

void __attribute__((constructor)) OnLoad() {
    LOGI("Hello from norgothrond!");
}
