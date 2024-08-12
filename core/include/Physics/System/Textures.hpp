#pragma once
#include <SFML/Graphics.hpp>

namespace physics
{
    class Textures
    {
    public:
        static sf::Texture* load(const std::string& filepath) 
        {
            auto f = s_textureCache.find(filepath);

            if(f != s_textureCache.end())
                return f->second;
            else 
            {
                auto* texture = new sf::Texture;
                texture->loadFromFile(filepath);
                texture->setSmooth(true);
                s_textureCache[filepath] = texture;

                return texture;
            }
        }

        static sf::Texture* button()
        {
            return load(
                #include <Physics/Config/TextureButton>
            );
        }

        static sf::Texture* buttonPressed()
        {
            return load(
                #include <Physics/Config/TextureButtonPressed>
            );
        }

        static sf::Texture* dropdownHead()
        {
            return load(
                #include <Physics/Config/TextureDropdownHead>
            );
        }

        static sf::Texture* dropdownHeadExpanded()
        {
            return load(
                #include <Physics/Config/TextureDropdownHeadExpanded>
            );
        }

        static sf::Texture* dropdownOption()
        {
            return load(
                #include <Physics/Config/TextureDropdownOption>
            );
        }

        static sf::Texture* dropdownOptionLast()
        {
            return load(
                #include <Physics/Config/TextureDropdownOptionLast>
            );
        }

        static void clearCache()
        {
            for(auto& item : s_textureCache)
                delete item.second;

            s_textureCache.clear();
        }
    private:
        static std::unordered_map<std::string, sf::Texture*> s_textureCache;
    };
}