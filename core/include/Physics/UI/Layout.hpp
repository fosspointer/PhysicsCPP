#pragma once
#include "Physics/UI/UIElementAbstract.hpp"
#include <Physics/UI/UIElement.hpp>
#include <vector>

namespace physics
{
    class Layout : public UIElement<Layout>
    {
    public:
        using ElementList = std::vector<UIElementAbstract*>;
        Layout(Application* application, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f})
            :UIElement(application, sf::Vector2f{0.0f, 0.0f}, margin)
        {}

        ~Layout()
        {
            for(ElementList::size_type i = 0; i < m_Children.size(); i++)
                if(m_Children[i])
                    delete m_Children[i];
        }

        void AddElement(UIElementAbstract* element)
        {
            m_Children.push_back(element);
        }

        void CustomUpdate(float delta_time) override final
        {
            for(auto& element : m_Children)
                element->Update(delta_time);
        }

        UIElementAbstract* GetElement(ElementList::size_type index)
        {
            return m_Children.at(index);
        }
    protected:
        ElementList m_Children;
    };
}