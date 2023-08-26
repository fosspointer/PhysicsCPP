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
        :m_Thickness(thickness), m_HeadSize(head_size), m_Vertices(sf::PrimitiveType::Triangles, 9), m_Color(color)
    {}

    void Arrow::SetStartPoint(const Vector2f& point)
    {
        m_StartPoint = point;
        UpdateVertices();
    }

    void Arrow::SetEndPoint(const Vector2f& point)
    {
        m_EndPoint = point;
        UpdateVertices();
    }

    void Arrow::SetHeadSize(const Vector2f& size)
    {
        m_HeadSize = size;
        UpdateVertices();
    }

    void Arrow::SetThickness(float thickness)
    {
        m_Thickness = thickness;
        UpdateVertices();
    }

    void Arrow::SetColor(const Color& color)
    {
        m_Color = color;
        UpdateVertices();
    }


    void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_Vertices, states);
    }

    void Arrow::UpdateVertices()
    {
        auto difference = m_EndPoint - m_StartPoint;
        auto end_base_point = m_StartPoint + normalize(difference) * (std::max(length(difference) - m_HeadSize.x, 0.0f));
        auto normal = normalize(sf::Vector2f{difference.y, -difference.x});

        m_Vertices[0] = sf::Vertex{m_StartPoint + normal * m_Thickness * 0.5f, m_Color};
        m_Vertices[1] = sf::Vertex{m_StartPoint - normal * m_Thickness * 0.5f, m_Color};
        m_Vertices[2] = sf::Vertex{end_base_point - normal * m_Thickness * 0.5f, m_Color};

        m_Vertices[3] = sf::Vertex{end_base_point - normal * m_Thickness * 0.5f, m_Color};
        m_Vertices[4] = sf::Vertex{end_base_point + normal * m_Thickness * 0.5f, m_Color};
        m_Vertices[5] = sf::Vertex{m_StartPoint + normal * m_Thickness * 0.5f, m_Color};

        m_Vertices[6] = sf::Vertex{end_base_point - normal * m_HeadSize.y * 0.5f, m_Color};
        m_Vertices[7] = sf::Vertex{m_EndPoint, m_Color};
        m_Vertices[8] = sf::Vertex{end_base_point + normal * m_HeadSize.y * 0.5f, m_Color};
    }
}