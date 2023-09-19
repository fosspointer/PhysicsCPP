#pragma once
#include <cmath>
#include <string>
#include <SFML/Graphics.hpp>
#include <Physics/System/Vector2.hpp>
#include <Physics/System/Color.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/System/Arrow.hpp>

namespace physics
{
    
    /// @brief Vector-string pair representing a named force
    struct Force
    {
        Vector2f Value;
        sf::String Name;
    };

    /// @brief Abstract class representing a body
    class Body
    {
    public:
        Body(Application* application, const Color& color);
        Body(Application* application, const sf::String& filepath);

        void SetTexture(const sf::String& filepath);

        void SetPosition(const Vector2f& position);
        void SetVelocity(const Vector2f& velocity);
        void SetAcceleration(const Vector2f& acceleration);
        
        void SetSize(const Vector2f& size, bool set_collider = true);
        void SetCollider(const Vector2f& offset, const Vector2f& size);
        bool CollidesWith(const Body* other) const;
        
        void AddForce(const Vector2f& force, sf::String name);
        void AddForce(const Vector2f& force);

        inline const sf::Vector2f& GetPosition() const { return m_Rectangle.getPosition(); }
        inline const sf::Vector2f& GetSize() const { return m_Rectangle.getSize(); }
        inline const Vector2f& GetVelocity() const { return m_Velocity; }
        inline const Vector2f& GetAcceleration() const { return m_Acceleration; }

        inline Vector2f& GetVelocity() { return m_Velocity; }
        inline Vector2f& GetAcceleration() { return m_Acceleration; }

        inline AABB GetOutterAABB() const { return m_Rectangle; }
        inline AABB GetInnerAABB() const { return AABB(m_Rectangle.getPosition() + m_Collider.Position, m_Collider.Size); }

        virtual const Vector2f GetTotalForce();

        virtual void DrawForces(float thickness = 5.0f) const;
        void Draw();

        virtual void Update(float delta_time) = 0;
    protected:
        void DrawForce(const Force& force, const Color& color = Color::White) const;
        AABB m_Collider;
        Application* m_Application;
        sf::RectangleShape m_Rectangle;
        sf::Texture m_Texture;
        Vector2f m_Velocity{0.0f};
        Vector2f m_Acceleration{0.0f};
        std::vector<Force> m_Forces, m_PreviousForces;

        Vector2f GetBoundSize(const sf::Text& text) const;
        Vector2f GetBoundPosition(const sf::Text& text) const;
        float m_DeltaTime = 0.0f;
    private:
        void Initialize();
    };
}