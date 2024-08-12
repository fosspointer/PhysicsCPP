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
        auto dist = first->getPosition() - second->getPosition();
        auto half_size = (first->getSize() + second->getSize()) / 2.0f;
        auto diff = Vector2f{dist.x / half_size.x, dist.y / half_size.y};

        if(std::abs(diff.x) > std::abs(diff.y))
            return diff.x > 0.0f ? Vector2f{1.0f, 0.0f} : Vector2f{-1.0f, 0.0f};
        else
            return diff.y > 0.0f ? Vector2f{0.0f, 1.0f} : Vector2f{0.0f, -1.0f};
    }

    BodyHandler::~BodyHandler()
    {
        for(const auto& body : m_staticBodies)
            delete body;
        
        for(const auto& body : m_kinematicBodies)
            delete body;
    }

    void BodyHandler::draw()
    {
        for(const auto& body : m_staticBodies)
            body->draw();
        for(const auto& body : m_kinematicBodies)
            body->draw();
    }

    void BodyHandler::drawForces()
    {
        for(const auto& body: m_kinematicBodies)
            body->drawForces(m_kinematicBodies.size() == 1ul);
    }

    void BodyHandler::update(float delta_time)
    {
        for(auto& static_body : m_staticBodies)
            static_body->update(delta_time);

        for(size_t i = 0; i < m_kinematicBodies.size(); i++)
        {
            auto& first = m_kinematicBodies[i];
            bool free = true;
            for(size_t j = 0; j < m_kinematicBodies.size(); j++)
            {
                if(i >= j) continue;
                auto& second = m_kinematicBodies[j];

                if(!first->collidesWith(second)) continue;

                free = false;

                first->addForce(-first->getWeight(), "N");
                second->addForce(-second->getWeight(), "N'");
            }
            first->m_free = free;
        }

        for(const auto& kinematic_body : m_kinematicBodies)
        {
            bool free = true;
            for(const auto& static_body : m_staticBodies)
            {
                if(!kinematic_body->collidesWith(static_body)) continue;

                free = false;

                kinematic_body->addForce(-kinematic_body->getWeight(), "N");

                auto normal = calculateNormal(kinematic_body, static_body) * length(kinematic_body->getWeight());
                auto friction_coefficient = FrictionCoefficients::get(kinematic_body->getMaterial(), static_body->getMaterial());
                
                handleImpulse(kinematic_body, static_body);
                
                auto friction = calculateFriction(friction_coefficient, kinematic_body->getVelocity(), normal);

                kinematic_body->addForce(friction, "T");
            }
            kinematic_body->m_free = kinematic_body->m_free && free;
        }
            
        for(auto& kinematic_body : m_kinematicBodies)
            kinematic_body->update(delta_time);
    }

    Vector2f calculateFriction(float friction_coefficient, const Vector2f& velocity, const Vector2f& normal)
    {
        return -Vector2f{normal.y, normal.x}.abs() * velocity * friction_coefficient * 0.1f;
    }

    Vector2f calculateDrag(float drag_coefficient, float density, const Vector2f& velocity, float area)
    {
        return velocity.normalize() * density * velocity.length() * velocity.length() * area * drag_coefficient * -0.5f;
    }

    void handleImpulse(KinematicBody* kinematic_body, StaticBody* static_body)
    {
        const Vector2f normal = calculateNormal(kinematic_body, static_body);
        const float mass = kinematic_body->getMass();
        const float dot = kinematic_body->getMomentum().dot(normal);
        const float impulse = -dot * (1.0f + PHYSICS_COEFFICIENT_OF_RESTITUTION) * kinematic_body->getMass();
        kinematic_body->addMomentum(normal * impulse);
    }
}