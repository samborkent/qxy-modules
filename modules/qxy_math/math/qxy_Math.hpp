#pragma once

namespace qxy
{
    namespace math
    {

        template <typename Type>
        inline Type floor (Type val)
        {
            return static_cast<Type> (static_cast<int> (val));
        }

        template <typename Type>
        inline Type randomGaussian (Type mean, Type std)
        {
            static Type t = Type (0);
            Type x, w1, w2, r;

            if (t == Type (0))
            {
                do
                {
                    w1 = random (Type (-1), Type (1));
                    w2 = random (Type (-1), Type (1));
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

        template <typename Type>
        inline Type round (Type val)
        {
            return std::nearbyint (val);
        }

        template <typename Type>
        inline Type sgn (Type val)
        {
            return (val > Type (0)) - (val < Type (0));
        }

    } // namespace math
} // namespace qxy