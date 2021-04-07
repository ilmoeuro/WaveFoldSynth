#ifndef VOICE_H
#define VOICE_H

#include "pluginterfaces/base/ftypes.h"
#include "pluginterfaces/vst/vsttypes.h"

using namespace Steinberg;

class Voice
{
public:
    Voice(int32 delay,
          float sampleRate,
          float velocity,
          float timbreAmt,
          float timbreA,
          float timbreD,
          float timbreS,
          float timbreR,
          float gainA,
          float gainD,
          float gainS,
          float gainR,
          int16 pitch)
        : m_delay(delay)
        , m_noteOffDelay(-1)
        , m_sampleRate(sampleRate)
        , m_velocity(velocity)
        , m_timbreAmt(timbreAmt)
        , m_timbreA(timbreA)
        , m_timbreD(timbreD)
        , m_timbreS(timbreS)
        , m_timbreR(timbreR)
        , m_gainA(gainA)
        , m_gainD(gainD)
        , m_gainS(gainS)
        , m_gainR(gainR)
        , m_pitch(pitch)
        , m_phase(0.f)
        , m_noteOn(true)
        , m_timbre(0.f)
        , m_gain(0.f)
        , m_timbreStage(EnvelopeStage::A)
        , m_gainStage(EnvelopeStage::A)
    {};

    Vst::Sample32 processSample();
    void noteOff(int32 delay);
    bool isDead();
    int16 pitch();

protected:
    enum class EnvelopeStage { A, D, S, R };
    int32 m_delay;
    int32 m_noteOffDelay;
    float m_sampleRate;
    float m_velocity;
    float m_timbreAmt;
    float m_timbreA;
    float m_timbreD;
    float m_timbreS;
    float m_timbreR;
    float m_gainA;
    float m_gainD;
    float m_gainS;
    float m_gainR;
    int16 m_pitch;
    float m_phase;
    bool m_noteOn;
    float m_timbre;
    float m_gain;
    EnvelopeStage m_timbreStage;
    EnvelopeStage m_gainStage;

    float fold(float value);
    float envelope(bool timbre);
};

#endif // VOICE_H
