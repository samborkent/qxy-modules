add_subdirectory(JUCE)

add_library(juce_dependencies STATIC)
target_link_libraries(juce_dependencies
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
)

target_include_directories(juce_dependencies
    INTERFACE
        $<TARGET_PROPERTY:juce_dependencies,INCLUDE_DIRECTORIES>
)

set_target_properties(juce_dependencies PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)