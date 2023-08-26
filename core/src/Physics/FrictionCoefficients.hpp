#pragma once
#include <unordered_map>
#include <Physics/Physics/Material.hpp>

namespace physics
{
    class FrictionCoefficients
    {
    public:
        static void Set(){}
    private:
        static std::unordered_map<std::pair<Material, Material>, float, PairHash> s_Map; 
    };
}