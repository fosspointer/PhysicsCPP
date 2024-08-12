#pragma once
#include <SFML/System/Vector2.hpp>
#include <Physics/UI/Layout.hpp>

namespace physics
{
    class VLayout : public Layout
    {
    public:
        VLayout(Application* application)
            :Layout(application)
        {}

        virtual void updateSize() override
        {
            if(m_children.size() == 0) return;

            ElementList::size_type max_size_x_index = 0;
            float size_y = 0.0f;
            for(ElementList::size_type i = 0; i < m_children.size(); i++)
            {
                auto& child = m_children[i];
                auto& max_x_child = m_children[max_size_x_index];
                
                if(child->getSize().x + child->getMargin().x > max_x_child->getSize().x + max_x_child->getMargin().x)
                    max_size_x_index = i;
                
                size_y += m_children[i]->getSize().y;
                if(i == m_children.size() - 1 || i == 0)
                    size_y += m_children[i]->getMargin().y / 2.0f;
                else size_y += m_children[i]->getMargin().y;
            }
            auto& max_x_child = m_children[max_size_x_index];
            m_size = sf::Vector2f{max_x_child->getSize().x + max_x_child->getMargin().x, size_y};
        }

        virtual void updatePositions() override
        {
            if(m_children.size() == 0) return;

            float child_position_y = m_position.y - m_size.y / 2.0f + m_children[0]->getSize().y / 2.0f;
            
            for(ElementList::size_type i = 0; i < m_children.size(); i++)
            {
                m_children[i]->abstractSetPosition(sf::Vector2f{m_position.x, child_position_y});
                float next_size_y = m_children.size() > i + 1 ? m_children[i + 1]->getSize().y / 2.0f : 0.0f;
                child_position_y += m_children[i]->getSize().y / 2.0f + m_children[i]->getMargin().y + next_size_y;
            }
        }
    };
}