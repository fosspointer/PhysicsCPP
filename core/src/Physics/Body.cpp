#include <Physics/Physics/Body.hpp>

inline static float vector_length(const sf::Vector2f& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline static sf::Vector2f vector_normalize(const sf::Vector2f& vector)
{
    return vector / vector_length(vector);
}

namespace physics
{
    Body::Body(Application* application, const sf::Color& color)
        :m_Application(application)
    {
        m_Rectangle.setFillColor(color);
        SetSize(sf::Vector2f{50.0f, 50.0f});
        Initialize();
    }

    Body::Body(Application* application, const sf::String& filepath)
        :m_Application(application)
    {
        SetTexture(filepath);
        SetSize((sf::Vector2f)m_Texture.getSize());
        Initialize();
    }

    void Body::SetTexture(const sf::String& filepath)
    {
        m_Texture.loadFromFile(filepath);
        m_Rectangle.setTexture(&m_Texture);
    }

    void Body::SetPosition(const sf::Vector2f& position)
    {
        m_Rectangle.setPosition(position);
    }

    void Body::SetVelocity(const sf::Vector2f& velocity)
    {
        m_Velocity = velocity;
    }

    void Body::SetAcceleration(const sf::Vector2f& acceleration)
    {
        m_Acceleration = acceleration;
    }

    void Body::SetSize(const sf::Vector2f& size, bool set_collider)
    {
        m_Rectangle.setSize(size);
        m_Rectangle.setOrigin(size / 2.0f);
        if(set_collider)
        {
            m_Collider.Position = {0.0f, 0.0f};
            m_Collider.Size = m_Rectangle.getSize(); 
        }
    }

    void Body::SetCollider(const sf::Vector2f& offset, const sf::Vector2f& size)
    {
        m_Collider.Position = offset;
        m_Collider.Size = size;
    }

    bool Body::CollidesWith(const Body* other) const
    {
        return AABB::RectangleToRectangle(
            AABB(m_Rectangle.getPosition() + m_Collider.Position, m_Collider.Size),
            AABB(other->m_Rectangle.getPosition() + other->m_Collider.Position, other->m_Collider.Size)
        );
    }

    void Body::AddForce(const sf::Vector2f& force, sf::String name)
    {
        name = name == "__default" ? sf::String("F") + std::to_string(m_Forces.size()) : name;
        m_Forces.push_back(Force{force, name});
    }

    const sf::Vector2f Body::GetTotalForce()
    {
        sf::Vector2f res {0.0f, 0.0f};

        for(auto& force : m_PreviousForces)
            res += force.Value;    

        return res;
    }

    void Body::DrawForces(float thickness) const
    {
        for(auto& force : m_PreviousForces)
            DrawForce(force);
    }

    void Body::Draw()
    {
        m_Application->GetWindow().draw(m_Rectangle);
    }

    void Body::DrawForce(const Force& force, const sf::Color& color) const
    {
        Arrow arrow(5.0f);
        arrow.SetStartPoint(m_Rectangle.getPosition());
        arrow.SetEndPoint(m_Rectangle.getPosition() + force.Value * Units::GetPixelsPerNewton());
        arrow.SetColor(color);
        m_Application->GetWindow().draw(arrow);

        sf::Text force_text(force.Name, Font::GetDefault().Get(), 30u);
        auto diff = arrow.GetEndPoint() - arrow.GetStartPoint();
        force_text.setPosition(
            arrow.GetStartPoint() + vector_normalize(diff) * (vector_length(diff) + 20.0f)
        );
        force_text.setOrigin(GetBoundSize(force_text) / 2.0f + GetBoundPosition(force_text));
        force_text.setFillColor(physics::Color::White);
        m_Application->GetWindow().draw(force_text);
    }

    sf::Vector2f Body::GetBoundSize(const sf::Text& text) const
    {
        return sf::Vector2f{text.getLocalBounds().width, text.getLocalBounds().height};
    }

    sf::Vector2f Body::GetBoundPosition(const sf::Text& text) const
    {
        return sf::Vector2f{text.getLocalBounds().left, text.getLocalBounds().top};
    }

    void Body::Initialize()
    {
        SetPosition((sf::Vector2f)m_Application->GetWindow().getSize() / 2.0f);
        m_Collider.Size = m_Rectangle.getSize();
        m_Collider.Position = sf::Vector2f{0.0f, 0.0f};
    }
}