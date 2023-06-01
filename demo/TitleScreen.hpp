#pragma once
#include "SFML/Graphics/Text.hpp"
#include <Physics.hpp>
#include <Physics/System/Language.hpp>

class TitleScreen : public physics::Application::State
{
public:
    TitleScreen()
    {}

    ~TitleScreen()
    {
        delete m_Layout;
        delete m_ExitButton;
    }

    void OnShow() override 
    {
        m_Application->SetBackgroundColor(physics::Colors::LightGray);
    }

    void OnCreate() override
    {
        m_Layout = new physics::VLayout(m_Application);

        m_TitleLabel = m_Layout->PushElement(new physics::Label(m_Application, "project_title_tmp", 34u))
            ->SetStyle((sf::Text::Style)(sf::Text::Bold | sf::Text::Underlined));

        m_LanguageDropdown = m_Layout->PushElement(new physics::Dropdown(m_Application, "select_language_dropdown_tmp", 25.0f, "dropdown_placeholder_tmp", 250.0f, 20.0f))
            ->AddOption(L"Ελληνικά")
            ->AddOption("English")
            ->SetDropdownColors(physics::Colors::White)
            ->SetOutline(5.0f)
            ->SetDirection(physics::Dropdown::Direction::Down)
            ->SetMargin(sf::Vector2f{25.0f, 100.0f})
            ->AddClickCallback([this](physics::Application* app, physics::Dropdown* dropdown, MouseButton)
            {
                auto index = dropdown->GetSelectionIndex();
                if(index <= 0) return;
                
                physics::Language::SelectedLanguage = (physics::Language::Option)(dropdown->GetSelectionIndex() - 1);
                UpdateLanguages();
            });
        
        m_NextButton = m_Layout->PushElement(new physics::Button(m_Application, "next_button_tmp", sf::Vector2f{300.0f, 70.0f}))
            ->SetOutline(5)
            ->SetButtonColors(physics::Colors::White)
            ->AddClickCallback([&](physics::Application* application, physics::Button*, MouseButton)
            {
                SetToBeDestroyed();
            });
        m_Layout->SetAnchor(physics::Anchor::Center);

        m_ExitButton = new physics::Button(m_Application, "X", {50, 50});
        m_ExitButton
            ->SetButtonColors(physics::Colors::White)
            ->SetOutline(5)
            ->SetAnchor(physics::Anchor::TopLeft);

        m_ExitButton->AddClickCallback([](physics::Application* app, physics::Button* btn, MouseButton)
        {
            app->GetWindow().close();
        });

        UpdateLanguages();
    }

    void OnUpdate(float delta_time) override
    {
        m_Layout->Update(delta_time);
        m_Layout->UpdateSize();
        m_Layout->UpdatePositions();
        m_Layout->Draw();

        m_ExitButton->Update(delta_time);
        m_ExitButton->Draw();
    }

    void UpdateLanguages()
    {
        m_LanguageDropdown->SetPlaceHolder(physics::Language::GetTextSFML("dropdown_placeholder"));
        m_LanguageDropdown->SetTitle(physics::Language::GetTextSFML("select_language"));
        m_TitleLabel->SetText(physics::Language::GetTextSFML("project_name"));
        m_NextButton->GetLabel().SetText(physics::Language::GetTextSFML("continue"));
    }
private:
    physics::Label* m_TitleLabel;
    physics::Dropdown* m_LanguageDropdown;
    physics::Button* m_NextButton;
    physics::VLayout* m_Layout;
    physics::Button* m_ExitButton;
};