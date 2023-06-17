#pragma once
#include <SFML/Graphics.hpp>

namespace physics
{
    class Textures
    {
    public:
        static const sf::Texture* LoadTexture(const std::string& filepath) 
        {
            auto f = s_TextureCache.find(filepath);
            
            if(f != s_TextureCache.end())
                return f->second;
            else 
            {
                auto* texture = new sf::Texture;
                texture->loadFromFile(filepath);
                return f->second;
                return texture;
            }
        }

        static void ClearCache()
        {
            for(auto& item : s_TextureCache)
            {
                delete item.second;
            }

            s_TextureCache.clear();
        }
    private:
        static std::unordered_map<std::string, sf::Texture*> s_TextureCache;

    };
}