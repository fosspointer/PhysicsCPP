#pragma once
#include <Physics/UI/UIElement.hpp>
#include <Physics/UI/Image.hpp>
#include <Physics/UI/VLayout.hpp>

namespace physics
{
    class LabeledImage : public UIElement<LabeledImage> 
    {
    public:
        LabeledImage(Application* application, const sf::String& filepath, const sf::String& label, unsigned char font_size = 20u)
            :UIElement(application)
        {
            m_image = new physics::Image(application, filepath);
            m_label = new physics::Label(application, label, font_size);
            updateSize();
            updatePositions();
        }

        bool isHovered() const override
        {
            return AABB::rectangleToPoint(this, Mouse::getPosition());
        }

        void customUpdate(float delta_time) override
        {
            if(!m_application->getResized() && !m_pragmaUpdated && !m_image->getPragmaUpdated() && !m_label->getPragmaUpdated()) return;

            updateSize();
            updatePositions();

            m_image->setColor(m_color);

            m_image->update(delta_time);
            m_label->update(delta_time);
        }

        void draw(int8_t layer = PHYSICS_LAYER_UI_2) override
        {
            m_image->draw(layer);
            m_label->draw(layer);
        }

        LabeledImage* setShader(sf::Shader* shader)
        {
            m_image->setShader(shader);
            return this;
        }

        template <typename T>
        LabeledImage* setUniform(const std::string& name, T value)
        {
            m_image->setUniform(name, value);
            return this;
        }

        inline Label* getLabel() { return m_label; }
        inline const Label* getLabel() const { return m_label; }

        inline Image* getImage() { return m_image; }
        inline const Image* getImage() const { return m_image; }
    private:
        void updatePositions()
        {
            m_image->setPosition(sf::Vector2f{m_position.x, m_position.y - m_size.y / 2.0f + m_image->getSize().y / 2.0f});
            m_label->setPosition(sf::Vector2f{m_position.x, m_image->getPosition().y + m_image->getSize().y / 2.0f + m_image->getMargin().y / 2.0f + m_label->getSize().y / 2.0f});
        }

        void updateSize()
        {
            m_size.x = std::max(m_image->getSize().x, m_label->getSize().x);
            m_size.y = m_image->getSize().y + m_label->getSize().y + m_image->getMargin().y / 2.0f + m_label->getMargin().y / 2.0f;
        }

        Image* m_image;
        Label* m_label;
    };
}