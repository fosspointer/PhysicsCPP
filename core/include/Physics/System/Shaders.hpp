#pragma once
#include <SFML/Graphics.hpp>

namespace physics
{
    class Shaders
    {
    public:
        static sf::Shader* load(const std::string& filepath, const sf::Shader::Type& type) 
        {
            auto f = s_shaderCache.find(filepath);

            if(f != s_shaderCache.end())
                return f->second;
            else 
            {
                auto* shader = new sf::Shader;
                shader->loadFromFile(filepath, type);
                s_shaderCache[filepath] = shader;

                return shader;
            }
        }

        static void clearCache()
        {
            for(auto& item : s_shaderCache)
                delete item.second;

            s_shaderCache.clear();
        }
    private:
        static std::unordered_map<std::string, sf::Shader*> s_shaderCache;
    };
}