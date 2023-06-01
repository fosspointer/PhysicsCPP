#pragma once
#include "Physics/System/Colors.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <Physics/System/Application.hpp>
#include <cmath>
#include <string>

namespace physics
{
    struct Force
    {
        sf::Vector2f Value;
        sf::String Name;
    };

    class Body
    {
    public:
        Body(Application* application, const sf::Color& color)
            :m_Application(application)
        {
            m_Rectangle.setFillColor(color);
            SetSize(sf::Vector2f{50.0f, 50.0f});
            Initialize();
        }

        Body(Application* application, const sf::String& filepath)
            :m_Application(application)
        {
            SetTexture(filepath);
            SetSize((sf::Vector2f)m_Texture.getSize());
            Initialize();
        }

        void SetTexture(const sf::String& filepath)
        {
            m_Texture.loadFromFile(filepath);
            m_Rectangle.setTexture(&m_Texture);
        }

        void SetPosition(const sf::Vector2f& position)
        {
            m_Rectangle.setPosition(position);
        }

        void SetVelocity(const sf::Vector2f& velocity)
        {
            m_Velocity = velocity;
        }

        void SetAcceleration(const sf::Vector2f& acceleration)
        {
            m_Acceleration = acceleration;
        }

        void SetSize(const sf::Vector2f& size, bool set_collider = true)
        {
            m_Rectangle.setSize(size);
            m_Rectangle.setOrigin(size / 2.0f);
            if(set_collider)
            {
                m_Collider.Position = {0.0f, 0.0f};
                m_Collider.Size = m_Rectangle.getSize(); 
            }
        }

        void SetCollider(const sf::Vector2f& offset, const sf::Vector2f& size)
        {
            m_Collider.Position = offset;
            m_Collider.Size = size;
        }

        bool CollidesWith(const Body* other) const
        {
            return AABB::RectangleToRectangle(
                AABB(m_Rectangle.getPosition() + m_Collider.Position, m_Collider.Size),
                AABB(other->m_Rectangle.getPosition() + other->m_Collider.Position, other->m_Collider.Size)
            );
        }

        void AddForce(const sf::Vector2f& force, sf::String name = "default")
        {
            name = name == "default" ? sf::String("F") + std::to_string(m_Forces.size()) : name;
            m_Forces.push_back(Force{force, name});
        }

        inline const sf::Vector2f& GetPosition() const { return m_Rectangle.getPosition(); }
        inline const sf::Vector2f& GetSize() const { return m_Rectangle.getSize(); }
        inline const sf::Vector2f& GetVelocity() const { return m_Velocity; }
        inline const sf::Vector2f& GetAcceleration() const { return m_Acceleration; }

        inline sf::Vector2f& GetVelocity() { return m_Velocity; }
        inline sf::Vector2f& GetAcceleration() { return m_Acceleration; }

        inline AABB GetOutterAABB() const { return m_Rectangle; }
        inline AABB GetInnerAABB() const { return AABB(m_Rectangle.getPosition() + m_Collider.Position, m_Collider.Size); }

        virtual const sf::Vector2f GetTotalForce()
        {
            sf::Vector2f res {0.0f, 0.0f};

            for(auto& force : m_PreviousForces)
                res += force.Value;    

            return res;
        }

        virtual void DrawForces(float thickness = 5.0f) const
        {
            for(auto& force : m_PreviousForces)
            {
                sf::RectangleShape force_rect;
                force_rect.setSize({std::sqrt(force.Value.x * force.Value.x + force.Value.y * force.Value.y) * Units::GetPixelsPerNewton(), thickness});
                force_rect.setRotation(std::atan2(force.Value.y, force.Value.x) * 57.2958f);
                force_rect.setPosition(m_Rectangle.getPosition());
                force_rect.setOrigin({0.0f, thickness / 2.0f});
                m_Application->GetWindow().draw(force_rect);

                sf::Text force_text(force.Name, Font::GetDefault().Get(), 30u);
                force_text.setPosition(force_rect.getPosition() - sf::Vector2f{0.0f, - thickness / 2.0f} + 
                    sf::Vector2f{
                        (force_rect.getSize().x + 20.0f) * std::cos(force_rect.getRotation() / physics::Units::DegreesPerRadian()),
                        (force_rect.getSize().x + 20.0f) * std::sin(force_rect.getRotation() / physics::Units::DegreesPerRadian())});
                force_text.setOrigin(GetBoundSize(force_text) / 2.0f + GetBoundPosition(force_text));
                force_text.setFillColor(physics::Colors::White);
                m_Application->GetWindow().draw(force_text);
            }
        }

        void Draw()
        {
            m_Application->GetWindow().draw(m_Rectangle);
        }

        virtual void Update(float delta_time)
        {
            m_PreviousForces = m_Forces;
            m_Forces.clear();
        }
    protected:
        AABB m_Collider;
        Application* m_Application;
        sf::RectangleShape m_Rectangle;
        sf::Texture m_Texture;
        sf::Vector2f m_Velocity;
        sf::Vector2f m_Acceleration;
        std::vector<Force> m_Forces, m_PreviousForces;

        sf::Vector2f GetBoundSize(const sf::Text& text) const
        {
            return sf::Vector2f{text.getLocalBounds().width, text.getLocalBounds().height};
        }

        sf::Vector2f GetBoundPosition(const sf::Text& text) const
        {
            return sf::Vector2f{text.getLocalBounds().left, text.getLocalBounds().top};
        }

    private:
        void Initialize()
        {
            SetPosition((sf::Vector2f)m_Application->GetWindow().getSize() / 2.0f);
            m_Collider.Size = m_Rectangle.getSize();
            m_Collider.Position = sf::Vector2f{0.0f, 0.0f};
        }
    };
}