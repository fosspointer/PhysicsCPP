#include <Physics/Physics/BodyHandler.hpp>

namespace physics
{   
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
            for(size_t j = 0; j < m_KinematicBodies.size(); j++)
            {
                if(i >= j) continue;
                auto& first = m_KinematicBodies[i];
                auto& second = m_KinematicBodies[j];

                if(!first->CollidesWith(second)) continue;

                first->AddForce(-first->GetWeight(), "N");
                second->AddForce(-second->GetWeight(), "N'");
            }

        for(const auto& kinematic_body : m_KinematicBodies)
        {
            bool free = true;
            for(const auto& static_body : m_StaticBodies)
            {
                if(!kinematic_body->CollidesWith(static_body)) continue;

                free = false;

                kinematic_body->AddForce(-kinematic_body->GetWeight(), "N");

                auto friction = CalculateFriction(0.25f, kinematic_body->GetVelocity(), length(kinematic_body->GetWeight()));
                
                kinematic_body->AddForce(friction, "T");
            }
            kinematic_body->m_Free = free;
        }
            
        for(auto& kinematic_body : m_KinematicBodies)
            kinematic_body->Update(delta_time);
    }

    float BodyHandler::length(const sf::Vector2f& vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y + vec.y);
    }

    sf::Vector2f BodyHandler::normalize(const sf::Vector2f& vec)
    {
        return vec / length(vec);
    }

    Vector2f CalculateFriction(float friction_coefficient, const Vector2f& velocity, float normal)
    {
        return velocity * -1.0f * friction_coefficient * normal;
    }

    Vector2f CalculateDrag(float drag_coefficient, const sf::Vector2f& velocity, const sf::Vector2f& normal_force, float area)
    {
        return {0.0f, 0.0f};
    }
}