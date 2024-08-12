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
            sf::Vector2f start, end;
        };

        struct Props
        {
            Border border;
            sf::Texture* texture;
        };

        Box(sf::Texture* texture = nullptr)
            :m_texture(texture)
        {}

        Box(sf::Texture* texture, const Border& border)
            :m_texture(texture), m_border(border)
        {}

        Box(const Props& props)
            :m_texture(props.texture), m_border(props.border)
        {}

        void setTexture(sf::Texture* texture, const Border& border)
        {
            m_texture = texture;
            setBorder(border);
        }

        inline void setTexture(sf::Texture* texture)
        {
            m_texture = texture;
        }

        void setBorder(const Border& border)
        {
            m_border = border;
            updateVertices();
        }

        void setPosition(const sf::Vector2f& position)
        {
            m_position = position;
            updateVertices();
        }

        void setSize(const sf::Vector2f& size)
        {
            m_size = size;
            updateVertices();
        }

        void setColor(const Color& color)
        {
            m_color = color;
            updateVertices();
        }
        
        operator sf::FloatRect() const
        {
            return sf::FloatRect(m_position - m_size / 2.0f, m_size);
        }

        operator AABB() const
        {
            return AABB((sf::FloatRect)(*this));
        }

        const sf::Vector2f& getPosition() const { return m_position; }
        const sf::Vector2f& getSize() const { return m_size; }
        const Border& getBorder() const { return m_border; }
        const sf::Vector2f& getBorderStart() const { return m_border.start; }
        const sf::Vector2f& getBorderEnd() const { return m_border.end; }
        const Color& getColor() const { return m_color; }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            states.texture = m_texture;
            target.draw(m_vertices, states);
        }

        void updateVertices()
        {   
            size_t index = 0;

            auto texture_size = m_texture? (Vector2f)m_texture->getSize(): Vector2f{0.0f};
            
            auto texture_minus_border = texture_size - m_border.end;
            auto size_minus_border = m_size - m_border.end;

            updateQuad(&index, Vector2f::zero(), m_border.start, Vector2f::zero(), m_border.start);
            updateQuad(&index, Vector2f::X(m_border.start.x), Vector2f{size_minus_border.x, m_border.start.y}, Vector2f::X(m_border.start.x), Vector2f{texture_minus_border.x, m_border.start.y});
            updateQuad(&index, Vector2f::X(size_minus_border.x), Vector2f{m_size.x, m_border.start.y}, Vector2f::X(texture_minus_border.x), Vector2f{texture_size.x, m_border.start.y});
            
            updateQuad(&index, Vector2f::Y(m_border.start.y), sf::Vector2f{m_border.start.x, size_minus_border.y}, Vector2f::Y(m_border.start.y), sf::Vector2f{m_border.start.x, texture_minus_border.y});
            updateQuad(&index, m_border.start, size_minus_border, m_border.start, texture_minus_border);
            updateQuad(&index, Vector2f{size_minus_border.x, m_border.start.y}, Vector2f{m_size.x, size_minus_border.y}, Vector2f{texture_minus_border.x, m_border.start.y}, Vector2f{texture_size.x, texture_minus_border.y});

            updateQuad(&index, Vector2f::Y(size_minus_border.y), sf::Vector2f{m_border.start.x, m_size.y}, Vector2f::Y(texture_minus_border.y), sf::Vector2f{m_border.start.x, texture_size.y});
            updateQuad(&index, Vector2f{m_border.start.x, size_minus_border.y}, Vector2f{size_minus_border.x, m_size.y}, Vector2f{m_border.start.x, texture_minus_border.y}, Vector2f{texture_minus_border.x, texture_size.y});
            updateQuad(&index, size_minus_border, m_size, texture_minus_border, texture_size);
        }

        void updateQuad(size_t* index, const sf::Vector2f& start_position, const sf::Vector2f& end_position, const sf::Vector2f& start_tex_coords, const sf::Vector2f& end_tex_coords)
        {
            auto top_left = m_position - m_size / 2.0f;
            auto actual_start_position = top_left + start_position;
            auto actual_end_position = top_left + end_position;

            m_vertices[*index    ] = sf::Vertex{sf::Vector2f{actual_start_position.x, actual_start_position.y}, m_color, sf::Vector2f{start_tex_coords.x, start_tex_coords.y}};
            m_vertices[*index + 1] = sf::Vertex{sf::Vector2f{actual_end_position.x, actual_start_position.y}, m_color, sf::Vector2f{end_tex_coords.x, start_tex_coords.y}};
            m_vertices[*index + 2] = sf::Vertex{sf::Vector2f{actual_end_position.x, actual_end_position.y}, m_color, sf::Vector2f{end_tex_coords.x, end_tex_coords.y}};
            m_vertices[*index + 3] = sf::Vertex{sf::Vector2f{actual_start_position.x, actual_end_position.y}, m_color, sf::Vector2f{start_tex_coords.x, end_tex_coords.y}};

            (*index)+=4;
        }

        sf::Vector2f m_position, m_size;
        Border m_border;
        Color m_color;
        sf::Texture* m_texture;
        sf::VertexArray m_vertices{sf::Quads, BOX_VERTICES};
    };
}