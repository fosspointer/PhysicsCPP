#pragma once
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <Physics/UI/UIElementAbstract.hpp>

namespace physics
{
    struct AABB
    {
    public:
        AABB(const sf::Vector2f& position, const sf::Vector2f& size)
            :Position(position), Size(size)
        {}

        AABB(const UIElementAbstract* element)
            :Position(element->GetPosition()), Size(element->GetSize())
        {}

        AABB(UIElementAbstract* element)
            :Position(element->GetPosition()), Size(element->GetSize())
        {}

        AABB(const sf::RectangleShape& rectangle_shape)
            :Position(rectangle_shape.getPosition() - rectangle_shape.getOrigin() + rectangle_shape.getSize() / 2.0f),
            Size(rectangle_shape.getSize())
        {}

        template <class T>
        AABB(const sf::Rect<T>& rectangle)
            :Position((float)rectangle.left + (float)rectangle.width / 2.0f, (float)rectangle.top + (float)rectangle.height / 2.0f),
            Size((float)rectangle.width, (float)rectangle.height)
        {}

        AABB()
            :Position(0.0f, 0.0f), Size(0.0f, 0.0f)
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