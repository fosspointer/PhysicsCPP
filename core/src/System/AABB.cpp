#include <Physics/System/AABB.hpp>

namespace physics
{
    AABB::AABB(const sf::Vector2f& position, const sf::Vector2f& size)
        :Position(position), Size(size)
    {}

    AABB::AABB(const UIElementAbstract* element)
        :Position(element->GetPosition()), Size(element->GetSize())
    {}

    AABB::AABB(UIElementAbstract* element)
        :Position(element->GetPosition()), Size(element->GetSize())
    {}

    AABB::AABB(const sf::RectangleShape& rectangle_shape)
        :Position(rectangle_shape.getPosition() - rectangle_shape.getOrigin() + rectangle_shape.getSize() / 2.0f),
        Size(rectangle_shape.getSize())
    {}

    AABB::AABB()
        :Position(0.0f, 0.0f), Size(0.0f, 0.0f)
    {}

    bool AABB::RectangleToPoint(const AABB& rect, const sf::Vector2f& point)
    {
        return point.x > rect.Position.x - rect.Size.x / 2.0f
            && point.x < rect.Position.x + rect.Size.x / 2.0f
            && point.y > rect.Position.y - rect.Size.y / 2.0f
            && point.y < rect.Position.y + rect.Size.y / 2.0f;
    }
        
    bool AABB::RectangleToRectangle(const AABB& first, const AABB& second)
    {
        return first.Position.x + first.Size.x / 2.0f > second.Position.x - second.Size.x / 2.0f
            && first.Position.x - first.Size.x / 2.0f < second.Position.x + second.Size.x / 2.0f
            && first.Position.y + first.Size.y / 2.0f > second.Position.y - second.Size.y / 2.0f
            && first.Position.y - first.Size.y / 2.0f < second.Position.y + second.Size.y / 2.0f;
    }
}