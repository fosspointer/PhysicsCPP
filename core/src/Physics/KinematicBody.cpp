#include <Physics/Physics/KinematicBody.hpp>
#include <Physics/Physics/Units.hpp>
#include <Physics/Physics/Body.hpp>
#include <SFML/System.hpp>

namespace physics
{
    KinematicBody::KinematicBody(Application* application, const sf::Color& color, float gravity_acceleration)
        :Body(application, color), m_gravityAcceleration(gravity_acceleration)
    {}

    KinematicBody::KinematicBody(Application* application, const sf::String& filepath, float gravity_acceleration)
        :Body(application, filepath), m_gravityAcceleration(gravity_acceleration)
    {}

    void KinematicBody::setMass(float mass)
    {
        m_mass = mass;
    }

    void KinematicBody::addMomentum(const Vector2f& momentum)
    {
        m_velocity += momentum / m_mass;
    }

    void KinematicBody::setGravityAcceleration(float g)
    {
        m_gravityAcceleration = g;
    }

    const Vector2f KinematicBody::getTotalForce()
    {
        Vector2f res = Body::getTotalForce();
        res += getWeight();

        return res;
    }

    void KinematicBody::update(float delta_time)
    {
        m_deltaTime = delta_time;

        if(m_updateCallback)
            m_updateCallback(this, delta_time);

        for(auto& force : m_forces)
            m_acceleration += force.value / m_mass;

        m_velocity += m_acceleration * delta_time * Units::getPixelsPerMeter() + sf::Vector2f{0.0f, m_gravityAcceleration * Units::getPixelsPerMeter()} * delta_time;
        sf::Vector2f new_position = m_rectangle.getPosition() + (sf::Vector2f)m_velocity * delta_time + (sf::Vector2f)m_acceleration * delta_time * delta_time * 0.5f;
        m_rectangle.setPosition(new_position);
        m_previousForces = m_forces;
        clearForces();
        m_acceleration = Vector2f::zero();
    }

    void KinematicBody::drawForces(bool ignore_zero, float thickness) const
    {
        Body::drawForces(ignore_zero, thickness);
        drawForce(Force{getWeight(), "w"}, ignore_zero, physics::Color::Red);
    }

    void KinematicBody::setUpdateCallback(updateCallbackFunc func)
    {
        m_updateCallback = func;
    }
}