cmake_minimum_required(VERSION 3.15)
project(qxy_modules VERSION 2.0.0)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(modules)

juce_add_modules(
    ALIAS_NAMESPACE qxy

    ${PROJECT_SOURCE_DIR}/modules/qxy_juce
    ${PROJECT_SOURCE_DIR}/modules/qxy_math
    ${PROJECT_SOURCE_DIR}/modules/qxy_param
    ${PROJECT_SOURCE_DIR}/modules/qxy_processors
    ${PROJECT_SOURCE_DIR}/modules/qxy_simd
)