#pragma once
#include <unordered_map>
#include <Physics/Physics/Materials.hpp>

#define PHYSICS_FRICTION_DEFAULT 0.25f

namespace physics
{
    template <typename T>
    struct PairHash
    {
        size_t operator() (const std::pair<T, T>& pair) const
        {
            return pair.first ^ (pair.second + 0x9e3779b9 + (pair.first << 6) + (pair.first >> 2));
        }
    };

    static Material::Pair OrderedPair(Material* first, Material* second)
    {
        const auto& first_name = first->GetName();
        const auto& second_name = second->GetName();
        auto comparison = first_name.compare(second_name);

        if(comparison > 0)
            return Material::Pair{first_name, second_name};
        else
            return Material::Pair{second_name, first_name};
    }

    class FrictionCoefficients
    {
    public:

        static void Set(Material* first, Material* second, float value)
        {
            s_FrictionMap[OrderedPair(first, second)] = value;
        }

        static void Set(const std::string& first, const std::string& second, float value)
        {
            Set(Materials::Get(first), Materials::Get(second), value);
        }

        static float& Get(Material* first, Material* second)
        {
            auto pair = OrderedPair(first, second);
            auto f = s_FrictionMap.find(pair);
            if(f != s_FrictionMap.end())
                return f->second;
            
            return s_FrictionMap[pair] = PHYSICS_FRICTION_DEFAULT;
        }

        static float& Get(const std::string& first, const std::string& second)
        {
            return Get(Materials::Get(first), Materials::Get(second));
        }
    private:
        static std::unordered_map<Material::Pair, float, PairHash<Material>> s_FrictionMap; 
    };
}