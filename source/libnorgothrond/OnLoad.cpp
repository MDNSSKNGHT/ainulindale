//
// Created by mdnssknght on 06/08/2024.
//

#include <shadowhook.h>

#define LOG_TAG "Hooks"
#include <logging.h>

//
// We include our hooks here.
#include "Hooks.hpp"

void __attribute__((constructor)) OnLoad() {
   // LOGI("AINULINDALE CORE: kaiserFIR hijack init");

    //
    // We initialize our hook framework.
    shadowhook_init(SHADOWHOOK_MODE_SHARED, true);

    //void hook_all_variants() {
        shadowhook_hook_sym_name("libaudioprocessing.so", "_ZN7android17AudioResamplerDynIfffE15createKaiserFirERNS1_9ConstantsEdd", (void*)createKaiserFir_fff, &orig_fff);
        shadowhook_hook_sym_name("libaudioprocessing.so", "_ZN7android17AudioResamplerDynIssiE15createKaiserFirERNS1_9ConstantsEdd", (void*)createKaiserFir_ssi, &orig_ssi);
        shadowhook_hook_sym_name("libaudioprocessing.so", "_ZN7android17AudioResamplerDynIisiE15createKaiserFirERNS1_9ConstantsEdd", (void*)createKaiserFir_isi, &orig_isi);
    //}
}