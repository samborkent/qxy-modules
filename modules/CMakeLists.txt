add_subdirectory(JUCE)
add_subdirectory(qxy-modules)

subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

include(cmake/WarningFlags.cmake)
add_library(juce_plugin_modules STATIC)
target_link_libraries(juce_plugin_modules
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        qxy::qxy_simd
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        warning_flags
)

add_diagnostic_info(juce_plugin_modules)

target_include_directories(juce_plugin_modules
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,INCLUDE_DIRECTORIES>
)

set_target_properties(juce_plugin_modules PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)