#include <Physics/System/Mouse.hpp>
#include <Physics/System/Application.hpp>

physics::Application* physics::Mouse::s_application = nullptr;

sf::Vector2f physics::Mouse::getPosition()
{
    return (sf::Vector2f)sf::Mouse::getPosition(s_application->getWindow());
}

auto physics::Mouse::getInstance() -> Mouse&
{
    static Mouse instance;
    return instance;
}