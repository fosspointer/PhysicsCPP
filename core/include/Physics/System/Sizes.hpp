#pragma once
#include <Physics/System/Vector2.hpp>
#include <Physics/System/Box.hpp>

namespace physics
{
    class Sizes
    {
    public:
        static Box::Border borderButton()
        {
            return Box::Border{
                {18.0f, 16.0f}, {18.0f, 28.0f}
            };
        }

        static Box::Border borderButtonPressed()
        {
            return Box::Border{
                {18.0f, 23.0f}, {18.0f, 21.0f}
            };
        }

        static Box::Border borderDropdownHead()
        {
            return Box::Border{
                {17.0f, 17.0f}, {65.0f, 34.0f}
            };
        }

        static Box::Border borderDropdownHeadExpanded()
        {
            return Box::Border{
                {17.0f, 17.0f}, {65.0f, 34.0f}
            };
        }

        static Box::Border borderDropdownOption()
        {
            return Box::Border{
                {16.0f, 10.0f}, {16.0f, 17.0f}
            };
        }

        static Box::Border borderDropdownOptionLast()
        {
            return Box::Border{
                {18.0f, 12.0f}, {18.0f, 26.0f}
            };
        }
    };
}