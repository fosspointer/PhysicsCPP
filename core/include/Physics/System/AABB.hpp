#pragma once
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <Physics/UI/UIElementAbstract.hpp>

namespace physics
{
    struct AABB
    {
    public:
        AABB(const sf::Vector2f& position, const sf::Vector2f& size);
        AABB(const UIElementAbstract* element);
        AABB(UIElementAbstract* element);
        AABB(const sf::RectangleShape& rectangle_shape);
        AABB();

        template <class T>
        AABB(const sf::Rect<T>& rectangle)
            :Position((float)rectangle.left + (float)rectangle.width / 2.0f, (float)rectangle.top + (float)rectangle.height / 2.0f),
            Size((float)rectangle.width, (float)rectangle.height)
        {}

        /**
        * @brief Function to test collision-intersection between a rectangle and a point
        * @param rect The rectangle, only position and size are accounted for
        * @param point The point
        * @return Boolean representing the collision test result
        */
        static bool RectangleToPoint(const AABB& rect, const sf::Vector2f& point);
        /**
        * @brief Function to test collision-intersection between two rectangles
        * @param rect The first rectangle, only position and size are accounted for
        * @param point The second rectangle, only position and size are accounted for
        * @return Boolean representing the collision test result
        */
        static bool RectangleToRectangle(const AABB& first, const AABB& second);

        sf::Vector2f Position, Size;
    };
}