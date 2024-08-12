#pragma once

namespace physics
{
    /// @brief Provides constants and unit conversions
    class Units
    {
    public:
        static void setPixelsPerMeter(float value) { s_pixelsPerMeter = value; }
        static inline const float& getPixelsPerMeter() { return s_pixelsPerMeter; }

        static void setPixelsPerNewton(float value) { s_pixelsPerNewton = value; }
        static inline const float& getPixelsPerNewton() { return s_pixelsPerNewton; }

        static void setGravityAcceleration(float value) { s_gravityAcceleration = value; }
        static inline const float& getGravityAcceleration() { return s_gravityAcceleration; }


        static const float& PI() { return s_pI; }
        static const float& DegreesPerRadian() { return s_degreesPerRadian; }
    private:
        static float s_pixelsPerMeter, s_pixelsPerNewton, s_gravityAcceleration;
        const static float s_degreesPerRadian, s_pI;
    };
}