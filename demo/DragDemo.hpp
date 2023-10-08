#include <Physics.hpp>
#include <Physics/UI/LabeledImage.hpp>
#include <Physics/UI/Slider.hpp>

#include <Physics/System/Language.hpp>

#include <Physics/Physics/Body.hpp>
#include <Physics/Physics/KinematicBody.hpp>
#include <Physics/Physics/StaticBody.hpp>
#include <Physics/Physics/BodyHandler.hpp>

#include <Physics/System/Box.hpp>
#include <Physics/System/TranslationControl.hpp>

#include <Physics/System/Renderer.hpp>
#include <Physics/System/Sizes.hpp>

#include <Physics/Physics/Materials.hpp>
#include <Physics/Physics/FrictionCoefficients.hpp>


class DragDemo : public physics::Application::State
{
public:
    ~DragDemo() noexcept
    {
        delete m_ExitButton;
        delete m_PropertiesButton;
        delete m_PropertiesLayout;
        delete m_BodyHandler;
    }

    void OnCreate() override
    {
        physics::Units::SetPixelsPerMeter(150.0f / 1.7f);

        m_ExitButton = new physics::Button(m_Application);
        m_ExitButton
            ->SetTitle("X")
            ->SetSize({50.0f, 50.0f})
            ->SetButtonColors(physics::Color::White)
            ->SetOutline(5)
            ->SetAnchor(physics::Anchor::TopLeft);

        m_ExitButton->AddClickCallback([](physics::Application* app, physics::Button* btn, MouseButton)
        {
            app->GetState()->SetToBeDestroyed();
        });

        m_PropertiesLayout = new physics::VLayout(m_Application);
        m_PropertiesLayout->SetBackground(physics::Color::White.ChangeAlpha(0.25f), 10.0f);

        m_MassLabel = m_PropertiesLayout->PushElement(new physics::Label(m_Application));
        m_MassSlider = m_PropertiesLayout->PushElement(new physics::Slider(m_Application, 40, 100))
            ->SetSliderColors(physics::Color::DarkGreen)
            ->SetSliderSize({100, 30}, 20u, 20.0f)
            ->SetValue(60.0f);

        m_GravityAccelerationLabel = m_PropertiesLayout->PushElement(new physics::Label(m_Application));
        m_GravityAccelerationSlider = m_PropertiesLayout->PushElement(new physics::Slider(m_Application, 1.62f, physics::Units::GetGravityAcceleration()))
            ->SetSliderColors(physics::Color::DarkBlue)
            ->SetSliderSize({100, 30}, 20u, 20.0f)
            ->SetValue(9.81f);

        m_AirResistanceLabel = m_PropertiesLayout->PushElement(new physics::Label(m_Application));
        m_AirResistanceSlider = m_PropertiesLayout->PushElement(new physics::Slider(m_Application, 0.0f, 100.0f))
            ->SetSliderColors(physics::Color::DarkYellow)
            ->SetSliderSize({100, 30}, 20u, 20.0f);

        m_FrictionLabel = m_PropertiesLayout->PushElement(new physics::Label(m_Application));
        m_FrictionSlider = m_PropertiesLayout->PushElement(new physics::Slider(m_Application, 0.0f, 0.7f))
            ->SetSliderColors(physics::Color::DarkRed)
            ->SetSliderSize({100, 30}, 20u, 20.0f)
            ->SetValue(0.25f);
        
        m_PropertiesLayout->SetAnchor(physics::Anchor::Right);

        m_PropertiesButton = new physics::Button(m_Application);
        m_PropertiesButton
            ->SetSize({512.0f, 100.0f})
            ->SetAnchor(physics::Anchor::BottomRight)
            ->SetOutline(5)
            ->AddClickCallback([this](physics::Application*, physics::Button*, MouseButton)
            {
                m_ViewProperties = !m_ViewProperties;
            });

        m_BodyHandler = new physics::BodyHandler;
        
        m_Body = m_BodyHandler->AddKinematicBody(m_Application, PHYSICS_ASSETS_DIR "images/person.png");
        m_Body->SetCollider({0.0f, 0.0f}, {20.0f, 150.0f});
        m_Body->SetUpdateCallback([](physics::KinematicBody* body, float delta)
        {
            if(body->GetFree()) return;

            auto mov = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) * 1.0f + 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left) * -1.0f;

            auto jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

            body->AddForce({mov * 700.0f, 0.0f}, "F");

            if(jump)
                body->SetVelocity(physics::Vector2f{body->GetVelocity().x, -230.0f});
        });
        m_Body->SetPosition(sf::Vector2f{m_Application->GetWindow().getSize()} / 2.0f - sf::Vector2f{0.0f, 100.0f});
        m_Body->SetMaterial(physics::Materials::Get("human"));
    
        m_Ground = m_BodyHandler->AddStaticBody(m_Application, physics::Color::DarkGray);
        m_Ground->SetSize({700, 300});
        m_Ground->SetPosition(sf::Vector2f{m_Application->GetWindow().getSize()} / 2.0f + sf::Vector2f{0.0f, 300.0f});
        m_Ground->SetMaterial(physics::Materials::Get("concrete"));

        m_Rod1 = m_BodyHandler->AddStaticBody(m_Application, physics::Color::DarkGray);
        m_Rod1->SetSize({20.0f, 100.0f});
        m_Rod1->SetPosition(m_Ground->GetPosition() - m_Ground->GetSize() / 2.0f + sf::Vector2f{50.0f, -50.0f});
        m_Rod1->SetMaterial(physics::Materials::Get("concrete"));

        m_Rod2 = m_BodyHandler->AddStaticBody(m_Application, physics::Color::DarkGray);
        m_Rod2->SetSize({20.0f, 100.0f});
        m_Rod2->SetPosition(m_Ground->GetPosition() - m_Ground->GetSize() / 2.0f + sf::Vector2f{470.0f, -50.0f});
        m_Rod2->SetMaterial(physics::Materials::Get("concrete"));

        m_Liquid = m_BodyHandler->AddStaticBody(m_Application, physics::Color::White.ChangeAlpha(0.25f));
        m_Liquid->SetSize({400.0f, 100.0f});
        m_Liquid->SetPosition(m_Rod1->GetPosition() + sf::Vector2f{m_Rod1->GetSize().x / 2.0f + 200.0f, 0.0f});
        m_Liquid->SetMaterial(physics::Materials::Get("water"));
    }

    void OnShow() override
    {
        m_PropertiesButton->GetLabel().SetText(physics::Language::GetTextSFML("properties"));
        m_MassLabel->SetText(physics::Language::GetTextSFML("mass"));
        m_GravityAccelerationLabel->SetText(physics::Language::GetTextSFML("gravitational_acceleration"));
        m_AirResistanceLabel->SetText(physics::Language::GetTextSFML("air_resistance"));
        m_FrictionLabel->SetText(physics::Language::GetTextSFML("friction_coefficient"));
    }

    void OnUpdate(float delta_time) override
    {
        physics::FrictionCoefficients::Set("human", "concrete", m_FrictionSlider->GetValue());
        m_Body->SetMass(m_MassSlider->GetValue());
        m_Body->SetGravityAcceleration(m_GravityAccelerationSlider->GetValue());
        
        // Demo

        m_BodyHandler->Draw();
        m_BodyHandler->Update(delta_time);

        // UI

        m_ExitButton->Update(delta_time);
        m_ExitButton->Draw();
        
        if(m_ViewProperties)
        {
            m_Body->DrawForces();
            m_PropertiesLayout->Update(delta_time);
            m_PropertiesLayout->Draw();
        }

        m_PropertiesButton->Update(delta_time);
        m_PropertiesButton->Draw();

    }
private:
    bool m_ViewProperties = false;
    physics::Button* m_ExitButton;
    physics::VLayout* m_PropertiesLayout;
    physics::Button* m_PropertiesButton;

    physics::Label* m_MassLabel;
    physics::Slider* m_MassSlider;

    physics::Label* m_GravityAccelerationLabel;
    physics::Slider* m_GravityAccelerationSlider;

    physics::Label* m_AirResistanceLabel;
    physics::Slider* m_AirResistanceSlider;

    physics::Label* m_FrictionLabel;
    physics::Slider* m_FrictionSlider;

    physics::KinematicBody* m_Body;
    physics::StaticBody* m_Ground, *m_Rod1, *m_Rod2, *m_Liquid;
    
    physics::BodyHandler* m_BodyHandler;
};