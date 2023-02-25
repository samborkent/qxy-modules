#pragma once

namespace qxy
{

    namespace simd
    {
        template <typename Float>
        using Type = juce::dsp::SIMDRegister<Float>;

        template <typename Float>
        inline Type<Float> sgn (Type<Float> x) noexcept
        {
            const auto zero = juce::dsp::SIMDRegister (Float (0));

            return (x & Type<Float>::greaterThan (x, zero)) - (x & Type<Float>::lessThan (x, zero));
        }

        template <typename Float>
        inline Type<Float> round (Type<Float> x) noexcept
        {
            const auto half = juce::dsp::SIMDRegister (Float (0));

            return Type<Float>::truncate (Type<Float>::abs (x) + half) * qxy::simd::sgn (x);
        }
    }

    template <typename Float>
    class Simd
    {
    public:
        Simd() noexcept = default;

        using Format = juce::AudioData::Format<juce::AudioData::Float32, juce::AudioData::NativeEndian>;

        void prepare (const juce::dsp::ProcessSpec& spec) noexcept
        {
            interleavedBlock = juce::dsp::AudioBlock<simd::Type<Float>> (interleavedBlockData, 1, spec.maximumBlockSize);
            zeroBlock = juce::dsp::AudioBlock<Float> (zeroBlockData, registerSize, spec.maximumBlockSize);
            zeroBlock.clear();
        }

        juce::dsp::AudioBlock<simd::Type<Float>> interleave (const juce::dsp::AudioBlock<const Float>& inBlock) noexcept
        {
            std::array<const Float*, registerSize> inChannels {};

            for (size_t c = 0; c < registerSize; ++c)
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

        void deinterleave (juce::dsp::AudioBlock<simd::Type<Float>>& _interleavedBlock, juce::dsp::AudioBlock<Float>& outBlock) noexcept
        {
            std::array<Float*, registerSize> outChannels {};

            for (size_t c = 0; c < registerSize; ++c)
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
        static constexpr size_t registerSize = simd::Type<Float>::size();

        juce::dsp::AudioBlock<simd::Type<Float>> interleavedBlock;
        juce::dsp::AudioBlock<Float> zeroBlock;
        juce::HeapBlock<char> interleavedBlockData, zeroBlockData;

        static Float* toBasePointer (simd::Type<Float>* _register) noexcept
        {
            return reinterpret_cast<Float*> (_register);
        }
    };

}