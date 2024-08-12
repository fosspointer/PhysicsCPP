#pragma once
#include <vector>
#include <Physics/Physics/Body.hpp>
#include <Physics/Physics/KinematicBody.hpp>
#include <Physics/Physics/StaticBody.hpp>
#include <Physics/System/Application.hpp>

#define PHYSICS_COEFFICIENT_OF_RESTITUTION 50.0f

namespace physics 
{
    Vector2f calculateFriction(float friction_coefficient, const Vector2f& velocity, const Vector2f& normal_force);
    Vector2f calculateDrag(float drag_coefficient, float density, const Vector2f& velocity, float area);
    void handleImpulse(KinematicBody* kinematic_body, StaticBody* static_body);

    class BodyHandler
    {
    public:
        BodyHandler() = default;
        ~BodyHandler();

        void update(float delta_time);
        void draw();
        void drawForces();

        template <class... Args>
        KinematicBody* addKinematicBody(Application* application, Args... args)
        {
            m_kinematicBodies.push_back(new KinematicBody(application, args...));
            return m_kinematicBodies.back();
        }

        template <class... Args>
        StaticBody* addStaticBody(Application* application, Args... args)
        {
            m_staticBodies.push_back(new StaticBody(application, args...));
            return m_staticBodies.back();
        }
    private:
        std::vector<KinematicBody*> m_kinematicBodies;
        std::vector<StaticBody*> m_staticBodies;
    };
}  