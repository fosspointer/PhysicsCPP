#include <Physics/Physics/BodyHandler.hpp>
#include <Physics/Physics/FrictionCoefficients.hpp>

namespace physics
{
    static float length(const Vector2f& vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    static Vector2f calculateNormal(Body* first, Body* second)
    {
        auto dist = first->GetPosition() - second->GetPosition();
        auto half_size = (first->GetSize() + second->GetSize()) / 2.0f;
        auto diff = Vector2f{dist.x / half_size.x, dist.y / half_size.y};

        if(std::abs(diff.x) > std::abs(diff.y))
            return diff.x > 0.0f ? Vector2f{1.0f, 0.0f} : Vector2f{-1.0f, 0.0f};
        else
            return diff.y > 0.0f ? Vector2f{0.0f, 1.0f} : Vector2f{0.0f, -1.0f};
    }

    BodyHandler::~BodyHandler()
    {
        for(const auto& body : m_StaticBodies)
            delete body;
        
        for(const auto& body : m_KinematicBodies)
            delete body;
    }

    void BodyHandler::Draw()
    {
        for(const auto& body : m_StaticBodies)
            body->Draw();
        for(const auto& body : m_KinematicBodies)
            body->Draw();
    }

    void BodyHandler::DrawForces()
    {
        for(const auto& body : m_StaticBodies)
            body->DrawForces();
        for(const auto& body : m_KinematicBodies)
            body->DrawForces();
    }

    void BodyHandler::Update(float delta_time)
    {
        for(auto& static_body : m_StaticBodies)
            static_body->Update(delta_time);

        for(size_t i = 0; i < m_KinematicBodies.size(); i++)
        {
            auto& first = m_KinematicBodies[i];
            bool free = true;
            for(size_t j = 0; j < m_KinematicBodies.size(); j++)
            {
                if(i >= j) continue;
                auto& second = m_KinematicBodies[j];

                if(!first->CollidesWith(second)) continue;

                free = false;

                first->AddForce(-first->GetWeight(), "N");
                second->AddForce(-second->GetWeight(), "N'");
            }
            first->m_Free = free;
        }

        for(const auto& kinematic_body : m_KinematicBodies)
        {
            bool free = true;
            for(const auto& static_body : m_StaticBodies)
            {
                if(!kinematic_body->CollidesWith(static_body)) continue;

                free = false;

                kinematic_body->AddForce(-kinematic_body->GetWeight(), "N");

                auto normal = calculateNormal(kinematic_body, static_body) * length(kinematic_body->GetWeight());
                auto friction_coefficient = FrictionCoefficients::Get(kinematic_body->GetMaterial(), static_body->GetMaterial());
                
                HandleImpulse(kinematic_body, static_body);
                
                auto friction = CalculateFriction(friction_coefficient, kinematic_body->GetVelocity(), normal);

                kinematic_body->AddForce(friction, "T");
            }
            kinematic_body->m_Free = kinematic_body->m_Free && free;
        }
            
        for(auto& kinematic_body : m_KinematicBodies)
            kinematic_body->Update(delta_time);
    }

    Vector2f CalculateFriction(float friction_coefficient, const Vector2f& velocity, const Vector2f& normal)
    {
        return -Vector2f{normal.y, normal.x}.abs() * velocity * friction_coefficient * 0.1f;
    }

    Vector2f CalculateDrag(float drag_coefficient, float density, const Vector2f& velocity, float area)
    {
        return velocity.normalize() * density * velocity.length() * velocity.length() * area * drag_coefficient * -0.5f;
    }

    void HandleImpulse(KinematicBody* kinematic_body, StaticBody* static_body)
    {
        const Vector2f normal = calculateNormal(kinematic_body, static_body);
        const float mass = kinematic_body->GetMass();
        const float dot = kinematic_body->GetMomentum().dot(normal);
        const float impulse = -dot * (1.0f + PHYSICS_COEFFICIENT_OF_RESTITUTION) * kinematic_body->GetMass();
        kinematic_body->AddMomentum(normal * impulse);
    }
}