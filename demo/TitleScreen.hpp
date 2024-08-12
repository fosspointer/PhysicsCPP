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
        delete m_layout;
        delete m_exitButton;
    }

    void onShow() override 
    {
        m_application->setBackgroundColor(physics::Color::LightGray);
    }

    void onCreate() override
    {
        m_layout = new physics::VLayout(m_application);

        m_titleLayout = m_layout->pushElement(new physics::VLayout(m_application));
        m_titleLayout
            ->pushElement(new physics::Label(m_application, "", 40))
            ->setStyle((sf::Text::Style)(sf::Text::Bold))
            ->setColor(physics::Color::White);
        
        m_titleLayout->setBackgroundColor(physics::Color::DarkCyan)
            ->setBackgroundVisible(true)
            ->setBackgroundGrowth(60.0f)
            ->setTexture(physics::Textures::button())
            ->setBorder(physics::Sizes::borderButton());

        m_languageDropdown = m_layout->pushElement(new physics::Textured::Dropdown(m_application))
            ->addOption(L"Ελληνικά")
            ->addOption("English")
            ->setDropdownColors(physics::Color::DarkCyan)
            ->setBoxSize({400.0f, 70.0f})
            ->addClickCallback([this](physics::Application* app, physics::Textured::Dropdown* dropdown, MouseButton)
            {
                auto index = dropdown->getSelectionIndex();
                if(index <= 0) return;
                
                physics::Language::SelectedLanguage = (physics::Language::Option)(dropdown->getSelectionIndex() - 1);
                updateLanguages();
            });
        
        m_nextButton = m_layout->pushElement(new physics::Textured::Button(m_application))
            ->setSize({400.0f, 70.0f})
            ->addClickCallback([&](physics::Application* application, physics::Textured::Button*, MouseButton)
            {
                setToBeDestroyed();
            })
            ->setButtonColors(physics::Color::DarkCyan);
        m_layout->setAnchor(physics::Anchor::Center);

        m_exitButton = new physics::Textured::Button(m_application);
        m_exitButton
            ->setSize({50.0f, 60.0f})
            ->setAnchor(physics::Anchor::TopLeft)
            ->setButtonColors(physics::Color::DarkCyan)
            ->getLabel().setText("X");

        m_exitButton->addClickCallback([](physics::Application* app, physics::Textured::Button* btn, MouseButton)
        {
            app->getWindow().close();
        });

        updateLanguages();
    }

    void onUpdate(float delta_time) override
    {
        m_layout->update(delta_time);
        m_layout->updateSize();
        m_layout->updatePositions();
        m_layout->draw();

        m_exitButton->update(delta_time);
        m_exitButton->draw();
    }

    void updateLanguages()
    {
        m_languageDropdown->setPlaceHolder(physics::Language::getTextSFML("dropdown_placeholder"));
        m_languageDropdown->setTitle(physics::Language::getTextSFML("select_language"));
        dynamic_cast<physics::Label*>(m_titleLayout->getElement(0))->setText(physics::Language::getTextSFML("project_name"));
        m_nextButton->getLabel().setText(physics::Language::getTextSFML("continue"));
    }
private:
    physics::VLayout* m_titleLayout;
    physics::Textured::Dropdown* m_languageDropdown;
    physics::Textured::Button* m_nextButton;
    physics::VLayout* m_layout;
    physics::Textured::Button* m_exitButton;
};