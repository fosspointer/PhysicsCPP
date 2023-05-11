#include <Physics/System/AABB.hpp>

bool physics::AABB::RectangleToPoint(const sf::RectangleShape &rect, const sf::Vector2f &point)
{
    return point.x > rect.getPosition().x && point.x < rect.getPosition().x + rect.getSize().x
        && point.y > rect.getPosition().y && point.y < rect.getPosition().y + rect.getSize().y;
}
    
bool physics::AABB::RectangleToRectangle(const sf::RectangleShape &first, const sf::RectangleShape &second)
{
    return first.getPosition().x + first.getSize().x > second.getPosition().x 
        && first.getPosition().x < second.getPosition().x + second.getSize().x
        && first.getPosition().y + first.getSize().y > second.getPosition().y 
        && first.getPosition().y < second.getPosition().y + second.getSize().y;
}