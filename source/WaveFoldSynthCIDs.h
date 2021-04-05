//------------------------------------------------------------------------
// Copyright(c) 2021 Ilmo Euro.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace IlmoEuro {
//------------------------------------------------------------------------
static const Steinberg::FUID kWaveFoldSynthProcessorUID (0xF92508FE, 0x540557E2, 0xB860061B, 0xC3E97A16);
static const Steinberg::FUID kWaveFoldSynthControllerUID (0x265C6139, 0xF8F85CF1, 0xACF3664A, 0x86AB925F);

#define WaveFoldSynthVST3Category "Instrument"

enum {
    kParamTimbreA = 100,
    kParamTimbreD,
    kParamTimbreS,
    kParamTimbreR,
    kParamTimbre,
    kParamGainA,
    kParamGainD,
    kParamGainS,
    kParamGainR
};

//------------------------------------------------------------------------
} // namespace MyCompanyName
