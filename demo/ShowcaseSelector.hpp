#include <Physics.hpp>
#include <Physics/System/Language.hpp>
#include <Physics/System/RectangleShape.hpp>
#include <Physics/UI/LabeledImage.hpp>
#include <Physics/UI/HGrid.hpp>
#include <Physics/UI/Slider.hpp>
#include "ConfigDemo.hpp"
#include "BackgroundDemo.hpp"
#include "FrictionDemo.hpp"
#include "DragDemo.hpp"

class ShowcaseSelector : public physics::Application::State
{
public:
    ~ShowcaseSelector() noexcept
    {
        delete m_grid;
        delete m_backButton;
    }

    void onShow() override 
    {
        m_application->setBackgroundColor(physics::Color::LightGray);
        m_backButton->setTitle(physics::Language::getTextSFML("back"));
        dynamic_cast<physics::LabeledImage*>(m_grid->getElement(0))->getLabel()->setText(physics::Language::getTextSFML("friction"));
        dynamic_cast<physics::LabeledImage*>(m_grid->getElement(1))->getLabel()->setText(physics::Language::getTextSFML("drag"));
    }

    void onCreate() override
    {
        m_backButton = new physics::Textured::Button(m_application);
        m_backButton
            ->setSize({80.0f, 60.0f})
            ->setAnchor(physics::Anchor::TopLeft)
            ->setButtonColors(physics::Color::DarkCyan)
            ->addClickCallback([](physics::Application* app, physics::Textured::Button* btn, MouseButton)
            {
                app->pushState(new TitleScreen());
            });
        
        m_grid = new physics::HGrid(m_application, 2);
        m_grid->setAnchor(physics::Anchor::Center);

        m_grid->pushElement(new physics::LabeledImage(m_application, PHYSICS_ASSETS_DIR "images/demonstration_friction.png", ""));        
        m_grid->pushElement(new physics::LabeledImage(m_application, PHYSICS_ASSETS_DIR "images/demonstration_drag.png", ""));
    
        for(size_t i = 0; i < m_grid->getElementCount(); i++)
        {
            auto* image = dynamic_cast<physics::LabeledImage*>(m_grid->getElement(i));
            image->setShader(physics::Shaders::load(PHYSICS_ASSETS_DIR "shaders/circle.frag", sf::Shader::Fragment));
            if(i == 0)
                image->setUniform("u_smooth", 0.1f);
        }

        m_grid->addElementClickCallback([](physics::Application* app, physics::Layout* grid, physics::ElementListSize index, MouseButton btn)
        {
            if(!(btn & MOUSE_LEFT)) return;

            switch (index)
            {
            case 0: app->pushState(new FrictionDemo()); break;
            case 1: app->pushState(new DragDemo()); break;
            }
        });
    }

    void onUpdate(float delta_time) override
    {
        m_grid->update(delta_time);
        m_grid->draw();

        m_backButton->update(delta_time);
        m_backButton->draw();
    }
private:
    physics::Textured::Button* m_backButton;
    physics::HGrid* m_grid;
};