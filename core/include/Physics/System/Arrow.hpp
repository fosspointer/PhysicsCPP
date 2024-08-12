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

        void setStartPoint(const Vector2f& point);
        void setEndPoint(const Vector2f& point);
        void setHeadSize(const Vector2f& size);
        void setThickness(float thickness);
        void setColor(const Color& color);

        inline const Color& getColor() const { return m_color; }
        inline float getThickness() const { return m_thickness; }
        inline const Vector2f& getHeadSize() const { return m_headSize; }
        inline const Vector2f& getStartPoint() const { return m_startPoint; }
        inline const Vector2f& getEndPoint() const { return m_endPoint; }

        bool collides(const Vector2f& point) const;
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void updateVertices();

        Color m_color;
        Vector2f m_startPoint, m_endPoint, m_headSize;
        float m_thickness;
        sf::VertexArray m_vertices;
    };
}