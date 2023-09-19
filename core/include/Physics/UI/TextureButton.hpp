#pragma once
#include <Physics/System/Color.hpp>
#include <Physics/System/Box.hpp>
#include <Physics/UI/UIElement.hpp>
#include <Physics/UI/Label.hpp>

namespace physics
{
    
    /// @brief Basic button element, suitable for most cases
    class TextureButton : public UIElement<TextureButton>
    {
    public:
        /// @brief Construct a new TextureButton object
        /// @param application The main application
        /// @param texture The texture for when the button is idle
        /// @param pressed_texture The texture for when the button is pressed
        /// @param text The title of the button 
        /// @param border_size The cutoff of the main texture
        /// @param border_size_pressed The cutoff of the pressed texture
        /// @param size The size of the button
        /// @param margin Margin around the Button, used by layouts
        TextureButton(Application* application, sf::Texture* texture, sf::Texture* pressed_texture, const sf::String& text, const sf::Vector2f& border_size, const sf::Vector2f& border_size_pressed, const sf::Vector2f& size, const sf::Vector2f margin = sf::Vector2f{25.0f, 25.0f})
            :m_Label(application, text, 25.0f, sf::Vector2f{0.0f, 0.0f}), UIElement(application, size, margin), m_BorderSize(border_size), m_BorderSizePressed(border_size_pressed),
            m_Texture(texture), m_PressedTexture(pressed_texture)
        {
            m_Color = physics::Color::Blue;
            m_Label.SetColor(physics::Color::White);
        }

        /// @brief Get the Label object
        /// @return Mutable reference to the label
        inline Label& GetLabel() { return m_Label; }

        void Draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_ButtonBox.SetPosition(m_Position);
            m_ButtonBox.SetSize(m_Size);
            if(IsPressed())
                m_ButtonBox.SetTexture(m_PressedTexture, m_BorderSizePressed);
            else
                m_ButtonBox.SetTexture(m_Texture, m_BorderSize);

            m_ButtonBox.SetColor(m_Color);
            m_Application->Draw(&m_ButtonBox, layer);
            
            m_Label.SetPosition(m_Position);
            m_Label.Draw();
        }

        void CustomUpdate(float delta_time) override
        {
            m_Label.Update(delta_time);
        }
        
        /// @brief Checks if the button is hovered by the mouse cursor, must be called after Update()
        /// @return The result of the test
        bool IsHovered() const override 
        {
            return AABB::RectangleToPoint(m_ButtonBox, Mouse::GetPosition());
        }
    private:
        sf::Texture* m_Texture, *m_PressedTexture;
        Vector2f m_BorderSize, m_BorderSizePressed;
        Label m_Label;
        Box m_ButtonBox;
    };
}