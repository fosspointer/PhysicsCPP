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

        Dropdown(Application* m_application, const sf::String& title, unsigned int title_font_size, const sf::String& placeholder, const Vector2f& box_size)
            :UIElement(m_application),
            m_expanded(false), m_hovered(false), m_optionColor(physics::Color::LightGray), m_hoverColor(physics::Color::Gray), m_outlineColor(physics::Color::Black),
            m_titleLabel(m_application, title, title_font_size), m_selectionLabel(m_application, placeholder, 25),
           m_selectionIndex(-1), m_direction(Direction::Down), m_boxSize(box_size)
        {
            m_titleLabel.setStyle(sf::Text::Bold);
            updateSize();
            addOption(placeholder);
            setDropdownColors(Color::White);
            addClickCallback([&](Application* application, Dropdown* dropdown, MouseButton)
            {
                if(dropdown->m_hovered)
                {
                    dropdown->m_selectionIndex = dropdown->getHoverIndex();
                    if(dropdown->m_selectionIndex != -1)
                    {
                        dropdown->m_selectionLabel.setText(dropdown->m_options[dropdown->m_selectionIndex].getText());
                        if(dropdown->m_selectionCallbackFunction)
                            dropdown->m_selectionCallbackFunction(application, dropdown, dropdown->m_selectionIndex);
                    }
                }
                dropdown->m_expanded = !dropdown->m_expanded;
            });
        }

        /// @brief Checks if the dropdown is hovered by the mouse cursor, must be called after update()
        /// @return The result of the test
        bool isHovered() const override
        {
            auto mouse = (sf::Vector2f)sf::Mouse::getPosition(m_application->getWindow());
            return AABB::rectangleToPoint(m_headBox, Mouse::getPosition()) 
                || (m_expanded && AABB::rectangleToPoint(m_optionBox, Mouse::getPosition()));
        }

        void draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            if(m_expanded)
            {
                m_application->draw(&m_optionBox, PHYSICS_LAYER_FLOATING_0);

                if(m_hovered)
                    m_application->getRenderer().append(&m_hoverOptionBox, PHYSICS_LAYER_FLOATING_0);
                    
                for(auto& label : m_options)
                    label.draw(PHYSICS_LAYER_FLOATING_0);
            }

            m_titleLabel.draw(layer);

            m_application->draw(&m_headBox, layer);
            m_selectionLabel.draw(layer);
        }

        inline const sf::Color& getOptionColor() const { return m_optionColor; }
        inline const sf::Color& getHoverColor() const { return m_hoverColor; }
        inline const sf::Color& getOutlineColor() const { return m_outlineColor; }
        inline const Direction& getDirection() const { return m_direction; }

        inline Label& getOption(LabelListSize index) { return m_options.at(index); }
        
        Dropdown* setFontSize(unsigned int size) 
        {
            m_selectionLabel.setFontSize(size);
            return this;
        }

        Dropdown* addOption(const sf::String& text)
        {
            m_options.push_back(Label(m_application, text, m_selectionLabel.getFontSize()));
            updateSize();
            return this;
        }

        Dropdown* setOption(LabelListSize index, const sf::String& text)
        {
            m_options[index].setText(text);
            updateText();
            return this;
        }

        Dropdown* setPlaceHolder(const sf::String& text)
        {
            m_options[0].setText(text);
            updateText();
            updateSize();
            return this;
        }

        Dropdown* setDropdownColors(const sf::Color& color)
        {
            m_color = color;
            m_optionColor = sf::Color(color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, color.a);
            m_hoverColor = sf::Color(color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a);
            m_outlineColor = sf::Color(color.r * 0.25f, color.g * 0.25f, color.b * 0.25f, color.a);
            updateColors();
            return this;
        }

        Dropdown* setOutline(unsigned int size)
        {
            m_headBox.setOutlineThickness(size);
            m_optionBox.setOutlineThickness(size);
            return this;
        }

        Dropdown* setSelectionCallback(SelectionCallbackFunc func)
        {
            m_selectionCallbackFunction = func;
            return this;
        }

        Dropdown* setTitle(const sf::String& text)
        {
            m_titleLabel.setText(text);
            updateSize();
            return this;
        }

        Dropdown* setDirection(const Direction& direction)
        {
            m_direction = direction;
            return this;
        }

        int64_t getHoverIndex()
        {
            LabelListSize index = (LabelListSize)getHoverIndexFloat();
            return index < m_options.size() ? index : -1;
        }

        int64_t getSelectionIndex()
        {
            return m_selectionIndex;
        }
    private:
        void customUpdate(float delta_time) override
        {
            m_titleLabel.update(delta_time);
            m_selectionLabel.update(delta_time);
            for(auto& option : m_options)
                option.update(delta_time);
        
            if(AABB::rectangleToPoint(m_headBox, Mouse::getPosition()))
                m_headBox.setFillColor(m_hoverColor);
            else m_headBox.setFillColor(m_color);

            updatePositions();
        }

        void updateSize()
        {
            m_size = Vector2f{std::max(m_boxSize.x, m_titleLabel.getSize().x), m_boxSize.y + m_margin.y + m_titleLabel.getMargin().y + m_titleLabel.getSize().y};
            m_headBox.setSize(sf::Vector2f{m_boxSize.x, m_boxSize.y});
            m_hoverOptionBox.setSize(m_headBox.getSize());
            m_optionBox.setSize(sf::Vector2f{m_boxSize.x, m_boxSize.y * m_options.size()});
        }

        void updatePositions()
        {
            auto mouse = Mouse::getPosition();
            
            switch(m_direction)
            {
            case Direction::Down:
                m_titleLabel.setPosition(sf::Vector2f{m_position.x, m_position.y - m_margin.y / 2.0f - m_titleLabel.getSize().y / 2.0f});
                m_headBox.setPosition(sf::Vector2f{m_position.x - m_headBox.getSize().x / 2.0f, m_position.y + m_titleLabel.getMargin().y / 2.0f});
                m_optionBox.setPosition(m_headBox.getPosition() + sf::Vector2f{0.0f, m_headBox.getSize().y});
                break;
            case Direction::Up:
                m_titleLabel.setPosition(sf::Vector2f{m_position.x, m_position.y + m_margin.y / 2.0f + m_titleLabel.getSize().y / 2.0f});
                m_headBox.setPosition(sf::Vector2f{m_position.x - m_headBox.getSize().x / 2.0f, m_position.y - m_titleLabel.getMargin().y / 2.0f - m_headBox.getSize().y});
                m_optionBox.setPosition(m_headBox.getPosition() - sf::Vector2f{-0.0f, (float)m_options.size() * m_headBox.getSize().y});
                break;
            }

            m_selectionLabel.setPosition(m_headBox.getPosition() + m_headBox.getSize() / 2.0f);

            if(m_expanded)
            {
                for(LabelListSize i = 0; i < m_options.size(); i++)
                    m_options[i].setPosition(sf::Vector2f{m_position.x, m_optionBox.getPosition().y + m_headBox.getSize().y * 0.5f + m_headBox.getSize().y * i});

                if(AABB::rectangleToPoint(m_optionBox, Mouse::getPosition()))
                {
                    m_hovered = true;
                    auto hover_index = getHoverIndexFloat();
                    m_hoverOptionBox.setPosition(sf::Vector2f{m_optionBox.getPosition().x, m_optionBox.getPosition().y + hover_index * m_headBox.getSize().y});
                }
                else m_hovered = false;
            }
            else m_hovered = false;
        }

        void updateText()
        {
            updateSize();
            if(m_selectionIndex != -1)
                m_selectionLabel.setText(m_options[m_selectionIndex].getText());
            else m_selectionLabel.setText(m_options[0].getText());
        }

        void updateColors()
        {
            m_headBox.setFillColor(m_color);
            m_headBox.setOutlineColor(m_outlineColor);
            m_optionBox.setFillColor(m_optionColor);
            m_optionBox.setOutlineColor(m_outlineColor);
            m_hoverOptionBox.setFillColor(m_hoverColor);
        }

        float getHoverIndexFloat() const
        {
            return std::floor((Mouse::getPosition().y - m_optionBox.getPosition().y) / m_headBox.getSize().y);
        }

        SelectionCallbackFunc m_selectionCallbackFunction;
        int64_t m_selectionIndex;
        float m_textPadding;
        bool m_expanded, m_hovered;
        sf::Color m_optionColor, m_hoverColor, m_outlineColor;
        Label m_titleLabel, m_selectionLabel;
        sf::RectangleShape m_headBox, m_optionBox, m_hoverOptionBox;
        LabelList m_options;
        Direction m_direction;
        Vector2f m_boxSize;
    };
}