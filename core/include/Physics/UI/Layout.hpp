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
            :UIElement(application), m_BackgroundVisible(false)
        {}

        virtual ~Layout()
        {
            for(ElementList::size_type i = 0; i < m_Children.size(); i++)
                if(m_Children[i])
                    delete m_Children[i];
        }

        virtual void Draw(int8_t layer = PHYSICS_LAYER_UI_0) override
        {
            if(m_BackgroundVisible)
                m_Application->Draw(&m_Background, layer);
            
            for(ElementList::size_type i = 0; i < m_Children.size(); i++)
                m_Children[i]->Draw(PHYSICS_LAYER_UI_1);
        }

        template <class UIElementImpl>
        UIElementImpl* PushElement(UIElementImpl* element)
        {
            m_Children.push_back(element);
            UpdateSize();
            UpdatePositions();
            return element;
        }


        ElementList& GetElements()
        {
            return m_Children;
        }

        const ElementList& GetElements() const
        {
            return m_Children;
        }

        UIElementAbstract* GetElement(ElementList::size_type index) const
        {
            return m_Children[index];
        }

        ElementList::size_type GetElementCount() const
        {
            return m_Children.size();
        }

        void CustomUpdate(float delta_time) override final
        {
            bool pragma_updated = false;

            for(auto& element : m_Children)
                if(element->GetPragmaUpdated())
                    pragma_updated = true;
                
            if(GetPragmaUpdated())
                pragma_updated = true;

            if(m_Application->GetResized() || pragma_updated)
            {
                UpdateSize();
                UpdatePositions();
            }
        #ifdef PHYSICS_DEBUG
            if(DisplayBounds)
            {
                for(auto& element : m_Children)
                    element->DisplayBounds = true;
            }
        #endif
            m_Background.SetPosition(m_Position);
            m_Background.SetSize(m_Size + Vector2f{m_BackgroundGrowth});
            for(auto& element : m_Children)
                element->Update(delta_time);
            
            if(!IsHovered() && !StoppedHover()) return; //The below code is for handling click and hover events.
            //Hence, if IsHovered() returns false, execution is stopped
            
            auto& mouse_state = Mouse::GetInstance().ClickState;

            for(ElementListSize i = 0; i < m_Children.size(); i++)
            {
                auto& child = m_Children[i];

                if(!child->m_CurrentHovered && !child->m_PreviousHovered) continue;
                
                for(const auto& func : m_ElementHoverCallbackFunctions)
                    func(m_Application, this, i, child->StoppedHover());

                //Bitwise arithmetic and function callback iterators
                //Note: State & MOUSE_<BUTTON> means that <BUTTON> is 'enabled' for the state

                if(!child->m_CurrentHovered) continue;

                if(mouse_state & MOUSE_LEFT)
                    for(const auto& func : m_ElementClickCallbackFunctions)
                        func(m_Application, this, i, MOUSE_LEFT);

                if(mouse_state & MOUSE_MIDDLE)
                    for(const auto& func : m_ElementClickCallbackFunctions)
                        func(m_Application, this, i, MOUSE_MIDDLE);

                if(mouse_state & MOUSE_RIGHT)
                    for(const auto& func : m_ElementClickCallbackFunctions)
                        func(m_Application, this, i, MOUSE_RIGHT);

            }
        }

        UIElementAbstract* GetElement(ElementListSize index)
        {
            return m_Children.at(index);
        }

        virtual bool IsHovered() const override
        {
            return AABB::RectangleToPoint(this, Mouse::GetPosition());
        }

        Layout* SetTexture(sf::Texture* texture)
        {
            m_Background.SetTexture(texture);
            return this;
        }

        Layout* SetBorder(const Box::Border& border)
        {
            m_Background.SetBorder(border);
            return this;
        }

        Layout* SetBorder(const Vector2f& border_start, const Vector2f& border_end)
        {
            m_Background.SetBorder(Box::Border{border_start, border_end});
            return this;
        }

        Layout* SetBackgroundVisible(bool option = true)
        {
            m_BackgroundVisible = option;
            return this;
        }

        Layout* SetBackgroundColor(const sf::Color& color)
        {
            m_Background.SetColor(color);
            return this;
        }

        Layout* SetBackgroundGrowth(float growth)
        {
            m_BackgroundGrowth = growth;
            return this;
        }

        Layout* SetBackground(const sf::Color& color, float growth)
        {
            return SetBackgroundColor(color)
                ->SetBackgroundGrowth(growth)
                ->SetBackgroundVisible(true);
        }

        Layout* AddElementClickCallback(ElementClickCallbackFunc func)
        {
            m_ElementClickCallbackFunctions.push_back(func);
            return this;
        }

        Layout* AddElementHoverCallback(ElementHoverCallbackFunc func)
        {
            m_ElementHoverCallbackFunctions.push_back(func);
            return this;
        }

        virtual void UpdateSize() = 0;
        virtual void UpdatePositions() = 0;
    protected:
        ElementList m_Children;
    private:
        std::vector<ElementClickCallbackFunc> m_ElementClickCallbackFunctions;
        std::vector<ElementHoverCallbackFunc> m_ElementHoverCallbackFunctions;
        Box m_Background;
        float m_BackgroundGrowth{0.0f};
        bool m_BackgroundVisible;
    };
}