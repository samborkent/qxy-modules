#pragma once

namespace qxy
{

    class EnvelopeFilter
    {
    public:
        EnvelopeFilter() noexcept = default;

        float apply (const float sample, const float prevSample) noexcept
        {
            return 0.5f * (sample + prevSample);
        }
    };

} // namespace qxy