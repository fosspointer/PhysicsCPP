#pragma once
#include <SFML/Graphics.hpp>
#include <Physics/System/Shaders.hpp>

namespace physics
{
    class RectangleShape : public sf::Drawable
    {
    public:
        RectangleShape(const sf::Window& window, const Vector2f& position = Vector2f::Zero(), const Vector2f& size = Vector2f(100.0f))
            :m_Window(window), m_Position(position), m_Size(size)
        {}

        void SetPosition(const Vector2f& position)
        {
            m_Position = position;
            UpdateVertexPositions();
        }

        void SetSize(const Vector2f& size)
        {
            m_Size = size;
            UpdateVertexPositions();
        }

        void SetColor(const Color& color)
        {
            m_Color = color;
            UpdateVertexColors();
        }

        void SetShader(sf::Shader* shader)
        {
            m_Shader = shader;
        }

        void SetTexture(sf::Texture* texture)
        {
            m_Texture = texture;
            UpdateVertexTexcoords();
        }

        operator sf::FloatRect() const 
        {
            return sf::FloatRect(m_Position - m_Size / 2.0f, m_Size);
        }

        inline const Vector2f& GetPosition() const { return m_Position; }
        inline const Vector2f& GetSize() const { return m_Size; }
        inline const Color& GetColor() const { return m_Color; }
        inline sf::Texture* GetTexture() { return m_Texture; }
        inline sf::Shader* GetShader() { return m_Shader; }
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            if(m_Shader)
            {
                states.shader = m_Shader;
                m_Shader->setUniform("u_Resolution", sf::Glsl::Vec2((sf::Vector2f)m_Window.getSize()));
                m_Shader->setUniform("u_Size", sf::Glsl::Vec2((sf::Vector2f)m_Size));
                m_Shader->setUniform("u_Position", sf::Glsl::Vec2((sf::Vector2f)m_Position));
                m_Shader->setUniform("u_Mouse", sf::Glsl::Vec2((sf::Vector2f)Mouse::GetPosition()));
            }
            states.texture = m_Texture;
            target.draw(m_Vertices, states);
        }

        void UpdateVertexPositions()
        {
            m_Vertices[0].position = sf::Vector2f{m_Position.x - m_Size.x / 2.0f, m_Position.y - m_Size.y / 2.0f};
            m_Vertices[1].position = sf::Vector2f{m_Position.x + m_Size.x / 2.0f, m_Position.y - m_Size.y / 2.0f};
            m_Vertices[2].position = sf::Vector2f{m_Position.x + m_Size.x / 2.0f, m_Position.y + m_Size.y / 2.0f};
            m_Vertices[3].position = sf::Vector2f{m_Position.x - m_Size.x / 2.0f, m_Position.y + m_Size.y / 2.0f};
        }

        void UpdateVertexTexcoords()
        {
            auto texture_size = m_Texture? (sf::Vector2f)m_Texture->getSize() : sf::Vector2f{0.0f, 0.0f};

            m_Vertices[0].texCoords = sf::Vector2f{          0.0f, 0.0f};
            m_Vertices[1].texCoords = sf::Vector2f{texture_size.x, 0.0f};
            m_Vertices[2].texCoords = sf::Vector2f{texture_size.x, texture_size.y};
            m_Vertices[3].texCoords = sf::Vector2f{          0.0f, texture_size.y};
        }

        void UpdateVertexColors()
        {
            for(unsigned char i = 0; i < 4; i++)
                m_Vertices[i].color = m_Color;
        }

        Vector2f m_Position, m_Size;
        Color m_Color;
        sf::VertexArray m_Vertices{sf::Quads, 4};
        sf::Shader* m_Shader{nullptr};
        sf::Texture* m_Texture{nullptr};
        const sf::Window& m_Window;
    };
}