#include <Physics/Physics/Body.hpp>

inline static float vector_length(const physics::Vector2f& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline static physics::Vector2f vector_normalize(const physics::Vector2f& vector)
{
    return vector / vector_length(vector);
}

namespace physics
{
    Body::Body(Application* application, const Color& color)
        :m_application(application)
    {
        m_rectangle.setFillColor(color);
        setSize(Vector2f{50.0f, 50.0f});
        initialize();
    }

    Body::Body(Application* application, const sf::String& filepath)
        :m_application(application)
    {
        setTexture(filepath);
        setSize((Vector2f)m_texture.getSize());
        initialize();
    }

    void Body::setTexture(const sf::String& filepath)
    {
        m_texture.loadFromFile(filepath);
        m_rectangle.setTexture(&m_texture);
    }

    void Body::setPosition(const Vector2f& position)
    {
        m_rectangle.setPosition((sf::Vector2f)position);
    }

    void Body::setVelocity(const Vector2f& velocity)
    {
        m_velocity = velocity;
    }

    void Body::setAcceleration(const Vector2f& acceleration)
    {
        m_acceleration = acceleration;
    }

    void Body::setMaterial(Material* material)
    {
        m_material = material;
    }

    void Body::setSize(const Vector2f& size, bool set_collider)
    {
        m_rectangle.setSize((sf::Vector2f)size);
        m_rectangle.setOrigin(size / 2.0f);
        if(set_collider)
        {
            m_collider.position = {0.0f, 0.0f};
            m_collider.size = m_rectangle.getSize(); 
        }
    }

    void Body::setCollider(const Vector2f& offset, const Vector2f& size)
    {
        m_collider.position = offset;
        m_collider.size = size;
    }

    bool Body::collidesWith(const Body* other) const
    {
        return AABB::rectangleToRectangle(
            AABB(m_rectangle.getPosition() + m_collider.position, m_collider.size),
            AABB(other->m_rectangle.getPosition() + other->m_collider.position, other->m_collider.size)
        );
    }

    void Body::addForce(const Vector2f& force, sf::String name)
    {
        auto find = m_forceNames.find(name);
        
        if(find == m_forceNames.end())
        {
            m_forceNames.insert(std::pair(name, 0ul));
            m_forces.push_back(Force{force, name, 0ul});
            return;
        }
        
        m_forces.push_back(Force{force, name, ++find->second});
    }

    void Body::addForce(const Vector2f& force)
    {
        addForce(force, std::string{1ul, 'F'});
    }

    const Vector2f Body::getTotalForce()
    {
        Vector2f res = Vector2f::zero();

        for(auto& force: m_previousForces)
            res += force.value;    

        return res;
    }

    void Body::drawForces(bool ignore_zero, float thickness) const
    {
        for(auto& force: m_previousForces)
            drawForce(force, ignore_zero);
    }

    void Body::draw()
    {
        m_application->getWindow().draw(m_rectangle);
    }

    void Body::drawForce(const Force& force, bool ignore_zero, const Color& color) const
    {
        Arrow arrow(5.0f);
        arrow.setStartPoint(m_rectangle.getPosition());
        arrow.setEndPoint((Vector2f)m_rectangle.getPosition() + force.value * Units::getPixelsPerNewton());
        arrow.setColor(color);
        m_application->getWindow().draw(arrow);

        sf::Text force_text(force.name, Font::getDefault().get(), 30u);
        auto diff = arrow.getEndPoint() - arrow.getStartPoint();
        force_text.setPosition(
            arrow.getStartPoint() + vector_normalize(diff) * (vector_length(diff) + 20.0f)
        );
        force_text.setOrigin(getBoundSize(force_text) / 2.0f + getBoundPosition(force_text));
        force_text.setFillColor(physics::Color::White);
        m_application->getWindow().draw(force_text);

        if(ignore_zero && !force.subscript) return;

        sf::Text force_subscript(std::to_string(force.subscript), Font::getDefault().get(), 15u);

        force_subscript.setPosition(sf::Vector2f{force_text.getGlobalBounds().left + force_text.getGlobalBounds().width, 
            force_text.getGlobalBounds().top + force_text.getGlobalBounds().height});

        force_subscript.setFillColor(physics::Color::White);
        m_application->getWindow().draw(force_subscript);
    }

    Vector2f Body::getBoundSize(const sf::Text& text) const
    {
        return Vector2f{text.getLocalBounds().width, text.getLocalBounds().height};
    }

    Vector2f Body::getBoundPosition(const sf::Text& text) const
    {
        return Vector2f{text.getLocalBounds().left, text.getLocalBounds().top};
    }

    void Body::initialize()
    {
        setPosition((Vector2f)m_application->getWindow().getSize() / 2.0f);
        m_collider.size = m_rectangle.getSize();
        m_collider.position = Vector2f{0.0f, 0.0f};
        m_material = Materials::get("default");
    }
}