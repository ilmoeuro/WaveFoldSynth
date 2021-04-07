#include "Voice.h"
#define _USE_MATH_DEFINES // for M_PI
#include <cmath>

Vst::Sample32 Voice::processSample()
{
    if (m_delay > 0)
    {
        m_delay--;
        return 0.f;
    }
    if (m_noteOffDelay > -1)
    {
        m_noteOffDelay--;
        if (m_noteOffDelay <= 0)
        {
            m_noteOffDelay = -1;
            m_noteOn = false;
        }
    }
    float pitchHz = 440.f * std::powf(2.f, (m_pitch - 69.f)/12.f);
    m_phase += (2.f * M_PI * pitchHz)/m_sampleRate;
    while (m_phase > 2.f * M_PI)
    {
        m_phase -= 2.f * M_PI;
    }
    return envelope(false) *
            fold((1.f + 15.f*m_timbreAmt*envelope(true)) * std::sinf(m_phase));
}

void Voice::noteOff(int32 delay)
{
    m_noteOffDelay = delay;
}

int16 Voice::pitch()
{
    return m_pitch;
}

bool Voice::isDead()
{
    return m_gain < 1e-4f && !m_noteOn;
}

float Voice::fold(float x)
{
    return std::sinf(0.9f*x + 0.05f*x*x);
}

float Voice::envelope(bool timbre)
{
    float dt = 1.f/m_sampleRate;
    EnvelopeStage &stage = timbre ? m_timbreStage : m_gainStage;
    float &value = timbre ? m_timbre : m_gain;
    float a = timbre ? m_timbreA : m_gainA;
    float d = timbre ? m_timbreD : m_gainD;
    float s = timbre ? m_timbreS : m_gainS;
    float r = timbre ? m_timbreR : m_gainR;
    if (!m_noteOn)
    {
        stage = EnvelopeStage::R;
    }
    switch (stage)
    {
    case EnvelopeStage::A:
        value += (1.f/a) * dt;
        if (value >= 1.f)
        {
            value = 1.f;
            stage = EnvelopeStage::D;
        }
        break;
    case EnvelopeStage::D:
        value -= (1.f/d) * dt;
        if (value <= s)
        {
            value = s;
            stage = EnvelopeStage::S;
        }
        break;
    case EnvelopeStage::R:
        value -= (1.f/r) * dt;
        if (value <= 0.f)
        {
            value = 0.f;
        }
    default:
        break;
    }
    return value;
}
