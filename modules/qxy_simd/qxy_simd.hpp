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

        void prepare (const juce::dsp::ProcessSpec& spec) noexcept
        {
            interleavedBlock = juce::dsp::AudioBlock<simd::Type> (interleavedBlockData, 1, spec.maximumBlockSize);
            zeroBlock = juce::dsp::AudioBlock<float> (zeroBlockData, simd::size, spec.maximumBlockSize);
            zeroBlock.clear();
        }

        juce::dsp::AudioBlock<simd::Type> interleave (const juce::dsp::AudioBlock<const float>& inBlock) noexcept
        {
            const int registerSize = static_cast<int> (simd::size);

            std::array<const float*, simd::size> inChannels {};

            for (size_t c = 0; c < inChannels.size(); ++c)
            {
                inChannels[c] = (c < inBlock.getNumChannels() ? inBlock.getChannelPointer (c) : zeroBlock.getChannelPointer (c));
            }

            juce::AudioData::interleaveSamples (
                juce::AudioData::NonInterleavedSource<Format> {
                    inChannels.data(),
                    registerSize },
                juce::AudioData::InterleavedDest<Format> {
                    toBasePointer (interleavedBlock.getChannelPointer (0)),
                    registerSize },
                static_cast<int> (inBlock.getNumSamples()));

            return interleavedBlock;
        }

        void deinterleave (juce::dsp::AudioBlock<simd::Type>& _interleavedBlock, juce::dsp::AudioBlock<float>& outBlock) noexcept
        {
            const int registerSize = static_cast<int> (simd::size);

            std::array<float*, simd::size> outChannels {};

            for (size_t c = 0; c < outChannels.size(); ++c)
            {
                outChannels[c] = (c < outBlock.getNumChannels() ? outBlock.getChannelPointer (c) : zeroBlock.getChannelPointer (c));
            }

            juce::AudioData::deinterleaveSamples (
                juce::AudioData::InterleavedSource<Format> {
                    toBasePointer (_interleavedBlock.getChannelPointer (0)),
                    registerSize },
                juce::AudioData::NonInterleavedDest<Format> {
                    outChannels.data(),
                    registerSize },
                static_cast<int> (outBlock.getNumSamples()));
        }

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