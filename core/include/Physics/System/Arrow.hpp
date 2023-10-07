#pragma once
#include <SFML/Graphics.hpp>

namespace physics
{
    /// @brief Arrow shape, used for displaying vectors
    class Arrow : public sf::Drawable
    {
    public:
        /// @brief Arrow constructor
        /// @param thickness thickness of the arrow
        /// @param head_size length of the arrow's head
        /// @param color the arrow's color
        Arrow(float thickness = 5.0f, const Vector2f& head_size = Vector2f{16.0f, 10.0f}, const Color& color = Color::White);

        void SetStartPoint(const Vector2f& point);
        void SetEndPoint(const Vector2f& point);
        void SetHeadSize(const Vector2f& size);
        void SetThickness(float thickness);
        void SetColor(const Color& color);

        inline const Color& GetColor() const { return m_Color; }
        inline float GetThickness() const { return m_Thickness; }
        inline const Vector2f& GetHeadSize() const { return m_HeadSize; }
        inline const Vector2f& GetStartPoint() const { return m_StartPoint; }
        inline const Vector2f& GetEndPoint() const { return m_EndPoint; }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void UpdateVertices();

        Color m_Color;
        Vector2f m_StartPoint, m_EndPoint, m_HeadSize;
        float m_Thickness;
        sf::VertexArray m_Vertices;
    };
}