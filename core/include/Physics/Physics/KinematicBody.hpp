#pragma once
#include <Physics/Physics/Units.hpp>
#include <Physics/Physics/Body.hpp>
#include <SFML/System.hpp>

namespace physics
{
    class KinematicBody : public Body
    {
    public: 
        KinematicBody(Application* application, const sf::Color& color, float gravity_acceleration = Units::GetGravityAcceleration());
        KinematicBody(Application* application, const sf::String& filepath, float gravity_acceleration = Units::GetGravityAcceleration());

        virtual void Update(float delta_time) override;
        virtual void DrawForces(float thickness = 5.0f) const override;
        
        void SetMass(float mass);
        inline const float& GetMass() const { return m_Mass; }

        void SetGravityAcceleration(float g);
        inline const float& GetGravityAcceleration() const { return m_GravityAcceleration; }

        virtual const sf::Vector2f GetTotalForce() override;

        inline sf::Vector2f GetWeight() const
        {
            return sf::Vector2f{0.0f, m_GravityAcceleration} * m_Mass;
        }

    private:
        float m_Mass = 1.0f, m_GravityAcceleration;
    };
}