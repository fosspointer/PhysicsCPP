#pragma once
#include <Physics/System/Arrow.hpp>

namespace physics
{
    /// @brief 2-dimentional generic translation control
    class TranslationControl : public sf::Drawable
    {
    public:
        TranslationControl(float thickness = 5.0f, float length = 70.0f, const Vector2f& head_size = {16.0f, 10.0f}, const Color& horizontal_color = Color::Green, const Color& vertical_color = Color::Blue)
            :m_Length(length)
        {
            SetColors(horizontal_color, vertical_color);
            SetParameters(thickness, head_size);
        }

        void SetColors(const Color& horizontal_color, const Color& vertical_color)
        {
            m_HorizontalControl.SetColor(horizontal_color);
            m_VerticalControl.SetColor(vertical_color);
        }

        void SetLength(float length)
        {
            m_Length = length;
        }

        void SetOrigin(const Vector2f& origin)
        {
            m_HorizontalControl.SetStartPoint(origin);
            m_HorizontalControl.SetEndPoint(origin + Vector2f{m_Length, 0.0f});
            m_VerticalControl.SetStartPoint(origin);
            m_VerticalControl.SetEndPoint(origin + Vector2f{0.0f, -m_Length});
        }
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(m_HorizontalControl);
            target.draw(m_VerticalControl);
        }

        void SetParameters(float thickness, const Vector2f& head_size)
        {
            m_HorizontalControl.SetThickness(thickness);
            m_HorizontalControl.SetHeadSize(head_size);

            m_VerticalControl.SetThickness(thickness);
            m_VerticalControl.SetHeadSize(head_size);
        }

        Arrow m_HorizontalControl, m_VerticalControl;
        float m_Length;
    };
}