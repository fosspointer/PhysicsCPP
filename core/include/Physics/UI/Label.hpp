#pragma once
#include "SFML/System/Vector2.hpp"
#include <Physics/UI/UIElement.hpp>
#include <Physics/System/Font.hpp>
#include <cmath>

namespace physics
{
    /**
     * @brief Basic label element, suitable for most cases
     */
    class Label : public UIElement<Label>
    {
    public:
        /**
         * @brief Label constructor
         * @param application The main application
         * @param text Title of the label
         * @param font_size Font-size of the label
         * @param margin Margin around the label, used by Layouts
         */
        Label(Application* application, const sf::String& text, unsigned int font_size, const sf::Vector2f& margin = {25.0f, 25.0f})
            :UIElement(application, sf::Vector2f{-1.0f, -1.0f}, margin, sf::Color::Black),
            m_Text(text, Font::GetDefault().Get(), font_size)
        {
            //Size is initially set to (-1, -1) and updated when appropriate
            UpdateSize();

            m_Text.setLineSpacing(0.0f);
        }

        /**
         * @brief Checks if the label is hovered by the mouse cursor, must be called after Update()
         * @return The result of the test
         */
        bool IsHovered() const override
        {
            return AABB::RectangleToPoint(m_Text.getGlobalBounds(), Mouse::GetPosition());
        }

        Label* SetFontSize(unsigned int size)
        {
            m_Text.setCharacterSize(size);
            UpdateSize();
            return this;
        }

        Label* SetOutline(const sf::Color& color, unsigned int size = 2)
        {
            m_Text.setOutlineColor(color);
            m_Text.setOutlineThickness(size);
            return this;
        }

        Label* SetText(const sf::String& text)
        {
            m_Text.setString(text);
            UpdateSize();
            return this;
        }

        Label* SetStyle(sf::Text::Style style)
        {
            m_Text.setStyle(style);
            return this;
        }

        inline unsigned int GetFontSize() const { return m_Text.getCharacterSize(); }
        const sf::String& GetText() const { return m_Text.getString(); }
        const sf::Uint32 GetStyle() const { return m_Text.getStyle(); }

        void Draw() override
        {
            m_Text.setPosition(m_Position);
            m_Text.setFillColor(m_Color);
            m_Text.setOrigin(GetBoundSize() / 2.0f + GetBoundPosition());

            m_Application->GetWindow().draw(m_Text);
        }   
    private:
        void UpdateSize()
        {
            m_Size = GetBoundSize();
            CalculateAnchor();
        }

        sf::Vector2f GetBoundSize() const
        {
            return sf::Vector2f{m_Text.getLocalBounds().width, m_Text.getLocalBounds().height};
        }

        sf::Vector2f GetBoundPosition() const
        {
            return sf::Vector2f{m_Text.getLocalBounds().left, m_Text.getLocalBounds().top};
        }

        sf::Text m_Text; 
    };
}