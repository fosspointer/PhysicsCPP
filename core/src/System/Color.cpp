#include <Physics/System/Color.hpp>

namespace physics
{
    ///Primary
    //bright
    const Color Color::Red = sf::Color(BRIGHT_DOM_HUE, BRIGHT_SEC_HUE, BRIGHT_SEC_HUE);
    const Color Color::Green = sf::Color(BRIGHT_SEC_HUE, BRIGHT_DOM_HUE, BRIGHT_SEC_HUE);
    const Color Color::Blue = sf::Color(BRIGHT_SEC_HUE, BRIGHT_SEC_HUE, BRIGHT_DOM_HUE);
    //dark
    const Color Color::DarkRed = sf::Color(DARK_DOM_HUE, DARK_SEC_HUE, DARK_SEC_HUE);
    const Color Color::DarkGreen = sf::Color(DARK_SEC_HUE, DARK_DOM_HUE, DARK_SEC_HUE);
    const Color Color::DarkBlue = sf::Color(DARK_SEC_HUE, DARK_SEC_HUE, DARK_DOM_HUE);

    ///Secondary
    //bright
    const Color Color::Yellow = sf::Color(BRIGHT_DOM_HUE, BRIGHT_DOM_HUE, BRIGHT_SEC_HUE);
    const Color Color::Cyan = sf::Color(BRIGHT_SEC_HUE, BRIGHT_DOM_HUE, BRIGHT_DOM_HUE);
    const Color Color::Magenta = sf::Color(BRIGHT_DOM_HUE, BRIGHT_SEC_HUE, BRIGHT_DOM_HUE);
    //dark
    const Color Color::DarkYellow = sf::Color(DARK_DOM_HUE, DARK_DOM_HUE, DARK_SEC_HUE);
    const Color Color::DarkCyan = sf::Color(DARK_SEC_HUE, DARK_DOM_HUE, DARK_DOM_HUE);
    const Color Color::DarkMagenta = sf::Color(DARK_DOM_HUE, DARK_SEC_HUE, DARK_DOM_HUE);

    ///Grayscale
    const Color Color::Black = sf::Color(VALUE_0, VALUE_0, VALUE_0);
    const Color Color::DarkGray = sf::Color(VALUE_25, VALUE_25, VALUE_25);
    const Color Color::Gray = sf::Color(VALUE_50, VALUE_50, VALUE_50);
    const Color Color::LightGray = sf::Color(VALUE_75, VALUE_75, VALUE_75); 
    const Color Color::White = sf::Color(VALUE_100, VALUE_100, VALUE_100);
}