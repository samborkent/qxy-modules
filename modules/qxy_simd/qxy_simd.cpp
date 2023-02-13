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

    juce::dsp::AudioBlock<simd::Type> Simd::interleave (const juce::dsp::AudioBlock<const float>& inBlock) noexcept
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

    void Simd::deinterleave (juce::dsp::AudioBlock<simd::Type>& _interleavedBlock, juce::dsp::AudioBlock<float>& outBlock) noexcept
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
}