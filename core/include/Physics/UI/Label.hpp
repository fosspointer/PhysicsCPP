#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <Physics/UI/UIElement.hpp>
#include <Physics/System/Font.hpp>

namespace physics
{
    /// @brief Basic label element, suitable for most cases
    class Label : public UIElement<Label>
    {
    public:
        /// @brief Label constructor
        /// @param application The main application
        /// @param text Title of the label
        /// @param font_size Font-size of the label
        /// @param margin Margin around the label, used by Layouts
        Label(Application* application, const sf::String& text = "Label", unsigned int font_size = 25)
            :UIElement(application),
            m_text(text, Font::getDefault().get(), font_size)
        {
            //Size is initially set to (-1, -1) and updated when appropriate
            updateSize();
            setColor(Color::Black);
            m_text.setLineSpacing(0.0f);
        }

        /// @brief Checks if the label is hovered by the mouse cursor, must be called after update()
        /// @return The result of the test
        bool isHovered() const override
        {
            return AABB::rectangleToPoint(m_text.getGlobalBounds(), Mouse::getPosition());
        }

        /// @brief Set the label's font size
        /// @param size The font size
        /// @return Pointer to this, to allow for method chaining
        Label* setFontSize(unsigned int size)
        {
            m_text.setCharacterSize(size);
            updateSize();
            return this;
        }

        /// @brief Configure the label's outline
        /// @param color The outline color
        /// @param size The outline 'thickness'
        /// @return Pointer to this, to allow for method chaining
        Label* setOutline(const sf::Color& color, unsigned int size = 2)
        {
            m_text.setOutlineColor(color);
            m_text.setOutlineThickness(size);
            return this;
        }

        /// @brief Configure the label's text (string)
        /// @param text The string to be used
        /// @return Pointer to this, to allow for method chaining
        Label* setText(const sf::String& text)
        {
            m_text.setString(text);
            updateSize();
            return this;
        }

        /// @brief Change the font style of the label (bold, italic, etc...)
        /// @param style The style to be used (from SFML)
        /// @return Pointer to this, to allow for method chaining
        Label* setStyle(sf::Text::Style style)
        {
            m_text.setStyle(style);
            return this;
        }

        /// @brief Get the label's font size
        /// @return Copy of the font size
        inline unsigned int getFontSize() const { return m_text.getCharacterSize(); }
        /// @brief Get the label's text (string)
        /// @return Immutable reference to the string
        const sf::String& getText() const { return m_text.getString(); }
        /// @brief Get the label's style (see the setter method)
        /// @return Copy of the size as an unsigned integer (that's what SFML returns)
        const sf::Uint32 getStyle() const { return m_text.getStyle(); }

        /// @brief Display the label object to the specified layer
        /// @param layer the layer's index
        void draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_text.setPosition(m_position);
            m_text.setFillColor(m_color);
            m_text.setOrigin(getBoundSize() / 2.0f + getBoundPosition());

            m_application->draw(&m_text, layer);
        }   
    private:
        /// @brief Update's the element's size, while also requesting that its anchor be recalculated  
        void updateSize()
        {
            m_size = getBoundSize();
            calculateAnchor();
        }

        /// @brief Get the label's bound size from its SFML local bounds
        /// @return Copy of the size
        sf::Vector2f getBoundSize() const
        {
            return sf::Vector2f{m_text.getLocalBounds().width, m_text.getLocalBounds().height};
        }

        /// @brief Get the label's bound position from its SFML local bounds
        /// @return Copy of the position
        sf::Vector2f getBoundPosition() const
        {
            return sf::Vector2f{m_text.getLocalBounds().left, m_text.getLocalBounds().top};
        }

        sf::Text m_text; 
    };
}