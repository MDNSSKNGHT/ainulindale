//
// Created by mdnssknght on 06/08/2024.
//

#include <shadowhook.h>
#include <logging.h>
#include "Hooks.hpp"

#define LOG_TAG "Norgothrond"

void hook(void) {
    //
    // Important! Do not remove. It must be at the beginning of function.
    SHADOWHOOK_STACK_SCOPE();

    LOGI("Norgothrond Init");
}

extern "C" __attribute__((constructor))
void init() {
    struct stat st;
    if (stat("/data/adb/modules/zyx_ainur_silmaril", &st) == 0 && S_ISDIR(st.st_mode)) {
        if (stat("/data/adb/modules/zyx_ainur_silmaril/disable", &st) != 0) {
            hook_all_variants();
        } else {
            LOGI("AINULINDALE CORE ERROR: Silmaril is disabled!");
        }
    } else {
        LOGI("AINULINDALE CORE ERROR: Silmaril not found (errno=%d)!", errno);
    }
}

void* orig_fff = nullptr;
void* orig_ssi = nullptr;
void* orig_isi = nullptr;

double bessi0(double x)
{
  double xh, sum, pow, ds;
  int k;
  xh = 0.5 * x;
  sum = 1.0;
  pow = 1.0;
  k = 0;
  ds = 1.0;
  while (ds > sum * DBL_EPSILON)
  {
    ++k;
    pow = pow * (xh / k);
    ds = pow * pow;
    sum = sum + ds;
  }
  return sum;
}

class SineGen {
    public:
        SineGen(double phase = 0., double step = 0.)
            : mVal(std::sin(phase)), mCosVal(std::cos(phase)), 
              mCosStep(std::cos(step)), mSinStep(std::sin(step)) {}
    
        double valueAdvance() {
            const double val = mVal;
            mVal = mVal * mCosStep + mCosVal * mSinStep;
            mCosVal = mCosVal * mCosStep - val * mSinStep;
            return val;
        }
    
        void advance() {
            const double val = mVal;
            mVal = mVal * mCosStep + mCosVal * mSinStep;
            mCosVal = mCosVal * mCosStep - val * mSinStep;
        }
    
    private:
        double mVal;
        double mCosVal;
        const double mCosStep;
        const double mSinStep;
    };
    
    class SineGenGen {
    public:
        SineGenGen(double phase = 0., double step = 0., double /* unused max_phase */ = 0.)
            : mPhase(phase), mStep(step) {}
    
        SineGen valueAdvance() {
            SineGen sg(mPhase, mStep);
            mPhase += mStep;
            return sg;
        }
    
    private:
        double mPhase;
        const double mStep;
    };
    
    double toint(double val, int64_t max_val, double* err) {
        double scaled = val * max_val + *err;
        double rounded = std::round(scaled);
        *err = scaled - rounded;
        return rounded;
    }
    
    double toint_plain(double val, int64_t max_val) {
        return std::round(val * max_val);
    }
    
    #define sqr(x) ((x) * (x))
    
    double computeBeta(double stopBandAtten) {
        if (stopBandAtten > 50.0) return 0.1102 * (stopBandAtten - 8.7);
        else if (stopBandAtten >= 21.0) return 0.5842 * std::pow(stopBandAtten - 21.0, 0.4) + 0.07886 * (stopBandAtten - 21.0);
        else return 0.0;
    }
    
    inline double computeWindowedSincMinimumPassbandValue(double stopBandAttenuationDb) {
        return 1. - std::pow(10., stopBandAttenuationDb * (-1. / 20.));
    }
    
    double get_stopband() {
        char value[PROP_VALUE_MAX];
        property_get("ro.audio.resampler.psd.stopband", value, "90");
        return std::atof(value);
    }
    
    int get_halflength() {
        char value[PROP_VALUE_MAX];
        property_get("ro.audio.resampler.psd.halflength", value, "960");
        return std::atoi(value);
    }

    // reversed firgen
    // https://cs.android.com/android/platform/superproject/main/+/main:frameworks/av/media/libaudioprocessing/AudioResamplerDyn.cpp;l=764;bpv=0;bpt=1
    void firKaiserGen(void* coef, int L, int halfNumCoef, double stopBandAtten, double fcr, double atten, int type_flag) {
        LOGI("AINULINDALE CORE: FIR exec");
    
        const int N = L * halfNumCoef;
        const double beta = computeBeta(stopBandAtten);
        const double xstep = (2.0 * M_PI) * fcr / L;
        const double xfrac = 1.0 / N;
        const double yscale = atten * L / (bessi0(beta) * M_PI);
    
        SineGenGen sgg(0.0, xstep, L * xstep);
        for (int i = 0; i <= L; ++i) {
            SineGen sg = sgg.valueAdvance();
            double err = 0;
            for (int j = 0, ix = i; j < halfNumCoef; ++j, ix += L) {
                double y;
                if (ix) {
                    double x = static_cast<double>(ix);
                    y = bessi0(beta * std::sqrt(1.0 - sqr(x * xfrac))) * yscale * sg.valueAdvance() / x;
                } else {
                    y = 2.0 * atten * fcr;
                    sg.advance();
                }
                if (type_flag == 0) { // int16_t
                    int64_t max_val = 1LL << 15;
                    *static_cast<int16_t*>(coef) = static_cast<int16_t>(toint(y, max_val, &err));
                    coef = static_cast<int16_t*>(coef) + 1;
                } else if (type_flag == 1) { // int32_t
                    int64_t max_val = 1LL << 31;
                    *static_cast<int32_t*>(coef) = static_cast<int32_t>(toint_plain(y, max_val));
                    coef = static_cast<int32_t*>(coef) + 1;
                } else { // float
                    *static_cast<float*>(coef) = static_cast<float>(y);
                    coef = static_cast<float*>(coef) + 1;
                }
            }
        }
    }
    
    // reversed orig templates
    // https://cs.android.com/android/platform/superproject/main/+/main:frameworks/av/media/libaudioprocessing/AudioResamplerDyn.cpp;l=764;bpv=0;bpt=1

    void createKaiserFir_fff(void* this_ptr, void* constants, double unused_stopBandAtten, double fcr) {
        if (orig_fff) {
            reinterpret_cast<void(*)(void*, void*, double, double)>(orig_fff)(this_ptr, constants, unused_stopBandAtten, fcr);
        }
        float* coef = *static_cast<float**>(constants);
        // needs offset from original lib, not needed with hardcoded vals & prop readouts
    //    int halfNumCoef = *static_cast<int*>(static_cast<char*>(constants) + sizeof(void*));
    //    int L = *static_cast<int*>(static_cast<char*>(this_ptr) + 0x8);
        int halfNumCoef = get_halflength_property();
        int L = 512;
        LOGI("FFF: L=%d, halfNumCoef=%d, fcr=%f", L, halfNumCoef, fcr);
        double stopBandAtten = get_stopband_property();
        double attenuation = computeWindowedSincMinimumPassbandValue(stopBandAtten);
        double atten = attenuation * attenuation;
        firKaiserGen(coef, L, halfNumCoef, stopBandAtten, fcr, atten, 2);
    }
    
    void createKaiserFir_ssi(void* this_ptr, void* constants, double unused_stopBandAtten, double fcr) {
        if (orig_ssi) {
            reinterpret_cast<void(*)(void*, void*, double, double)>(orig_ssi)(this_ptr, constants, unused_stopBandAtten, fcr);
        }
        int16_t* coef = *static_cast<int16_t**>(constants);
        // needs offset from original lib, not needed with hardcoded vals & prop readouts
    //    int halfNumCoef = *static_cast<int*>(static_cast<char*>(constants) + sizeof(void*));
    //    int L = *static_cast<int*>(static_cast<char*>(this_ptr) + 0x8);
        int halfNumCoef = get_halflength_property();
        int L = 512;
        LOGI("SSI: L=%d, halfNumCoef=%d, fcr=%f", L, halfNumCoef, fcr);
        double stopBandAtten = get_stopband_property();
        double attenuation = computeWindowedSincMinimumPassbandValue(stopBandAtten);
        double atten = attenuation * attenuation;
        firKaiserGen(coef, L, halfNumCoef, stopBandAtten, fcr, atten, 0);
    }

    void createKaiserFir_isi(void* this_ptr, void* constants, double unused_stopBandAtten, double fcr) {
        if (orig_isi) {
            reinterpret_cast<void(*)(void*, void*, double, double)>(orig_isi)(this_ptr, constants, unused_stopBandAtten, fcr);
        }
        int16_t* coef = *static_cast<int16_t**>(constants);
        // needs offset from original lib, not needed with hardcoded vals & prop readouts
    //    int halfNumCoef = *static_cast<int*>(static_cast<char*>(constants) + sizeof(void*));
    //    int L = *static_cast<int*>(static_cast<char*>(this_ptr) + 0x8);
        int halfNumCoef = get_halflength_property();
        int L = 512;
        LOGI("ISI: L=%d, halfNumCoef=%d, fcr=%f", L, halfNumCoef, fcr);
        double stopBandAtten = get_stopband_property();
        double attenuation = computeWindowedSincMinimumPassbandValue(stopBandAtten);
        double atten = attenuation * attenuation;
        firKaiserGen(coef, L, halfNumCoef, stopBandAtten, fcr, atten, 1);
    }
