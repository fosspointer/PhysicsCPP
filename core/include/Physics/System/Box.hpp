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
        struct Border
        {
            sf::Vector2f Start, End;
        };

        struct Props
        {
            Border border;
            sf::Texture* texture;
        };

        Box(sf::Texture* texture = nullptr)
            :m_Texture(texture)
        {}

        Box(sf::Texture* texture, const Border& border)
            :m_Texture(texture), m_Border(border)
        {}

        Box(const Props& props)
            :m_Texture(props.texture), m_Border(props.border)
        {}

        void SetTexture(sf::Texture* texture, const Border& border)
        {
            m_Texture = texture;
            SetBorder(border);
        }

        inline void SetTexture(sf::Texture* texture)
        {
            m_Texture = texture;
        }

        void SetBorder(const Border& border)
        {
            m_Border = border;
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
        const Border& GetBorder() const { return m_Border; }
        const sf::Vector2f& GetBorderStart() const { return m_Border.Start; }
        const sf::Vector2f& GetBorderEnd() const { return m_Border.End; }
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

            auto texture_size = m_Texture ? (Vector2f)m_Texture->getSize() : Vector2f{0.0f};
            
            auto texture_minus_border = texture_size - m_Border.End;
            auto size_minus_border = m_Size - m_Border.End;

            UpdateQuad(&index, Vector2f::Zero(), m_Border.Start, Vector2f::Zero(), m_Border.Start);
            UpdateQuad(&index, Vector2f::X(m_Border.Start.x), Vector2f{size_minus_border.x, m_Border.Start.y}, Vector2f::X(m_Border.Start.x), Vector2f{texture_minus_border.x, m_Border.Start.y});
            UpdateQuad(&index, Vector2f::X(size_minus_border.x), Vector2f{m_Size.x, m_Border.Start.y}, Vector2f::X(texture_minus_border.x), Vector2f{texture_size.x, m_Border.Start.y});
            
            UpdateQuad(&index, Vector2f::Y(m_Border.Start.y), sf::Vector2f{m_Border.Start.x, size_minus_border.y}, Vector2f::Y(m_Border.Start.y), sf::Vector2f{m_Border.Start.x, texture_minus_border.y});
            UpdateQuad(&index, m_Border.Start, size_minus_border, m_Border.Start, texture_minus_border);
            UpdateQuad(&index, Vector2f{size_minus_border.x, m_Border.Start.y}, Vector2f{m_Size.x, size_minus_border.y}, Vector2f{texture_minus_border.x, m_Border.Start.y}, Vector2f{texture_size.x, texture_minus_border.y});

            UpdateQuad(&index, Vector2f::Y(size_minus_border.y), sf::Vector2f{m_Border.Start.x, m_Size.y}, Vector2f::Y(texture_minus_border.y), sf::Vector2f{m_Border.Start.x, texture_size.y});
            UpdateQuad(&index, Vector2f{m_Border.Start.x, size_minus_border.y}, Vector2f{size_minus_border.x, m_Size.y}, Vector2f{m_Border.Start.x, texture_minus_border.y}, Vector2f{texture_minus_border.x, texture_size.y});
            UpdateQuad(&index, size_minus_border, m_Size, texture_minus_border, texture_size);
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

        sf::Vector2f m_Position, m_Size;
        Border m_Border;
        Color m_Color;
        sf::Texture* m_Texture;
        sf::VertexArray m_Vertices{sf::Quads, BOX_VERTICES};
    };
}