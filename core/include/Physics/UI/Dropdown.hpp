#pragma once
#include <Physics/UI/UIElement.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/UI/Label.hpp>
#include <Physics/System/Color.hpp>
#include <cmath>
#include <cstdlib>
#include <vector>

namespace physics
{
    using SelectionCallbackFunc = std::function<void(Application*, class Dropdown*, int64_t)>;
    using LabelList = std::vector<Label>;
    using LabelListSize = LabelList::size_type;

    class Dropdown : public UIElement<Dropdown>
    {
    public:
        enum class Direction
        {
            Down, Up
        };

        Dropdown(Application* m_Application, const sf::String& title, unsigned int title_font_size, const sf::String& placeholder, float width, unsigned int font_size, float text_padding = 5.0f, const sf::Vector2f& margin = sf::Vector2f{25.0f, 25.0f})
            :UIElement(m_Application, sf::Vector2f{width, -1.0f}, margin, physics::Color::White),
            m_Expanded(false), m_Hovered(false), m_OptionColor(physics::Color::LightGray), m_HoverColor(physics::Color::Gray), m_OutlineColor(physics::Color::Black),
            m_TitleLabel(m_Application, title, title_font_size, sf::Vector2f{0.0f, 10.0f}), m_SelectionLabel(m_Application, placeholder, font_size, sf::Vector2f{0.0f, 0.0f}),
            m_TextPadding(text_padding), m_SelectionIndex(-1), m_Direction(Direction::Down)
        {
            m_TitleLabel.SetStyle(sf::Text::Bold);
            UpdateSize();
            AddOption(placeholder);
            SetDropdownColors(Color::White);
            AddClickCallback([&](Application*, Dropdown*, MouseButton)
            {
                if(m_Hovered)
                {
                    m_SelectionIndex = GetHoverIndex();
                    if(m_SelectionIndex != -1)
                    {
                        m_SelectionLabel.SetText(m_Options[m_SelectionIndex].GetText());
                        if(m_SelectionCallbackFunction)
                            m_SelectionCallbackFunction(m_Application, this, m_SelectionIndex);
                    }
                }
                m_Expanded = !m_Expanded;
            });
        }

        /**
         * @brief Checks if the dropdown is hovered by the mouse cursor, must be called after Update()
         * @return The result of the test
         */
        bool IsHovered() const override
        {
            auto mouse = (sf::Vector2f)sf::Mouse::getPosition(m_Application->GetWindow());
            return AABB::RectangleToPoint(m_SelectionBox, Mouse::GetPosition()) 
                || (m_Expanded && AABB::RectangleToPoint(m_OptionBox, Mouse::GetPosition()));
        }

        void Draw() override
        {
            if(m_Expanded)
            {
                m_Application->GetWindow().draw(m_OptionBox);

                if(m_Hovered)
                    m_Application->GetWindow().draw(m_HoverOptionBox);

                for(auto& label : m_Options)
                    label.Draw();
            }

            m_TitleLabel.Draw();

            m_Application->GetWindow().draw(m_SelectionBox);
            m_SelectionLabel.Draw();
        }

        inline const sf::Color& GetOptionColor() const { return m_OptionColor; }
        inline const sf::Color& GetHoverColor() const { return m_HoverColor; }
        inline const sf::Color& GetOutlineColor() const { return m_OutlineColor; }
        inline const Direction& GetDirection() const { return m_Direction; }

        inline Label& GetOption(LabelListSize index) { return m_Options.at(index); }
        
        Dropdown* AddOption(const sf::String& text)
        {
            m_Options.push_back(physics::Label(m_Application, text, m_SelectionLabel.GetFontSize()));
            UpdateSize();
            return this;
        }

        Dropdown* SetOption(LabelListSize index, const sf::String& text)
        {
            m_Options[index].SetText(text);
            UpdateText();
            return this;
        }

        Dropdown* SetPlaceHolder(const sf::String& text)
        {
            m_Options[0].SetText(text);
            UpdateText();
            UpdateSize();
            return this;
        }

        Dropdown* SetDropdownColors(const sf::Color& color)
        {
            m_Color = color;
            m_OptionColor = sf::Color(color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, color.a);
            m_HoverColor = sf::Color(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a);
            m_OutlineColor = sf::Color(color.r * 0.25f, color.g * 0.25f, color.b * 0.25f, color.a);
            UpdateColors();
            return this;
        }

        Dropdown* SetOutline(unsigned int size)
        {
            m_SelectionBox.setOutlineThickness(size);
            m_OptionBox.setOutlineThickness(size);
            return this;
        }

        Dropdown* SetSelectionCallback(SelectionCallbackFunc func)
        {
            m_SelectionCallbackFunction = func;
            return this;
        }

        Dropdown* SetTitle(const sf::String& text)
        {
            m_TitleLabel.SetText(text);
            UpdateSize();
            return this;
        }

        Dropdown* SetDirection(const Direction& direction)
        {
            m_Direction = direction;
            return this;
        }

        int64_t GetHoverIndex()
        {
            LabelListSize index = (LabelListSize)GetHoverIndexFloat();
            return index < m_Options.size() ? index : -1;
        }

        int64_t GetSelectionIndex()
        {
            return m_SelectionIndex;
        }
    private:
        void CustomUpdate(float delta_time) override
        {
            m_TitleLabel.Update(delta_time);
            m_SelectionLabel.Update(delta_time);
            for(auto& option : m_Options)
                option.Update(delta_time);
        
            if(AABB::RectangleToPoint(m_SelectionBox, Mouse::GetPosition()))
                m_SelectionBox.setFillColor(m_HoverColor);
            else m_SelectionBox.setFillColor(m_Color);

            UpdatePositions();
        }

        void UpdateSize()
        {
            m_Size.y = m_SelectionLabel.GetSize().y + m_TitleLabel.GetMargin().y + m_TitleLabel.GetSize().y;
            m_SelectionBox.setSize(sf::Vector2f{m_Size.x, m_SelectionLabel.GetSize().y + 2.0f * m_TextPadding});
            m_HoverOptionBox.setSize(m_SelectionBox.getSize());
            m_OptionBox.setSize(sf::Vector2f{m_Size.x, m_SelectionBox.getSize().y * m_Options.size()});
        }

        void UpdatePositions()
        {
            auto mouse = (sf::Vector2f)sf::Mouse::getPosition(m_Application->GetWindow());
            
            m_SelectionLabel.SetPosition(m_SelectionBox.getPosition() + m_SelectionBox.getSize() / 2.0f);
            
            switch(m_Direction)
            {
            case Direction::Down:
                m_TitleLabel.SetPosition(sf::Vector2f{m_Position.x, m_Position.y - m_TitleLabel.GetMargin().y / 2.0f - m_TitleLabel.GetSize().y / 2.0f});
                m_SelectionBox.setPosition(sf::Vector2f{m_Position.x - m_SelectionBox.getSize().x / 2.0f, m_Position.y + m_TitleLabel.GetMargin().y / 2.0f});
                m_OptionBox.setPosition(m_SelectionBox.getPosition() + sf::Vector2f{0.0f, m_SelectionBox.getSize().y});
                break;
            case Direction::Up:
                m_TitleLabel.SetPosition(sf::Vector2f{m_Position.x, m_Position.y + m_TitleLabel.GetMargin().y / 2.0f + m_TitleLabel.GetSize().y / 2.0f});
                m_SelectionBox.setPosition(sf::Vector2f{m_Position.x - m_SelectionBox.getSize().x / 2.0f, m_Position.y - m_TitleLabel.GetMargin().y / 2.0f - m_SelectionBox.getSize().y});
                m_OptionBox.setPosition(m_SelectionBox.getPosition() - sf::Vector2f{-0.0f, (float)m_Options.size() * m_SelectionBox.getSize().y});
                break;
            }

            if(m_Expanded)
            {
                for(LabelListSize i = 0; i < m_Options.size(); i++)
                    switch(m_Direction)
                    {
                    case Direction::Down:
                        m_Options[i].SetPosition(sf::Vector2f{m_Position.x, m_OptionBox.getPosition().y + m_SelectionBox.getSize().y * 0.5f + m_SelectionBox.getSize().y * i});
                        break;
                    case Direction::Up:
                        m_Options[i].SetPosition(sf::Vector2f{m_Position.x, m_OptionBox.getPosition().y + m_SelectionBox.getSize().y * 0.5f + m_SelectionBox.getSize().y * i});
                        break;
                    }

                if(AABB::RectangleToPoint(m_OptionBox, Mouse::GetPosition()))
                {
                    m_Hovered = true;
                    auto hover_index = GetHoverIndexFloat();
                    m_HoverOptionBox.setPosition(sf::Vector2f{m_Position.x - m_Size.x / 2.0f, m_OptionBox.getPosition().y + hover_index * m_SelectionBox.getSize().y});
                }
                else m_Hovered = false;
            }
            else m_Hovered = false;
        }

        void UpdateText()
        {
            UpdateSize();
            if(m_SelectionIndex != -1)
                m_SelectionLabel.SetText(m_Options[m_SelectionIndex].GetText());
            else m_SelectionLabel.SetText(m_Options[0].GetText());
        }

        void UpdateColors()
        {
            m_SelectionBox.setFillColor(m_Color);
            m_SelectionBox.setOutlineColor(m_OutlineColor);
            m_OptionBox.setFillColor(m_OptionColor);
            m_OptionBox.setOutlineColor(m_OutlineColor);
            m_HoverOptionBox.setFillColor(m_HoverColor);
        }

        float GetHoverIndexFloat() const
        {
            return std::floor((Mouse::GetPosition().y - m_OptionBox.getPosition().y) / m_SelectionBox.getSize().y);
        }

        SelectionCallbackFunc m_SelectionCallbackFunction;
        int64_t m_SelectionIndex;
        float m_TextPadding;
        bool m_Expanded, m_Hovered;
        sf::Color m_OptionColor, m_HoverColor, m_OutlineColor;
        Label m_TitleLabel, m_SelectionLabel;
        sf::RectangleShape m_SelectionBox, m_OptionBox, m_HoverOptionBox;
        LabelList m_Options;
        Direction m_Direction;
    };
}