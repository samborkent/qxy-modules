#pragma once

namespace qxy
{

    class PitchTracker
    {
    public:
        PitchTracker() noexcept = default;

        void prepare (const juce::dsp::ProcessSpec& spec) noexcept
        {
            fs = static_cast<float> (spec.sampleRate);
            maxSamples = static_cast<int> (fs / fm);
        }

        void update (const float sample) noexcept
        {
            if (zeroSamples > maxSamples)
                zeroSamples = 0.0f;

            zeroSamples++;

            if (sample * prevSample < 0.0f)
            {
                zeroCount++;

                if (zeroCount == 2)
                {
                    freq = 0.5f * fs / static_cast<float> (zeroSamples);
                    zeroSamples = 0;
                    zeroCount = 0;
                }
            }

            prevSample = sample;
        }

        void setMinFreq (const float minFreq) noexcept
        {
            fm = minFreq;

            if (freq < fm)
                freq = minFreq;
        }

        float getFreq() const noexcept
        {
            return freq;
        }

    private:
        float prevSample = 0.0f;
        float fs = 44100.0f;
        float fm = 20.0f;
        int zeroSamples, maxSamples, zeroCount = 0;
        float freq = fm;
    };

} // namespace qxy