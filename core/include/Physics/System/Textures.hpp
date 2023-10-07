#pragma once
#include <SFML/Graphics.hpp>

namespace physics
{
    class Textures
    {
    public:
        static sf::Texture* Load(const std::string& filepath) 
        {
            auto f = s_TextureCache.find(filepath);

            if(f != s_TextureCache.end())
                return f->second;
            else 
            {
                auto* texture = new sf::Texture;
                texture->loadFromFile(filepath);
                texture->setSmooth(true);
                s_TextureCache[filepath] = texture;

                return texture;
            }
        }

        static sf::Texture* Button()
        {
            return Load(
                #include <Physics/Config/TextureButton>
            );
        }

        static sf::Texture* ButtonPressed()
        {
            return Load(
                #include <Physics/Config/TextureButtonPressed>
            );
        }

        static sf::Texture* DropdownHead()
        {
            return Load(
                #include <Physics/Config/TextureDropdownHead>
            );
        }

        static sf::Texture* DropdownHeadExpanded()
        {
            return Load(
                #include <Physics/Config/TextureDropdownHeadExpanded>
            );
        }

        static sf::Texture* DropdownOption()
        {
            return Load(
                #include <Physics/Config/TextureDropdownOption>
            );
        }

        static sf::Texture* DropdownOptionLast()
        {
            return Load(
                #include <Physics/Config/TextureDropdownOptionLast>
            );
        }

        static void ClearCache()
        {
            for(auto& item : s_TextureCache)
                delete item.second;

            s_TextureCache.clear();
        }
    private:
        static std::unordered_map<std::string, sf::Texture*> s_TextureCache;
    };
}