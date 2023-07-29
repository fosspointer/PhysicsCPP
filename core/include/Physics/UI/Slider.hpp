#pragma once
#include <Physics/UI/UIElement.hpp>

namespace physics
{

    template <typename T>
    static std::string to_string(const T value, const int precision = 2)
    {
        std::ostringstream out;
        out.precision(precision);
        out << std::fixed << value;
        return std::move(out).str();
    }

    class Slider : public UIElement<Slider>
    {
    public:
        Slider(Application* application, float min, float max, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f})
            :UIElement(application, sf::Vector2f{150.0f, 20.0f}, margin), 
            m_MinValue(min), m_MaxValue(max), m_CurrentValue(min), m_Label(m_Application, to_string(min), 20u)
        {
            m_SelectedRectangle.setFillColor(Color::DarkGray);
        }

        virtual bool IsHovered() const override
        {
            return AABB::RectangleToPoint(this, Mouse::GetPosition());
        }

        virtual void Draw() override
        {
            m_Application->GetWindow().draw(m_Rectangle);
            m_Application->GetWindow().draw(m_SelectedRectangle);
            m_Application->GetWindow().draw(m_Knob);
            m_Label.Draw();
        }

        void CustomUpdate(float delta_time) override
        {
            if(IsPressed())
                UpdatePercentage();
            UpdateSize();
            UpdatePositions();
        }

        Slider* SetSliderColors(const sf::Color& color)
        {
            m_Color = color;
            m_KnobColor = sf::Color(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a);
            m_UnselectedColor = sf::Color(color.r * 0.25f, color.g * 0.25f, color.b * 0.25f, color.a);
            m_OutlineColor = sf::Color(color.r * 0.125f, color.g * 0.125f, color.b * 0.125f, color.a);
            m_Label.SetColor(Color::Black);
            UpdateColors();
            return this;
        }

        Slider* SetOutlineThickness(float thickness)
        {
            m_Rectangle.setOutlineThickness(thickness);
            return this;
        }

        Slider* SetSliderSize(const sf::Vector2f& size, float label_font_size, float control_knob_width)
        {
            m_Label.SetFontSize(label_font_size);
            m_Size = sf::Vector2f{size.x, size.y + m_Label.GetSize().y + 20.0f};
            m_KnobWidth = control_knob_width;
            return this;
        }

        void SetValue(float value) { m_CurrentValue = value; }

        inline const float& GetValue() const { return m_CurrentValue; }
    private:
        void UpdateSize()
        {
            m_Rectangle.setSize(sf::Vector2f{m_Size.x, m_Size.y - m_Label.GetSize().y - 20.0f});
            m_SelectedRectangle.setSize({m_SelectedRectangle.getSize().x, m_Rectangle.getSize().y});
            m_Knob.setSize(sf::Vector2f{m_KnobWidth, m_Rectangle.getSize().y * 1.5f});
        }

        void UpdatePositions()
        {
            m_Rectangle.setPosition(m_Position - m_Size / 2.0f);
            m_SelectedRectangle.setPosition(m_Position - m_Size / 2.0f);
            m_Knob.setPosition(m_Rectangle.getPosition() + sf::Vector2f{m_SelectedRectangle.getSize().x, m_SelectedRectangle.getSize().y / 2.0f});
            m_Knob.setOrigin(m_Knob.getSize() / 2.0f);
            m_Label.SetPosition(sf::Vector2f{m_Position.x, m_Position.y + m_Rectangle.getSize().y / 2.0f});
        }

        void UpdatePercentage()
        {   
            float size = std::clamp(Mouse::GetPosition().x - m_Rectangle.getPosition().x, 0.0f, m_Rectangle.getSize().x);
            size = size < m_Rectangle.getSize().x * 0x1p-5f ? 0.0f : 
                size > m_Rectangle.getSize().x * (1.0f - 0x1p-5f) ? m_Rectangle.getSize().x : size;
            m_SelectedRectangle.setSize(sf::Vector2f{
                size,
                20.0f});
            m_CurrentValue = (m_SelectedRectangle.getSize().x / m_Rectangle.getSize().x) * (m_MaxValue - m_MinValue) + m_MinValue;
            m_Label.SetText(to_string(m_CurrentValue));
        }

        void UpdateColors()
        {
            m_Rectangle.setFillColor(m_UnselectedColor);
            m_Rectangle.setOutlineColor(m_OutlineColor);
            m_Knob.setFillColor(m_KnobColor);
            m_SelectedRectangle.setFillColor(m_Color); 
        }

        sf::Color m_KnobColor, m_OutlineColor, m_UnselectedColor;
        float m_MinValue, m_MaxValue, m_CurrentValue, m_KnobWidth;
        sf::RectangleShape m_Rectangle, m_SelectedRectangle, m_Knob;
        Label m_Label;
    };
}