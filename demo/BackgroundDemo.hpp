#include <unordered_map>
#include <Physics/System/Application.hpp>
#include <Physics/System/Color.hpp>
#include <Physics/System/Mouse.hpp>
#include <Physics/UI/Button.hpp>
#include <Physics.hpp>
#include <Physics/UI/LabeledImage.hpp>
#include <Physics/UI/Slider.hpp>
#include <Physics/System/Textures.hpp>
#include <Physics/System/Box.hpp>

class BackgroundDemo : public physics::Application::State
{
public:
    BackgroundDemo(){}

    ~BackgroundDemo() noexcept
    {
        delete m_info;
        delete m_exitButton;
    }

    void onCreate() override
    {
        m_info = new physics::VLayout(m_application);
        m_info->setAnchor(physics::Anchor::Center);
        
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

        red_slider = m_info->pushElement(new physics::Slider(m_application, 0.0f, 255.0f));
        green_slider = m_info->pushElement(new physics::Slider(m_application, 0.0f, 255.0f));
        blue_slider = m_info->pushElement(new physics::Slider(m_application, 0.0f, 255.0f));
        
        red_slider->setSliderColors(physics::Color::Red)
            ->setOutlineThickness(5.0f)
            ->setSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);

        green_slider->setSliderColors(physics::Color::Green)
            ->setOutlineThickness(5.0f)
            ->setSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);

        blue_slider->setSliderColors(physics::Color::Blue)
            ->setOutlineThickness(5.0f)
            ->setSliderSize(sf::Vector2f{200.0f, 50.0f}, 20.0f, 10.0f);
    }

    void onUpdate(float delta_time) override
    {
        m_application->setBackgroundColor(sf::Color{(uint8_t)red_slider->getValue(), (uint8_t)green_slider->getValue(), (uint8_t)blue_slider->getValue()});
        
        m_info->update(delta_time);
        m_info->draw();

        m_exitButton->update(delta_time);
        m_exitButton->draw();

    }
private:
    physics::Button* m_exitButton;
    physics::Slider* red_slider;
    physics::Slider* green_slider;
    physics::Slider* blue_slider;
    physics::VLayout* m_info;
};