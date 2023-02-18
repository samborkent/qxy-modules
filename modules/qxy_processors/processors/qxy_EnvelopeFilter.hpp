#pragma once

namespace qxy
{

    template <typename Type>
    class EnvelopeFilter
    {
    public:
        EnvelopeFilter() noexcept = default;

        Type processSample (const Type sample) noexcept
        {
            const Type filteredSample = Type (0.5) * (prevSample + sample);
            prevSample = sample;
            return filteredSample;
        }

        Type processSample (const Type sample, const Type nextSample) noexcept
        {
            const Type filteredSample = Type (0.5) * (sample + Type (0.5) * (prevSample + nextSample));
            prevSample = sample;
            return filteredSample;
        }

    private:
        Type prevSample = 0.0f;
    };

} // namespace qxy