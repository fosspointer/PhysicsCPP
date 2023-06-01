#include <Physics/System/AABB.hpp>

bool physics::AABB::RectangleToPoint(const AABB& rect, const sf::Vector2f& point)
{
    return point.x > rect.Position.x - rect.Size.x / 2.0f
        && point.x < rect.Position.x + rect.Size.x / 2.0f
        && point.y > rect.Position.y - rect.Size.y / 2.0f
        && point.y < rect.Position.y + rect.Size.y / 2.0f;
}
    
bool physics::AABB::RectangleToRectangle(const AABB& first, const AABB& second)
{
    return first.Position.x + first.Size.x / 2.0f > second.Position.x - second.Size.x / 2.0f
        && first.Position.x - first.Size.x / 2.0f < second.Position.x + second.Size.x / 2.0f
        && first.Position.y + first.Size.y / 2.0f > second.Position.y - second.Size.y / 2.0f
        && first.Position.y - first.Size.y / 2.0f < second.Position.y + second.Size.y / 2.0f;
}