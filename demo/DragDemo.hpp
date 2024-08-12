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
        delete m_exitButton;
        delete m_propertiesButton;
        delete m_propertiesLayout;
        delete m_bodyHandler;
    }

    void onCreate() override
    {
        physics::Units::setPixelsPerMeter(150.0f / 1.7f);

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

        m_propertiesLayout = new physics::VLayout(m_application);
        m_propertiesLayout->setBackground(physics::Color::White.changeAlpha(0.25f), 10.0f);

        m_massLabel = m_propertiesLayout->pushElement(new physics::Label(m_application));
        m_massSlider = m_propertiesLayout->pushElement(new physics::Slider(m_application, 40, 100))
            ->setSliderColors(physics::Color::DarkGreen)
            ->setSliderSize({100, 30}, 20u, 20.0f)
            ->setValue(60.0f);

        m_gravityAccelerationLabel = m_propertiesLayout->pushElement(new physics::Label(m_application));
        m_gravityAccelerationSlider = m_propertiesLayout->pushElement(new physics::Slider(m_application, 1.62f, physics::Units::getGravityAcceleration()))
            ->setSliderColors(physics::Color::DarkBlue)
            ->setSliderSize({100, 30}, 20u, 20.0f)
            ->setValue(9.81f);

        m_airResistanceLabel = m_propertiesLayout->pushElement(new physics::Label(m_application));
        m_airResistanceSlider = m_propertiesLayout->pushElement(new physics::Slider(m_application, 0.0f, 100.0f))
            ->setSliderColors(physics::Color::DarkYellow)
            ->setSliderSize({100, 30}, 20u, 20.0f);

        m_frictionLabel = m_propertiesLayout->pushElement(new physics::Label(m_application));
        m_frictionSlider = m_propertiesLayout->pushElement(new physics::Slider(m_application, 0.0f, 0.7f))
            ->setSliderColors(physics::Color::DarkRed)
            ->setSliderSize({100, 30}, 20u, 20.0f)
            ->setValue(0.25f);
        
        m_propertiesLayout->setAnchor(physics::Anchor::Right);

        m_propertiesButton = new physics::Button(m_application);
        m_propertiesButton
            ->setSize({512.0f, 100.0f})
            ->setAnchor(physics::Anchor::BottomRight)
            ->setOutline(5)
            ->addClickCallback([this](physics::Application*, physics::Button*, MouseButton)
            {
                m_viewProperties = !m_viewProperties;
            });

        m_bodyHandler = new physics::BodyHandler;
        
        m_body = m_bodyHandler->addKinematicBody(m_application, PHYSICS_ASSETS_DIR "images/person.png");
        m_body->setCollider({0.0f, 0.0f}, {20.0f, 150.0f});
        m_body->setUpdateCallback([](physics::KinematicBody* body, float delta)
        {
            if(body->getFree()) return;

            auto mov = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) * 1.0f + 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left) * -1.0f;

            auto jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

            body->addForce({mov * 700.0f, 0.0f}, "F");

            if(jump)
                body->setVelocity(physics::Vector2f{body->getVelocity().x, -230.0f});
        });
        m_body->setPosition(sf::Vector2f{m_application->getWindow().getSize()} / 2.0f - sf::Vector2f{0.0f, 100.0f});
        m_body->setMaterial(physics::Materials::get("human"));
    
        m_ground = m_bodyHandler->addStaticBody(m_application, physics::Color::DarkGray);
        m_ground->setSize({700, 300});
        m_ground->setPosition(sf::Vector2f{m_application->getWindow().getSize()} / 2.0f + sf::Vector2f{0.0f, 300.0f});
        m_ground->setMaterial(physics::Materials::get("concrete"));

        m_rod1 = m_bodyHandler->addStaticBody(m_application, physics::Color::DarkGray);
        m_rod1->setSize({20.0f, 100.0f});
        m_rod1->setPosition(m_ground->getPosition() - m_ground->getSize() / 2.0f + sf::Vector2f{50.0f, -50.0f});
        m_rod1->setMaterial(physics::Materials::get("concrete"));

        m_rod2 = m_bodyHandler->addStaticBody(m_application, physics::Color::DarkGray);
        m_rod2->setSize({20.0f, 100.0f});
        m_rod2->setPosition(m_ground->getPosition() - m_ground->getSize() / 2.0f + sf::Vector2f{470.0f, -50.0f});
        m_rod2->setMaterial(physics::Materials::get("concrete"));

        m_liquid = m_bodyHandler->addStaticBody(m_application, physics::Color::White.changeAlpha(0.25f));
        m_liquid->setSize({400.0f, 100.0f});
        m_liquid->setPosition(m_rod1->getPosition() + sf::Vector2f{m_rod1->getSize().x / 2.0f + 200.0f, 0.0f});
        m_liquid->setMaterial(physics::Materials::get("water"));
    }

    void onShow() override
    {
        m_propertiesButton->getLabel().setText(physics::Language::getTextSFML("properties"));
        m_massLabel->setText(physics::Language::getTextSFML("mass"));
        m_gravityAccelerationLabel->setText(physics::Language::getTextSFML("gravitational_acceleration"));
        m_airResistanceLabel->setText(physics::Language::getTextSFML("air_resistance"));
        m_frictionLabel->setText(physics::Language::getTextSFML("friction_coefficient"));
    }

    void onUpdate(float delta_time) override
    {
        physics::FrictionCoefficients::set("human", "concrete", m_frictionSlider->getValue());
        m_body->setMass(m_massSlider->getValue());
        m_body->setGravityAcceleration(m_gravityAccelerationSlider->getValue());
        
        // Demo

        m_bodyHandler->draw();
        m_bodyHandler->update(delta_time);

        // UI

        m_exitButton->update(delta_time);
        m_exitButton->draw();
        
        if(m_viewProperties)
        {
            m_body->drawForces();
            m_propertiesLayout->update(delta_time);
            m_propertiesLayout->draw();
        }

        m_propertiesButton->update(delta_time);
        m_propertiesButton->draw();

    }
private:
    bool m_viewProperties = false;
    physics::Button* m_exitButton;
    physics::VLayout* m_propertiesLayout;
    physics::Button* m_propertiesButton;

    physics::Label* m_massLabel;
    physics::Slider* m_massSlider;

    physics::Label* m_gravityAccelerationLabel;
    physics::Slider* m_gravityAccelerationSlider;

    physics::Label* m_airResistanceLabel;
    physics::Slider* m_airResistanceSlider;

    physics::Label* m_frictionLabel;
    physics::Slider* m_frictionSlider;

    physics::KinematicBody* m_body;
    physics::StaticBody* m_ground, *m_rod1, *m_rod2, *m_liquid;
    
    physics::BodyHandler* m_bodyHandler;
};