#include <shadowhook.h>

#define LOG_TAG "libnorgothrond"
#include "logging.hpp"

#include "Hooks.hpp"

void __attribute__((constructor)) OnLoad() {
    LOGI("Hello from norgothrond!");

    shadowhook_init(SHADOWHOOK_MODE_SHARED, true);

    shadowhook_hook_sym_name("/system/lib64",
        "_ZN7android12AudioFlinger11instantiateEv",
        (void *)AudioFlinger_instantiate, nullptr);

    shadowhook_hook_sym_name("/system/lib64/libaudioprocessing.so",
        "_ZN7android17AudioResamplerDynIfffE15createKaiserFirERNS1_9ConstantsEdd",
        (void *)createKaiserFir_fff, nullptr);

    shadowhook_hook_sym_name("/system/lib64/libaudioprocessing.so",
        "_ZN7android17AudioResamplerDynIssiE15createKaiserFirERNS1_9ConstantsEdd",
        (void *)createKaiserFir_ssi, nullptr);

    shadowhook_hook_sym_name("/system/lib64/libaudioprocessing.so",
        "_ZN7android17AudioResamplerDynIisiE15createKaiserFirERNS1_9ConstantsEdd",
        (void *)createKaiserFir_isi, nullptr);
}
