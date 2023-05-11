#include <Physics/System/Mouse.hpp>
#include <Physics/System/Application.hpp>

physics::Application* physics::Mouse::s_Application = nullptr;

sf::Vector2f physics::Mouse::GetPosition()
{
    return (sf::Vector2f)sf::Mouse::getPosition(s_Application->GetWindow());
}

auto physics::Mouse::GetInstance() -> Mouse&
{
    static Mouse instance;
    return instance;
}