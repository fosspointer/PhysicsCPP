#pragma once
#include <SFML/Graphics.hpp>
#include <Physics/System/Shaders.hpp>

namespace physics
{
    class RectangleShape : public sf::Drawable
    {
    public:
        RectangleShape(const sf::Window& window, const Vector2f& position = Vector2f::zero(), const Vector2f& size = Vector2f(100.0f))
            :m_window(window), m_position(position), m_size(size)
        {}

        void setPosition(const Vector2f& position)
        {
            m_position = position;
            updateVertexPositions();
        }

        void setSize(const Vector2f& size)
        {
            m_size = size;
            updateVertexPositions();
        }

        void setColor(const Color& color)
        {
            m_color = color;
            updateVertexColors();
        }

        void setShader(sf::Shader* shader)
        {
            m_shader = shader;
        }

        void setTexture(sf::Texture* texture)
        {
            m_texture = texture;
            updateVertexTexcoords();
        }

        operator sf::FloatRect() const 
        {
            return sf::FloatRect(m_position - m_size / 2.0f, m_size);
        }

        inline const Vector2f& getPosition() const { return m_position; }
        inline const Vector2f& getSize() const { return m_size; }
        inline const Color& getColor() const { return m_color; }
        inline sf::Texture* getTexture() { return m_texture; }
        inline sf::Shader* getShader() { return m_shader; }
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            if(m_shader)
            {
                states.shader = m_shader;
                m_shader->setUniform("u_resolution", sf::Glsl::Vec2((sf::Vector2f)m_window.getSize()));
                m_shader->setUniform("u_size", sf::Glsl::Vec2((sf::Vector2f)m_size));
                m_shader->setUniform("u_position", sf::Glsl::Vec2((sf::Vector2f)m_position));
                m_shader->setUniform("u_mouse", sf::Glsl::Vec2((sf::Vector2f)Mouse::getPosition()));
            }
            states.texture = m_texture;
            target.draw(m_vertices, states);
        }

        void updateVertexPositions()
        {
            m_vertices[0].position = sf::Vector2f{m_position.x - m_size.x / 2.0f, m_position.y - m_size.y / 2.0f};
            m_vertices[1].position = sf::Vector2f{m_position.x + m_size.x / 2.0f, m_position.y - m_size.y / 2.0f};
            m_vertices[2].position = sf::Vector2f{m_position.x + m_size.x / 2.0f, m_position.y + m_size.y / 2.0f};
            m_vertices[3].position = sf::Vector2f{m_position.x - m_size.x / 2.0f, m_position.y + m_size.y / 2.0f};
        }

        void updateVertexTexcoords()
        {
            auto texture_size = m_texture? (sf::Vector2f)m_texture->getSize() : sf::Vector2f{0.0f, 0.0f};

            m_vertices[0].texCoords = sf::Vector2f{          0.0f, 0.0f};
            m_vertices[1].texCoords = sf::Vector2f{texture_size.x, 0.0f};
            m_vertices[2].texCoords = sf::Vector2f{texture_size.x, texture_size.y};
            m_vertices[3].texCoords = sf::Vector2f{          0.0f, texture_size.y};
        }

        void updateVertexColors()
        {
            for(unsigned char i = 0; i < 4; i++)
                m_vertices[i].color = m_color;
        }

        Vector2f m_position, m_size;
        Color m_color;
        sf::VertexArray m_vertices{sf::Quads, 4};
        sf::Shader* m_shader{nullptr};
        sf::Texture* m_texture{nullptr};
        const sf::Window& m_window;
    };
}