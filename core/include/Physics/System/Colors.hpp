#pragma once
#include <SFML/Graphics/Color.hpp>

#define BRIGHT_DOM_HUE 204
#define BRIGHT_SEC_HUE 41

#define DARK_DOM_HUE 102
#define DARK_SEC_HUE 20

#define VALUE_100 255
#define VALUE_75 191
#define VALUE_50 128
#define VALUE_25 64
#define VALUE_0 0

namespace physics::Colors
{
    ///Primary
    //bright
    const sf::Color Red = sf::Color(BRIGHT_DOM_HUE, BRIGHT_SEC_HUE, BRIGHT_SEC_HUE);
    const sf::Color Green = sf::Color(BRIGHT_SEC_HUE, BRIGHT_DOM_HUE, BRIGHT_SEC_HUE);
    const sf::Color Blue = sf::Color(BRIGHT_SEC_HUE, BRIGHT_SEC_HUE, BRIGHT_DOM_HUE);
    //dark
    const sf::Color DarkRed = sf::Color(DARK_DOM_HUE, DARK_SEC_HUE, DARK_SEC_HUE);
    const sf::Color DarkGreen = sf::Color(DARK_SEC_HUE, DARK_DOM_HUE, DARK_SEC_HUE);
    const sf::Color DarkBlue = sf::Color(DARK_SEC_HUE, DARK_SEC_HUE, DARK_DOM_HUE);

    ///Secondary
    //bright
    const sf::Color Yellow = sf::Color(BRIGHT_DOM_HUE, BRIGHT_DOM_HUE, BRIGHT_SEC_HUE);
    const sf::Color Cyan = sf::Color(BRIGHT_SEC_HUE, BRIGHT_DOM_HUE, BRIGHT_DOM_HUE);
    const sf::Color Magenta = sf::Color(BRIGHT_DOM_HUE, BRIGHT_SEC_HUE, BRIGHT_DOM_HUE);
    //dark
    const sf::Color DarkYellow = sf::Color(DARK_DOM_HUE, DARK_DOM_HUE, DARK_SEC_HUE);
    const sf::Color DarkCyan = sf::Color(DARK_SEC_HUE, DARK_DOM_HUE, DARK_DOM_HUE);
    const sf::Color DarkMagenta = sf::Color(DARK_DOM_HUE, DARK_SEC_HUE, DARK_DOM_HUE);

    ///Grayscale
    const sf::Color Black = sf::Color(VALUE_0, VALUE_0, VALUE_0);
    const sf::Color DarkGray = sf::Color(VALUE_25, VALUE_25, VALUE_25);
    const sf::Color Gray = sf::Color(VALUE_50, VALUE_50, VALUE_50);
    const sf::Color LightGray = sf::Color(VALUE_75, VALUE_75, VALUE_75); 
    const sf::Color White = sf::Color(VALUE_100, VALUE_100, VALUE_100);
}