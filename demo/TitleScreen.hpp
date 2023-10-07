#pragma once
#include <SFML/Graphics/Text.hpp>
#include <Physics.hpp>
#include <Physics/System/Language.hpp>
#include <Physics/UI/UIElementAbstract.hpp>
#include <Physics/UI/Textured/Button.hpp>
#include <Physics/UI/Textured/Dropdown.hpp>

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
        m_Application->SetBackgroundColor(physics::Color::LightGray);
    }

    void OnCreate() override
    {
        m_Layout = new physics::VLayout(m_Application);

        m_TitleLayout = m_Layout->PushElement(new physics::VLayout(m_Application));
        m_TitleLayout
            ->PushElement(new physics::Label(m_Application, "", 40))
            ->SetStyle((sf::Text::Style)(sf::Text::Bold))
            ->SetColor(physics::Color::White);
        
        m_TitleLayout->SetBackgroundColor(physics::Color::DarkCyan)
            ->SetBackgroundVisible(true)
            ->SetBackgroundGrowth(60.0f)
            ->SetTexture(physics::Textures::Button())
            ->SetBorder(physics::Sizes::BorderButton());

        m_LanguageDropdown = m_Layout->PushElement(new physics::Textured::Dropdown(m_Application))
            ->AddOption(L"Ελληνικά")
            ->AddOption("English")
            ->SetDropdownColors(physics::Color::DarkCyan)
            ->SetBoxSize({400.0f, 70.0f})
            ->AddClickCallback([this](physics::Application* app, physics::Textured::Dropdown* dropdown, MouseButton)
            {
                auto index = dropdown->GetSelectionIndex();
                if(index <= 0) return;
                
                physics::Language::SelectedLanguage = (physics::Language::Option)(dropdown->GetSelectionIndex() - 1);
                UpdateLanguages();
            });
        
        m_NextButton = m_Layout->PushElement(new physics::Textured::Button(m_Application))
            ->SetSize({400.0f, 70.0f})
            ->AddClickCallback([&](physics::Application* application, physics::Textured::Button*, MouseButton)
            {
                SetToBeDestroyed();
            })
            ->SetButtonColors(physics::Color::DarkCyan);
        m_Layout->SetAnchor(physics::Anchor::Center);

        m_ExitButton = new physics::Textured::Button(m_Application);
        m_ExitButton
            ->SetSize({50.0f, 60.0f})
            ->SetAnchor(physics::Anchor::TopLeft)
            ->SetButtonColors(physics::Color::DarkCyan)
            ->GetLabel().SetText("X");

        m_ExitButton->AddClickCallback([](physics::Application* app, physics::Textured::Button* btn, MouseButton)
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
        dynamic_cast<physics::Label*>(m_TitleLayout->GetElement(0))->SetText(physics::Language::GetTextSFML("project_name"));
        m_NextButton->GetLabel().SetText(physics::Language::GetTextSFML("continue"));
    }
private:
    physics::VLayout* m_TitleLayout;
    physics::Textured::Dropdown* m_LanguageDropdown;
    physics::Textured::Button* m_NextButton;
    physics::VLayout* m_Layout;
    physics::Textured::Button* m_ExitButton;
};