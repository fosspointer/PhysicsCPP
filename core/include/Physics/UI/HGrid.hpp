#pragma once
#include <Physics/UI/Layout.hpp>

namespace physics
{
    class HGrid : public Layout
    {
    public:
        HGrid(Application* application, const size_t row_count, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f})
            :Layout(application, margin), m_RowCount(row_count)
        {
            if(row_count == 0)
                throw PHYSICS_EXCEPTION("A grid cannot be composed of 0 rows!");
        }

        virtual void UpdateSize() override
        {
            if(m_Children.size() == 0) return;
            m_Size = sf::Vector2f{0.0f, 0.0f};

            for(ElementListSize i = 0; i < m_Children.size(); i += m_RowCount)
            {
                float size_y = 0.0f;
                for(ElementListSize j = 0; j < m_RowCount; j++)
                {
                    auto current_index = i + j;
                    if (current_index > m_Children.size() - 1) break;
                    auto* child = m_Children[current_index];
                    
                    size_y += child->GetSize().y;
                    
                    if(j == 0 || j == m_RowCount - 1)
                        size_y += child->GetMargin().y / 2.0f;
                    else size_y += child->GetMargin().y;
                }

                if(size_y > m_Size.y) m_Size.y = size_y;
            }
            for(ElementListSize j = 0; j < m_RowCount; j++)
            {
                float size_x = 0.0f;
                for(ElementListSize i = j; i < m_Children.size(); i += m_RowCount)
                {
                    auto current_index = i + j;
                    if (current_index > m_Children.size() - 1) break;
                    auto* child = m_Children[current_index];

                    size_x += child->GetSize().x;
                    if(std::ceil((float)m_Children.size() / (float)m_RowCount) == 1);
                    else if(i < m_RowCount || i > m_Children.size() - m_RowCount - 1)
                        size_x += child->GetMargin().x / 2.0f;
                    else size_x += child->GetMargin().x;
                }

                if(size_x > m_Size.x) m_Size.x = size_x;
            }
        }

        virtual void UpdatePositions() override
        {
            if(m_Children.size() == 0) return;

            float child_position_x = m_Position.x - m_Size.x / 2.0f + m_Children[0]->GetSize().x / 2.0f;
            float initial_child_position_y = m_Position.y - m_Size.y / 2.0f + m_Children[0]->GetSize().y / 2.0f;
            float child_position_y;

            for(ElementListSize i = 0; i < m_Children.size(); i += m_RowCount)
            {
                child_position_y = initial_child_position_y;
                for(ElementListSize j = 0; j < m_RowCount; j++)
                {
                    auto current_index = i + j;
                    if (current_index > m_Children.size() - 1) break;
                    auto* child = m_Children[current_index];
                    
                    m_Children[current_index]->AbstractSetPosition(sf::Vector2f{child_position_x, child_position_y});
                    float next_size_y;
                    next_size_y = j == m_RowCount - 1 || current_index + 1 > m_Children.size() - 1 ? 0.0f : m_Children[current_index + 1]->GetSize().y + m_Children[current_index + 1]->GetMargin().y;
                    child_position_y += child->GetSize().y / 2.0f + child->GetMargin().y / 2.0f + next_size_y / 2.0f;
                }
                float next_size_x = m_Children.size() > i + m_RowCount ? m_Children[i + m_RowCount]->GetSize().x + m_Children[i + m_RowCount]->GetMargin().x: 0.0f;
                child_position_x += m_Children[i]->GetSize().x / 2.0f + m_Children[i]->GetMargin().x / 2.0f + next_size_x / 2.0f;
            }
        }
    private:
        const size_t m_RowCount;
    };
}