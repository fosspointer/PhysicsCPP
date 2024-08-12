#include <Physics/System/Arrow.hpp>

namespace physics
{
    static float length(const Vector2f& vector)
    {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y);
    }

    static sf::Vector2f normalize(const Vector2f& vector)
    {
        auto len = length(vector);
        return vector / len;
    }

    Arrow::Arrow(float thickness, const Vector2f& head_size, const Color& color)
        :m_thickness(thickness), m_headSize(head_size), m_vertices(sf::PrimitiveType::Triangles, 9), m_color(color)
    {}

    void Arrow::setStartPoint(const Vector2f& point)
    {
        m_startPoint = point;
        updateVertices();
    }

    void Arrow::setEndPoint(const Vector2f& point)
    {
        m_endPoint = point;
        updateVertices();
    }

    void Arrow::setHeadSize(const Vector2f& size)
    {
        m_headSize = size;
        updateVertices();
    }

    void Arrow::setThickness(float thickness)
    {
        m_thickness = thickness;
        updateVertices();
    }

    void Arrow::setColor(const Color& color)
    {
        m_color = color;
        updateVertices();
    }


    void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_vertices, states);
    }

    void Arrow::updateVertices()
    {
        auto difference = m_endPoint - m_startPoint;
        auto end_base_point = m_startPoint + normalize(difference) * (std::max(length(difference) - m_headSize.x, 0.0f));
        auto normal = normalize(sf::Vector2f{difference.y, -difference.x});

        m_vertices[0] = sf::Vertex{m_startPoint + normal * m_thickness * 0.5f, m_color};
        m_vertices[1] = sf::Vertex{m_startPoint - normal * m_thickness * 0.5f, m_color};
        m_vertices[2] = sf::Vertex{end_base_point - normal * m_thickness * 0.5f, m_color};

        m_vertices[3] = sf::Vertex{end_base_point - normal * m_thickness * 0.5f, m_color};
        m_vertices[4] = sf::Vertex{end_base_point + normal * m_thickness * 0.5f, m_color};
        m_vertices[5] = sf::Vertex{m_startPoint + normal * m_thickness * 0.5f, m_color};

        m_vertices[6] = sf::Vertex{end_base_point - normal * m_headSize.y * 0.5f, m_color};
        m_vertices[7] = sf::Vertex{m_endPoint, m_color};
        m_vertices[8] = sf::Vertex{end_base_point + normal * m_headSize.y * 0.5f, m_color};
    }

    bool Arrow::collides(const Vector2f& point) const
    {
        auto difference = (m_endPoint - m_startPoint) / 2.0f;
        auto half_length = difference.length();
        auto origin = point - (m_startPoint + difference);
        auto angle = -std::atan2f(origin.y, origin.x);
        auto rotated = sf::Vector2f{origin.x * std::cosf(angle) - origin.y * std::sinf(angle), origin.x * std::sinf(angle) + origin.y * std::cosf(angle)};
        return rotated.x > -half_length && rotated.x < half_length && rotated.y > -0.5f * m_thickness && rotated.y < 0.5f * m_thickness;
    }
}