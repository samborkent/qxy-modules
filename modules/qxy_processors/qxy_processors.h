/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            qxy_processors
   vendor:        Qxy
   version:       0.0.1
   name:          Qxy Processors
   description:   Collection of DSP Processors
   dependencies:

   website:       https://github.com/samborkent/qxy-modules
   license:       BSD 3-Clause

   END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once

#include <juce_dsp/juce_dsp.h>

#include "processors/qxy_AmplitudeSplitter.hpp"
#include "processors/qxy_EnvelopeFilter.hpp"
#include "processors/qxy_EnvelopeFollower.hpp"
#include "processors/qxy_MixerProcessor.hpp"
#include "processors/qxy_PitchTracker.hpp"

namespace qxy
{

} // namespace qxy
