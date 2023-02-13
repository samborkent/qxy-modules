/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            qxy_simd
   vendor:        Qxy
   version:       0.0.1
   name:          Qxy SIMD
   description:   SIMD code
   dependencies:

   website:       https://github.com/samborkent/qxy-modules
   license:       BSD 3-Clause

   END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once

#include <juce_dsp/juce_dsp.h>

namespace qxy
{

    namespace simd
    {
        using Type = juce::dsp::SIMDRegister<float>;
        constexpr size_t size = Type::size();

        inline Type sgn (Type x) noexcept;
        inline Type round (Type x) noexcept;
    }

    class Simd
    {
    public:
        Simd() noexcept = default;

        using Format = juce::AudioData::Format<juce::AudioData::Float32, juce::AudioData::NativeEndian>;

        void prepare (const juce::dsp::ProcessSpec& spec) noexcept;

        juce::dsp::AudioBlock<simd::Type> interleave (const juce::dsp::AudioBlock<const float>& inBlock) noexcept;

        void deinterleave (juce::dsp::AudioBlock<simd::Type>& _interleavedBlock, juce::dsp::AudioBlock<float>& outBlock) noexcept;

    private:
        juce::dsp::AudioBlock<simd::Type> interleavedBlock;
        juce::dsp::AudioBlock<float> zeroBlock;
        juce::HeapBlock<char> interleavedBlockData, zeroBlockData;

        static float* toBasePointer (simd::Type* _register) noexcept
        {
            return reinterpret_cast<float*> (_register);
        }
    };

}