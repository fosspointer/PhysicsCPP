#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <Physics/UI/UIElementAbstract.hpp>

namespace physics
{
    /// @brief An axis-alined bounding box collider (AABB) that's centered at its position
    struct AABB
    {
    public:
        /// @brief AABB collider from two vectors  
        /// @param position The position-center of the collider
        /// @param size The size of the collider
        AABB(const sf::Vector2f& position, const sf::Vector2f& size);

        /// @brief AABB collider from immutable UIElement
        /// @param element The element to be used 
        AABB(const UIElementAbstract* element);

        /// @brief AABB collider from mutable UIElement
        /// @param element The element to be used 
        AABB(UIElementAbstract* element);

        /// @brief AABB collider from sfml rectangle shape
        /// @param rectangle_shape 
        AABB(const sf::RectangleShape& rectangle_shape);
        
        /// @brief Default AABB collider constructor
        AABB();

        /// @brief AABB collider from sfml rectangle 
        /// @tparam T the type of the rectangle
        /// @param rectangle The rectangle
        template <class T>
        AABB(const sf::Rect<T>& rectangle)
            :Position((float)rectangle.left + (float)rectangle.width / 2.0f, (float)rectangle.top + (float)rectangle.height / 2.0f),
            Size((float)rectangle.width, (float)rectangle.height)
        {}

        /// @brief Function to test collision-intersection between a rectangle and a point
        /// @param rect The rectangle, only position and size are accounted for
        /// @param point The point
        /// @return Boolean representing the collision test result
        static bool RectangleToPoint(const AABB& rect, const sf::Vector2f& point);

        ///@brief Function to test collision-intersection between two rectangles
        ///@param rect The first rectangle, only position and size are accounted for
        ///@param point The second rectangle, only position and size are accounted for
        ///@return Boolean representing the collision test result
        static bool RectangleToRectangle(const AABB& first, const AABB& second);

        sf::Vector2f Position, Size;
    };
}