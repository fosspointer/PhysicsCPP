#include <Physics/System/AABB.hpp>

bool physics::AABB::RectangleToPoint(const sf::RectangleShape &rect, const sf::Vector2f &point)
{
    auto rect_actual_position = rect.getPosition() - rect.getOrigin();
    return point.x > rect_actual_position.x && point.x < rect_actual_position.x + rect.getSize().x
        && point.y > rect_actual_position.y && point.y < rect_actual_position.y + rect.getSize().y;
}
    
bool physics::AABB::RectangleToRectangle(const sf::RectangleShape &first, const sf::RectangleShape &second)
{
    auto first_actual_position = first.getPosition() - first.getOrigin();
    auto second_actual_position = second.getPosition() - second.getOrigin();
    return first_actual_position.x + first.getSize().x > second_actual_position.x 
        && first_actual_position.x < second_actual_position.x + second.getSize().x
        && first_actual_position.y + first.getSize().y > second_actual_position.y 
        && first_actual_position.y < second_actual_position.y + second.getSize().y;
}