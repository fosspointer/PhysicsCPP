#pragma once
#include <Physics/UI/Layout.hpp>

namespace physics
{
    class VGrid : public Layout
    {
    public:
        VGrid(Application* application, const size_t column_count, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f})
            :Layout(application, margin), m_ColumnCount(column_count)
        {}

        virtual void UpdateSize() override
        {
            if(m_Children.size() == 0) return;

            ElementList::size_type max_size_y_index = 0;
            float size_x = 0.0f;
            for(ElementList::size_type i = 0; i < m_Children.size(); i++)
            {
                auto& child = m_Children[i];
                auto& max_y_child = m_Children[max_size_y_index];

                if(child->GetSize().y + child->GetMargin().y > max_y_child->GetSize().y + max_y_child->GetMargin().y)
                    max_size_y_index = i;
                
                size_x += m_Children[i]->GetSize().x;
                if(i == m_Children.size() - 1 || i == 0)
                    size_x += m_Children[i]->GetMargin().x / 2.0f;
                else size_x += m_Children[i]->GetMargin().x;
            }
            auto& max_y_child = m_Children[max_size_y_index];
            m_Size = sf::Vector2f{size_x, max_y_child->GetSize().y + max_y_child->GetMargin().y};
        }

        virtual void UpdatePositions() override
        {
            if(m_Children.size() == 0) return;

            float child_position_x = m_Position.x - m_Size.x / 2.0f + m_Children[0]->GetSize().x / 2.0f;
            
            for(ElementList::size_type i = 0; i < m_Children.size(); i++)
            {
                m_Children[i]->AbstractSetPosition(sf::Vector2f{child_position_x, m_Position.y});
                float next_size_x = m_Children.size() > i + 1 ? m_Children[i + 1]->GetSize().x / 2.0f + m_Children[i + 1]->GetMargin().x / 2.0f: 0.0f;
                child_position_x += m_Children[i]->GetSize().x / 2.0f + m_Children[i]->GetMargin().x / 2.0f + next_size_x;
            }
        }
    private:
        const size_t m_ColumnCount;
    };
}