#pragma once
#include <Physics/System/Color.hpp>
#include <Physics/System/Box.hpp>
#include <Physics/System/Textures.hpp>
#include <Physics/System/Sizes.hpp>
#include <Physics/UI/UIElement.hpp>
#include <Physics/UI/Label.hpp>

namespace physics::Textured
{
    /// @brief Basic button element, suitable for most cases
    class Button : public UIElement<Button>
    {
    public:
        /// @brief Construct a new Button object
        /// @param application The main application
        /// @param texture The texture for when the button is idle
        /// @param pressed_texture The texture for when the button is pressed
        /// @param text The title of the button 
        /// @param border_size The cutoff of the main texture
        /// @param border_size_pressed The cutoff of the pressed texture
        /// @param size The size of the button
        Button(Application* application, sf::Texture* texture, sf::Texture* pressed_texture, 
            const Box::Border& border, const Box::Border& border_pressed)
            :m_Label(application, "Button", 25.0f), UIElement(application), m_Texture(texture), m_PressedTexture(pressed_texture), 
            m_Border(border), m_BorderPressed(border_pressed)
        {
            SetSize({300.0f, 70.0f});
        }

        /// @brief Construct a new Button object
        /// @param application The main application
        /// @param text The title of the button 
        /// @param size The size of the button
        /// @param margin Margin around the Button, used by layouts
        Button(Application* application)
            :m_Label(application, "Button", 25.0f), UIElement(application), 
            m_Border(Sizes::BorderButton()), m_BorderPressed(Sizes::BorderButtonPressed()),
            m_Texture(Textures::Button()), m_PressedTexture(Textures::ButtonPressed())
        {
            SetSize({300.0f, 70.0f});
        }

        /// @brief Get the Label object
        /// @return Mutable reference to the label
        inline Label& GetLabel() { return m_Label; }

        float GetVerticalOffset() const { return m_VerticalOffset; }
        float GetVerticalOffsetPressed() const { return m_VerticalOffsetPressed; }

        void Draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_ButtonBox.SetPosition(m_Position);
            m_ButtonBox.SetSize(m_Size);
            if(IsPressed())
                m_ButtonBox.SetTexture(m_PressedTexture, m_BorderPressed);
            else
                m_ButtonBox.SetTexture(m_Texture, m_Border);

            m_Label.SetColor(m_LabelColor);
            m_ButtonBox.SetColor(IsHovered()? m_HoveredBoxColor: m_IdleBoxColor);
            m_Application->Draw(&m_ButtonBox, layer);
            
            auto mod = Vector2f::Y(IsPressed()? m_VerticalOffsetPressed: m_VerticalOffset);
            m_Label.SetPosition((Vector2f)m_ButtonBox.GetPosition() + mod);
            m_Label.Draw();
        }

        void CustomUpdate(float delta_time) override
        {
            m_Label.Update(delta_time);
        }

        Button* SetVerticalOffset(float offset)
        {
            m_VerticalOffset = offset;
            return this;
        }

        Button* SetVerticalOffsetPressed(float offset)
        {
            m_VerticalOffsetPressed = offset;
            return this;
        }

        Button* SetButtonColors(const Color& box_color, const Color& label_color = Color::White)
        {
            m_IdleBoxColor = box_color; m_HoveredBoxColor = box_color * 1.5f;
            m_LabelColor = label_color;
            return this;
        }
        
        /// @brief Checks if the button is hovered by the mouse cursor, must be called after Update()
        /// @return The result of the test
        bool IsHovered() const override 
        {
            return AABB::RectangleToPoint(m_ButtonBox, Mouse::GetPosition());
        }

        Button* SetTitle(const sf::String& title)
        {
            m_Label.SetText(title);
            return this;
        }

        Button* SetFontSize(unsigned int size) 
        {
            m_Label.SetFontSize(size);
            return this;
        }
    private:
        Color m_IdleBoxColor{Color::White}, m_HoveredBoxColor{Color::LightGray}, m_LabelColor{Color::Black};
        float m_VerticalOffset{-6.0f}, m_VerticalOffsetPressed{3.5f};
        sf::Texture* m_Texture, *m_PressedTexture;
        Label m_Label;
        Box m_ButtonBox;
        Box::Border m_Border, m_BorderPressed;
    };
}