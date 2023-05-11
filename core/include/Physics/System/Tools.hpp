#pragma once
#include <Physics/System/Exception.hpp>
#include <Physics/System/Value.hpp>

namespace physics
{
    class Tools
    {
    public:
        static Value Position(const Value& delta_time, const Value& initial_position, const Value& velocity, const Value& acceleration = Value(0.0f))
        {
            if(delta_time.m_Type != Value::Type::Scalar
            || initial_position.m_Type != Value::Type::Vector
            || velocity.m_Type != Value::Type::Vector
            || acceleration.m_Type != Value::Type::Vector) 
                throw PHYSICS_EXCEPTION("Incorrect argument units provided: mismatch between scalar and vector values");
            
            return Value(initial_position.m_Vector + velocity.m_Vector * delta_time.m_Scalar + acceleration.m_Vector * delta_time.m_Scalar * delta_time.m_Scalar * 0.5f);
        }

        static Value Velocity(const Value& delta_time, const Value& initial_velocity, const Value& acceleration)
        {
            if(delta_time.m_Type != Value::Type::Scalar
            || initial_velocity.m_Type != Value::Type::Vector
            || acceleration.m_Type != Value::Type::Vector)
                throw PHYSICS_EXCEPTION("Incorrect argument units provided: mismatch between scalar and vector values");
            
            return Value(initial_velocity.m_Vector + acceleration.m_Vector * delta_time.m_Scalar);
        }
    };
}