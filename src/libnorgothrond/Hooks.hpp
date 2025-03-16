#pragma once

void AudioFlinger_instantiate(bool, int);

void createKaiserFir_fff(void* audioSamplerDyn, void* constants, double stopBandAtten, double fcr);
void createKaiserFir_ssi(void* audioSamplerDyn, void* constants, double stopBandAtten, double fcr);
void createKaiserFir_isi(void* audioSamplerDyn, void* constants, double stopBandAtten, double fcr);
