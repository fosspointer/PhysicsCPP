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
            :m_Application(application), m_Size(size), m_Margin(margin), m_Color(color), m_FloatingElement(floating_element)
        {}
        
        virtual ~UIElementAbstract() = default;
        
        /// @brief Draws the element, utilizing the main application's window
        virtual void Draw(int8_t layer) = 0;
        
        /// @brief Checks if the element is hovered by the mouse cursor, must be called after Update()
        /// @return The result of the test
        virtual bool IsHovered() const = 0;

        inline bool StoppedHover() const 
        {
            return !m_CurrentHovered && m_PreviousHovered;
        }

        /// @brief The element's update function
        /// @param delta_time Time elapsed since previous frame
        virtual void Update(float delta_time) = 0;
        
        /// @brief Calculates the element's position for the specified Anchor enumerator
        virtual void CalculateAnchor()
        {
            auto size = (Vector2f)m_Application->GetWindow().getSize();
            auto center = size / 2.0f;
            auto space = m_Margin + m_Size / 2.0f;
            auto space_inverse = size - space;

            switch(m_Anchor)
            {
            case Anchor::None: break;
            case Anchor::Center: m_Position = center; break;
            case Anchor::Top:   m_Position = sf::Vector2f{center.x, space.y}; break;
            case Anchor::Bottom: m_Position = sf::Vector2f{center.x, space_inverse.y}; break;
            case Anchor::Left: m_Position = sf::Vector2f{space.x, center.y}; break;
            case Anchor::Right: m_Position = sf::Vector2f{space_inverse.x, center.y}; break;
            case Anchor::TopLeft: m_Position = space; break;
            case Anchor::TopRight: m_Position = sf::Vector2f{space_inverse.x, space.y}; break;
            case Anchor::BottomLeft: m_Position = sf::Vector2f{space.x, space_inverse.y}; break;
            case Anchor::BottomRight: m_Position = space_inverse; break;
            }
        }

        /// @brief Returns whether the element is being pressed by the specified button
        /// @param button Mouse button to test for
        /// @return The result of the test
        bool IsPressed(MouseButton button = MOUSE_LEFT) const
        {
            return (IsHovered() && Mouse::GetInstance().CurrentState & button) && (m_FloatingElement || !m_Application->HasFloating());
        } 
        
        /// @brief Get the Position object
        /// @return Immutable reference to the element's position
        inline const Vector2f& GetPosition() const { return m_Position; }
        
        /// @brief Get the Size object
        /// @return Immutable reference to the element's size
        inline const Vector2f& GetSize() const { return m_Size; }
        
        /// @brief Get the Margin object
        /// @return Immutable reference to the element's margin
        inline const Vector2f& GetMargin() const { return m_Margin; }
        
        /// @brief Get the Color object
        /// @return Immutable reference to the element's color
        inline const Color& GetColor() const { return m_Color; }
        
        /// @brief Get the Anchor object
        /// @return Immutable reference to the element's anchor
        inline const Anchor& GetAnchor() const { return m_Anchor; }

        /// @brief Get the boolean corresponding to the pragma(size, position, anchor) updated 'event'
        /// @return Copy of the boolean member
        inline bool GetPragmaUpdated() const { return m_PragmaUpdated; }
        
        /// @brief (INTERNAL FUNCTION) Sets the element's position
        /// @param position The new position
        inline void AbstractSetPosition(const sf::Vector2f& position)
        {
            m_PragmaUpdated = true;
            m_Position = position;
            m_Anchor = Anchor::None;  
        }
        
        /// @brief (INTERNAL FUNCTION) Sets the element's size
        /// @param position The new size
        inline void AbstractSetSize(const sf::Vector2f& size)
        {
            m_PragmaUpdated = true;
            m_Size = size;
        }
        
        /// @brief (INTERNAL FUNCTION) Sets the element's margin
        /// @param position The new margin
        inline void AbstractSetMargin(const sf::Vector2f& margin)
        {
            m_PragmaUpdated = true;
            m_Margin = margin;
        }
        
        /// @brief (INTERNAL FUNCTION) Sets the element's color
        /// @param position The new color
        inline void AbstractSetColor(const sf::Color& color)
        {
            m_Color = color; 
        }

        /// @brief (INTERNAL FUNCTION) Sets the element's anchor and calculates anchor
        /// @param position The new anchor
        inline void AbstractSetAnchor(const Anchor& anchor)
        {
            m_PragmaUpdated = true;
            m_Anchor = anchor;
            CalculateAnchor();
        }
    protected:
        bool m_PragmaUpdated{true}, m_PreviousHovered{false}, m_CurrentHovered{false};
        const bool m_FloatingElement;
        Application* m_Application;
        Vector2f m_Position, m_Size, m_Margin;
        Color m_Color;
        Anchor m_Anchor{Anchor::None};
        friend class Layout;
        friend class HLayout;
        friend class VLayout;
    public:
        //Debug specific code, ignored by the preprocessor on release builds
    #ifdef PHYSICS_DEBUG
        bool DisplayBounds = false;
    #endif
    };
}