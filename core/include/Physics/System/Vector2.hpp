#pragma once
#include <SFML/System/Vector2.hpp>
#include <type_traits>
#include <cmath>
#include <iostream>

namespace physics
{
    template <typename T>
    struct Vector2
    {
        T x, y;

        template <typename U>
        Vector2(const sf::Vector2<U>& sfml_vector)
        {
            if constexpr(std::is_same<T, U>::value)
            {
                x = sfml_vector.x;
                y = sfml_vector.y;
            }
            else
            {
                x = static_cast<T>(sfml_vector.x);
                y = static_cast<T>(sfml_vector.y);
            }
        }

        template <typename U>
        Vector2(const Vector2& other)
        {
            if constexpr(std::is_same<T, U>::value)
            {
                x = other.x;
                y = other.y;
            }
            else
            {
                x = static_cast<T>(other.x);
                y = static_cast<T>(other.y);
            }
        }

        explicit Vector2(T value = 0)
            :x(value), y(value)
        {}

        Vector2(T x, T y)
            :x(x), y(y)
        {}

        static const Vector2& zero()
        {
            static const Vector2 _zero{0, 0};
            return _zero; 
        }

        static const Vector2& one()
        {
            static const Vector2 _one{1, 1};
            return _one;
        }

        static constexpr Vector2 X(T value)
        {
            return Vector2(value, 0);
        }

        static constexpr Vector2 Y(T value)
        {
            return Vector2(0, value);
        }

        template <typename U>
        constexpr operator sf::Vector2<U>() const
        {
            if constexpr (std::is_same<T, U>::value)
                return sf::Vector2<U>(x, y);
            else return sf::Vector2<U>(static_cast<U>(x), static_cast<U>(y));
        }

        explicit constexpr operator bool() const
        {
            std::cout << "asdf\n";
            if constexpr (std::is_same<T, float>::value)
                return *this != Vector2{-1, -1};
            else return *this == Vector2{static_cast<T>(-1), static_cast<T>(-1)};
        }

        constexpr T dot(const Vector2& other) const
        {
            return x * other.x + y * other.y;
        }

        constexpr T dot() const
        {
            return x * x + y * y;
        }

        constexpr T length() const
        {
            return std::sqrt(dot());
        }

        constexpr Vector2 abs() const
        {
            return Vector2{std::abs(x), std::abs(y)};
        }

        constexpr Vector2 clamp(const Vector2 min, const Vector2 max) const
        {
            return Vector2{std::clamp(x, min.x, max.x), std::clamp(y, min.y, max.y)};
        }

        constexpr Vector2 normalize() const
        {
            return *this / length();
        }

        constexpr Vector2 operator*() const
        {
            return std::sqrt(dot());
        }

        constexpr Vector2 operator-() const
        {
            return Vector2{-x, -y};
        }

        constexpr Vector2 operator+(const Vector2& other) const
        {
            return Vector2{x + other.x, y + other.y};
        }

        constexpr Vector2 operator-(const Vector2& other) const
        {
            return Vector2{x - other.x, y - other.y};
        }

        constexpr Vector2 operator*(const Vector2& other) const
        {
            return Vector2{x * other.x, y * other.y};
        }

        constexpr Vector2 operator/(const Vector2& other) const
        {
            return Vector2{x / other.x, y / other.y};
        }

        constexpr Vector2 operator*(T scalar) const
        {
            return Vector2{x * scalar, y * scalar};
        }

        constexpr Vector2 operator/(T scalar) const
        {
            return Vector2{x / scalar, y / scalar};
        }

        bool operator==(const Vector2& other) const
        {
            return x == other.x && y == other.y;
        }
        
        bool operator!=(const Vector2& other) const
        {
            return x != other.x || y != other.y;
        }

        Vector2& operator+=(const Vector2& other)
        {
            *this = *this + other;
            return *this;
        }

        Vector2& operator-=(const Vector2& other)
        {
            *this = *this - other;
            return *this;
        }

        Vector2& operator*=(T scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        Vector2& operator/=(T scalar)
        {
            *this = *this / scalar;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& vector)
        {
            os << '[' << vector.x << ", " << vector.y << ']';
            return os;
        }

        friend std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& vector)
        {
            os << '[' << vector.x << ", " << vector.y << ']';
            return os;
        }
    };

    using Vector2f32 = Vector2<float>;
    using Vector2f64 = Vector2<double>;
    
    using Vector2b = Vector2<bool>;

    using Vector2f = Vector2f32;
    using Vector2d = Vector2f64;

    using Vector2i8 = Vector2<int8_t>;
    using Vector2i16 = Vector2<int16_t>;
    using Vector2i32 = Vector2<int32_t>;
    using Vector2i64 = Vector2<int64_t>;
    using Vector2i = Vector2i32;

    using Vector2u8 = Vector2<uint8_t>;
    using Vector2u16 = Vector2<uint16_t>;
    using Vector2u32 = Vector2<uint32_t>;
    using Vector2u64 = Vector2<uint64_t>;
    using Vector2u = Vector2u32;
}