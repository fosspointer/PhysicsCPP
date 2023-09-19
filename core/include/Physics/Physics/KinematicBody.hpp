#pragma once
#include <functional>
#include <SFML/System.hpp>
#include <Physics/Physics/Units.hpp>
#include <Physics/Physics/Body.hpp>

namespace physics
{
    class KinematicBody : public Body
    {
    public: 
        using UpdateCallbackFunc = std::function<void(KinematicBody*, float)>;
        KinematicBody(Application* application, const sf::Color& color, float gravity_acceleration = Units::GetGravityAcceleration());
        KinematicBody(Application* application, const sf::String& filepath, float gravity_acceleration = Units::GetGravityAcceleration());

        virtual void Update(float delta_time) override;
        virtual void DrawForces(float thickness = 5.0f) const override;
        
        void SetMass(float mass);
        inline const float& GetMass() const { return m_Mass; }

        void SetGravityAcceleration(float g);
        inline const float& GetGravityAcceleration() const { return m_GravityAcceleration; }

        virtual const Vector2f GetTotalForce() override;

        void SetUpdateCallback(UpdateCallbackFunc func);

        inline sf::Vector2f GetWeight() const
        {
            return sf::Vector2f{0.0f, m_GravityAcceleration} * m_Mass;
        }

        inline bool GetFree() const 
        {
            return m_Free;
        }

    private:
        bool m_Free = false;
        friend class BodyHandler;
        UpdateCallbackFunc m_UpdateCallback;
        float m_Mass = 1.0f, m_GravityAcceleration;
    };
}