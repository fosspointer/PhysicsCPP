#pragma once
#include <cmath>
#include <cstdlib>
#include <vector>
#include <Physics/System/Color.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/UI/UIElement.hpp>
#include <Physics/UI/Label.hpp>

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

        Dropdown(Application* m_Application, const sf::String& title, unsigned int title_font_size, const sf::String& placeholder, const Vector2f& box_size)
            :UIElement(m_Application),
            m_Expanded(false), m_Hovered(false), m_OptionColor(physics::Color::LightGray), m_HoverColor(physics::Color::Gray), m_OutlineColor(physics::Color::Black),
            m_TitleLabel(m_Application, title, title_font_size), m_SelectionLabel(m_Application, placeholder, 25),
           m_SelectionIndex(-1), m_Direction(Direction::Down), m_BoxSize(box_size)
        {
            m_TitleLabel.SetStyle(sf::Text::Bold);
            UpdateSize();
            AddOption(placeholder);
            SetDropdownColors(Color::White);
            AddClickCallback([&](Application* application, Dropdown* dropdown, MouseButton)
            {
                if(dropdown->m_Hovered)
                {
                    dropdown->m_SelectionIndex = dropdown->GetHoverIndex();
                    if(dropdown->m_SelectionIndex != -1)
                    {
                        dropdown->m_SelectionLabel.SetText(dropdown->m_Options[dropdown->m_SelectionIndex].GetText());
                        if(dropdown->m_SelectionCallbackFunction)
                            dropdown->m_SelectionCallbackFunction(application, dropdown, dropdown->m_SelectionIndex);
                    }
                }
                dropdown->m_Expanded = !dropdown->m_Expanded;
            });
        }

        /// @brief Checks if the dropdown is hovered by the mouse cursor, must be called after Update()
        /// @return The result of the test
        bool IsHovered() const override
        {
            auto mouse = (sf::Vector2f)sf::Mouse::getPosition(m_Application->GetWindow());
            return AABB::RectangleToPoint(m_HeadBox, Mouse::GetPosition()) 
                || (m_Expanded && AABB::RectangleToPoint(m_OptionBox, Mouse::GetPosition()));
        }

        void Draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            if(m_Expanded)
            {
                m_Application->Draw(&m_OptionBox, PHYSICS_LAYER_FLOATING_0);

                if(m_Hovered)
                    m_Application->GetRenderer().Append(&m_HoverOptionBox, PHYSICS_LAYER_FLOATING_0);
                    
                for(auto& label : m_Options)
                    label.Draw(PHYSICS_LAYER_FLOATING_0);
            }

            m_TitleLabel.Draw(layer);

            m_Application->Draw(&m_HeadBox, layer);
            m_SelectionLabel.Draw(layer);
        }

        inline const sf::Color& GetOptionColor() const { return m_OptionColor; }
        inline const sf::Color& GetHoverColor() const { return m_HoverColor; }
        inline const sf::Color& GetOutlineColor() const { return m_OutlineColor; }
        inline const Direction& GetDirection() const { return m_Direction; }

        inline Label& GetOption(LabelListSize index) { return m_Options.at(index); }
        
        Dropdown* SetFontSize(unsigned int size) 
        {
            m_SelectionLabel.SetFontSize(size);
            return this;
        }

        Dropdown* AddOption(const sf::String& text)
        {
            m_Options.push_back(Label(m_Application, text, m_SelectionLabel.GetFontSize()));
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
            m_HeadBox.setOutlineThickness(size);
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
        
            if(AABB::RectangleToPoint(m_HeadBox, Mouse::GetPosition()))
                m_HeadBox.setFillColor(m_HoverColor);
            else m_HeadBox.setFillColor(m_Color);

            UpdatePositions();
        }

        void UpdateSize()
        {
            m_Size = Vector2f{std::max(m_BoxSize.x, m_TitleLabel.GetSize().x), m_BoxSize.y + m_Margin.y + m_TitleLabel.GetMargin().y + m_TitleLabel.GetSize().y};
            m_HeadBox.setSize(sf::Vector2f{m_BoxSize.x, m_BoxSize.y});
            m_HoverOptionBox.setSize(m_HeadBox.getSize());
            m_OptionBox.setSize(sf::Vector2f{m_BoxSize.x, m_BoxSize.y * m_Options.size()});
        }

        void UpdatePositions()
        {
            auto mouse = Mouse::GetPosition();
            
            switch(m_Direction)
            {
            case Direction::Down:
                m_TitleLabel.SetPosition(sf::Vector2f{m_Position.x, m_Position.y - m_Margin.y / 2.0f - m_TitleLabel.GetSize().y / 2.0f});
                m_HeadBox.setPosition(sf::Vector2f{m_Position.x - m_HeadBox.getSize().x / 2.0f, m_Position.y + m_TitleLabel.GetMargin().y / 2.0f});
                m_OptionBox.setPosition(m_HeadBox.getPosition() + sf::Vector2f{0.0f, m_HeadBox.getSize().y});
                break;
            case Direction::Up:
                m_TitleLabel.SetPosition(sf::Vector2f{m_Position.x, m_Position.y + m_Margin.y / 2.0f + m_TitleLabel.GetSize().y / 2.0f});
                m_HeadBox.setPosition(sf::Vector2f{m_Position.x - m_HeadBox.getSize().x / 2.0f, m_Position.y - m_TitleLabel.GetMargin().y / 2.0f - m_HeadBox.getSize().y});
                m_OptionBox.setPosition(m_HeadBox.getPosition() - sf::Vector2f{-0.0f, (float)m_Options.size() * m_HeadBox.getSize().y});
                break;
            }

            m_SelectionLabel.SetPosition(m_HeadBox.getPosition() + m_HeadBox.getSize() / 2.0f);

            if(m_Expanded)
            {
                for(LabelListSize i = 0; i < m_Options.size(); i++)
                    m_Options[i].SetPosition(sf::Vector2f{m_Position.x, m_OptionBox.getPosition().y + m_HeadBox.getSize().y * 0.5f + m_HeadBox.getSize().y * i});

                if(AABB::RectangleToPoint(m_OptionBox, Mouse::GetPosition()))
                {
                    m_Hovered = true;
                    auto hover_index = GetHoverIndexFloat();
                    m_HoverOptionBox.setPosition(sf::Vector2f{m_OptionBox.getPosition().x, m_OptionBox.getPosition().y + hover_index * m_HeadBox.getSize().y});
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
            m_HeadBox.setFillColor(m_Color);
            m_HeadBox.setOutlineColor(m_OutlineColor);
            m_OptionBox.setFillColor(m_OptionColor);
            m_OptionBox.setOutlineColor(m_OutlineColor);
            m_HoverOptionBox.setFillColor(m_HoverColor);
        }

        float GetHoverIndexFloat() const
        {
            return std::floor((Mouse::GetPosition().y - m_OptionBox.getPosition().y) / m_HeadBox.getSize().y);
        }

        SelectionCallbackFunc m_SelectionCallbackFunction;
        int64_t m_SelectionIndex;
        float m_TextPadding;
        bool m_Expanded, m_Hovered;
        sf::Color m_OptionColor, m_HoverColor, m_OutlineColor;
        Label m_TitleLabel, m_SelectionLabel;
        sf::RectangleShape m_HeadBox, m_OptionBox, m_HoverOptionBox;
        LabelList m_Options;
        Direction m_Direction;
        Vector2f m_BoxSize;
    };
}