//------------------------------------------------------------------------
// Copyright(c) 2021 Ilmo Euro.
//------------------------------------------------------------------------

#include "WaveFoldSynthProcessor.h"
#include "WaveFoldSynthCIDs.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

using namespace Steinberg;

namespace IlmoEuro {
//------------------------------------------------------------------------
// WaveFoldSynthProcessor
//------------------------------------------------------------------------
WaveFoldSynthProcessor::WaveFoldSynthProcessor ()
{
    //--- set the wanted controller for our processor
    setControllerClass (kWaveFoldSynthControllerUID);
    m_voices.reserve(16);
}

//------------------------------------------------------------------------
WaveFoldSynthProcessor::~WaveFoldSynthProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::initialize (FUnknown* context)
{
    // Here the Plug-in will be instanciated

    //---always initialize the parent-------
    tresult result = AudioEffect::initialize (context);
    // if everything Ok, continue
    if (result != kResultOk)
    {
        return result;
    }

    //--- create Audio IO ------
    addAudioInput (STR16 ("Stereo In"), Vst::SpeakerArr::kStereo);
    addAudioOutput (STR16 ("Stereo Out"), Vst::SpeakerArr::kStereo);

    /* If you don't need an event bus, you can remove the next line */
    addEventInput (STR16 ("Event In"), 1);

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::terminate ()
{
    // Here the Plug-in will be de-instanciated, last possibility to remove some memory!

    //---do not forget to call parent ------
    return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::setActive (TBool state)
{
    //--- called when the Plug-in is enable/disable (On/Off) -----
    return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::process (Vst::ProcessData& data)
{
    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                if (numPoints == 0)
                {
                    continue;
                }
                // TODO: process params changing during frame
                paramQueue->getPoint(numPoints-1, sampleOffset, value);
                switch (paramQueue->getParameterId ())
                {
                case kParamTimbre: m_timbre = value; break;
                case kParamTimbreA: m_timbreA = value; break;
                case kParamTimbreD: m_timbreD = value; break;
                case kParamTimbreS: m_timbreS = value; break;
                case kParamTimbreR: m_timbreR = value; break;
                case kParamGainA: m_gainA = value; break;
                case kParamGainD: m_gainD = value; break;
                case kParamGainS: m_gainS = value; break;
                case kParamGainR: m_gainR = value; break;
                }
            }
        }
    }

    for (int32 i = 0; data.inputEvents && i < data.inputEvents->getEventCount(); i++)
    {
        Vst::Event event;
        data.inputEvents->getEvent(i, event);
        if (event.type == Vst::Event::EventTypes::kNoteOnEvent)
        {
            m_voices.emplace_back(
                        event.sampleOffset,
                        data.processContext->sampleRate,
                        m_timbre,
                        m_timbreA,
                        m_timbreD,
                        m_timbreS,
                        m_timbreR,
                        m_gainA,
                        m_gainD,
                        m_gainS,
                        m_gainR,
                        event.noteOn.pitch
            );
        }
        if (event.type == Vst::Event::EventTypes::kNoteOffEvent)
        {
            for (auto &voice: m_voices)
            {
                if (voice.pitch() == event.noteOff.pitch)
                {
                    voice.noteOff(event.sampleOffset);
                }
            }
        }
    }

    Vst::SpeakerArrangement arr;
    getBusArrangement (Vst::kOutput, 0, arr);
    int32 numChannels = Vst::SpeakerArr::getChannelCount (arr);

    for (int32 sample = 0; sample < data.numSamples; sample++)
    {
        Vst::Sample32 y = 0.f;
        for (auto &voice: m_voices)
        {
            y += voice.processSample();
        }
        for (int32 channel = 0; channel < numChannels; channel++)
        {
            float* outputChannel = data.outputs[0].channelBuffers32[channel];
            outputChannel[sample] = y;
        }
    }

    for (int i = int(m_voices.size()) - 1; i >= 0; i--)
    {
        if (m_voices.at(i).isDead())
        {
            m_voices.erase(m_voices.begin() + i);
        }
    }

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
    //--- called before any processing ----
    return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
    // by default kSample32 is supported
    if (symbolicSampleSize == Vst::kSample32)
        return kResultTrue;

    // disable the following comment if your processing support kSample64
    /* if (symbolicSampleSize == Vst::kSample64)
        return kResultTrue; */

    return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::setState (IBStream* state)
{
    // called when we load a preset, the model has to be reloaded
    IBStreamer s (state, kLittleEndian);

    if (s.readFloat(m_timbre) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_timbreA) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_timbreD) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_timbreS) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_timbreR) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_gainA) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_gainD) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_gainS) == false)
    {
        return kResultFalse;
    }

    if (s.readFloat(m_gainR) == false)
    {
        return kResultFalse;
    }

    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API WaveFoldSynthProcessor::getState (IBStream* state)
{
    // here we need to save the model
    IBStreamer s (state, kLittleEndian);

    s.writeFloat(m_timbre);
    s.writeFloat(m_timbreA);
    s.writeFloat(m_timbreD);
    s.writeFloat(m_timbreS);
    s.writeFloat(m_timbreR);
    s.writeFloat(m_gainA);
    s.writeFloat(m_gainD);
    s.writeFloat(m_gainS);
    s.writeFloat(m_gainR);

    return kResultOk;
}

//------------------------------------------------------------------------
} // namespace MyCompanyName
