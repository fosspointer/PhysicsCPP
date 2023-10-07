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
            m_Text(text, Font::GetDefault().Get(), font_size)
        {
            //Size is initially set to (-1, -1) and updated when appropriate
            UpdateSize();
            SetColor(Color::Black);
            m_Text.setLineSpacing(0.0f);
        }

        /// @brief Checks if the label is hovered by the mouse cursor, must be called after Update()
        /// @return The result of the test
        bool IsHovered() const override
        {
            return AABB::RectangleToPoint(m_Text.getGlobalBounds(), Mouse::GetPosition());
        }

        /// @brief Set the label's font size
        /// @param size The font size
        /// @return Pointer to this, to allow for method chaining
        Label* SetFontSize(unsigned int size)
        {
            m_Text.setCharacterSize(size);
            UpdateSize();
            return this;
        }

        /// @brief Configure the label's outline
        /// @param color The outline color
        /// @param size The outline 'thickness'
        /// @return Pointer to this, to allow for method chaining
        Label* SetOutline(const sf::Color& color, unsigned int size = 2)
        {
            m_Text.setOutlineColor(color);
            m_Text.setOutlineThickness(size);
            return this;
        }

        /// @brief Configure the label's text (string)
        /// @param text The string to be used
        /// @return Pointer to this, to allow for method chaining
        Label* SetText(const sf::String& text)
        {
            m_Text.setString(text);
            UpdateSize();
            return this;
        }

        /// @brief Change the font style of the label (bold, italic, etc...)
        /// @param style The style to be used (from SFML)
        /// @return Pointer to this, to allow for method chaining
        Label* SetStyle(sf::Text::Style style)
        {
            m_Text.setStyle(style);
            return this;
        }

        /// @brief Get the label's font size
        /// @return Copy of the font size
        inline unsigned int GetFontSize() const { return m_Text.getCharacterSize(); }
        /// @brief Get the label's text (string)
        /// @return Immutable reference to the string
        const sf::String& GetText() const { return m_Text.getString(); }
        /// @brief Get the label's style (see the setter method)
        /// @return Copy of the size as an unsigned integer (that's what SFML returns)
        const sf::Uint32 GetStyle() const { return m_Text.getStyle(); }

        /// @brief Display the label object to the specified layer
        /// @param layer the layer's index
        void Draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_Text.setPosition(m_Position);
            m_Text.setFillColor(m_Color);
            m_Text.setOrigin(GetBoundSize() / 2.0f + GetBoundPosition());

            m_Application->Draw(&m_Text, layer);
        }   
    private:
        /// @brief Update's the element's size, while also requesting that its anchor be recalculated  
        void UpdateSize()
        {
            m_Size = GetBoundSize();
            CalculateAnchor();
        }

        /// @brief Get the label's bound size from its SFML local bounds
        /// @return Copy of the size
        sf::Vector2f GetBoundSize() const
        {
            return sf::Vector2f{m_Text.getLocalBounds().width, m_Text.getLocalBounds().height};
        }

        /// @brief Get the label's bound position from its SFML local bounds
        /// @return Copy of the position
        sf::Vector2f GetBoundPosition() const
        {
            return sf::Vector2f{m_Text.getLocalBounds().left, m_Text.getLocalBounds().top};
        }

        sf::Text m_Text; 
    };
}