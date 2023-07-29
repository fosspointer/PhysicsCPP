#pragma once

namespace physics
{
    /// @brief Provides constants and unit conversions
    class Units
    {
    public:
        static void SetPixelsPerMeter(float value) { s_PixelsPerMeter = value; }
        static inline const float& GetPixelsPerMeter() { return s_PixelsPerMeter; }

        static void SetPixelsPerNewton(float value) { s_PixelsPerNewton = value; }
        static inline const float& GetPixelsPerNewton() { return s_PixelsPerNewton; }

        static void SetGravityAcceleration(float value) { s_GravityAcceleration = value; }
        static inline const float& GetGravityAcceleration() { return s_GravityAcceleration; }


        static const float& PI() { return s_PI; }
        static const float& DegreesPerRadian() { return s_DegreesPerRadian; }
    private:
        static float s_PixelsPerMeter, s_PixelsPerNewton, s_GravityAcceleration;
        const static float s_DegreesPerRadian, s_PI;
    };
}