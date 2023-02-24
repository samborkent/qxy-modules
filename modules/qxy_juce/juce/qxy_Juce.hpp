#pragma once

namespace qxy
{

    namespace JUCE
    {

        template <typename Type>
        inline Type random (Type min, Type max)
        {
            return (max - min) * static_cast<Type> (juce::Random::getSystemRandom().nextDouble()) + min;
        }

        inline float getBpm (juce::AudioProcessor* audioProcessor)
        {
            juce::AudioPlayHead* playHead = audioProcessor->getPlayHead();

            return playHead->getPosition()->getBpm().hasValue() ? static_cast<float> (*playHead->getPosition()->getBpm()) : -120.0f;
        }

    } // namespace JUCE

} // namespace qxy