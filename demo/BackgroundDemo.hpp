#include <Physics/System/Application.hpp>
#include <Physics/System/Color.hpp>
#include <Physics/System/Mouse.hpp>
#include <Physics/UI/Button.hpp>
#include <Physics/UI/TextureButton.hpp>
#include <Physics.hpp>
#include <Physics/UI/LabeledImage.hpp>
#include <Physics/UI/Slider.hpp>
#include <unordered_map>
#include <Physics/System/Textures.hpp>
#include <Physics/System/Box.hpp>

class BackgroundDemo : public physics::Application::State
{
public:
    BackgroundDemo()
        :m_Box(physics::Textures::LoadTexture(PHYSICS_ASSETS_DIR "images/button.png"))
    {}

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
            ->SetButtonColors(physics::Color::White)
            ->SetOutline(5)
            ->SetAnchor(physics::Anchor::TopLeft);

        m_ExitButton->AddClickCallback([](physics::Application* app, physics::Button* btn, MouseButton)
        {
            app->GetState()->SetToBeDestroyed();
        });

        // m_Info->PushElement(new physics::TextureButton(m_Application, 
        //     physics::Textures::LoadTexture(PHYSICS_ASSETS_DIR "images/button.png"),
        //     physics::Textures::LoadTexture(PHYSICS_ASSETS_DIR "images/button_pressed.png"),
        //     sf::Vector2f{32.0f, 32.0f}, "Press me!", {200.0f, 50.0f}))
        //     ->SetButtonColors(physics::Color::Yellow);

        red_slider = m_Info->PushElement(new physics::Slider(m_Application, 0.0f, 255.0f));
        green_slider = m_Info->PushElement(new physics::Slider(m_Application, 0.0f, 255.0f));
        blue_slider = m_Info->PushElement(new physics::Slider(m_Application, 0.0f, 255.0f));
        
        red_slider->SetSliderColors(physics::Color::Red)
            ->SetOutlineThickness(5.0f)
            ->SetSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);

        green_slider->SetSliderColors(physics::Color::Green)
            ->SetOutlineThickness(5.0f)
            ->SetSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);

        blue_slider->SetSliderColors(physics::Color::Blue)
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
    physics::Box m_Box;
};