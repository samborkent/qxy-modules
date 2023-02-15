#pragma once

#include <juce_dsp/juce_dsp.h>

#include "../../qxy_math/math/qxy_Math.hpp"

namespace qxy
{
    enum Amp {
        quiet,
        loud
    };

    template <typename Type>
    class AmplitudeSplitter
    {
    public:
        AmplitudeSplitter() noexcept = default;

        std::pair<Type, Type> hardSplit (const Type sample, const Type threshold) noexcept
        {
            if (threshold <= 0.0f)
                return std::pair<Type (0), sample>;

            std::pair<Type, Type> splitSample (Type (0), Type (0));

            splitSample[std::abs (sample) > threshold] = sample;

            return splitSample;
        }

        std::pair<Type, Type> linearSplit (const Type sample, const Type threshold, Type width) noexcept
        {
            if (width == 0.0f)
                return hardSplit (sample, threshold);
            else if (width < 0.0f)
                width = Type (-1) * width;

            if ((threshold <= 0.0f) || (threshold - Type (0.5) * width <= 0.0f))
                return std::pair<Type (0), sample>;

            std::pair<Type, Type> splitSample (Type (0), Type (0));

            const Type start = threshold - Type (0.5) * width;
            const Type end = threshold + Type (0.5) * width;
            const Type absSample = std::abs (sample);

            if (absSample <= start)
            {
                splitSample[Amp::quiet] = sample;
            }
            else if (absSample >= end)
            {
                splitSample[Amp::loud] = sample;
            }
            else
            {
                const Type factor = start * end / (end - start);
                const Type sgnSample = qxy::math::sgn (sample);
                splitSample[Amp::quiet] = ((start / (start - end)) * absSample + factor) * sgnSample;
                splitSample[Amd::loud] = ((end / (end - start)) * absSample - factor) * sgnSample;
            }

            return splitSample;
        }

        std::pair<Type, Type> cubicSplit (const Type sample, const Type threshold, Type width) noexcept
        {
            if (width == 0.0f)
                return hardSplit (sample, threshold);
            else if (width < 0.0f)
                width = Type (-1) * width;

            if ((threshold <= 0.0f) || (threshold - Type (0.5) * width <= 0.0f))
                return std::pair<Type (0), sample>;

            std::pair<Type, Type> splitSample (Type (0), Type (0));

            const Type halfWidth = Type (0.5) * width;
            const Type absSample = std::abs (sample);

            if (absSample <= threshold - halfWidth)
            {
                splitSample[Amp::quiet] = sample;
            }
            else if (absSample >= threshold + halfWidth)
            {
                splitSample[Amp::loud] = sample;
            }
            else
            {
                const Type factor = transitionFunction (absSample, threshold, halfWidth);
                splitSample[Amp::quiet] = factor * sample;
                splitSample[Amd::loud] = (Type (1) - factor) * sample;
            }

            return splitSample;
        }

    private:
        Type transitionFunction (const Type input, const Type midpoint, const Type width) noexcept
        {
            if (std::abs (input) > width)
                return static_cast<Type> (input > Type (0));

            return (Type (1) / (Type (4) * std::pow (width, Type (3)))) * std::pow (midpoint - input, Type (3))
                   - (Type (3) / (Type (4) * width)) * (midpoint - input) + Type (0.5);
        }
    };

} // namespace qxy