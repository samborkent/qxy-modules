#/bin/sh

clang-format -i ../modules/qxy_juce/qxy_juce.h
clang-format -i ../modules/qxy_juce/juce/qxy_Juce.hpp

clang-format -i ../modules/qxy_math/qxy_math.h
clang-format -i ../modules/qxy_math/math/qxy_Math.hpp
clang-format -i ../modules/qxy_math/math/qxy_TransitionFunctions.hpp

clang-format -i ../modules/qxy_param/qxy_param.h
clang-format -i ../modules/qxy_param/core/qxy_ParameterCore.hpp

clang-format -i ../modules/qxy_processors/qxy_processors.h
clang-format -i ../modules/qxy_processors/processors/qxy_AmplitudeSplitter.hpp
clang-format -i ../modules/qxy_processors/processors/qxy_EnvelopeFilter.hpp
clang-format -i ../modules/qxy_processors/processors/qxy_EnvelopeFollower.hpp
clang-format -i ../modules/qxy_processors/processors/qxy_MixerProcessor.hpp
clang-format -i ../modules/qxy_processors/processors/qxy_PitchTracker.hpp

clang-format -i ../modules/qxy_simd/qxy_simd.h
clang-format -i ../modules/qxy_simd/simd/qxy_Simd.hpp