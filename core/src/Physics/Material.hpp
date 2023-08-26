#pragma once
#include <unordered_map>

namespace physics
{
    struct Material
    {
        float Density = 1.0f;
        operator float()
        {
            return Density;
        }
    };

    template <typename T>
    struct PairHash
    {
        size_t operator() (const std::pair<T, T>& pair) const
        {
            return pair.first ^ (pair.second + 0x9e3779b9 + (pair.first << 6) + (pair.first >> 2));
        }
    };
}