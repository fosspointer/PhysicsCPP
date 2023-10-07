#pragma once
#include <Physics/System/RectangleShape.hpp>
#include <Physics/UI/UIElement.hpp>

namespace physics
{
    class Image : public UIElement<Image> 
    {
    public:
        Image(Application* application, const sf::String& filepath)
            :UIElement(application), m_Image(m_Application->GetWindow())
        {
            m_Image.SetTexture(physics::Textures::Load(filepath));
            m_Size = (sf::Vector2f)m_Image.GetTexture()->getSize();
        }

        bool IsHovered() const override
        {
            return AABB::RectangleToPoint((sf::FloatRect)m_Image, Mouse::GetPosition());
        }

        Image* SetShader(sf::Shader* shader)
        {
            m_Image.SetShader(shader);
            return this;
        }

        template <typename T>
        Image* SetUniform(const std::string& name, T value)
        {
            m_Image.GetShader()->setUniform(name, value);
            return this;
        }

        void Draw(int8_t layer = PHYSICS_LAYER_UI_2) override
        {
            m_Image.SetPosition(m_Position);
            m_Image.SetSize(m_Size);
            m_Image.SetColor(m_Color);
            m_Application->Draw(&m_Image, layer);
        }
    private:
        RectangleShape m_Image;
    };
}