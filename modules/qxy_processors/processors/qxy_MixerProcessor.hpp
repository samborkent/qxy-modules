#pragma once

namespace qxy
{

    template <class InnerProcessor>
    class MixerProcessor : public InnerProcessor
    {
    public:
        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            InnerProcessor::prepare (spec);
            dryWetMixer.prepare (spec);
        }

        template <class ProcessContext>
        void process (const ProcessContext& context)
        {
            dryWetMixer.pushDrySamples (context.getInputBlock());

            InnerProcessor::process (context);

            dryWetMixer.mixWetSamples (context.getOutputBlock());
        }

        template <class ProcessContext>
        void mixWetSamples (const ProcessContext& context)
        {
            dryWetMixer.mixWetSamples (context.getOutputBlock());
        }

        template <class ProcessContext>
        void pushDrySamples (const ProcessContext& context)
        {
            dryWetMixer.pushDrySamples (context.getInputBlock());
        }

        void setWetMixProportion (float wetMixProportion)
        {
            dryWetMixer.setWetMixProportion (wetMixProportion);
        }

        void setMixingRule (juce::dsp::DryWetMixingRule mixingRule)
        {
            dryWetMixer.setMixingRule (mixingRule);
        }

    private:
        juce::dsp::DryWetMixer<float> dryWetMixer;
    };

} // namespace qxy