#pragma once
#include <Physics/System/NonCopyable.hpp>
#include <SFML/Graphics/Font.hpp>

namespace physics
{
    /// @brief Abstraction class for sfml fonts
    class Font : public NonCopyable
    {
    public:
        /// @brief Load font from file
        /// @param filepath path to the file
        Font(const sf::String& filepath)
        {
            m_font.loadFromFile(filepath);
        }
        
        /// @brief Loads and returns the default font as set in "Physics/Config/Font" (relative filepath)
        /// @return Font& 
        static Font& getDefault()
        {
            static Font font(
                #include <Physics/Config/Font>
            );
            return font;
        }

        /// @brief Get the sf::Font object (mutable)
        /// @return Mutable reference to the font
         
        sf::Font& get() { return m_font; }
        
        /// @brief Get the sf::Font object (immutable)
        /// @return Immutable reference to the font
         
        const sf::Font& get() const { return m_font; }
    private:
        sf::Font m_font;
    };
}