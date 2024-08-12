#pragma once
#include <Physics/System/Arrow.hpp>

namespace physics
{
    /// @brief 2-dimentional generic translation control
    class TranslationControl : public sf::Drawable
    {
    public:
        TranslationControl(float thickness = 5.0f, float length = 70.0f, const Vector2f& head_size = {16.0f, 10.0f}, float square_size = 25.0f,
            const Color& horizontal_color = Color::Red, const Color& vertical_color = Color::Green, const Color& square_color = Color::Blue)
            :m_length(length)
        {
            setColors(horizontal_color, vertical_color, square_color);
            setParameters(thickness, head_size, square_size);
        }

        void setColors(const Color& horizontal_color, const Color& vertical_color, const Color& square_color)
        {
            m_horizontalControl.setColor(horizontal_color);
            m_verticalControl.setColor(vertical_color);
            m_square.setFillColor(square_color);
        }

        void setHorizontalColor(const Color& color)
        {
            m_horizontalControl.setColor(color);
        }

        void setVerticalColor(const Color& color)
        {
            m_verticalControl.setColor(color);
        }

        void setSquareColor(const Color& color)
        {
            m_square.setFillColor(color);
        }

        void setLength(float length)
        {
            m_length = length;
        }

        void setOrigin(const Vector2f& origin)
        {
            m_horizontalControl.setStartPoint(origin);
            m_horizontalControl.setEndPoint(origin + Vector2f{m_length, 0.0f});
            m_verticalControl.setStartPoint(origin);
            m_verticalControl.setEndPoint(origin + Vector2f{0.0f, -m_length});
            m_square.setPosition(origin);
        }

        inline bool collidesHorizontal(const Vector2f& point) const { return m_horizontalControl.collides(point); }
        inline bool collidesVertical(const Vector2f& point) const { return m_verticalControl.collides(point); }
        inline bool collidesSquare(const Vector2f& point) const { return m_square.getGlobalBounds().contains(point); }
        inline const Arrow& getHorizontal() const { return m_horizontalControl; }
        inline const Arrow& getVertical() const { return m_verticalControl; }
        inline const sf::RectangleShape& getSquare() const { return m_square; }
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(m_square);
            target.draw(m_horizontalControl);
            target.draw(m_verticalControl);
        }

        void setParameters(float thickness, const Vector2f& head_size, float square_size)
        {
            m_horizontalControl.setThickness(thickness);
            m_horizontalControl.setHeadSize(head_size);

            m_verticalControl.setThickness(thickness);
            m_verticalControl.setHeadSize(head_size);

            m_square.setSize(sf::Vector2f{std::min(square_size, 20.0f), std::max(-square_size, -20.0f)});
        }

        Arrow m_horizontalControl, m_verticalControl;
        sf::RectangleShape m_square;
        float m_length;
    };
}