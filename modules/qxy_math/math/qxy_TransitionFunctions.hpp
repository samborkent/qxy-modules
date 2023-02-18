#pragma once

namespace qxy
{
    namespace math
    {

        namespace transition
        {
            template <typename Float>
            constexpr Float firstOrder (const Float input, const Float midpoint, const Float width) noexcept
            {
                const Float halfWidth = Float (0.5) * width;

                if (std::abs (input - midpoint) >= halfWidth)
                    return static_cast<Float> (input >= midpoint);

                return (Float (1) / (Float (4) * qxy::math::pow (halfWidth, Float (3)))) * qxy::math::pow (midpoint - input, Float (3))
                       - (Float (3) / (Float (4) * halfWidth)) * (midpoint - input) + Float (0.5);
            }

            template <typename Float>
            constexpr Float secondOrder (const Float input, const Float midpoint, const Float width) noexcept
            {
                const Float halfWidth = Float (0.5) * width;

                if (std::abs (input - midpoint) >= halfWidth)
                    return static_cast<Float> (input >= midpoint);

                return (Float (3) / (Float (16) * qxy::math::pow (halfWidth, Float (5)))) * qxy::math::pow (input - midpoint, Float (5))
                       - (Float (10) / (Float (16) * qxy::math::pow (halfWidth, Float (3)))) * qxy::math::pow (input - midpoint, Float (3))
                       + (Float (15) / (Float (16) * halfWidth)) * (input - midpoint) + Float (0.5);
            }

        }

    } // namespace math
} // namespace qxy

//============================================================================
// Tests

constexpr float floatTolerance = 1.0f / qxy::math::pow (2.0f, 32.0f);
constexpr double doubleTolerance = 1.0 / qxy::math::pow (2.0, 32);

// First order transition fucntion
static_assert (qxy::math::transition::firstOrder (1.0f, 0.5f, 0.1f) == 1.0f);
static_assert (qxy::math::transition::firstOrder (0.0f, 0.5f, 0.1f) == 0.0f);
static_assert (qxy::math::transition::firstOrder (0.5f, 0.5f, 0.1f) == 0.5f);
static_assert (std::abs (qxy::math::transition::firstOrder (0.525, 0.5, 0.1) - 0.84375) < doubleTolerance);
static_assert (std::abs (qxy::math::transition::firstOrder (0.46, 0.5, 0.1) - 0.028) < doubleTolerance);

// First order transition fucntion
static_assert (qxy::math::transition::secondOrder (1.0f, 0.5f, 0.1f) == 1.0f);
static_assert (qxy::math::transition::secondOrder (0.0f, 0.5f, 0.1f) == 0.0f);
static_assert (qxy::math::transition::secondOrder (0.5f, 0.5f, 0.1f) == 0.5f);
static_assert (std::abs (qxy::math::transition::secondOrder (0.525, 0.5, 0.1) - 0.896484375) < doubleTolerance);
static_assert (std::abs (qxy::math::transition::secondOrder (0.46, 0.5, 0.1) - 0.00856) < doubleTolerance);