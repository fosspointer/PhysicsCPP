#pragma once
#include <Physics/UI/UIElementAbstract.hpp>
#include <functional>
#include <SFML/Graphics.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/System/Mouse.hpp>

namespace physics
{
    ///Forward declaration of the UIElement<Impl> class
    template <class UIElementImpl>
    class UIElement;

    /**
     * @brief UIElement hover callback function pointer type
     * @tparam UIElementImpl 
     */
    template <class UIElementImpl>
    using HoverCallbackFunc = std::function<void(Application*, UIElementImpl*)>;
    
    /**
     * @brief UIElement click callback function pointer type
     * @tparam UIElementImpl 
     */
    template <class UIElementImpl>
    using ClickCallbackFunc = std::function<void(Application*, UIElementImpl*, MouseButton)>;

    /**
     * @brief Dependency-injection, polymorphic base class implementation of UIElementAbstract, build-specific, contains some debug-only code
     * @tparam UIElementImpl 
     */
    template <class UIElementImpl>
    class UIElement : public UIElementAbstract
    {
    public:
        /**
         * @brief UIElement constructor
         * @param application pointer to the main application
         * @param size the size of the ui element, use (-1, -1) if not yet calculated
         * @param margin the element's margin, used by layouts
         * @param color the color of the element
         */
        UIElement(Application* application, const sf::Vector2f& size, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f}, const sf::Color& color = sf::Color::White)
            :UIElementAbstract(application, size, margin, color)
        //Debug specific code, ignored by the preprocessor on release builds
        #ifdef PHYSICS_DEBUG 
            , m_DebugModeColor(rand() % 255, rand() % 255, rand() % 255, 255)
        #endif
        {}

        /**
         * @brief The element's update function
         * @param delta_time Time elapsed since previous frame
         */
        void Update(float delta_time) override final
        {
        //Debug specific code, ignored by the preprocessor on release builds
        #ifdef PHYSICS_DEBUG
            if(DisplayBounds) //If the DisplayBounds 'flag' is set
            //Render a rectangle representating the element's bounds
            {
                sf::RectangleShape aabb;
                aabb.setFillColor(sf::Color::Transparent);
                aabb.setOutlineColor(m_DebugModeColor);
                //aabb is drawn only in debug- this hardcoded number is ok here
                aabb.setOutlineThickness(10);
                aabb.setPosition(m_Position - m_Size / 2.0f);
                aabb.setSize(m_Size);
                m_Application->GetWindow().draw(aabb);
            }
        #endif

            //Configuration done by classes deriving
            CustomUpdate(delta_time);

            if(m_Application->GetResized())
                CalculateAnchor();

            if(!IsHovered()) return; //The below code is for handling click and hover events.
            //Hence, if IsHovered() returns false, execution is stopped

            for(const auto& func : m_HoverCallbackFunctions)
                func(m_Application, dynamic_cast<UIElementImpl*>(this));
            
            auto& mouse_state = Mouse::GetInstance().ClickState;

            //Bitwise arithmetic and function callback iterators
            //Note: State & MOUSE_<BUTTON> means that <BUTTON> is 'enabled' for the state

            if(mouse_state & MOUSE_LEFT)
                for(const auto& func : m_ClickCallbackFunctions)
                    func(m_Application, dynamic_cast<UIElementImpl*>(this), MOUSE_LEFT);

            if(mouse_state & MOUSE_MIDDLE)
                for(const auto& func : m_ClickCallbackFunctions)
                    func(m_Application, dynamic_cast<UIElementImpl*>(this), MOUSE_MIDDLE);

            if(mouse_state & MOUSE_RIGHT)
                for(const auto& func : m_ClickCallbackFunctions)
                    func(m_Application, dynamic_cast<UIElementImpl*>(this), MOUSE_RIGHT);
        }

        /**
         * @brief Adds a function to the hover-callback list, so as to be called whenever the element is hovered by the mouse cursor
         * @param func A pointer to the callback function
         * @return this casted as UIElementImpl*, to allow for method chaining
         */
        UIElementImpl* AddHoverCallback(HoverCallbackFunc<UIElementImpl> func)
        {
            m_HoverCallbackFunctions.push_back(func);
            return dynamic_cast<UIElementImpl*>(this);
        }

        /**
         * @brief Adds a function to the click-callback list, so as to be called whenever the element is clicked by the mouse cursror
         * @param func A pointer to the callback function
         * @return this casted as UIElementImpl*, to allow for method chaining 
         */
        UIElementImpl* AddClickCallback(ClickCallbackFunc<UIElementImpl> func)
        {
            m_ClickCallbackFunctions.push_back(func);
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /**
         * @brief Sets the element's position, as well as resetting the anchor
         * @param position The new position
         * @return this casted as UIElementImpl*, to allow for method chaining 
         */
        UIElementImpl* SetPosition(const sf::Vector2f& position)
        {
            m_Position = position;
            m_Anchor = Anchor::None;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /**
         * @brief Sets the element's size
         * @param size The new size
         * @return this casted as UIElementImpl*, to allow for method chaining 
         */
        UIElementImpl* SetSize(const sf::Vector2f& size)
        {
            m_Size = size;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /**
         * @brief Sets the element's margin
         * @param margin The new margin
         * @return this casted as UIElementImpl*, to allow for method chaining 
         */
        UIElementImpl* SetMargin(const sf::Vector2f& margin)
        {
            m_Margin = margin;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /**
         * @brief Sets the element's color
         * @param color The new color
         * @return this casted as UIElementImpl*, to allow for method chaining 
         */
        UIElementImpl* SetColor(const sf::Color& color)
        {
            m_Color = color;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /**
         * @brief Sets the element's anchor, as well as calling CalcluateAnchor()
         * @param anchor The new anchor
         * @return this casted as UIElementImpl*, to allow for method chaining 
         */
        UIElementImpl* SetAnchor(const Anchor& anchor)
        {
            m_Anchor = anchor;
            CalculateAnchor();
            return dynamic_cast<UIElementImpl*>(this);  
        }

    //Debug specific code, ignored by the preprocessor on release builds
    #ifdef PHYSICS_DEBUG
        static bool DisplayBounds;
    #endif
    protected:
        virtual void CustomUpdate(float delta_time) {}
    private:
    #ifdef PHYSICS_DEBUG
        sf::Color m_DebugModeColor;
    #endif
        std::vector<HoverCallbackFunc<UIElementImpl>> m_HoverCallbackFunctions;
        std::vector<ClickCallbackFunc<UIElementImpl>> m_ClickCallbackFunctions;
    };

#ifdef PHYSICS_DEBUG
    template <class T>
    bool UIElement<T>::DisplayBounds = false;
#endif
}