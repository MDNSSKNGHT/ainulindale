#include <shadowhook.h>

#define LOG_TAG "libnorgothrond"
#include "logging.hpp"

#include "Hooks.hpp"

void AudioFlinger_instantiate(bool, int) {
    SHADOWHOOK_POP_STACK();

    LOGI("%s", __FUNCTION__);
}

void createKaiserFir_fff(void* audioSamplerDyn, void* constants, double stopBandAtten, double fcr) {
    SHADOWHOOK_POP_STACK();

    LOGI("%s", __FUNCTION__);
}

void createKaiserFir_ssi(void* audioSamplerDyn, void* constants, double stopBandAtten, double fcr) {
    SHADOWHOOK_POP_STACK();

    LOGI("%s", __FUNCTION__);
}

void createKaiserFir_isi(void* audioSamplerDyn, void* constants, double stopBandAtten, double fcr) {
    SHADOWHOOK_POP_STACK();

    LOGI("%s", __FUNCTION__);
}
