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
        delete m_Grid;
        delete m_BackButton;
    }

    void OnShow() override 
    {
        m_Application->SetBackgroundColor(physics::Color::LightGray);
        m_BackButton->SetTitle(physics::Language::GetTextSFML("back"));
        dynamic_cast<physics::LabeledImage*>(m_Grid->GetElement(0))->GetLabel()->SetText(physics::Language::GetTextSFML("friction"));
        dynamic_cast<physics::LabeledImage*>(m_Grid->GetElement(1))->GetLabel()->SetText(physics::Language::GetTextSFML("drag"));
    }

    void OnCreate() override
    {
        m_BackButton = new physics::Textured::Button(m_Application);
        m_BackButton
            ->SetSize({80.0f, 60.0f})
            ->SetAnchor(physics::Anchor::TopLeft)
            ->SetButtonColors(physics::Color::DarkCyan)
            ->AddClickCallback([](physics::Application* app, physics::Textured::Button* btn, MouseButton)
            {
                app->PushState(new TitleScreen());
            });
        
        m_Grid = new physics::HGrid(m_Application, 2);
        m_Grid->SetAnchor(physics::Anchor::Center);

        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/demonstration_friction.png", ""));        
        m_Grid->PushElement(new physics::LabeledImage(m_Application, PHYSICS_ASSETS_DIR "images/demonstration_drag.png", ""));
    
        for(size_t i = 0; i < m_Grid->GetElementCount(); i++)
        {
            auto* image = dynamic_cast<physics::LabeledImage*>(m_Grid->GetElement(i));
            image->SetShader(physics::Shaders::Load(PHYSICS_ASSETS_DIR "shaders/circle.frag", sf::Shader::Fragment));
            if(i == 0)
                image->SetUniform("u_Smooth", 0.1f);
        }

        m_Grid->AddElementClickCallback([](physics::Application* app, physics::Layout* grid, physics::ElementListSize index, MouseButton btn)
        {
            if(!(btn & MOUSE_LEFT)) return;

            switch (index)
            {
            case 0: app->PushState(new FrictionDemo()); break;
            case 1: app->PushState(new DragDemo()); break;
            }
        });
    }

    void OnUpdate(float delta_time) override
    {
        m_Grid->Update(delta_time);
        m_Grid->Draw();

        m_BackButton->Update(delta_time);
        m_BackButton->Draw();
    }
private:
    physics::Textured::Button* m_BackButton;
    physics::HGrid* m_Grid;
};