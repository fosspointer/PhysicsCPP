#pragma once
#include <Physics/System/AABB.hpp>
#include <Physics/UI/UIElement.hpp>

namespace physics
{
    class Image : public UIElement<Image> 
    {
    public:
        Image(Application* application, const sf::String& filepath, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f})
            :UIElement(application, sf::Vector2f{0.0f, 0.0f}, margin, sf::Color::White)
        {
            m_Image.setTexture(physics::Textures::LoadTexture(filepath));
            m_Size = (sf::Vector2f)m_Image.getTexture()->getSize();
        }

        bool IsHovered() const override
        {
            return AABB::RectangleToPoint(m_Image, Mouse::GetPosition());
        }

        void Draw(int8_t layer = PHYSICS_LAYER_UI_2) override
        {
            m_Image.setPosition(m_Position - m_Size / 2.0f);
            m_Image.setSize(m_Size);
            m_Image.setFillColor(m_Color);
            m_Application->GetWindow().draw(m_Image);
        }

        Image* SetOutline(float thickness, const sf::Color& color = sf::Color::Black)
        {
            m_Image.setOutlineThickness(thickness);
            m_Image.setOutlineColor(color);
            return this;
        }

        Image* SetOutlineColor(const sf::Color& color)
        {
            m_Image.setOutlineColor(color);
            return this;
        }

        Image* SetOutlineThickness(float thickness)
        {
            m_Image.setOutlineThickness(thickness);
            return this;
        }
    private:
        sf::RectangleShape m_Image;
    };
}