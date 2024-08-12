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

    static Material::Pair orderedPair(Material* first, Material* second)
    {
        const auto& first_name = first->getName();
        const auto& second_name = second->getName();
        auto comparison = first_name.compare(second_name);

        if(comparison > 0)
            return Material::Pair{first_name, second_name};
        else
            return Material::Pair{second_name, first_name};
    }

    class FrictionCoefficients
    {
    public:

        static void set(Material* first, Material* second, float value)
        {
            s_frictionMap[orderedPair(first, second)] = value;
        }

        static void set(const std::string& first, const std::string& second, float value)
        {
            set(Materials::get(first), Materials::get(second), value);
        }

        static float& get(Material* first, Material* second)
        {
            auto pair = orderedPair(first, second);
            auto f = s_frictionMap.find(pair);
            if(f != s_frictionMap.end())
                return f->second;
            
            return s_frictionMap[pair] = PHYSICS_FRICTION_DEFAULT;
        }

        static float& get(const std::string& first, const std::string& second)
        {
            return get(Materials::get(first), Materials::get(second));
        }
    private:
        static std::unordered_map<Material::Pair, float, PairHash<Material>> s_frictionMap; 
    };
}