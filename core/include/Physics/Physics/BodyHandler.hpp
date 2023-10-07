#pragma once
#include <vector>
#include <Physics/Physics/Body.hpp>
#include <Physics/Physics/KinematicBody.hpp>
#include <Physics/Physics/StaticBody.hpp>
#include <Physics/System/Application.hpp>

#define PHYSICS_COEFFICIENT_OF_RESTITUTION 50.0f

namespace physics 
{
    Vector2f CalculateFriction(float friction_coefficient, const Vector2f& velocity, const Vector2f& normal_force);
    Vector2f CalculateDrag(float drag_coefficient, float density, const Vector2f& velocity, float area);
    void HandleImpulse(KinematicBody* kinematic_body, StaticBody* static_body);

    class BodyHandler
    {
    public:
        BodyHandler() = default;
        ~BodyHandler();

        void Update(float delta_time);
        void Draw();
        void DrawForces();

        template <class... Args>
        KinematicBody* AddKinematicBody(Application* application, Args... args)
        {
            m_KinematicBodies.push_back(new KinematicBody(application, args...));
            return m_KinematicBodies.back();
        }

        template <class... Args>
        StaticBody* AddStaticBody(Application* application, Args... args)
        {
            m_StaticBodies.push_back(new StaticBody(application, args...));
            return m_StaticBodies.back();
        }
    private:
        std::vector<KinematicBody*> m_KinematicBodies;
        std::vector<StaticBody*> m_StaticBodies;
    };
}  