#include <Physics.hpp>
#include <unordered_map>
#include <Physics/System/Language.hpp>

class ConfigDemo : public physics::Application::State
{
public:
    ~ConfigDemo() noexcept
    {
        delete m_LanguageDropdown;
    }

    void OnShow() override
    {
        m_Application->SetBackgroundColor(physics::Colors::DarkMagenta);
    }

    void OnCreate() override
    {
        m_Layout = new physics::VLayout(m_Application);
        m_Layout->PushElement(new physics::Label(m_Application, "project_name_tmp", 20u));
        m_Layout->PushElement(new physics::Button(m_Application, "continue_tmp", {200, 130}));
        m_Layout->SetAnchor(physics::Anchor::Center);

        m_LanguageDropdown = new physics::Dropdown(m_Application, "select_language_tmp", 25, "placeholder_tmp", 300, 20, 10.0f);
        UpdateLanguages();
        m_LanguageDropdown
            ->AddOption(L"Ελληνικά")
            ->AddOption("English")
            ->SetDirection(physics::Dropdown::Direction::Up)
            ->SetAnchor(physics::Anchor::BottomRight)
            ->SetDropdownColors(physics::Colors::Magenta)
            ->SetOutline(5)
            ->AddClickCallback([this](physics::Application* app, physics::Dropdown* dropdown, MouseButton)
            {
                auto index = dropdown->GetSelectionIndex();
                if(index <= 0) return;
                
                physics::Language::SelectedLanguage = (physics::Language::Option)(dropdown->GetSelectionIndex() - 1);
                UpdateLanguages();
            });

        m_ExitButton = new physics::Button(m_Application, "X", {50, 50});
        m_ExitButton
            ->SetButtonColors(physics::Colors::White)
            ->SetOutline(5)
            ->SetAnchor(physics::Anchor::TopLeft);

        m_ExitButton->AddClickCallback([](physics::Application* app, physics::Button* btn, MouseButton)
        {
            app->GetState()->SetToBeDestroyed();
        });
    }

    void UpdateLanguages()
    {
        m_LanguageDropdown->SetPlaceHolder(physics::Language::GetTextSFML("dropdown_placeholder"));
        m_LanguageDropdown->SetTitle(physics::Language::GetTextSFML("select_language"));
        dynamic_cast<physics::Label*>(m_Layout->GetElement(0))->SetText(physics::Language::GetTextSFML("project_name"));
        dynamic_cast<physics::Button*>(m_Layout->GetElement(1))->GetLabel().SetText(physics::Language::GetTextSFML("continue"));
    }

    void OnUpdate(float delta_time) override
    {
        m_LanguageDropdown->Update(delta_time);
        m_LanguageDropdown->Draw();

        m_Layout->Update(delta_time);
        m_Layout->Draw();

        m_ExitButton->Update(delta_time);
        m_ExitButton->Draw();
    }
private:
    physics::VLayout* m_Layout;
    physics::Dropdown* m_LanguageDropdown;
    physics::Button* m_ExitButton;
};