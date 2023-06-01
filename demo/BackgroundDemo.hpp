#include "Physics/System/Application.hpp"
#include "Physics/System/Colors.hpp"
#include "Physics/System/Mouse.hpp"
#include "Physics/UI/Button.hpp"
#include "Physics/UI/UIElementAbstract.hpp"
#include <Physics.hpp>
#include <Physics/UI/LabeledImage.hpp>
#include <Physics/UI/Slider.hpp>
#include <unordered_map>

class BackgroundDemo : public physics::Application::State
{
public:
    ~BackgroundDemo() noexcept
    {
        delete m_Info;
        delete m_ExitButton;
    }

    void OnCreate() override
    {
        m_Info = new physics::VLayout(m_Application);
        m_Info->SetAnchor(physics::Anchor::Center);
        
        m_ExitButton = new physics::Button(m_Application, "X", {50, 50});
        m_ExitButton
            ->SetButtonColors(physics::Colors::White)
            ->SetOutline(5)
            ->SetAnchor(physics::Anchor::TopLeft);

        m_ExitButton->AddClickCallback([](physics::Application* app, physics::Button* btn, MouseButton)
        {
            app->GetState()->SetToBeDestroyed();
        });

        // physics::UIElementAbstract::DisplayBounds = true;

        red_slider = m_Info->PushElement(new physics::Slider(m_Application, 0.0f, 255.0f));
        green_slider = m_Info->PushElement(new physics::Slider(m_Application, 0.0f, 255.0f));
        blue_slider = m_Info->PushElement(new physics::Slider(m_Application, 0.0f, 255.0f));
        
        red_slider->SetSliderColors(physics::Colors::Red)
            ->SetOutlineThickness(5.0f)
            ->SetSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);

        green_slider->SetSliderColors(physics::Colors::Green)
            ->SetOutlineThickness(5.0f)
            ->SetSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);

        blue_slider->SetSliderColors(physics::Colors::Blue)
            ->SetOutlineThickness(5.0f)
            ->SetSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);
    }

    void OnUpdate(float delta_time) override
    {
        m_Application->SetBackgroundColor(sf::Color{(uint8_t)red_slider->GetValue(), (uint8_t)green_slider->GetValue(), (uint8_t)blue_slider->GetValue()});
        m_Info->Update(delta_time);
        m_Info->Draw();

        m_ExitButton->Update(delta_time);
        m_ExitButton->Draw();
    }
private:
    physics::Button* m_ExitButton;
    physics::Slider* red_slider;
    physics::Slider* green_slider;
    physics::Slider* blue_slider;
    physics::VLayout* m_Info;
};