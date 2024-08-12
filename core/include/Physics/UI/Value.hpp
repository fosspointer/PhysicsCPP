#pragma once
#include <Physics/System/Exception.hpp>
#include <Physics/System/Application.hpp>

namespace physics
{
    class Value
    {
    public:
        enum Type : uint8_t
        {
            Pixels, PercentageX, PercentageY, PercentageAVG
        };

        Value(float amount, Type type)
            :m_amount(amount), m_type(type)
        {}

        float get(Application* application)
        {
            switch(m_type)
            {
            case Type::Pixels: return m_amount;
            case Type::PercentageX: return (m_amount * 0.01f) / (float)application->getWindow().getSize().x; 
            case Type::PercentageY: return (m_amount * 0.01f) / (float)application->getWindow().getSize().y; 
            case Type::PercentageAVG: return (m_amount * 0.01f) / (0.5f * ((float)application->getWindow().getSize().y + (float)application->getWindow().getSize().x)); 
            default: throw PHYSICS_EXCEPTION("Value type not in enum range");
            }
        }
    private:
        float m_amount;
        Type m_type;
    };
}