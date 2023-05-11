#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

namespace physics::AABB
{
    /**
     * @brief Function to test collision-intersection between a rectangle and a point
     * @param rect The rectangle, only position and size are accounted for
     * @param point The point
     * @return Boolean representing the collision test result
     */
    bool RectangleToPoint(const sf::RectangleShape& rect, const sf::Vector2f& point);
    /**
     * @brief Function to test collision-intersection between two rectangles
     * @param rect The first rectangle, only position and size are accounted for
     * @param point The second rectangle, only position and size are accounted for
     * @return Boolean representing the collision test result
     */
    bool RectangleToRectangle(const sf::RectangleShape& first, const sf::RectangleShape& second);
}