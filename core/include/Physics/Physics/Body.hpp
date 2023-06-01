#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace physics
{
    class Body
    {
    public:
        Body(sf::Texture& texture)
        {
            m_Sprite.setTexture(&m_Texture);
        }

        void SetTexture(const sf::String& filepath)
        {
            m_Texture.loadFromFile(filepath);
        }

        void SetPosition(const sf::Vector2f& position)
        {
            m_Sprite.setPosition(position);
        }

        void SetSize(const sf::Vector2f& size)
        {
            m_Sprite.setScale(size);
            m_Sprite.setOrigin(size / 2.0f);
        }
    private:
        float m_Mass = 1.0f;
        sf::Sprite m_Sprite;
        sf::Texture m_Texture;
        sf::Vector2f m_Velocity;
        sf::Vector2f m_Acceleration;
        std::vector<sf::Vector2f> m_Forces;
    };
}