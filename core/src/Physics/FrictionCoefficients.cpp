#include <Physics/Physics/FrictionCoefficients.hpp>
namespace physics
{
    std::unordered_map<Material::Pair, float, PairHash<Material>> FrictionCoefficients::s_FrictionMap; 
}