#pragma once
#include <cmath>
#include <SFML/Graphics/Color.hpp>
#include <Physics/System/Exception.hpp>

#define BRIGHT_DOM_HUE 168
#define BRIGHT_SEC_HUE 24

#define DARK_DOM_HUE 84
#define DARK_SEC_HUE 12

#define VALUE_100 255
#define VALUE_75 191
#define VALUE_50 128
#define VALUE_25 64
#define VALUE_0 0

#define NORMALIZE(component_uchar_255) (static_cast<float>(component_uchar_255) / 255.0f)
#define DENORMALIZE(component_float_1) (static_cast<uint8_t>(component_float_1 * 255.0f))

namespace physics
{
    struct Color
    {
        enum Type
        {
            RGB, HSV
        } type;

        union { float h; float r; };
        union { float s; float g; };
        union { float v; float b; };
        float a;

        explicit constexpr Color(uint32_t color)
            :r((color & 0xff000000) >> 24),
            g((color & 0x00ff0000) >> 16),
            b((color & 0x0000ff00) >> 8 ),
            a((color & 0x000000ff) >> 0 ),
            type(Type::RGB)
        {}

        constexpr Color(Type type = Type::RGB)
            :r{0.0f}, g{0.0f}, b{0.0f}, a{1.0f}, type(Type::RGB)
        {}

        constexpr Color(Type type, float first, float second, float third, float alpha = 1.0f)
            :type(type), r(first), g(second), b(third), a(alpha)
        {}

        constexpr Color FromRGB(float red, float green, float blue, float alpha = 1.0f) const
        {
            return Color(Type::RGB, red, green, blue, alpha);
        }

        constexpr Color FromHSV(float hue, float saturation, float value, float alpha = 1.0f) const
        {
            return Color(Type::HSV, hue, saturation, value, alpha);
        }

        const Color ToRGB() const
        {
            float hue, value, saturation, alpha;
            if(this->type == Type::RGB)
                throw PHYSICS_EXCEPTION("Color is already in RGB format; cannot convert it to RGB");

            float color_max = std::max(this->r, std::max(this->g, this->b));
            float color_min = std::min(this->r, std::min(this->g, this->b));
            float delta = color_max - color_min;

            if(color_max == this->r)
                hue = 60.0f * std::fmod(((this->g - this->b) / delta), 6.0f);
            else if(color_max == this->g)
                hue = 60.0f * ((this->b - this->r) / delta + 2.0f);
            else 
                hue = 60.0f * ((this->r - this->g) / delta + 4.0f);
            
            if(color_max == 0.0f)
                saturation = 0.0f;
            else saturation = delta / color_max;

            value = color_max;
            alpha = this->a;

            return Color(Type::HSV, hue, saturation, value, alpha);
        }

        constexpr Color(const sf::Color& color)
            :r(NORMALIZE(color.r)),
            g(NORMALIZE(color.g)),
            b(NORMALIZE(color.b)),
            a(NORMALIZE(color.a)),
            type(Type::RGB)
        {}

        constexpr operator sf::Color() const
        {
            Color color = type == Type::HSV ? this->ToRGB() : *this;
            return sf::Color(DENORMALIZE(color.r), DENORMALIZE(color.g), DENORMALIZE(color.b), DENORMALIZE(color.a));
        }

        Color ChangeAlpha(float alpha) const 
        {
            return Color(type, r, g, b, alpha);
        }

        Color MultiplyAlpha(float coefficient) const
        {
            return Color(type, r, g, b, a * coefficient);
        }

        static const Color 
            Red, Green, Blue, DarkRed, DarkGreen, DarkBlue, 
            Yellow, Cyan, Magenta, DarkYellow, DarkCyan, DarkMagenta,
            Black, DarkGray, Gray, LightGray, White;

    };
}