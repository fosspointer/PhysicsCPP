#pragma once
#include <Physics/System/NonCopyable.hpp>
#include <SFML/Graphics/Font.hpp>

namespace physics
{
    class Font : public NonCopyable
    {
    public:
        /**
         * @brief sf::Font abstraction class
         * @param filepath the filepath of the font to be loaded
         */
        Font(const sf::String& filepath)
        {
            m_Font.loadFromFile(filepath);
        }

        /**
         * @brief Loads and returns the default font as set in "Physics/Config/Font" (relative filepath)
         * @return Font& 
         */
        static Font& GetDefault()
        {
            static Font font(
                #include <Physics/Config/Font>
            );
            return font;
        }

        /**
         * @brief Get the sf::Font object (mutable)
         * @return Mutable reference to the font
         */
        sf::Font& Get() { return m_Font; }
        /**
         * @brief Get the sf::Font object (immutable)
         * @return Immutable reference to the font
         */
        const sf::Font& Get() const { return m_Font; }
    private:
        sf::Font m_Font;
    };
}