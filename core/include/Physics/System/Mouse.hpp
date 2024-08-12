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
        static Mouse& getInstance();
        static sf::Vector2f getPosition();

        MouseButton currentState, previousState, clickState;
    private:
        Mouse()
            :currentState(0), previousState(0)
        {}

        static Application* s_application;
        friend class Application;
    };
}