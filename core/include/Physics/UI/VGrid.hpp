#pragma once
#include <Physics/UI/Layout.hpp>

namespace physics
{
    class VGrid : public Layout
    {
    public:
        VGrid(Application* application, const size_t column_count)
            :Layout(application), m_columnCount(column_count)
        {}

        virtual void updateSize() override
        {
            if(m_children.size() == 0) return;

            ElementList::size_type max_size_y_index = 0;
            float size_x = 0.0f;
            for(ElementList::size_type i = 0; i < m_children.size(); i++)
            {
                auto& child = m_children[i];
                auto& max_y_child = m_children[max_size_y_index];

                if(child->getSize().y + child->getMargin().y > max_y_child->getSize().y + max_y_child->getMargin().y)
                    max_size_y_index = i;
                
                size_x += m_children[i]->getSize().x;
                if(i == m_children.size() - 1 || i == 0)
                    size_x += m_children[i]->getMargin().x / 2.0f;
                else size_x += m_children[i]->getMargin().x;
            }
            auto& max_y_child = m_children[max_size_y_index];
            m_size = sf::Vector2f{size_x, max_y_child->getSize().y + max_y_child->getMargin().y};
        }

        virtual void updatePositions() override
        {
            if(m_children.size() == 0) return;

            float child_position_x = m_position.x - m_size.x / 2.0f + m_children[0]->getSize().x / 2.0f;
            
            for(ElementList::size_type i = 0; i < m_children.size(); i++)
            {
                m_children[i]->abstractsetPositionsf::Vector2f{child_position_x, m_position.y});
                float next_size_x = m_children.size() > i + 1 ? m_children[i + 1]->getSize().x / 2.0f + m_children[i + 1]->getMargin().x / 2.0f: 0.0f;
                child_position_x += m_children[i]->getSize().x / 2.0f + m_children[i]->getMargin().x / 2.0f + next_size_x;
            }
        }
    private:
        const size_t m_columnCount;
    };
}