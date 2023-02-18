#pragma once

#include "../../qxy_math/math/qxy_Math.hpp"
#include "../../qxy_math/math/qxy_TransitionFunctions.hpp"

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

        std::array<Type, 2> hardSplit (const Type sample, const Type threshold) noexcept
        {
            if (threshold <= 0.0f)
                return std::pair<Type (0), sample>;

            std::array<Type, 2> splitSample = { Type (0) };

            splitSample[std::abs (sample) > threshold] = sample;

            return splitSample;
        }

        std::array<Type, 2> linearSplit (const Type sample, const Type threshold, Type width) noexcept
        {
            if (width == 0.0f)
                return hardSplit (sample, threshold);
            else if (width < 0.0f)
                width = Type (-1) * width;

            if ((threshold <= 0.0f) || (threshold - Type (0.5) * width <= 0.0f))
                return std::array<Type, 2> (Type (0), sample);

            std::array<Type, 2> splitSample = { Type (0) };

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

        std::array<Type, 2> cubicSplit (const Type sample, const Type threshold, Type width) noexcept
        {
            if (width == 0.0f)
                return hardSplit (sample, threshold);
            else if (width < 0.0f)
                width = Type (-1) * width;

            if ((threshold <= 0.0f) || (threshold - Type (0.5) * width <= 0.0f))
                return std::array<Type, 2> (Type (0), sample);

            std::array<Type, 2> splitSample = { Type (0) };

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
                const Type factor = qxy::math::transition::firstOrder (absSample, threshold, halfWidth);
                splitSample[Amp::quiet] = factor * sample;
                splitSample[Amd::loud] = (Type (1) - factor) * sample;
            }

            return splitSample;
        }
    };

} // namespace qxy