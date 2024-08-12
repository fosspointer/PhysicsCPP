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
            :m_label(application, "Button", 25.0f), UIElement(application), m_texture(texture), m_pressedTexture(pressed_texture), 
            m_border(border), m_borderPressed(border_pressed)
        {
            setSize({300.0f, 70.0f});
        }

        /// @brief Construct a new Button object
        /// @param application The main application
        /// @param text The title of the button 
        /// @param size The size of the button
        /// @param margin Margin around the Button, used by layouts
        Button(Application* application)
            :m_label(application, "Button", 25.0f), UIElement(application), 
            m_border(Sizes::borderButton()), m_borderPressed(Sizes::borderButtonPressed()),
            m_texture(Textures::button()), m_pressedTexture(Textures::buttonPressed())
        {
            setSize({300.0f, 70.0f});
        }

        /// @brief Get the Label object
        /// @return Mutable reference to the label
        inline Label& getLabel() { return m_label; }

        float getVerticalOffset() const { return m_verticalOffset; }
        float getVerticalOffsetPressed() const { return m_verticalOffsetPressed; }

        void draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_buttonBox.setPosition(m_position);
            m_buttonBox.setSize(m_size);
            if(isPressed())
                m_buttonBox.setTexture(m_pressedTexture, m_borderPressed);
            else
                m_buttonBox.setTexture(m_texture, m_border);

            m_label.setColor(m_labelColor);
            m_buttonBox.setColor(isHovered()? m_hoveredBoxColor: m_idleBoxColor);
            m_application->draw(&m_buttonBox, layer);
            
            auto mod = Vector2f::Y(isPressed()? m_verticalOffsetPressed: m_verticalOffset);
            m_label.setPosition((Vector2f)m_buttonBox.getPosition() + mod);
            m_label.draw();
        }

        void customUpdate(float delta_time) override
        {
            m_label.update(delta_time);
        }

        Button* setVerticalOffset(float offset)
        {
            m_verticalOffset = offset;
            return this;
        }

        Button* setVerticalOffsetPressed(float offset)
        {
            m_verticalOffsetPressed = offset;
            return this;
        }

        Button* setButtonColors(const Color& box_color, const Color& label_color = Color::White)
        {
            m_idleBoxColor = box_color; m_hoveredBoxColor = box_color * 1.5f;
            m_labelColor = label_color;
            return this;
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
    private:
        Color m_idleBoxColor{Color::White}, m_hoveredBoxColor{Color::LightGray}, m_labelColor{Color::Black};
        float m_verticalOffset{-6.0f}, m_verticalOffsetPressed{3.5f};
        sf::Texture* m_texture, *m_pressedTexture;
        Label m_label;
        Box m_buttonBox;
        Box::Border m_border, m_borderPressed;
    };
}