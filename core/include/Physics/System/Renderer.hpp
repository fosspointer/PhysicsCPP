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
            void append(sf::Drawable* drawable) const
            {
                m_drawables.push_back(drawable);
            }

            void clear() const
            {
                m_drawables.clear();
            }

            inline size_t getDrawableCount() const 
            {
                return m_drawables.size();
            }
        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
            {
                for(const auto& drawable : m_drawables)
                    target.draw(*drawable);
                m_drawables.clear();
            }
            mutable std::vector<sf::Drawable*> m_drawables;
        };
        Renderer(sf::RenderWindow& window)
            :m_window(window)
        {}

        bool occupied(uint8_t index) const
        {
            return m_layers[index].getDrawableCount() > 0;
        }

        void append(sf::Drawable* drawable, uint8_t index) const
        {
            m_layers[index].append(drawable);
        }

        void clear(uint8_t index) const
        {
            m_layers[index].clear();
        }

        void clear() const
        {
            for(auto& layer : m_layers)
                layer.clear();
        }

        void draw() const
        {
            for(const auto& layer : m_layers)
                m_window.draw(layer);
        }
    private:
        mutable std::array<Layer, PHYSICS_LAYER_COUNT> m_layers;
        sf::RenderWindow& m_window;
    };
}