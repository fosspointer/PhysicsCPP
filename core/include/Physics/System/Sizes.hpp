#pragma once
#include <Physics/System/Vector2.hpp>
#include <Physics/System/Box.hpp>

namespace physics
{
    class Sizes
    {
    public:
        static Box::Border BorderButton()
        {
            return Box::Border{
                {18.0f, 16.0f}, {18.0f, 28.0f}
            };
        }

        static Box::Border BorderButtonPressed()
        {
            return Box::Border{
                {18.0f, 23.0f}, {18.0f, 21.0f}
            };
        }

        static Box::Border BorderDropdownHead()
        {
            return Box::Border{
                {17.0f, 17.0f}, {65.0f, 34.0f}
            };
        }

        static Box::Border BorderDropdownHeadExpanded()
        {
            return Box::Border{
                {17.0f, 17.0f}, {65.0f, 34.0f}
            };
        }

        static Box::Border BorderDropdownOption()
        {
            return Box::Border{
                {16.0f, 10.0f}, {16.0f, 17.0f}
            };
        }

        static Box::Border BorderDropdownOptionLast()
        {
            return Box::Border{
                {18.0f, 12.0f}, {18.0f, 26.0f}
            };
        }
    };
}