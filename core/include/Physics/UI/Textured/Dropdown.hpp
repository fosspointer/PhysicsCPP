#pragma once
#include <cmath>
#include <cstdlib>
#include <vector>
#include <Physics/System/Color.hpp>
#include <Physics/System/Application.hpp>
#include <Physics/System/Box.hpp>
#include <Physics/System/Sizes.hpp>
#include <Physics/UI/UIElement.hpp>
#include <Physics/UI/Label.hpp>

namespace physics::Textured
{
    class Dropdown : public UIElement<Dropdown>
    {
    public:
        struct Option
        {
            Label label;
            Box box;
        };
        
        using SelectionCallbackFunc = std::function<void(Application*, Dropdown*, int64_t)>;
        using OptionList = std::vector<Option>;
        using OptionListSize = OptionList::size_type;

        Dropdown(Application* application)
            :UIElement(application, true), m_titleLabel(application), m_selectionLabel(application),
            m_head(Textures::dropdownHead(), Sizes::borderDropdownHead()),
            m_textureOptionLast(Textures::dropdownOptionLast()), m_borderOptionLast(Sizes::borderDropdownOptionLast()),
            m_textureHead(Textures::dropdownHead()), m_textureHeadExpanded(Textures::dropdownHeadExpanded()),
            m_textureOption(Textures::dropdownOption()), m_borderOption(Sizes::borderDropdownOption())
        {
            m_selectionLabel.setColor(Color::White);
            m_titleLabel.setStyle(sf::Text::Bold);
            addOption("Placeholder");
            addClickCallback([&](Application* application, Dropdown* dropdown, MouseButton){
                if(dropdown->isHovered())
                {
                    dropdown->m_selectionIndex = dropdown->getHoverIndex();
                    if(dropdown->m_selectionIndex != -1)
                    {
                        dropdown->m_selectionLabel.setText(dropdown->m_options[dropdown->m_selectionIndex].label.getText());
                        if(dropdown->m_selectionCallbackFunction)
                            dropdown->m_selectionCallbackFunction(application, dropdown, dropdown->m_selectionIndex);
                    }
                }    
                dropdown->m_expanded = !dropdown->m_expanded;
            });
        }

        bool isHovered() const override
        {
            auto mouse = (sf::Vector2f)sf::Mouse::getPosition(m_application->getWindow());
            return AABB::rectangleToPoint(m_head, Mouse::getPosition())
                || (m_expanded && AABB::rectangleToPoint(getOptionAABB(), Mouse::getPosition()));
        }

        void draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_titleLabel.draw(layer);
            m_application->draw(&m_head, layer);
            m_selectionLabel.draw(layer);

            if(!m_expanded) return;
            
            for(auto& option: m_options)
            {
                m_application->draw(&option.box, PHYSICS_LAYER_FLOATING_0);
                option.label.draw(PHYSICS_LAYER_FLOATING_0);
            }
        }

        inline Option& getOption(OptionListSize index) { return m_options.at(index); }
        inline Label& getOptionLabel(OptionListSize index) { return m_options.at(index).label; }
        inline Box& getOptionBox(OptionListSize index) { return m_options.at(index).box; }

        Dropdown* setDropdownColors(const Color& box_color, const Color& label_color = Color::White)
        {
            m_idleBoxColor = box_color; m_hoveredBoxColor = box_color * 2.0f;
            m_idleLabelColor = label_color * 0.75f; m_hoveredLabelColor = label_color;
            return this;
        }

        Dropdown* setFontSize(unsigned int size)
        {
            m_selectionLabel.setFontSize(size);
            return this;
        }

        Dropdown* addOption(const sf::String& text)
        {
            m_options.push_back(Option{Label(m_application, text, m_selectionLabel.getFontSize()), Box(m_textureOption)});
            m_options[m_options.size() - 1].box.setBorder(m_borderOption);
            updateSize();
            return this;
        }

        Dropdown* setOption(OptionListSize index, const sf::String& text)
        {
            m_options[index].label.setText(text);
            updateText();
            return this;
        }

        Dropdown* setPlaceHolder(const sf::String& text)
        {
            m_options[0].label.setText(text);
            updateText();
            updateSize();
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

        Dropdown* setBoxSize(const Vector2f& size)
        {
            m_boxSize = size;
            return this;
        }

        int64_t getHoverIndex()
        {
            OptionListSize index = (OptionListSize)getHoverIndexFloat();
            return index < m_options.size() ? index : -1;
        }

        int64_t getSelectionIndex()
        {
            return m_selectionIndex;
        }
    private:
        Vector2f getOptionSize() const
        {
            return sf::Vector2f{m_boxSize.x, m_boxSize.y - m_removedBesel};
        }

        AABB getOptionAABB() const
        {
            return AABB{sf::FloatRect{m_head.getPosition() + sf::Vector2f{-m_head.getSize().x / 2.0f, m_head.getSize().y / 2.0f}, sf::Vector2f{getOptionSize().x, getOptionSize().y * m_options.size()}}};
        }

        void customUpdate(float delta_time) override
        {
            if(m_expanded)
                m_application->setFloating();
        
            m_titleLabel.update(delta_time);
            m_selectionLabel.update(delta_time);
            
            for(auto& option : m_options)
                option.label.update(delta_time);
            
            m_head.setTexture(m_expanded? m_textureHeadExpanded: m_textureHead);
            
            updateOptions();
            updatePositions();
            updateColors();
        }

        void updateOptions()
        {
            for(OptionListSize i = 0; i < m_options.size(); i++)
            {
                if(i == m_options.size() - 1)
                {
                    m_options[i].box.setTexture(m_textureOptionLast);
                    m_options[i].box.setBorder(m_borderOptionLast);
                    m_options[i].box.setSize(getOptionSize() + sf::Vector2f{0.0f, 6.0f});
                    m_options[i].box.setPosition(m_options[i - 1].box.getPosition() + sf::Vector2f{0.0f, getOptionSize().y / 2.0f + m_options[i].box.getSize().y / 2.0f});
                }
                else 
                {
                    m_options[i].box.setTexture(m_textureOption);
                    m_options[i].box.setBorder(m_borderOption);
                    m_options[i].box.setSize(getOptionSize());
                    m_options[i].box.setPosition(m_head.getPosition() + sf::Vector2f{0.0f, (m_head.getSize().y + getOptionSize().y) / 2.0f + getOptionSize().y * i});
                }
            }
        }

        void updateSize()
        {
            auto options = m_options.size();
            m_size = Vector2f{std::max(m_boxSize.x, m_titleLabel.getSize().x), m_boxSize.y + m_margin.y + m_titleLabel.getMargin().y + m_titleLabel.getSize().y};
            
            m_head.setSize(m_boxSize);
        }

        void updatePositions()
        {
            auto mouse = Mouse::getPosition();
            auto last_option_box = m_options[m_options.size() - 1].box;

            m_titleLabel.setPosition(sf::Vector2f{m_position.x, m_position.y - (m_margin.y - m_titleLabel.getSize().y) / 2.0f});
            m_head.setPosition(sf::Vector2f{m_position.x, m_position.y + (m_head.getSize().y + m_titleLabel.getMargin().y) / 2.0f});

            auto mod = Vector2f{-m_head.getBorderEnd().x / 2.0f, -6.0f};
            m_selectionLabel.setPosition((Vector2f)m_head.getPosition() + mod);
            
            if(m_expanded)
                for(OptionListSize i = 0; i < m_options.size(); i++)
                    m_options[i].label.setPosition(m_options[i].box.getPosition());
        }

        void updateText()
        {
            updateSize();
            if(m_selectionIndex != -1)
                m_selectionLabel.setText(m_options[m_selectionIndex].label.getText());
            else m_selectionLabel.setText(m_options[0].label.getText());
        }

        void updateColors()
        {
            m_head.setColor(AABB::rectangleToPoint(m_head, Mouse::getPosition())? m_hoveredBoxColor * 0.75f: m_idleBoxColor);

            auto index = getHoverIndex();
            if(!isHovered() && !Mouse::getInstance().clickState) return;

            for(OptionListSize i = 0; i < m_options.size(); i++)
            {
                if(i == index)
                {
                    m_options[i].box.setColor(m_hoveredBoxColor);
                    m_options[i].label.setColor(m_hoveredLabelColor);
                }
                else
                {
                    m_options[i].box.setColor(m_idleBoxColor);
                    m_options[i].label.setColor(m_idleLabelColor);
                } 
            }
        }

        float getHoverIndexFloat() const
        {
            return std::floor((Mouse::getPosition().y - m_head.getPosition().y - m_head.getSize().y / 2.0f) / getOptionSize().y);
        }

        Box m_head;
        Label m_titleLabel, m_selectionLabel;
        OptionList m_options;
        
        Box::Border m_borderOption, m_borderOptionLast;
        sf::Texture* m_textureHead, *m_textureHeadExpanded, *m_textureOption, *m_textureOptionLast;
        SelectionCallbackFunc m_selectionCallbackFunction;

        Color m_idleBoxColor, m_hoveredBoxColor, m_idleLabelColor, m_hoveredLabelColor;

        float m_removedBesel{24.0f};
        int64_t m_selectionIndex{-1};
        bool m_expanded{false};
        
        Vector2f m_boxSize{300.0f, 70.0f};
    };
}