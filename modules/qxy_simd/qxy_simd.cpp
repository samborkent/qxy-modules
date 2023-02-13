#include "qxy_simd.hpp"

namespace qxy
{
    namespace simd
    {
        inline Type sgn (Type x) noexcept
        {
            const Type zero = juce::dsp::SIMDRegister (0.0f);

            return (x & Type::greaterThan (x, zero)) - (x & Type::lessThan (x, zero));
        }

        inline Type round (Type x) noexcept
        {
            const Type half = juce::dsp::SIMDRegister (0.5f);

            return Type::truncate (Type::abs (x) + half) * sgn (x);
        }
    }

    void Simd::prepare (const juce::dsp::ProcessSpec& spec) noexcept
    {
        interleavedBlock = juce::dsp::AudioBlock<simd::Type> (interleavedBlockData, 1, spec.maximumBlockSize);
        zeroBlock = juce::dsp::AudioBlock<float> (zeroBlockData, simd::size, spec.maximumBlockSize);
        zeroBlock.clear();
    }
}