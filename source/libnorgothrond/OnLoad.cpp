//
// Created by mdnssknght on 03/06/2025.
//

#include <cstdlib>
#include <linux/stat.h>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <shadowhook.h>

#define LOG_TAG "Hooks"
#include <logging.hpp>

#include "AudioProps.hpp"
#include "Constants.hpp"
#include "Hooks.hpp"

void set_audio_props() {
    const prop_info *pi;
    char value[PROP_VALUE_MAX];

    auto cb = [](void *cookie, const char *, const char *value, uint32_t) {
        *(double *)cookie = std::atof(value);
    };

    pi = __system_property_find("ro.audio.resampler.psd.stopband");
    if (pi != nullptr)
        __system_property_read_callback(pi, cb,  &stopBand);

    pi = __system_property_find("ro.audio.resampler.psd.halflength");
    if (pi != nullptr)
        __system_property_read_callback(pi, cb,  &halfLength);
}

void __attribute__((constructor)) OnLoad() {
   struct stat st;

   // Bail if MODULE_AINUR path does not exist or if it's not a directory or if
   // MODULE_AINUR_DOWN is present otherwise continue with our hooks.
   if (stat(MODULE_AINUR, &st) != 0 || !S_ISDIR(st.st_mode) || stat(MODULE_AINUR_DOWN, &st) == 0) {
       return;
   }

   // Set the audio props.
   // Because system property lookup is pretty expensive we decide to cache it.
   set_audio_props();

   // Initialize shadowhook framework with debug active.
   shadowhook_init(SHADOWHOOK_MODE_SHARED, true);

   shadowhook_hook_sym_name(LIBAUDIOPROCESSING, LIBAUDIOPROCESSING_KAISER_FFF, (void *)createKaiserFir_fff, nullptr);
   shadowhook_hook_sym_name(LIBAUDIOPROCESSING, LIBAUDIOPROCESSING_KAISER_SSI, (void *)createKaiserFir_ssi, nullptr);
   shadowhook_hook_sym_name(LIBAUDIOPROCESSING, LIBAUDIOPROCESSING_KAISER_ISI, (void *)createKaiserFir_isi, nullptr);
}
