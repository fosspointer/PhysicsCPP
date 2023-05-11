#pragma once
#include <Physics/System/Application.hpp>
#include <Physics/System/NonCopyable.hpp>

#define MouseButton unsigned int

#define MOUSE_NONE 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2
#define MOUSE_LEFT 4

#define MOUSE_ANY 7
#define MOUSE_LEFT_OR_RIGHT 5

namespace physics
{
    class Mouse : public NonCopyable
    {
    public:
        static Mouse& GetInstance();
        static sf::Vector2f GetPosition();

        MouseButton CurrentState, PreviousState, ClickState;
    private:
        Mouse()
            :CurrentState(0), PreviousState(0)
        {}

        static Application* s_Application;
        friend class Application;
    };
}