#pragma once
#include <SFML/Graphics.hpp>

namespace physics
{
    /// @brief Arrow shape, used for displaying vectors
    class Arrow : public sf::Drawable
    {
    public:
        Arrow(float thickness = 5.0f, const sf::Vector2f& head_size = {16.0f, 10.0f}, const sf::Color& color = sf::Color::White);

        void SetStartPoint(const sf::Vector2f& point);
        void SetEndPoint(const sf::Vector2f& point);
        void SetHeadSize(const sf::Vector2f& size);
        void SetThickness(float thickness);
        void SetColor(const sf::Color& color);

        inline const sf::Color& GetColor() const { return m_Color; }
        inline float GetThickness() const { return m_Thickness; }
        inline const sf::Vector2f& GetHeadSize() const { return m_HeadSize; }
        inline const sf::Vector2f& GetStartPoint() const { return m_StartPoint; }
        inline const sf::Vector2f& GetEndPoint() const { return m_EndPoint; }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void UpdateVertices();

        sf::Color m_Color;
        sf::Vector2f m_StartPoint, m_EndPoint, m_HeadSize;
        float m_Thickness;
        sf::VertexArray m_Vertices;
    };
}