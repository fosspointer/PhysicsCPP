#pragma once
#include <Physics/UI/Layout.hpp>

namespace physics
{
    class HGrid : public Layout
    {
    public:
        HGrid(Application* application, const size_t row_count)
            :Layout(application), m_rowCount(row_count)
        {
            if(row_count == 0)
                throw PHYSICS_EXCEPTION("A grid cannot be composed of 0 rows!");
        }

        virtual void updateSize() override
        {
            if(m_children.size() == 0) return;
            m_size = sf::Vector2f{0.0f, 0.0f};

            for(ElementListSize i = 0; i < m_children.size(); i += m_rowCount)
            {
                float size_y = 0.0f;
                for(ElementListSize j = 0; j < m_rowCount; j++)
                {
                    auto current_index = i + j;
                    if (current_index > m_children.size() - 1) break;
                    auto* child = m_children[current_index];
                    
                    size_y += child->getSize().y;
                    
                    if(j == 0 || j == m_rowCount - 1)
                        size_y += child->getMargin().y / 2.0f;
                    else size_y += child->getMargin().y;
                }

                if(size_y > m_size.y) m_size.y = size_y;
            }
            for(ElementListSize j = 0; j < m_rowCount; j++)
            {
                float size_x = 0.0f;
                for(ElementListSize i = j; i < m_children.size(); i += m_rowCount)
                {
                    auto current_index = i + j;
                    if (current_index > m_children.size() - 1) break;
                    auto* child = m_children[current_index];

                    size_x += child->getSize().x;
                    if(std::ceil((float)m_children.size() / (float)m_rowCount) == 1);
                    else if(i < m_rowCount || i > m_children.size() - m_rowCount - 1)
                        size_x += child->getMargin().x / 2.0f;
                    else size_x += child->getMargin().x;
                }

                if(size_x > m_size.x) m_size.x = size_x;
            }
        }

        virtual void updatePositions() override
        {
            if(m_children.size() == 0) return;

            float child_position_x = m_position.x - m_size.x / 2.0f + m_children[0]->getSize().x / 2.0f;
            float initial_child_position_y = m_position.y - m_size.y / 2.0f + m_children[0]->getSize().y / 2.0f;
            float child_position_y;

            for(ElementListSize i = 0; i < m_children.size(); i += m_rowCount)
            {
                child_position_y = initial_child_position_y;
                for(ElementListSize j = 0; j < m_rowCount; j++)
                {
                    auto current_index = i + j;
                    if (current_index > m_children.size() - 1) break;
                    auto* child = m_children[current_index];
                    
                    m_children[current_index]->abstractSetPosition(sf::Vector2f{child_position_x, child_position_y});
                    float next_size_y;
                    next_size_y = j == m_rowCount - 1 || current_index + 1 > m_children.size() - 1 ? 0.0f : m_children[current_index + 1]->getSize().y + m_children[current_index + 1]->getMargin().y;
                    child_position_y += child->getSize().y / 2.0f + child->getMargin().y / 2.0f + next_size_y / 2.0f;
                }
                float next_size_x = m_children.size() > i + m_rowCount ? m_children[i + m_rowCount]->getSize().x + m_children[i + m_rowCount]->getMargin().x: 0.0f;
                child_position_x += m_children[i]->getSize().x / 2.0f + m_children[i]->getMargin().x / 2.0f + next_size_x / 2.0f;
            }
        }
    private:
        const size_t m_rowCount;
    };
}