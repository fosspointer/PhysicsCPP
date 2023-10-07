#pragma once
#include <SFML/Graphics.hpp>

namespace physics
{
    class Shaders
    {
    public:
        static sf::Shader* Load(const std::string& filepath, const sf::Shader::Type& type) 
        {
            auto f = s_ShaderCache.find(filepath);

            if(f != s_ShaderCache.end())
                return f->second;
            else 
            {
                auto* shader = new sf::Shader;
                shader->loadFromFile(filepath, type);
                s_ShaderCache[filepath] = shader;

                return shader;
            }
        }

        static void ClearCache()
        {
            for(auto& item : s_ShaderCache)
                delete item.second;

            s_ShaderCache.clear();
        }
    private:
        static std::unordered_map<std::string, sf::Shader*> s_ShaderCache;
    };
}