#pragma once
#include "SFML/System/Vector2.hpp"
#include <Physics/Physics/Units.hpp>
#include <Physics/Physics/Body.hpp>

namespace physics
{
    class KinematicBody : public Body
    {
    public: 
        KinematicBody(Application* application, const sf::Color& color)
            :Body(application, color)
        {
        }

        KinematicBody(Application* application, const sf::String& filepath)
            :Body(application, filepath)
        {
        }

        void SetMass(float mass)
        {
            m_Mass = mass;
        }

        void SetGravityAcceleration(float g)
        {
            m_GravityAcceleration = g;
        }

        virtual const sf::Vector2f GetTotalForce() override
        {
            sf::Vector2f res = Body::GetTotalForce();
            res += GetWeight();

            return res;
        }

        sf::Vector2f GetWeight() const
        {
            return sf::Vector2f{0.0f, m_GravityAcceleration} * m_Mass;
        }

        virtual void Update(float delta_time) override
        {
            for(auto& force : m_Forces)
                m_Acceleration += force.Value / m_Mass;

            m_Velocity += m_Acceleration * delta_time * Units::GetPixelsPerMeter() + sf::Vector2f{0.0f, m_GravityAcceleration * Units::GetPixelsPerMeter()} * delta_time;
            auto new_position = m_Rectangle.getPosition() + m_Velocity * delta_time + m_Acceleration * delta_time * delta_time * 0.5f;
            m_Rectangle.setPosition(new_position);
            m_PreviousForces = m_Forces;
            m_Forces.clear();
            m_Acceleration = sf::Vector2f{0.0f, 0.0f};
        }

        virtual void DrawForces(float thickness = 5.0f) const override
        {
            Body::DrawForces(thickness);

            auto weight = GetWeight();
            sf::RectangleShape weight_rect;
            weight_rect.setSize({std::sqrt(weight.x * weight.x + weight.y * weight.y) * Units::GetPixelsPerNewton(), thickness});
            weight_rect.setRotation(std::atan2(weight.y, weight.x) * 57.2958f);
            weight_rect.setPosition(m_Rectangle.getPosition());
            weight_rect.setFillColor(sf::Color::Red);
            weight_rect.setOrigin({0.0f, thickness / 2.0f});
            m_Application->GetWindow().draw(weight_rect);

            sf::Text weight_text("w", Font::GetDefault().Get(), 30u);
            weight_text.setPosition(weight_rect.getPosition() - sf::Vector2f{0.0f, - thickness / 2.0f} + 
                sf::Vector2f{
                    (weight_rect.getSize().x + 20.0f) * std::cos(weight_rect.getRotation() / physics::Units::DegreesPerRadian()),
                    (weight_rect.getSize().x + 20.0f) * std::sin(weight_rect.getRotation() / physics::Units::DegreesPerRadian())});
            weight_text.setOrigin(weight_text.getGlobalBounds().width / 2.0f, weight_text.getGlobalBounds().height / 2.0f);
            weight_text.setFillColor(physics::Colors::White);
            m_Application->GetWindow().draw(weight_text);
        }
    private:
        float m_Mass = 1.0f, m_GravityAcceleration = 9.8066f;
    };
}