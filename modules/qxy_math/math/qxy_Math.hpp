#pragma once

#include "../../qxy_juce/juce/qxy_Juce.hpp"

namespace qxy
{
    namespace math
    {

        template <typename Float>
        constexpr Float floor (Float x) noexcept
        {
            return static_cast<Float> (static_cast<int> (x));
        }

        template <typename Type>
        inline Type randomGaussian (Type mean, Type std) noexcept
        {
            static Type t = Type (0);
            Type x, w1, w2, r;

            if (t == Type (0))
            {
                do
                {
                    w1 = qxy::JUCE::random (Type (-1), Type (1));
                    w2 = qxy::JUCE::random (Type (-1), Type (1));
                    r = w1 * w1 + w2 * w2;
                } while (r >= Type (1));

                r = std::sqrt (Type (-2) * std::log (r) / r);
                t = w2 * r;
                return mean + w1 * r * std;
            }
            else
            {
                x = t;
                t = Type (0);
                return mean + x * std;
            }
        }

        template <typename Float>
        constexpr Float round (Float x) noexcept
        {
            return floor (x + Float (0.5) * sgn (x));
        }

        template <typename Type>
        constexpr Type sgn (Type x) noexcept
        {
            return (x > Type (0)) - (x < Type (0));
        }

        template <typename T, typename E>
        [[nodiscard]] constexpr T pow (const T x, const E p) noexcept
        {
            return p > E (0) ? x * pow (x, p - E (1)) : T (1);
        }

    } // namespace math
} // namespace qxy

/*
//============================================================================
// Tests

constexpr float tolerance = 1.0f / qxy::math::pow (2.0f, 32.0f);

// Floor
static_assert (qxy::math::floor (1.1f) == 1.0f);
static_assert (qxy::math::floor (1.9) == 1.0);
static_assert (qxy::math::floor (-1.9) == -1.0);

// Round
static_assert (qxy::math::round (1.1f) == 1.0f);
static_assert (qxy::math::round (1.9f) == 2.0f);
static_assert (qxy::math::round (1.5f) == 2.0f);
static_assert (qxy::math::round (-1.5f) == -2.0f);

// Sign
static_assert (qxy::math::sgn (1.0) == 1.0);
static_assert (qxy::math::sgn (-1.0) == -1.0);
static_assert (qxy::math::sgn (-1) == -1);

// Power
static_assert (std::abs (qxy::math::pow (5.0, 0) - 1.0) < tolerance);
static_assert (qxy::math::pow (2, 4) == 16);
static_assert (qxy::math::pow (3, 4) == 81);
static_assert (qxy::math::pow (7.0, 2) == 49.0);
static_assert (std::abs ((1.0f / qxy::math::pow (2.0f, 4)) - 0.0625f) < tolerance);
*/