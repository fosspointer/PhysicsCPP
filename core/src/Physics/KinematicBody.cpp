#include <Physics/Physics/KinematicBody.hpp>
#include <Physics/Physics/Units.hpp>
#include <Physics/Physics/Body.hpp>
#include <SFML/System.hpp>

namespace physics
{
    KinematicBody::KinematicBody(Application* application, const sf::Color& color, float gravity_acceleration)
        :Body(application, color), m_GravityAcceleration(gravity_acceleration)
    {}

    KinematicBody::KinematicBody(Application* application, const sf::String& filepath, float gravity_acceleration)
        :Body(application, filepath), m_GravityAcceleration(gravity_acceleration)
    {}

    void KinematicBody::SetMass(float mass)
    {
        m_Mass = mass;
    }

    void KinematicBody::AddMomentum(const Vector2f& momentum)
    {
        m_Velocity += momentum / m_Mass;
    }

    void KinematicBody::SetGravityAcceleration(float g)
    {
        m_GravityAcceleration = g;
    }

    const Vector2f KinematicBody::GetTotalForce()
    {
        Vector2f res = Body::GetTotalForce();
        res += GetWeight();

        return res;
    }

    void KinematicBody::Update(float delta_time)
    {
        m_DeltaTime = delta_time;

        if(m_UpdateCallback)
            m_UpdateCallback(this, delta_time);

        for(auto& force : m_Forces)
            m_Acceleration += force.Value / m_Mass;

        m_Velocity += m_Acceleration * delta_time * Units::GetPixelsPerMeter() + sf::Vector2f{0.0f, m_GravityAcceleration * Units::GetPixelsPerMeter()} * delta_time;
        sf::Vector2f new_position = m_Rectangle.getPosition() + (sf::Vector2f)m_Velocity * delta_time + (sf::Vector2f)m_Acceleration * delta_time * delta_time * 0.5f;
        m_Rectangle.setPosition(new_position);
        m_PreviousForces = m_Forces;
        m_Forces.clear();
        m_Acceleration = Vector2f::Zero();
    }

    void KinematicBody::DrawForces(float thickness) const
    {
        Body::DrawForces(thickness);
        DrawForce(Force{GetWeight(), "w"}, physics::Color::Red);
    }

    void KinematicBody::SetUpdateCallback(UpdateCallbackFunc func)
    {
        m_UpdateCallback = func;
    }
}