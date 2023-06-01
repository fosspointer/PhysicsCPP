#pragma once
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>

namespace physics
{
    class Texture
    {
    public:
        static const sf::Texture* LoadTexture(const sf::String& filepath) 
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
        static std::unordered_map<sf::String, sf::Texture*> s_TextureCache;

    };
}