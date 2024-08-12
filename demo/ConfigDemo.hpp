#include <unordered_map>
#include <Physics.hpp>
#include <Physics/System/Language.hpp>

class ConfigDemo : public physics::Application::State
{
public:
    ~ConfigDemo() noexcept
    {
        delete m_languageDropdown;
    }

    void onShow() override
    {
        m_application->setBackgroundColor(physics::Color::DarkMagenta);
    }

    void onCreate() override
    {
        m_layout = new physics::VLayout(m_application);
        m_layout->pushElement(new physics::Label(m_application, "project_name_tmp", 20u));
        m_layout->pushElement(new physics::Button(m_application))
            ->setSize({200.0f, 130.0f});
        m_layout->setAnchor(physics::Anchor::Center);

        m_languageDropdown = new physics::Dropdown(m_application, "select_language_tmp", 25, "placeholder_tmp", {300.0f, 70.0f});
        updateLanguages();
        m_languageDropdown
            ->addOption(L"Ελληνικά")
            ->addOption("English")
            ->setDirection(physics::Dropdown::Direction::Up)
            ->setAnchor(physics::Anchor::BottomRight)
            ->setDropdownColors(physics::Color::Magenta)
            ->setOutline(5)
            ->addClickCallback([this](physics::Application* app, physics::Dropdown* dropdown, MouseButton)
            {
                auto index = dropdown->getSelectionIndex();
                if(index <= 0) return;
                
                physics::Language::SelectedLanguage = (physics::Language::Option)(dropdown->getSelectionIndex() - 1);
                updateLanguages();
            });

        m_exitButton = new physics::Button(m_application);
        m_exitButton
            ->setTitle("X")
            ->setSize({50.0f, 50.0f})
            ->setButtonColors(physics::Color::White)
            ->setOutline(5)
            ->setAnchor(physics::Anchor::TopLeft);

        m_exitButton->addClickCallback([](physics::Application* app, physics::Button* btn, MouseButton)
        {
            app->getState()->setToBeDestroyed();
        });
    }

    void updateLanguages()
    {
        m_languageDropdown->setPlaceHolder(physics::Language::getTextSFML("dropdown_placeholder"));
        m_languageDropdown->setTitle(physics::Language::getTextSFML("select_language"));
        dynamic_cast<physics::Label*>(m_layout->getElement(0))->setText(physics::Language::getTextSFML("project_name"));
        dynamic_cast<physics::Button*>(m_layout->getElement(1))->getLabel().setText(physics::Language::getTextSFML("continue"));
    }

    void onUpdate(float delta_time) override
    {
        m_languageDropdown->update(delta_time);
        m_languageDropdown->draw();

        m_layout->update(delta_time);
        m_layout->draw();

        m_exitButton->update(delta_time);
        m_exitButton->draw();
    }
private:
    physics::VLayout* m_layout;
    physics::Dropdown* m_languageDropdown;
    physics::Button* m_exitButton;
};