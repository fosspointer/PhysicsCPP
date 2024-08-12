#pragma once
#include <Physics/UI/UIElement.hpp>

namespace physics
{
    class Slider final : public UIElement<Slider>
    {
    public:
        Slider(Application* application, float min, float max)
            :UIElement(application),
            m_minValue(min), m_maxValue(max), m_currentValue(min), m_label(m_application, toPrecisionString(min), 20u)
        {
            m_selected.setFillColor(Color::DarkGray);
        }

        virtual bool isHovered() const override final
        {
            return AABB::rectangleToPoint(this, Mouse::getPosition())
                || AABB::rectangleToPoint(m_knob, Mouse::getPosition());
        }

        virtual void draw(int8_t layer = PHYSICS_LAYER_UI_1) override final
        {
            m_application->draw(&m_base, layer);
            m_application->draw(&m_selected, layer);
            m_application->draw(&m_knob, layer);
            m_label.draw(layer);
        }

        virtual void customUpdate(float delta_time) override final
        {
            if(interacted())
                updatePercentage();
            updateSize();
            updatePositions();
        }

        Slider* setSliderColors(const Color& color)
        {
            m_color = color;
            m_knobColor = Color::fromRGB(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a);
            m_unselectedColor = Color::fromRGB(color.r * 0.25f, color.g * 0.25f, color.b * 0.25f, color.a);
            m_outlineColor = Color::fromRGB(color.r * 0.125f, color.g * 0.125f, color.b * 0.125f, color.a);
            m_label.setColor(Color::Black);
            updateColors();
            return this;
        }

        Slider* setOutlineThickness(float thickness)
        {
            m_base.setOutlineThickness(thickness);
            return this;
        }

        Slider* setSliderSize(const sf::Vector2f& size, float label_font_size, float control_knob_width)
        {
            m_label.setFontSize(label_font_size);
            m_size = Vector2f{size.x, size.y + m_label.getSize().y + 20.0f};
            m_knobWidth = control_knob_width;
            return this;
        }

        Slider* setValue(float value) 
        { 
            updatePercentage(value);
            return this;    
        }

        inline const float& getValue() const { return m_currentValue; }
    private:
        template <typename T>
        static std::string toPrecisionString(const T value, const int precision = 2)
        {
            std::ostringstream out;
            out.precision(precision);
            out << std::fixed << value;
            return std::move(out).str();
        }

        void updateSize()
        {
            m_base.setSize(sf::Vector2f{m_size.x, m_size.y - m_label.getSize().y - 20.0f});
            m_selected.setSize({m_selected.getSize().x, m_base.getSize().y});
            m_knob.setSize(sf::Vector2f{m_knobWidth, m_base.getSize().y * 1.5f});
        }

        void updatePositions()
        {
            m_base.setPosition(m_position - m_size / 2.0f);
            m_selected.setPosition(m_position - m_size / 2.0f);
            m_knob.setPosition(m_base.getPosition() + sf::Vector2f{m_selected.getSize().x, m_selected.getSize().y / 2.0f});
            m_knob.setOrigin(m_knob.getSize() / 2.0f);
            m_label.setPosition(sf::Vector2f{m_position.x, m_position.y + m_base.getSize().y / 2.0f});
        }

        bool interacted()
        {
            auto& mouse = Mouse::getInstance();

            if(isHovered() && !mouse.previousState && mouse.currentState)
                m_interaction = true;
            else if(!mouse.currentState) 
                m_interaction = false;
            return m_interaction;
        }

        void updatePercentage()
        {   
            float size = std::clamp(Mouse::getPosition().x - m_base.getPosition().x, 0.0f, m_base.getSize().x);
            m_currentValue = (size / m_base.getSize().x) * (m_maxValue - m_minValue) + m_minValue;
            
            m_selected.setSize(sf::Vector2f{
                size,
                20.0f});
            m_label.setText(toPrecisionString(m_currentValue));
        }

        void updatePercentage(float value)
        {
            float size = (value - m_minValue) / (m_maxValue - m_minValue) * m_size.x;
            m_selected.setSize(sf::Vector2f{
                size,
                20.0f});
            m_currentValue = value;
            m_label.setText(toPrecisionString(m_currentValue));
        }

        void updateColors()
        {
            m_base.setFillColor(m_unselectedColor);
            m_base.setOutlineColor(m_outlineColor);
            m_knob.setFillColor(m_knobColor);
            m_selected.setFillColor(m_color); 
        }

        bool m_interaction{false};
        Color m_knobColor, m_outlineColor, m_unselectedColor;
        float m_minValue, m_maxValue, m_currentValue, m_knobWidth;
        sf::RectangleShape m_base, m_selected, m_knob;
        Label m_label;
    };
}