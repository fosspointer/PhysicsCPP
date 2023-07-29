#pragma once
#include <SFML/Graphics.hpp>
#include <Physics/System/Textures.hpp>

#define QUAD_VERTICES 4
#define BOX_VERTICES QUAD_VERTICES * 9

namespace physics
{
    class Box : public sf::Drawable
    {
    public:
        Box(sf::Texture* texture = nullptr)
            :m_Texture(texture), m_Vertices(sf::Quads, BOX_VERTICES)
        {}

        void SetTexture(sf::Texture* texture, const sf::Vector2f& border_size)
        {
            m_Texture = texture;
            SetBorder(border_size);
        }

        void SetBorder(const sf::Vector2f& border_size)
        {
            m_Border = border_size;
            UpdateVertices();
        }

        void SetPosition(const sf::Vector2f& position)
        {
            m_Position = position;
            UpdateVertices();
        }

        void SetSize(const sf::Vector2f& size)
        {
            m_Size = size;
            UpdateVertices();
        }

        void SetColor(const Color& color)
        {
            m_Color = color;
            UpdateVertices();
        }
        
        operator sf::FloatRect() const
        {
            return sf::FloatRect(m_Position - m_Size / 2.0f, m_Size);
        }

        operator AABB() const
        {
            return AABB((sf::FloatRect)(*this));
        }

        const sf::Vector2f& GetPosition() const { return m_Position; }
        const sf::Vector2f& GetSize() const { return m_Size; }
        const sf::Vector2f& GetBorder() const { return m_Border; }
        const Color& GetColor() const { return m_Color; }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            states.texture = m_Texture;
            target.draw(m_Vertices, states);
        }

        void UpdateVertices()
        {   
            size_t index = 0;

            auto t_size = m_Texture ? (Vector2f)m_Texture->getSize() : Vector2f{0.0f};
            auto t_border = t_size - m_Border;
            auto s_border = m_Size - m_Border;

            UpdateQuad(&index, Vector2f::Zero(), m_Border, Vector2f::Zero(), m_Border);
            UpdateQuad(&index, Vector2f{m_Border.x, 0.0f}, Vector2f{s_border.x, m_Border.y}, Vector2f{m_Border.x, 0.0f}, Vector2f{t_border.x, m_Border.y});
            UpdateQuad(&index, Vector2f{s_border.x, 0.0f}, Vector2f{m_Size.x, m_Border.y}, Vector2f{t_border.x, 0.0f}, Vector2f{t_size.x, m_Border.y});
            
            UpdateQuad(&index, Vector2f{0.0f, m_Border.y}, sf::Vector2f{m_Border.x, s_border.y}, Vector2f{0.0f, m_Border.y}, sf::Vector2f{m_Border.x, t_border.y});
            UpdateQuad(&index, m_Border, s_border, m_Border, t_border);
            UpdateQuad(&index, Vector2f{s_border.x, m_Border.y}, Vector2f{m_Size.x, s_border.y}, Vector2f{t_border.x, m_Border.y}, Vector2f{t_size.x, t_border.y});

            UpdateQuad(&index, Vector2f{0.0f, s_border.y}, sf::Vector2f{m_Border.x, m_Size.y}, Vector2f{0.0f, t_border.y}, sf::Vector2f{m_Border.x, t_size.y});
            UpdateQuad(&index, Vector2f{m_Border.x, s_border.y}, Vector2f{s_border.x, m_Size.y}, Vector2f{m_Border.x, t_border.y}, Vector2f{t_border.x, t_size.y});
            UpdateQuad(&index, s_border, m_Size, t_border, t_size);
        }

        void UpdateQuad(size_t* index, const sf::Vector2f& start_position, const sf::Vector2f& end_position, const sf::Vector2f& start_tex_coords, const sf::Vector2f& end_tex_coords)
        {
            auto top_left = m_Position - m_Size / 2.0f;
            auto actual_start_position = top_left + start_position;
            auto actual_end_position = top_left + end_position;

            m_Vertices[*index    ] = sf::Vertex{sf::Vector2f{actual_start_position.x, actual_start_position.y}, m_Color, sf::Vector2f{start_tex_coords.x, start_tex_coords.y}};
            m_Vertices[*index + 1] = sf::Vertex{sf::Vector2f{actual_end_position.x, actual_start_position.y}, m_Color, sf::Vector2f{end_tex_coords.x, start_tex_coords.y}};
            m_Vertices[*index + 2] = sf::Vertex{sf::Vector2f{actual_end_position.x, actual_end_position.y}, m_Color, sf::Vector2f{end_tex_coords.x, end_tex_coords.y}};
            m_Vertices[*index + 3] = sf::Vertex{sf::Vector2f{actual_start_position.x, actual_end_position.y}, m_Color, sf::Vector2f{start_tex_coords.x, end_tex_coords.y}};

            (*index)+=4;
        }

        sf::Vector2f m_Position, m_Size, m_Border;
        Color m_Color;
        sf::Texture* m_Texture;
        sf::VertexArray m_Vertices;
    };
}