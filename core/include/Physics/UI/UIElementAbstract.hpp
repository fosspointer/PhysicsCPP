#pragma once
#include <Physics/System/Vector2.hpp>
#include <Physics/System/Color.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/System/Mouse.hpp>

namespace physics
{
    /// @brief Enum representing options for which UIElements can be anchored
    enum class Anchor : unsigned char
    {
        None, Center, Top, Bottom, Left, Right, TopLeft, TopRight, BottomLeft, BottomRight
    };
    
    /// @brief Base of the UI system, polymorphic class, inheritted by UIElement<Impl> for dependency injection
    class UIElementAbstract
    {
    public:
        /// @brief UIElementAbstract constructor, is called by UIElement
        /// @param application pointer to the main application
        /// @param size the size of the ui element, use -1 where not calculated (e.g. [width, -1], [-1, -1], etc...)
        /// @param margin the element's margin, used by layouts
        /// @param color the color of the element
        UIElementAbstract(Application* application, const Vector2f& size, const Vector2f& margin, const sf::Color& color, bool floating_element)
            :m_application(application), m_size(size), m_margin(margin), m_color(color), m_floatingElement(floating_element)
        {}
        
        virtual ~UIElementAbstract() = default;
        
        /// @brief Draws the element, utilizing the main application's window
        virtual void draw(int8_t layer) = 0;
        
        /// @brief Checks if the element is hovered by the mouse cursor, must be called after update()
        /// @return The result of the test
        virtual bool isHovered() const = 0;

        inline bool stoppedHover() const 
        {
            return !m_currentHovered && m_previousHovered;
        }

        /// @brief The element's update function
        /// @param delta_time Time elapsed since previous frame
        virtual void update(float delta_time) = 0;
        
        /// @brief Calculates the element's position for the specified Anchor enumerator
        virtual void calculateAnchor()
        {
            auto size = (Vector2f)m_application->getWindow().getSize();
            auto center = size / 2.0f;
            auto space = m_margin + m_size / 2.0f;
            auto space_inverse = size - space;

            switch(m_anchor)
            {
            case Anchor::None: break;
            case Anchor::Center: m_position = center; break;
            case Anchor::Top:   m_position = sf::Vector2f{center.x, space.y}; break;
            case Anchor::Bottom: m_position = sf::Vector2f{center.x, space_inverse.y}; break;
            case Anchor::Left: m_position = sf::Vector2f{space.x, center.y}; break;
            case Anchor::Right: m_position = sf::Vector2f{space_inverse.x, center.y}; break;
            case Anchor::TopLeft: m_position = space; break;
            case Anchor::TopRight: m_position = sf::Vector2f{space_inverse.x, space.y}; break;
            case Anchor::BottomLeft: m_position = sf::Vector2f{space.x, space_inverse.y}; break;
            case Anchor::BottomRight: m_position = space_inverse; break;
            }
        }

        /// @brief Returns whether the element is being pressed by the specified button
        /// @param button Mouse button to test for
        /// @return The result of the test
        bool isPressed(MouseButton button = MOUSE_LEFT) const
        {
            return (isHovered() && Mouse::getInstance().currentState & button) && (m_floatingElement || !m_application->hasFloating());
        } 
        
        /// @brief Get the Position object
        /// @return Immutable reference to the element's position
        inline const Vector2f& getPosition() const { return m_position; }
        
        /// @brief Get the Size object
        /// @return Immutable reference to the element's size
        inline const Vector2f& getSize() const { return m_size; }
        
        /// @brief Get the Margin object
        /// @return Immutable reference to the element's margin
        inline const Vector2f& getMargin() const { return m_margin; }
        
        /// @brief Get the Color object
        /// @return Immutable reference to the element's color
        inline const Color& getColor() const { return m_color; }
        
        /// @brief Get the Anchor object
        /// @return Immutable reference to the element's anchor
        inline const Anchor& getAnchor() const { return m_anchor; }

        /// @brief Get the boolean corresponding to the pragma(size, position, anchor) updated 'event'
        /// @return Copy of the boolean member
        inline bool getPragmaUpdated() const { return m_pragmaUpdated; }
        
        /// @brief (INTERNAL FUNCTION) sets the element's position
        /// @param position The new position
        inline void abstractSetPosition(const sf::Vector2f& position)
        {
            m_pragmaUpdated = true;
            m_position = position;
            m_anchor = Anchor::None;  
        }
        
        /// @brief (INTERNAL FUNCTION) sets the element's size
        /// @param position The new size
        inline void abstractSetSize(const sf::Vector2f& size)
        {
            m_pragmaUpdated = true;
            m_size = size;
        }
        
        /// @brief (INTERNAL FUNCTION) sets the element's margin
        /// @param position The new margin
        inline void abstractSetMargin(const sf::Vector2f& margin)
        {
            m_pragmaUpdated = true;
            m_margin = margin;
        }
        
        /// @brief (INTERNAL FUNCTION) sets the element's color
        /// @param position The new color
        inline void abstractSetColor(const sf::Color& color)
        {
            m_color = color; 
        }

        /// @brief (INTERNAL FUNCTION) sets the element's anchor and calculates anchor
        /// @param position The new anchor
        inline void abstractSetAnchor(const Anchor& anchor)
        {
            m_pragmaUpdated = true;
            m_anchor = anchor;
            calculateAnchor();
        }
    protected:
        bool m_pragmaUpdated{true}, m_previousHovered{false}, m_currentHovered{false};
        const bool m_floatingElement;
        Application* m_application;
        Vector2f m_position, m_size, m_margin;
        Color m_color;
        Anchor m_anchor{Anchor::None};
        friend class Layout;
        friend class HLayout;
        friend class VLayout;
    public:
        //Debug specific code, ignored by the preprocessor on release builds
    #ifdef PHYSICS_DEBUG
        bool displayBounds = false;
    #endif
    };
}