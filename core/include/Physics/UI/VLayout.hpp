#pragma once
#include <SFML/System/Vector2.hpp>
#include <Physics/UI/Layout.hpp>

namespace physics
{
    class VLayout : public Layout
    {
    public:
        VLayout(Application* application, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f})
            :Layout(application, margin)
        {}

        virtual void UpdateSize() override
        {
            if(m_Children.size() == 0) return;

            ElementList::size_type max_size_x_index = 0;
            float size_y = 0.0f;
            for(ElementList::size_type i = 0; i < m_Children.size(); i++)
            {
                auto& child = m_Children[i];
                auto& max_x_child = m_Children[max_size_x_index];
                
                if(child->GetSize().x + child->GetMargin().x > max_x_child->GetSize().x + max_x_child->GetMargin().x)
                    max_size_x_index = i;
                
                size_y += m_Children[i]->GetSize().y;
                if(i == m_Children.size() - 1 || i == 0)
                    size_y += m_Children[i]->GetMargin().y / 2.0f;
                else size_y += m_Children[i]->GetMargin().y;
            }
            auto& max_x_child = m_Children[max_size_x_index];
            m_Size = sf::Vector2f{max_x_child->GetSize().x + max_x_child->GetMargin().x, size_y};
        }

        virtual void UpdatePositions() override
        {
            if(m_Children.size() == 0) return;

            float child_position_y = m_Position.y - m_Size.y / 2.0f + m_Children[0]->GetSize().y / 2.0f;
            
            for(ElementList::size_type i = 0; i < m_Children.size(); i++)
            {
                m_Children[i]->AbstractSetPosition(sf::Vector2f{m_Position.x, child_position_y});
                float next_size_y = m_Children.size() > i + 1 ? m_Children[i + 1]->GetSize().y / 2.0f : 0.0f;
                child_position_y += m_Children[i]->GetSize().y / 2.0f + m_Children[i]->GetMargin().y + next_size_y;
            }
        }
    };
}