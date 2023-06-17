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

        for(size_t i = 0; i < m_KinematicBodies.size(); i++)
        {
            auto& kinematic_body = m_KinematicBodies[i];
            for(size_t j = 0; j < m_StaticBodies.size(); j++)
            {
                auto& static_Body = m_StaticBodies[j];

                if(!kinematic_body->CollidesWith(static_Body)) continue;

                kinematic_body->AddForce(-kinematic_body->GetWeight(), "N");

                auto mov = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) * 1.0f + 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Left) * -1.0f;

                auto jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

                kinematic_body->AddForce({mov * 500.0f, 0.0f}, "F");

                if(jump)
                    kinematic_body->GetVelocity().y = -230.0f;
                else kinematic_body->GetVelocity().y = 0.0f;
                
                auto f = CalculateFriction(0.25f, kinematic_body->GetVelocity(), length(kinematic_body->GetWeight()));
                std::cout << "[" << f.x << ", " << f.y << "]\n";

                kinematic_body->AddForce(
                    f, "T");
            }
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