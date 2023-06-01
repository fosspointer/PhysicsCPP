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
            :m_Amount(amount), m_Type(type)
        {}

        float Get(Application* application)
        {
            switch(m_Type)
            {
            case Type::Pixels: return m_Amount;
            case Type::PercentageX: return (m_Amount * 0.01f) / (float)application->GetWindow().getSize().x; 
            case Type::PercentageY: return (m_Amount * 0.01f) / (float)application->GetWindow().getSize().y; 
            case Type::PercentageAVG: return (m_Amount * 0.01f) / (0.5f * ((float)application->GetWindow().getSize().y + (float)application->GetWindow().getSize().x)); 
            default: throw PHYSICS_EXCEPTION("Value type not in enum range");
            }
        }
    private:
        float m_Amount;
        Type m_Type;
    };
}