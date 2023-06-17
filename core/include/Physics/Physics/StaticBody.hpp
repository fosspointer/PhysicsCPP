#pragma once
#include <Physics/Physics/Body.hpp>

namespace physics
{
    class StaticBody : public Body
    {
    public:
        StaticBody(Application* application, const sf::Color& color)
            :Body(application, color) {}

        StaticBody(Application* application, const sf::String& filepath)
            :Body(application, filepath) {}

        void Update(float delta_time) override
        {
            m_Forces.clear();
            m_PreviousForces = m_Forces;
        }
    };
}