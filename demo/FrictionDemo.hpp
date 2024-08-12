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

enum Mode
{
    Default, Properties, Bodies
};

class FrictionDemo : public physics::Application::State
{
public:
    ~FrictionDemo() noexcept
    {
        delete m_exitButton;
        delete m_propertiesLayout;
        delete m_bodyHandler;
        delete m_buttonLayout;
        delete m_controlLayout;
    }

    void onCreate() override
    {
        physics::Units::setPixelsPerMeter(150.0f / 1.7f);

        m_exitButton = new physics::Textured::Button(m_application);
        m_exitButton
            ->setTitle("X")
            ->setSize({50.0f, 60.0f})
            ->setButtonColors(physics::Color::DarkCyan)
            ->setAnchor(physics::Anchor::TopLeft);

        m_exitButton->addClickCallback([](physics::Application* app, physics::Textured::Button* btn, MouseButton)
        {
            app->getState()->setToBeDestroyed();
        });

        m_propertiesLayout = new physics::VLayout(m_application);
        m_propertiesLayout
            ->setBackground(physics::Color::White.changeAlpha(0.25f), 10.0f)
            ->setTexture(physics::Textures::button())
            ->setBorder(physics::Sizes::borderButton());

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

        m_buttonLayout = new physics::HLayout(m_application);
        m_buttonLayout->setAnchor(physics::Anchor::BottomRight);

        m_controlLayout = new physics::HLayout(m_application);
        m_controlLayout->setAnchor(physics::Anchor::BottomLeft)->pushElement(new physics::Textured::Button(m_application))
            ->setTitle("<")
            ->setButtonColors(physics::Color::DarkCyan)
            ->setSize({64.0f, 64.0f})
            ->addClickCallback([this](physics::Application*, physics::Textured::Button*, MouseButton)
            {
                m_moveLeftTimer = 0.0f;
            });

        m_controlLayout->pushElement(new physics::Textured::Button(m_application))
            ->setTitle("^")
            ->setButtonColors(physics::Color::DarkCyan)
            ->setSize({64.0f, 64.0f});

        m_controlLayout->pushElement(new physics::Textured::Button(m_application))
            ->setTitle(">")
            ->setButtonColors(physics::Color::DarkCyan)
            ->setSize({64.0f, 64.0f})
            ->addClickCallback([this](physics::Application*, physics::Textured::Button*, MouseButton)
            {
                m_moveRightTimer = 0.0f;
            });

        m_propertiesButton = m_buttonLayout->pushElement(new physics::Textured::Button(m_application))
            ->setButtonColors(physics::Color::DarkCyan)
            ->setSize({175.0f, 80.0f})
            ->addClickCallback([this](physics::Application*, physics::Textured::Button*, MouseButton)
            {
                m_mode != Mode::Properties? m_mode = Mode::Properties : m_mode = Mode::Default;
            });

        m_bodyButton = m_buttonLayout->pushElement(new physics::Textured::Button(m_application))
            ->setButtonColors(physics::Color::DarkCyan)
            ->setSize({175.0f, 80.0f})
            ->addClickCallback([this](physics::Application*, physics::Textured::Button*, MouseButton)
            {
                m_mode != Mode::Bodies? m_mode = Mode::Bodies : m_mode = Mode::Default;
            });

        m_bodyHandler = new physics::BodyHandler;
        
        m_body = m_bodyHandler->addKinematicBody(m_application, PHYSICS_ASSETS_DIR "images/person.png");
        m_body->setCollider({0.0f, 0.0f}, {20.0f, 150.0f});
        m_body->setUpdateCallback([this](physics::KinematicBody* body, float delta)
        {
            if(body->getFree()) return;

            auto left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || m_moveLeftTimer < 1.0f;
            auto right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || m_moveRightTimer < 1.0f;
            auto movement = right * 1.0f + left * -1.0f;

            auto jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
                || m_controlLayout->getElement(1)->isPressed();

            body->addForce({movement * 700.0f, 0.0f}, "F");

            if(jump)
                body->addMomentum(sf::Vector2f{0.0f, -20000.0f});
        });
        m_body->setMaterial(physics::Materials::get("human"));
    
        m_ground = m_bodyHandler->addStaticBody(m_application, physics::Color::DarkGray);
        m_ground->setSize({700, 300});
        m_ground->setPosition(m_body->getPosition() + sf::Vector2f{0.0f, 300.0f});
        m_ground->setMaterial(physics::Materials::get("concrete"));
    }

    void onShow() override
    {
        m_propertiesButton->getLabel().setText(physics::Language::getTextSFML("properties"));
        m_massLabel->setText(physics::Language::getTextSFML("mass"));
        m_gravityAccelerationLabel->setText(physics::Language::getTextSFML("gravitational_acceleration"));
        m_airResistanceLabel->setText(physics::Language::getTextSFML("air_resistance"));
        m_frictionLabel->setText(physics::Language::getTextSFML("friction_coefficient"));
        m_bodyButton->setTitle(physics::Language::getTextSFML("bodies"));
    }

    void onUpdate(float delta_time) override
    {
        m_moveLeftTimer += delta_time;
        m_moveRightTimer += delta_time;

        physics::FrictionCoefficients::set("human", "concrete", m_frictionSlider->getValue());
        m_body->setMass(m_massSlider->getValue());
        m_body->setGravityAcceleration(m_gravityAccelerationSlider->getValue());
        
        // Demo
        m_bodyHandler->draw();
        m_bodyHandler->update(delta_time);

        // UI
        m_exitButton->update(delta_time);
        m_exitButton->draw();
        
        if(m_mode == Mode::Properties)
        {
            m_bodyHandler->drawForces();
            m_propertiesLayout->update(delta_time);
            m_propertiesLayout->draw();
        }
        else if(m_mode == Mode::Bodies)
        {
            physics::TranslationControl control;
            control.setOrigin(m_body->getPosition());

            if(physics::Mouse::getInstance().currentState && !physics::Mouse::getInstance().previousState)
            {
                auto mouse_position = physics::Mouse::getPosition();
                if(control.collidesSquare(mouse_position))
                    m_mouseDistance = mouse_position - m_body->getPosition();
                
                else m_mouseDistance = physics::Vector2f{
                    control.collidesHorizontal(mouse_position)? mouse_position.x - m_body->getPosition().x: 0.0f, 
                    control.collidesVertical(mouse_position)? mouse_position.y - m_body->getPosition().y: 0.0f};
            }
            else if(!physics::Mouse::getInstance().currentState)
                m_mouseDistance = physics::Vector2f::zero();
            else if(physics::AABB::rectangleToPoint(m_body->getOutterAABB(), physics::Mouse::getPosition()))
                m_body->setVelocity(physics::Vector2f::zero());

            if(m_mouseDistance.x != 0.0f)
            {
                m_body->setPosition(physics::Vector2f{physics::Mouse::getPosition().x - m_mouseDistance.x, m_body->getPosition().y});
                control.setHorizontalColor(physics::Color::White);
            }
            if(m_mouseDistance.y != 0.0f)
            {
                m_body->setPosition(physics::Vector2f{m_body->getPosition().x, physics::Mouse::getPosition().y - m_mouseDistance.y});
                control.setVerticalColor(physics::Color::White);
            }

            if(m_mouseDistance.x != 0.0f && m_mouseDistance.y != 0.0f)
                control.setSquareColor(physics::Color::White);

            m_application->getWindow().draw(control);
        }

        m_buttonLayout->update(delta_time);
        m_buttonLayout->draw();

        m_controlLayout->update(delta_time);
        m_controlLayout->draw();
    }
private:
    float m_moveLeftTimer, m_moveRightTimer;

    physics::Vector2f m_mouseDistance;

    physics::Textured::Button* m_exitButton;

    physics::HLayout* m_buttonLayout, *m_controlLayout;
    
    physics::Textured::Button* m_propertiesButton, *m_bodyButton;
    
    physics::VLayout* m_propertiesLayout;

    physics::Label* m_massLabel;
    physics::Slider* m_massSlider;

    physics::Label* m_gravityAccelerationLabel;
    physics::Slider* m_gravityAccelerationSlider;

    physics::Label* m_airResistanceLabel;
    physics::Slider* m_airResistanceSlider;

    physics::Label* m_frictionLabel;
    physics::Slider* m_frictionSlider;

    physics::KinematicBody* m_body;
    physics::StaticBody* m_ground;
    
    physics::BodyHandler* m_bodyHandler;

    Mode m_mode{Mode::Default};
};