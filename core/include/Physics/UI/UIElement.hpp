#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/System/Mouse.hpp>
#include <Physics/UI/UIElementAbstract.hpp>

namespace physics
{
    ///Forward declaration of the UIElement<Impl> class
    template <class UIElementImpl>
    class UIElement;
    
    /// @brief UIElement hover callback function pointer type
    /// @tparam UIElementImpl 
    template <class UIElementImpl>
    using HoverCallbackFunc = std::function<void(Application*, UIElementImpl*, bool)>;
    
    /// @brief UIElement click callback function pointer type
    /// @tparam UIElementImpl 
    template <class UIElementImpl>
    using ClickCallbackFunc = std::function<void(Application*, UIElementImpl*, MouseButton)>;

    /// @brief Dependency-injection, polymorphic base class implementation of UIElementAbstract, build-specific, contains some debug-only code
    /// @tparam UIElementImpl 
    template <class UIElementImpl>
    class UIElement : public UIElementAbstract
    {
    public:
        /// @brief UIElement constructor, must be called by classes deriving from here, with defaults
        /// @param application pointer to the main application
        UIElement(Application* application, bool floating_element = false)
            :UIElementAbstract(application, Vector2f{-1.0f, -1.0f}, Vector2f{25.0f, 25.0f}, Color::White, floating_element)
        {}

        virtual ~UIElement() = default;
        
        /// @brief The element's update function
        /// @param delta_time Time elapsed since previous frame
        void update(float delta_time) override final
        {
            m_previousHovered = m_currentHovered;
            m_currentHovered = isHovered();
        //Debug specific code, ignored by the preprocessor on release builds
        #ifdef PHYSICS_DEBUG
            if(displayBounds) //If the DisplayBounds 'flag' is set
            //Render a rectangle representing the element's bounds
            {
                sf::RectangleShape aabb;
                aabb.setFillColor(sf::Color::Transparent);
                aabb.setOutlineColor(m_debugModeColor);
                //AABB is only drawn in debug mode- this hardcoded number is ok here
                aabb.setOutlineThickness(10);
                aabb.setPosition(m_position - m_size / 2.0f);
                aabb.setSize(m_size);
                m_application->getWindow().draw(aabb);
            }
        #endif
            //Configuration done by deriving classes
            customUpdate(delta_time);

            if(m_application->getResized() || getPragmaUpdated())
                calculateAnchor();

            if((!isHovered() && !stoppedHover()) || (m_application->hasFloating() && !m_floatingElement)) return; //The below code is for handling click and hover events.
            //Hence, if isHovered() returns false, execution is stopped
            for(const auto& func : m_hoverCallbackFunctions)
                func(m_application, dynamic_cast<UIElementImpl*>(this), stoppedHover());
            
            auto& mouse_state = Mouse::getInstance().clickState;

            //Bitwise arithmetic and function callback iterators
            //Note: State & MOUSE_<BUTTON> means that <BUTTON> is 'enabled' for the state
            if(mouse_state & MOUSE_LEFT)
                for(const auto& func : m_clickCallbackFunctions)
                    func(m_application, dynamic_cast<UIElementImpl*>(this), MOUSE_LEFT);

            if(mouse_state & MOUSE_MIDDLE)
                for(const auto& func : m_clickCallbackFunctions)
                    func(m_application, dynamic_cast<UIElementImpl*>(this), MOUSE_MIDDLE);

            if(mouse_state & MOUSE_RIGHT)
                for(const auto& func : m_clickCallbackFunctions)
                    func(m_application, dynamic_cast<UIElementImpl*>(this), MOUSE_RIGHT);

            m_pragmaUpdated = false;
        }

        /// @brief Adds a function to the hover-callback list, so as to be called whenever the element is hovered by the mouse cursor
        /// @param func A pointer to the callback function
        /// @return this casted as UIElementImpl*, to allow for method chaining
        UIElementImpl* addHoverCallback(HoverCallbackFunc<UIElementImpl> func)
        {
            m_hoverCallbackFunctions.push_back(func);
            return dynamic_cast<UIElementImpl*>(this);
        }

        /// @brief Adds a function to the click-callback list, so as to be called whenever the element is clicked by the mouse cursror
        /// @param func A pointer to the callback function
        /// @return this casted as UIElementImpl*, to allow for method chaining          
        UIElementImpl* addClickCallback(ClickCallbackFunc<UIElementImpl> func)
        {
            m_clickCallbackFunctions.push_back(func);
            return dynamic_cast<UIElementImpl*>(this);  
        }
        
        /// @brief sets the element's position, as well as resetting the anchor
        /// @param position The new position
        /// @return this casted as UIElementImpl*, to allow for method chaining 
        UIElementImpl* setPosition(const sf::Vector2f& position)
        {
            m_pragmaUpdated = true;
            m_position = position;
            m_anchor = Anchor::None;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /// @brief sets the element's size
        /// @param size The new size
        /// @return this casted as UIElementImpl*, to allow for method chaining          
        UIElementImpl* setSize(const sf::Vector2f& size)
        {
            m_pragmaUpdated = true;
            m_size = size;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /// @brief sets the element's margin
        /// @param margin The new margin
        /// @return this casted as UIElementImpl*, to allow for method chaining  
        UIElementImpl* setMargin(const sf::Vector2f& margin)
        {
            m_pragmaUpdated = true;
            m_margin = margin;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /// @brief sets the element's color
        /// @param color The new color
        /// @return this casted as UIElementImpl*, to allow for method chaining 
        UIElementImpl* setColor(const sf::Color& color)
        {
            m_pragmaUpdated = true;
            m_color = color;
            return dynamic_cast<UIElementImpl*>(this);  
        }

        /// @brief sets the element's anchor, as well as calling calcluateAnchor()
        /// @param anchor The new anchor
        /// @return this casted as UIElementImpl*, to allow for method chaining          
        UIElementImpl* setAnchor(const Anchor& anchor)
        {
            m_pragmaUpdated = true;
            m_anchor = anchor;
            calculateAnchor();
            return dynamic_cast<UIElementImpl*>(this);  
        }
    protected:
        virtual void customUpdate(float delta_time) {}
    private:
    #ifdef PHYSICS_DEBUG
        sf::Color m_debugModeColor{rand() % 255, rand() % 255, rand() % 255};
    #endif
        std::vector<HoverCallbackFunc<UIElementImpl>> m_hoverCallbackFunctions;
        std::vector<ClickCallbackFunc<UIElementImpl>> m_clickCallbackFunctions;
    };
}