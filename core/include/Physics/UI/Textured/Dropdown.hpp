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
            :UIElement(application, true), m_TitleLabel(application), m_SelectionLabel(application),
            m_Head(Textures::DropdownHead(), Sizes::BorderDropdownHead()),
            m_TextureOptionLast(Textures::DropdownOptionLast()), m_BorderOptionLast(Sizes::BorderDropdownOptionLast()),
            m_TextureHead(Textures::DropdownHead()), m_TextureHeadExpanded(Textures::DropdownHeadExpanded()),
            m_TextureOption(Textures::DropdownOption()), m_BorderOption(Sizes::BorderDropdownOption())
        {
            m_SelectionLabel.SetColor(Color::White);
            m_TitleLabel.SetStyle(sf::Text::Bold);
            AddOption("Placeholder");
            AddClickCallback([&](Application* application, Dropdown* dropdown, MouseButton){
                if(dropdown->IsHovered())
                {
                    dropdown->m_SelectionIndex = dropdown->GetHoverIndex();
                    if(dropdown->m_SelectionIndex != -1)
                    {
                        dropdown->m_SelectionLabel.SetText(dropdown->m_Options[dropdown->m_SelectionIndex].label.GetText());
                        if(dropdown->m_SelectionCallbackFunction)
                            dropdown->m_SelectionCallbackFunction(application, dropdown, dropdown->m_SelectionIndex);
                    }
                }    
                dropdown->m_Expanded = !dropdown->m_Expanded;
            });
        }

        bool IsHovered() const override
        {
            auto mouse = (sf::Vector2f)sf::Mouse::getPosition(m_Application->GetWindow());
            return AABB::RectangleToPoint(m_Head, Mouse::GetPosition())
                || (m_Expanded && AABB::RectangleToPoint(GetOptionAABB(), Mouse::GetPosition()));
        }

        void Draw(int8_t layer = PHYSICS_LAYER_UI_1) override
        {
            m_TitleLabel.Draw(layer);
            m_Application->Draw(&m_Head, layer);
            m_SelectionLabel.Draw(layer);

            if(!m_Expanded) return;
            
            for(auto& option: m_Options)
            {
                m_Application->Draw(&option.box, PHYSICS_LAYER_FLOATING_0);
                option.label.Draw(PHYSICS_LAYER_FLOATING_0);
            }
        }

        inline Option& GetOption(OptionListSize index) { return m_Options.at(index); }
        inline Label& GetOptionLabel(OptionListSize index) { return m_Options.at(index).label; }
        inline Box& GetOptionBox(OptionListSize index) { return m_Options.at(index).box; }

        Dropdown* SetDropdownColors(const Color& box_color, const Color& label_color = Color::White)
        {
            m_IdleBoxColor = box_color; m_HoveredBoxColor = box_color * 2.0f;
            m_IdleLabelColor = label_color * 0.75f; m_HoveredLabelColor = label_color;
            return this;
        }

        Dropdown* SetFontSize(unsigned int size)
        {
            m_SelectionLabel.SetFontSize(size);
            return this;
        }

        Dropdown* AddOption(const sf::String& text)
        {
            m_Options.push_back(Option{Label(m_Application, text, m_SelectionLabel.GetFontSize()), Box(m_TextureOption)});
            m_Options[m_Options.size() - 1].box.SetBorder(m_BorderOption);
            UpdateSize();
            return this;
        }

        Dropdown* SetOption(OptionListSize index, const sf::String& text)
        {
            m_Options[index].label.SetText(text);
            UpdateText();
            return this;
        }

        Dropdown* SetPlaceHolder(const sf::String& text)
        {
            m_Options[0].label.SetText(text);
            UpdateText();
            UpdateSize();
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

        Dropdown* SetBoxSize(const Vector2f& size)
        {
            m_BoxSize = size;
            return this;
        }

        int64_t GetHoverIndex()
        {
            OptionListSize index = (OptionListSize)GetHoverIndexFloat();
            return index < m_Options.size() ? index : -1;
        }

        int64_t GetSelectionIndex()
        {
            return m_SelectionIndex;
        }
    private:
        Vector2f GetOptionSize() const
        {
            return sf::Vector2f{m_BoxSize.x, m_BoxSize.y - m_RemovedBesel};
        }

        AABB GetOptionAABB() const
        {
            return AABB{sf::FloatRect{m_Head.GetPosition() + sf::Vector2f{-m_Head.GetSize().x / 2.0f, m_Head.GetSize().y / 2.0f}, sf::Vector2f{GetOptionSize().x, GetOptionSize().y * m_Options.size()}}};
        }

        void CustomUpdate(float delta_time) override
        {
            if(m_Expanded)
                m_Application->SetFloating();
        
            m_TitleLabel.Update(delta_time);
            m_SelectionLabel.Update(delta_time);
            
            for(auto& option : m_Options)
                option.label.Update(delta_time);
            
            m_Head.SetTexture(m_Expanded? m_TextureHeadExpanded: m_TextureHead);
            
            UpdateOptions();
            UpdatePositions();
            UpdateColors();
        }

        void UpdateOptions()
        {
            for(OptionListSize i = 0; i < m_Options.size(); i++)
            {
                if(i == m_Options.size() - 1)
                {
                    m_Options[i].box.SetTexture(m_TextureOptionLast);
                    m_Options[i].box.SetBorder(m_BorderOptionLast);
                    m_Options[i].box.SetSize(GetOptionSize() + sf::Vector2f{0.0f, 6.0f});
                    m_Options[i].box.SetPosition(m_Options[i - 1].box.GetPosition() + sf::Vector2f{0.0f, GetOptionSize().y / 2.0f + m_Options[i].box.GetSize().y / 2.0f});
                }
                else 
                {
                    m_Options[i].box.SetTexture(m_TextureOption);
                    m_Options[i].box.SetBorder(m_BorderOption);
                    m_Options[i].box.SetSize(GetOptionSize());
                    m_Options[i].box.SetPosition(m_Head.GetPosition() + sf::Vector2f{0.0f, (m_Head.GetSize().y + GetOptionSize().y) / 2.0f + GetOptionSize().y * i});
                }
            }
        }

        void UpdateSize()
        {
            auto options = m_Options.size();
            m_Size = Vector2f{std::max(m_BoxSize.x, m_TitleLabel.GetSize().x), m_BoxSize.y + m_Margin.y + m_TitleLabel.GetMargin().y + m_TitleLabel.GetSize().y};
            
            m_Head.SetSize(m_BoxSize);
        }

        void UpdatePositions()
        {
            auto mouse = Mouse::GetPosition();
            auto last_option_box = m_Options[m_Options.size() - 1].box;

            m_TitleLabel.SetPosition(sf::Vector2f{m_Position.x, m_Position.y - (m_Margin.y - m_TitleLabel.GetSize().y) / 2.0f});
            m_Head.SetPosition(sf::Vector2f{m_Position.x, m_Position.y + (m_Head.GetSize().y + m_TitleLabel.GetMargin().y) / 2.0f});

            auto mod = Vector2f{-m_Head.GetBorderEnd().x / 2.0f, -6.0f};
            m_SelectionLabel.SetPosition((Vector2f)m_Head.GetPosition() + mod);
            
            if(m_Expanded)
                for(OptionListSize i = 0; i < m_Options.size(); i++)
                    m_Options[i].label.SetPosition(m_Options[i].box.GetPosition());
        }

        void UpdateText()
        {
            UpdateSize();
            if(m_SelectionIndex != -1)
                m_SelectionLabel.SetText(m_Options[m_SelectionIndex].label.GetText());
            else m_SelectionLabel.SetText(m_Options[0].label.GetText());
        }

        void UpdateColors()
        {
            m_Head.SetColor(AABB::RectangleToPoint(m_Head, Mouse::GetPosition())? m_HoveredBoxColor * 0.75f: m_IdleBoxColor);

            auto index = GetHoverIndex();
            if(!IsHovered() && !Mouse::GetInstance().ClickState) return;

            for(OptionListSize i = 0; i < m_Options.size(); i++)
            {
                if(i == index)
                {
                    m_Options[i].box.SetColor(m_HoveredBoxColor);
                    m_Options[i].label.SetColor(m_HoveredLabelColor);
                }
                else
                {
                    m_Options[i].box.SetColor(m_IdleBoxColor);
                    m_Options[i].label.SetColor(m_IdleLabelColor);
                } 
            }
        }

        float GetHoverIndexFloat() const
        {
            return std::floor((Mouse::GetPosition().y - m_Head.GetPosition().y - m_Head.GetSize().y / 2.0f) / GetOptionSize().y);
        }

        Box m_Head;
        Label m_TitleLabel, m_SelectionLabel;
        OptionList m_Options;
        
        Box::Border m_BorderOption, m_BorderOptionLast;
        sf::Texture* m_TextureHead, *m_TextureHeadExpanded, *m_TextureOption, *m_TextureOptionLast;
        SelectionCallbackFunc m_SelectionCallbackFunction;

        Color m_IdleBoxColor, m_HoveredBoxColor, m_IdleLabelColor, m_HoveredLabelColor;

        float m_RemovedBesel{24.0f};
        int64_t m_SelectionIndex{-1};
        bool m_Expanded{false};
        
        Vector2f m_BoxSize{300.0f, 70.0f};
    };
}