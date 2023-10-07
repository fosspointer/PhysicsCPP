#pragma once
#include <map>
#include <cstdint>
#include <SFML/Graphics.hpp>

#define PHYSICS_LAYER_BACKGROUND_0 0
#define PHYSICS_LAYER_BACKGROUND_1 1
#define PHYSICS_LAYER_BACKGROUND_2 2
#define PHYSICS_LAYER_BACKGROUND_3 3

#define PHYSICS_LAYER_SHOWCASE_0 4
#define PHYSICS_LAYER_SHOWCASE_1 5
#define PHYSICS_LAYER_SHOWCASE_2 6
#define PHYSICS_LAYER_SHOWCASE_3 7

#define PHYSICS_LAYER_FOREGROUND_0 8
#define PHYSICS_LAYER_FOREGROUND_1 9
#define PHYSICS_LAYER_FOREGROUND_2 10
#define PHYSICS_LAYER_FOREGROUND_3 11

#define PHYSICS_LAYER_UI_0 12
#define PHYSICS_LAYER_UI_1 13
#define PHYSICS_LAYER_UI_2 14
#define PHYSICS_LAYER_UI_3 15

#define PHYSICS_LAYER_FLOATING_0 16
#define PHYSICS_LAYER_FLOATING_1 17
#define PHYSICS_LAYER_FLOATING_2 18
#define PHYSICS_LAYER_FLOATING_3 19

#define PHYSICS_LAYER_CUSTOM_0 20
#define PHYSICS_LAYER_CUSTOM_1 21
#define PHYSICS_LAYER_CUSTOM_2 22
#define PHYSICS_LAYER_CUSTOM_3 23

#define PHYSICS_LAYER_COUNT 24

namespace physics
{
    class Renderer
    {
    public:
        class Layer : public sf::Drawable
        {
        public:
            void Append(sf::Drawable* drawable) const
            {
                m_Drawables.push_back(drawable);
            }

            void Clear() const
            {
                m_Drawables.clear();
            }

            inline size_t GetDrawableCount() const 
            {
                return m_Drawables.size();
            }
        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
            {
                for(const auto& drawable : m_Drawables)
                    target.draw(*drawable);
                m_Drawables.clear();
            }
            mutable std::vector<sf::Drawable*> m_Drawables;
        };
        Renderer(sf::RenderWindow& window)
            :m_Window(window)
        {}

        bool Occupied(uint8_t index) const
        {
            return m_Layers[index].GetDrawableCount() > 0;
        }

        void Append(sf::Drawable* drawable, uint8_t index) const
        {
            m_Layers[index].Append(drawable);
        }

        void Clear(uint8_t index) const
        {
            m_Layers[index].Clear();
        }

        void Clear() const
        {
            for(auto& layer : m_Layers)
                layer.Clear();
        }

        void Draw() const
        {
            for(const auto& layer : m_Layers)
                m_Window.draw(layer);
        }
    private:
        mutable std::array<Layer, PHYSICS_LAYER_COUNT> m_Layers;
        sf::RenderWindow& m_Window;
    };
}