#include "ConfigDemo.hpp"
#include "BackgroundDemo.hpp"
#include <Physics.hpp>
#include <Physics/UI/LabeledImage.hpp>
#include <Physics/UI/HGrid.hpp>
#include <Physics/UI/Slider.hpp>
#include <algorithm>
#include <unordered_map>

class ShowcaseSelector : public physics::Application::State
{
public:
    ~ShowcaseSelector() noexcept
    {
        delete m_Grid;
    }

    void OnShow() override 
    {
        m_Application->SetBackgroundColor(physics::Colors::LightGray);
    }

    void OnCreate() override
    {
        m_Grid = new physics::HGrid(m_Application, 3);
        m_Grid->SetAnchor(physics::Anchor::Center);

        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/bg_demo.png", "Background Demo"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/cfg_demo.png", "Config Demo"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/test.png", "Showcase"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/test.png", "unavailable"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/test.png", "unavailable"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/test.png", "unavailable"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/test.png", "unavailable"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/test.png", "unavailable"));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/test.png", "unavailable"));        
    
        m_Grid->AddElementHoverCallback([](physics::Application*, physics::Layout* grid, physics::ElementListSize index, bool stopped)
        {
            grid->GetElement(index)->AbstractSetColor(stopped ? sf::Color::White : physics::Colors::Gray);
        });

        m_Grid->AddElementClickCallback([](physics::Application* app, physics::Layout* grid, physics::ElementListSize index, MouseButton btn)
        {
            if(!(btn & MOUSE_LEFT)) return;

            switch (index)
            {
            case 0: app->PushState(new BackgroundDemo()); break;
            case 1: app->PushState(new ConfigDemo()); break;
            }
        });
    }

    void OnUpdate(float delta_time) override
    {
        m_Grid->Update(delta_time);
        m_Grid->Draw();
    }
private:
    physics::HGrid* m_Grid;
};