#include <Physics/System/Language.hpp>
#include <Physics/UI/Label.hpp>
#include <Physics/UI/Button.hpp>
#include <Physics/UI/Dropdown.hpp>
#include <Physics/System/MessageBox.hpp>
#include <bitset>
#include <cstdint>

class TitleScreen : public physics::Application::State
{
public:
    TitleScreen()
    {}

    ~TitleScreen()
    {
        delete m_Drop;
        delete m_ContinueButton;
    }

    void OnCreate()
    {
        m_Application->SetBackgroundColor(physics::Colors::LightGray);
        m_Drop = new physics::Dropdown(m_Application, "", 25.0f, "", 500.0f, 20.0f);
        m_Drop
            ->SetOutline(2)
            ->SetAnchor(physics::Anchor::Center)
            ->SetDropdownColors(physics::Colors::Magenta)
            ->AddOption(L"Ελληνικά")
            ->AddOption("English")
            ->SetDirection(physics::Dropdown::Direction::Up)
            ->SetSelectionCallback([&](physics::Application*, physics::Dropdown*, int64_t index)
            {
                SetLanguage(index);
            });
        m_ContinueButton = new physics::Button(m_Application, "Continue", sf::Vector2f{150.0f, 80.0f});
        m_ContinueButton->SetAnchor(physics::Anchor::TopRight)
            ->SetButtonColors(physics::Colors::White)
            ->SetOutline(2)
            ->AddClickCallback([&](physics::Application* application, physics::Button* button, MouseButton)
            {
                SetToBeDestroyed();
            });
        SetLanguage(2);
    }

    void SetLanguage(int64_t index)
    {
        if(index != 0)
        {
            physics::Language::SelectedLanguage = (physics::Language::Option)(index - 1);
            m_Application->Rename(physics::Language::GetTextSFML("project_name"));
            m_Drop->SetTitle(physics::Language::GetTextSFML("select_language"))
                ->SetPlaceHolder(physics::Language::GetTextSFML("dropdown_placeholder"));
        }
    }

    void OnUpdate(float dt)
    {
        static float time = 0.0f;
        time += dt;

        m_Drop->Update(dt);
        m_Drop->Draw();

        m_ContinueButton->Update(dt);
        m_ContinueButton->Draw();
    }
private:
    physics::Dropdown* m_Drop;
    physics::Button* m_ContinueButton;
};

class Demo : public physics::Application
{
public:
    void OnCreate() override
    {
        PushState(new TitleScreen());
    }

    void OnUpdate(float dt) override {}
private:
    
};

int main()
{
    srand(time(0));
    Demo application;
    return application.Start(sf::Vector2u{800, 600}, "");
}