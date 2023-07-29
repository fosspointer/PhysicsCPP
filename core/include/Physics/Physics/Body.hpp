#pragma once
#include <SFML/Graphics.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/System/Arrow.hpp>
#include <cmath>
#include <string>

namespace physics
{
    
    /// @brief Vector-string pair representing a named force
    struct Force
    {
        sf::Vector2f Value;
        sf::String Name;
    };

    /// @brief Abstract class representing a body
    class Body
    {
    public:
        Body(Application* application, const sf::Color& color);
        Body(Application* application, const sf::String& filepath);

        void SetTexture(const sf::String& filepath);

        void SetPosition(const sf::Vector2f& position);
        void SetVelocity(const sf::Vector2f& velocity);
        void SetAcceleration(const sf::Vector2f& acceleration);
        
        void SetSize(const sf::Vector2f& size, bool set_collider = true);
        void SetCollider(const sf::Vector2f& offset, const sf::Vector2f& size);
        bool CollidesWith(const Body* other) const;
        
        void AddForce(const sf::Vector2f& force, sf::String name = "__default");

        inline const sf::Vector2f& GetPosition() const { return m_Rectangle.getPosition(); }
        inline const sf::Vector2f& GetSize() const { return m_Rectangle.getSize(); }
        inline const sf::Vector2f& GetVelocity() const { return m_Velocity; }
        inline const sf::Vector2f& GetAcceleration() const { return m_Acceleration; }

        inline sf::Vector2f& GetVelocity() { return m_Velocity; }
        inline sf::Vector2f& GetAcceleration() { return m_Acceleration; }

        inline AABB GetOutterAABB() const { return m_Rectangle; }
        inline AABB GetInnerAABB() const { return AABB(m_Rectangle.getPosition() + m_Collider.Position, m_Collider.Size); }

        virtual const sf::Vector2f GetTotalForce();

        virtual void DrawForces(float thickness = 5.0f) const;
        void Draw();

        virtual void Update(float delta_time) = 0;
    protected:
        void DrawForce(const Force& force, const sf::Color& color = sf::Color::White) const;
        AABB m_Collider;
        Application* m_Application;
        sf::RectangleShape m_Rectangle;
        sf::Texture m_Texture;
        sf::Vector2f m_Velocity;
        sf::Vector2f m_Acceleration;
        std::vector<Force> m_Forces, m_PreviousForces;

        sf::Vector2f GetBoundSize(const sf::Text& text) const;
        sf::Vector2f GetBoundPosition(const sf::Text& text) const;

    private:
        void Initialize();
    };
}