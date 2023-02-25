#pragma once

#include "../../qxy_math/math/qxy_Math.hpp"
#include "../../qxy_math/math/qxy_TransitionFunctions.hpp"

namespace qxy
{
    enum SplitType {
        hard,
        linear,
        cubic,
        quintic
    };

    enum Amp {
        quiet,
        loud
    };

    template <typename Type>
    class AmplitudeSplitter
    {
    public:
        AmplitudeSplitter() noexcept = default;

        void process (juce::dsp::ProcessContextNonReplacing<Type> context) noexcept
        {
            auto inBlock = context.getInputBlock();
            auto outBlock = context.getInputBlock();

            jassert (inBlock.getNumChannels() == 2);
            jassert (outBlock.getNumChannels() >= 4);

            for (size_t s = 0; s < inBlock.getNumSamples(); ++s)
            {
                std::array<Type, 2> resultL;
                std::array<Type, 2> resultR;

                switch (splitType)
                {
                case SplitType::hard:
                    resultL = hardSplit (inBlock.getSample(0, s), threshold);
                    resultR = hardSplit (inBlock.getSample(1, s), threshold);
                case SplitType::linear:
                    resultL = linearSplit (inBlock.getSample(0, s), threshold, width);
                    resultR = linearSplit (inBlock.getSample(1, s), threshold, width);
                case SplitType::cubic:
                    resultL = cubicSplit (inBlock.getSample(0, s), threshold, width);
                    resultR = cubicSplit (inBlock.getSample(1, s), threshold, width);
                case SplitType::quintic:
                    resultL = quinticSplit (inBlock.getSample(0, s), threshold, width);
                    resultR = quinticSplit (inBlock.getSample(1, s), threshold, width);
                }

                outBlock.setSample(0, s, resultL[0]);
                outBlock.setSample(2, s, resultL[1]);

                outBlock.setSample(1, s, resultR[0]);
                outBlock.setSample(3, s, resultR[1]);
            }
        }

        void setSplitType (const SplitType _splitType) noexcept
        {
            splitType = _splitType;
        }

        void setThreshold (const Type _threshold) noexcept
        {
            threshold = _threshold;
        }

        void setWidth (const Type _width) noexcept
        {
            width = _width;
        }

    private:
        SplitType splitType = SplitType::hard;
        Type threshold, width = Type (0);

        std::array<Type, 2> hardSplit (const Type sample, const Type threshold) noexcept
        {
            if (threshold <= 0.0f)
                return std::array<Type, 2> (Type(0), sample);

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
                splitSample[Amp::loud] = ((end / (end - start)) * absSample - factor) * sgnSample;
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
                splitSample[Amp::loud] = (Type (1) - factor) * sample;
            }

            return splitSample;
        }

        std::array<Type, 2> quinticSplit (const Type sample, const Type threshold, Type width) noexcept
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
                const Type factor = qxy::math::transition::secondOrder (absSample, threshold, halfWidth);
                splitSample[Amp::quiet] = factor * sample;
                splitSample[Amp::loud] = (Type (1) - factor) * sample;
            }

            return splitSample;
        }
    };

    template <typename Type>
    class AmplitudeRecombiner
    {
    public:
        AmplitudeRecombiner() noexcept = default;

        void process (juce::dsp::ProcessContextNonReplacing<Type> context) noexcept
        {
            auto inBlock = context.getInputBlock();
            auto outBlock = context.getInputBlock();

            jassert (inBlock.getNumChannels() == 2);
            jassert (outBlock.getNumChannels() >= 4);
        }

    private:
    };

} // namespace qxy