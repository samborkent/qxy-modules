#pragma once

namespace qxy
{

   namespace param
   {

        namespace attribute
        {
            auto dB = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (0.1f * static_cast<float> (static_cast<int> (x * 10.0f))) + " dB"; });
            auto percent = juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto x, auto) { return juce::String (static_cast<int> (x * 100.0f)) + "%"; });
        } // namespace attribute

        struct Float
        {
            const juce::String& id;
            const juce::String& name;
            const float min;
            const float max;
            const float init;
            juce::AudioParameterFloatAttributes attribute;
        };

        struct Int
        {
            const juce::String& id;
            const juce::String& name;
            const int min;
            const int max;
            const int init;
            juce::AudioParameterIntAttributes attribute;
        };

        struct Bool
        {
            const juce::String& id;
            const juce::String& name;
            const bool init;
            juce::AudioParameterBoolAttributes attribute;
        };

        struct Choice
        {
            const juce::String& id;
            const juce::String& name;
            const juce::StringArray& choices;
            const int init;
        };

        std::unique_ptr<juce::AudioParameterFloat> createParameterFloat (const Float& param) noexcept
        {
            return std::make_unique<juce::AudioParameterFloat> (param.id, param.name, juce::NormalisableRange<float> (param.min, param.max), param.init, param.attribute);
        }

        std::unique_ptr<juce::AudioParameterInt> createParameterInt (const Int& param) noexcept
        {
            return std::make_unique<juce::AudioParameterInt> (param.id, param.name, param.min, param.max, param.init, param.attribute);
        }

        std::unique_ptr<juce::AudioParameterBool> createParameterBool (const Bool& param) noexcept
        {
            return std::make_unique<juce::AudioParameterBool> (param.id, param.name, param.init, param.attribute);
        }

        std::unique_ptr<juce::AudioParameterChoice> createParameterChoice (const Choice& param) noexcept
        {
            return std::make_unique<juce::AudioParameterChoice> (param.id, param.name, param.choices, param.init);
        }

        juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

   } // namespace param
   
} // namespace qxy