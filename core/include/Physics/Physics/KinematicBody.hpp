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
        using updateCallbackFunc = std::function<void(KinematicBody*, float)>;
        KinematicBody(Application* application, const sf::Color& color, float gravity_acceleration = Units::getGravityAcceleration());
        KinematicBody(Application* application, const sf::String& filepath, float gravity_acceleration = Units::getGravityAcceleration());

        virtual void update(float delta_time) override;
        virtual void drawForces(bool ignore_zero = false, float thickness = 5.0f) const override;
        
        void setMass(float mass);
        inline const float& getMass() const { return m_mass; }

        void addMomentum(const Vector2f& momentum);
        Vector2f getMomentum() const { return m_velocity / m_mass; }

        void setGravityAcceleration(float g);
        inline const float& getGravityAcceleration() const { return m_gravityAcceleration; }

        virtual const Vector2f getTotalForce() override;

        void setUpdateCallback(updateCallbackFunc func);

        inline sf::Vector2f getWeight() const
        {
            return sf::Vector2f{0.0f, m_gravityAcceleration} * m_mass;
        }

        inline bool getFree() const 
        {
            return m_free;
        }

    private:
        bool m_free = false;
        friend class BodyHandler;
        updateCallbackFunc m_updateCallback;
        float m_mass = 1.0f, m_gravityAcceleration;
    };
}