#include <Physics/System/Shaders.hpp>

namespace physics
{
    std::unordered_map<std::string, sf::Shader*> Shaders::s_ShaderCache;
}