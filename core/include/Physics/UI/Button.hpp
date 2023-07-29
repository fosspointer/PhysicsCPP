#pragma once
#include "Physics/System/AABB.hpp"
#include <Physics/System/Color.hpp>
#include <Physics/UI/UIElement.hpp>
#include <Physics/UI/Label.hpp>

namespace physics
{
    /**
     * @brief Basic button element, suitable for most cases
     */
    class Button : public UIElement<Button>
    {
    public:

        /**
         * @brief Construct a new Button object
         * 
         * @param application The main application
         * @param text The title of the button 
         * @param size The size of the button
         * @param margin Margin around the Button, used by Layouts
         */
        Button(Application* application, const sf::String text, const sf::Vector2f& size, const sf::Vector2f margin = sf::Vector2f{25.0f, 25.0f})
            :m_Label(application, text, 25.0f, sf::Vector2f{0.0f, 0.0f}), UIElement(application, size, margin)
        {
            SetButtonColors(physics::Color::White);
        }

        /**
         * @brief Get the Label object
         * @return Mutable reference to the label
         */
        inline Label& GetLabel() { return m_Label; }

        void Draw() override
        {
            m_ButtonBox.setPosition(m_Position - m_Size / 2.0f);
            m_ButtonBox.setSize(m_Size);

            if(IsHovered())
            {
                if(Mouse::GetInstance().CurrentState)
                    m_ButtonBox.setFillColor(m_PressColor);
                else m_ButtonBox.setFillColor(m_HoverColor);
            }
            else 
                m_ButtonBox.setFillColor(m_Color);
            m_Application->GetWindow().draw(m_ButtonBox);
            
            m_Label.SetPosition(m_Position);
            m_Label.Draw();
        }

        void CustomUpdate(float delta_time) override
        {
            m_Label.Update(delta_time);
        }

        /**
         * @brief Checks if the button is hovered by the mouse cursor, must be called after Update()
         * @return The result of the test
         */
        bool IsHovered() const override 
        {
            return AABB::RectangleToPoint(m_ButtonBox, Mouse::GetPosition());
        }

        Button* SetHoverColor(const sf::Color& color)
        {
            m_HoverColor = color;
            return this;
        }

        Button* SetPressColor(const sf::Color& color)
        {
            m_PressColor = color;
            return this;
        }

        Button* SetOutlineColor(const sf::Color& color)
        {
            m_ButtonBox.setOutlineColor(color);
            return this;
        }

        Button* SetOutline(unsigned int size)
        {
            m_ButtonBox.setOutlineThickness(size);
            return this;
        }

        Button* SetButtonColors(const sf::Color& color)
        {
            m_Color = color;
            m_HoverColor = sf::Color(color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, color.a);
            m_PressColor = sf::Color(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a);
            m_ButtonBox.setOutlineColor(sf::Color(color.r * 0.25f, color.g * 0.25f, color.b * 0.25f, color.a));
            return this;
        }
    private:
        Label m_Label;
        sf::Color m_HoverColor = physics::Color::LightGray, m_PressColor = physics::Color::Gray;
        sf::RectangleShape m_ButtonBox;
    };
}