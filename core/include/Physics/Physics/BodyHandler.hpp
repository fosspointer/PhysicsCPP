#pragma once
#include <vector>
#include <Physics/Physics/Body.hpp>
#include <Physics/Physics/KinematicBody.hpp>
#include <Physics/Physics/StaticBody.hpp>
#include <Physics/System/Application.hpp>

namespace physics 
{
    class BodyHandler
    {
    public:
        BodyHandler() = default;
        
        ~BodyHandler()
        {
            for(const auto& body : m_StaticBodies)
                delete body;

            for(const auto& body : m_KinematicBodies)
                delete body;
        }

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

        void Update(float delta_time)
        {
            for(auto& static_body : m_StaticBodies)
                static_body->Update(delta_time);

            // for(size_t i = 0; i < m_KinematicBodies.size(); i++)
            //     for(size_t j = 0; j < m_KinematicBodies.size(); j++)
            //     {
            //         if(i >= j) continue;
            //     }

            for(size_t i = 0; i < m_KinematicBodies.size(); i++)
            {
                auto& kinematic_body = m_KinematicBodies[i];
                for(size_t j = 0; j < m_StaticBodies.size(); j++)
                {
                    auto& static_Body = m_StaticBodies[j];

                    if(kinematic_body->CollidesWith(static_Body))
                    {
                        kinematic_body->AddForce(-kinematic_body->GetWeight(), "N");
                        kinematic_body->GetVelocity().y = 0.0f;
                        // kinematic_body->AddForce({2.5f, 0.0f}, L"Fк");
                        // kinematic_body->AddForce({-2.5f, 0.0f}, "T");
                    }
                }
            }
                

            for(auto& kinematic_body : m_KinematicBodies)
                kinematic_body->Update(delta_time);
        }

        void Draw()
        {
            for(const auto& body : m_StaticBodies)
                body->Draw();
            for(const auto& body : m_StaticBodies)
                body->DrawForces();

            for(const auto& body : m_KinematicBodies)
                body->Draw();
            for(const auto& body : m_KinematicBodies)
                body->DrawForces();
        }
    private:
        std::vector<KinematicBody*> m_KinematicBodies;
        std::vector<StaticBody*> m_StaticBodies;
    };
}