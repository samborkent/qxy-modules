#pragma once

#include "qxy_EnvelopeFilter.hpp"

namespace qxy
{

    class EnvelopeFollower
    {
    public:
        EnvelopeFollower (float attackFraction, float decayFraction)
        {
            aFrac = attackFraction;
            dFrac = decayFraction;
        }

        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            fs = static_cast<float> (spec.sampleRate);
        }

        void setBpm (float tempo)
        {
            bpm = tempo;
        }

        void reset()
        {
            decay = (60.0f / bpm) / dFrac;
            attack = decay / aFrac;
            a = std::pow (0.01f, 1.0f / (fs * attack));
            b = std::pow (0.01f, 1.0f / (fs * decay));
        }

        void update (const float sample) noexcept
        {
            const float filteredInput = lpFilter->processSample (std::abs (sample));

            if (filteredInput > envelope)
            {
                // Attack
                envelope = a * envelope + (1.0f - a) * filteredInput;
                isMax = false;
            }
            else
            {
                if (!isMax)
                {
                    isMax = true;
                    maxEnvelope = envelope;
                }
                // Decay
                envelope = b * envelope + (1.0f - b) * filteredInput;
            }

            prevSample = filteredInput;
        }

        float getMax() const noexcept
        {
            return maxEnvelope;
        }

    private:
        std::unique_ptr<EnvelopeFilter<float>> lpFilter = std::make_unique<EnvelopeFilter<float>>();

        float fs = 44100.0f;
        float bpm = 120.0f;

        float aFrac, dFrac;

        float attack, decay = 0.0f;
        float a, b = 0.0f;
        float prevSample, envelope, maxEnvelope = 0.0f;
        bool isMax = false;
    };

} // namespace qxy