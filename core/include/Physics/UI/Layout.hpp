#pragma once
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Physics/System/AABB.hpp>
#include <Physics/System/Box.hpp>
#include <Physics/UI/UIElement.hpp>

namespace physics
{
    using ElementList = std::vector<UIElementAbstract*>;
    using ElementListSize = std::vector<UIElementAbstract*>::size_type;

    // @brief UIElement hover callback function pointer type
    // @tparam UIElementImpl 
    using ElementHoverCallbackFunc = std::function<void(Application*, class Layout*, ElementListSize, bool)>;
    
    // @brief UIElement click callback function pointer type
    // @tparam UIElementImpl  
    using ElementClickCallbackFunc = std::function<void(Application*, class Layout*, ElementListSize, MouseButton)>;

    class Layout : public UIElement<Layout>
    {
    public:
        Layout(Application* application)
            :UIElement(application), m_backgroundVisible(false)
        {}

        virtual ~Layout()
        {
            for(ElementList::size_type i = 0; i < m_children.size(); i++)
                if(m_children[i])
                    delete m_children[i];
        }

        virtual void draw(int8_t layer = PHYSICS_LAYER_UI_0) override
        {
            if(m_backgroundVisible)
                m_application->draw(&m_background, layer);
            
            for(ElementList::size_type i = 0; i < m_children.size(); i++)
                m_children[i]->draw(PHYSICS_LAYER_UI_1);
        }

        template <class UIElementImpl>
        UIElementImpl* pushElement(UIElementImpl* element)
        {
            m_children.push_back(element);
            updateSize();
            updatePositions();
            return element;
        }


        ElementList& getElements()
        {
            return m_children;
        }

        const ElementList& getElements() const
        {
            return m_children;
        }

        UIElementAbstract* getElement(ElementList::size_type index) const
        {
            return m_children[index];
        }

        ElementList::size_type getElementCount() const
        {
            return m_children.size();
        }

        void customUpdate(float delta_time) override final
        {
            bool pragma_updated = false;

            for(auto& element : m_children)
                if(element->getPragmaUpdated())
                    pragma_updated = true;
                
            pragma_updated |= getPragmaUpdated();

            // if(m_application->getResized() || pragma_updated)
            // {
                updateSize();
                updatePositions();
            // }
        #ifdef PHYSICS_DEBUG
            if(displayBounds)
            {
                for(auto& element: m_children)
                    element->displayBounds = true;
            }
        #endif
            m_background.setPosition(m_position);
            m_background.setSize(m_size + Vector2f{m_backgroundGrowth});
            for(auto& element : m_children)
                element->update(delta_time);
            
            if(!isHovered() && !stoppedHover()) return; //The below code is for handling click and hover events.
            //Hence, if isHovered() returns false, execution is stopped
            
            auto& mouse_state = Mouse::getInstance().clickState;

            for(ElementListSize i = 0; i < m_children.size(); i++)
            {
                auto& child = m_children[i];

                if(!child->m_currentHovered && !child->m_previousHovered) continue;
                
                for(const auto& func : m_elementHoverCallbackFunctions)
                    func(m_application, this, i, child->stoppedHover());

                //Bitwise arithmetic and function callback iterators
                //Note: State & MOUSE_<BUTTON> means that <BUTTON> is 'enabled' for the state

                if(!child->m_currentHovered) continue;

                if(mouse_state & MOUSE_LEFT)
                    for(const auto& func : m_elementClickCallbackFunctions)
                        func(m_application, this, i, MOUSE_LEFT);

                if(mouse_state & MOUSE_MIDDLE)
                    for(const auto& func : m_elementClickCallbackFunctions)
                        func(m_application, this, i, MOUSE_MIDDLE);

                if(mouse_state & MOUSE_RIGHT)
                    for(const auto& func : m_elementClickCallbackFunctions)
                        func(m_application, this, i, MOUSE_RIGHT);

            }
        }

        UIElementAbstract* getElement(ElementListSize index)
        {
            return m_children.at(index);
        }

        virtual bool isHovered() const override
        {
            return AABB::rectangleToPoint(this, Mouse::getPosition());
        }

        Layout* setTexture(sf::Texture* texture)
        {
            m_background.setTexture(texture);
            return this;
        }

        Layout* setBorder(const Box::Border& border)
        {
            m_background.setBorder(border);
            return this;
        }

        Layout* setBorder(const Vector2f& border_start, const Vector2f& border_end)
        {
            m_background.setBorder(Box::Border{border_start, border_end});
            return this;
        }

        Layout* setBackgroundVisible(bool option = true)
        {
            m_backgroundVisible = option;
            return this;
        }

        Layout* setBackgroundColor(const sf::Color& color)
        {
            m_background.setColor(color);
            return this;
        }

        Layout* setBackgroundGrowth(float growth)
        {
            m_backgroundGrowth = growth;
            return this;
        }

        Layout* setBackground(const sf::Color& color, float growth)
        {
            return setBackgroundColor(color)
                ->setBackgroundGrowth(growth)
                ->setBackgroundVisible(true);
        }

        Layout* addElementClickCallback(ElementClickCallbackFunc func)
        {
            m_elementClickCallbackFunctions.push_back(func);
            return this;
        }

        Layout* addElementHoverCallback(ElementHoverCallbackFunc func)
        {
            m_elementHoverCallbackFunctions.push_back(func);
            return this;
        }

        virtual void updateSize() = 0;
        virtual void updatePositions() = 0;
    protected:
        ElementList m_children;
    private:
        std::vector<ElementClickCallbackFunc> m_elementClickCallbackFunctions;
        std::vector<ElementHoverCallbackFunc> m_elementHoverCallbackFunctions;
        Box m_background;
        float m_backgroundGrowth{0.0f};
        bool m_backgroundVisible;
    };
}