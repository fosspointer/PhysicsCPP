#pragma once
#include <Physics/System/RectangleShape.hpp>
#include <Physics/UI/UIElement.hpp>

namespace physics
{
    class Image : public UIElement<Image> 
    {
    public:
        Image(Application* application, const sf::String& filepath)
            :UIElement(application), m_image(m_application->getWindow())
        {
            m_image.setTexture(physics::Textures::load(filepath));
            m_size = (sf::Vector2f)m_image.getTexture()->getSize();
        }

        bool isHovered() const override
        {
            return AABB::rectangleToPoint((sf::FloatRect)m_image, Mouse::getPosition());
        }

        Image* setShader(sf::Shader* shader)
        {
            m_image.setShader(shader);
            return this;
        }

        template <typename T>
        Image* setUniform(const std::string& name, T value)
        {
            m_image.getShader()->setUniform(name, value);
            return this;
        }

        void draw(int8_t layer = PHYSICS_LAYER_UI_2) override
        {
            m_image.setPosition(m_position);
            m_image.setSize(m_size);
            m_image.setColor(m_color);
            m_application->draw(&m_image, layer);
        }
    private:
        RectangleShape m_image;
    };
}