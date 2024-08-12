#pragma once
#include <cmath>
#include <string>
#include <SFML/Graphics.hpp>
#include <Physics/System/Vector2.hpp>
#include <Physics/System/Color.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/System/Arrow.hpp>
#include <Physics/Physics/Materials.hpp>

namespace physics
{
    
    /// @brief Vector-string pair representing a named force
    struct Force
    {
        Vector2f value;
        sf::String name;
        std::size_t subscript;
    };

    /// @brief Abstract class representing a body
    class Body
    {
    public:
        Body(Application* application, const Color& color);
        Body(Application* application, const sf::String& filepath);

        void setTexture(const sf::String& filepath);

        void setPosition(const Vector2f& position);
        void setVelocity(const Vector2f& velocity);
        void setAcceleration(const Vector2f& acceleration);
        void setMaterial(Material* material);
        
        void setSize(const Vector2f& size, bool set_collider = true);
        void setCollider(const Vector2f& offset, const Vector2f& size);
        bool collidesWith(const Body* other) const;
        
        void addForce(const Vector2f& force, sf::String name);
        void addForce(const Vector2f& force);

        inline const sf::Vector2f& getPosition() const { return m_rectangle.getPosition(); }
        inline const sf::Vector2f& getSize() const { return m_rectangle.getSize(); }
        inline const Vector2f& getVelocity() const { return m_velocity; }
        inline const Vector2f& getAcceleration() const { return m_acceleration; }
        inline Material* getMaterial() const { return m_material; }

        inline Vector2f& getVelocity() { return m_velocity; }
        inline Vector2f& getAcceleration() { return m_acceleration; }

        inline AABB getOutterAABB() const { return m_rectangle; }
        inline AABB getInnerAABB() const { return AABB(m_rectangle.getPosition() + m_collider.position, m_collider.size); }

        virtual const Vector2f getTotalForce();

        virtual void drawForces(bool ignore_zero = false, float thickness = 5.0f) const;
        void draw();

        virtual void update(float delta_time) = 0;
    protected:
        void clearForces()
        {
            m_forceNames = {};
            m_forces.clear();
        }
        void drawForce(const Force& force, bool ignore_zero, const Color& color = Color::White) const;
        AABB m_collider;
        Application* m_application;
        sf::RectangleShape m_rectangle;
        sf::Texture m_texture;
        Vector2f m_velocity{0.0f};
        Vector2f m_acceleration{0.0f};
        std::vector<Force> m_forces, m_previousForces;
        Material* m_material;

        Vector2f getBoundSize(const sf::Text& text) const;
        Vector2f getBoundPosition(const sf::Text& text) const;
        float m_deltaTime = 0.0f;
        std::unordered_map<std::string, std::size_t> m_forceNames;
    private:
        void initialize();
    };
}