#pragma once
#include <Physics/System/Color.hpp>
#include <Physics/UI/UIElement.hpp>
#include <Physics/UI/Label.hpp>

namespace physics
{
    /// @brief Basic button element, suitable for most cases
    class Button : public UIElement<Button>
    {
    public:
        /// @brief Construct a new Button object
        /// @param application The main application
        /// @param text The title of the button 
        /// @param size The size of the button
        Button(Application* application)
            :m_label(application, "Button", 25.0f), UIElement(application)
        {
            setButtonColors(physics::Color::White);
        }

        /// @brief Get the Label object
        /// @return Mutable reference to the label
        inline Label& getLabel() { return m_label; }

        void draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_buttonBox.setPosition(m_position - m_size / 2.0f);
            m_buttonBox.setSize(m_size);

            if(isHovered())
            {
                if(Mouse::getInstance().currentState)
                    m_buttonBox.setFillColor(m_pressColor);
                else m_buttonBox.setFillColor(m_hoverColor);
            }
            else 
                m_buttonBox.setFillColor(m_color);
            m_application->draw(&m_buttonBox, layer);
            
            m_label.setPosition(m_position);
            m_label.draw(layer);
        }

        void customUpdate(float delta_time) override
        {
            m_label.update(delta_time);
        }

        /// @brief Checks if the button is hovered by the mouse cursor, must be called after update()
        /// @return The result of the test
        bool isHovered() const override 
        {
            return AABB::rectangleToPoint(m_buttonBox, Mouse::getPosition());
        }

        Button* setTitle(const sf::String& title)
        {
            m_label.setText(title);
            return this;
        }

        Button* setFontSize(unsigned int size) 
        {
            m_label.setFontSize(size);
            return this;
        }

        Button* setHoverColor(const sf::Color& color)
        {
            m_hoverColor = color;
            return this;
        }

        Button* setPressColor(const sf::Color& color)
        {
            m_pressColor = color;
            return this;
        }

        Button* setOutlineColor(const sf::Color& color)
        {
            m_buttonBox.setOutlineColor(color);
            return this;
        }

        Button* setOutline(unsigned int size)
        {
            m_buttonBox.setOutlineThickness(size);
            return this;
        }

        Button* setButtonColors(const sf::Color& color)
        {
            m_color = color;
            m_hoverColor = sf::Color(color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, color.a);
            m_pressColor = sf::Color(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a);
            m_buttonBox.setOutlineColor(sf::Color(color.r * 0.25f, color.g * 0.25f, color.b * 0.25f, color.a));
            return this;
        }
    private:
        Label m_label;
        sf::Color m_hoverColor = physics::Color::LightGray, m_pressColor = physics::Color::Gray;
        sf::RectangleShape m_buttonBox;
    };
}