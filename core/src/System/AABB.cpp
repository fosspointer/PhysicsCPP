#include <Physics/System/AABB.hpp>

namespace physics
{
    AABB::AABB(const sf::Vector2f& position, const sf::Vector2f& size)
        :position(position), size(size)
    {}

    AABB::AABB(const UIElementAbstract* element)
        :position(element->getPosition()), size(element->getSize())
    {}

    AABB::AABB(UIElementAbstract* element)
        :position(element->getPosition()), size(element->getSize())
    {}

    AABB::AABB(const sf::RectangleShape& rectangle_shape)
        :position(rectangle_shape.getPosition() - rectangle_shape.getOrigin() + rectangle_shape.getSize() / 2.0f),
        size(rectangle_shape.getSize())
    {}

    AABB::AABB()
        :position(0.0f, 0.0f), size(0.0f, 0.0f)
    {}

    bool AABB::rectangleToPoint(const AABB& rect, const sf::Vector2f& point)
    {
        return point.x > rect.position.x - rect.size.x / 2.0f
            && point.x < rect.position.x + rect.size.x / 2.0f
            && point.y > rect.position.y - rect.size.y / 2.0f
            && point.y < rect.position.y + rect.size.y / 2.0f;
    }
        
    bool AABB::rectangleToRectangle(const AABB& first, const AABB& second)
    {
        return first.position.x + first.size.x / 2.0f > second.position.x - second.size.x / 2.0f
            && first.position.x - first.size.x / 2.0f < second.position.x + second.size.x / 2.0f
            && first.position.y + first.size.y / 2.0f > second.position.y - second.size.y / 2.0f
            && first.position.y - first.size.y / 2.0f < second.position.y + second.size.y / 2.0f;
    }
}