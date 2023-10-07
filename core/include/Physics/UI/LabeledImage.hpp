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
            m_Image = new physics::Image(application, filepath);
            m_Label = new physics::Label(application, label, font_size);
            UpdateSize();
            UpdatePositions();
        }

        bool IsHovered() const override
        {
            return AABB::RectangleToPoint(this, Mouse::GetPosition());
        }

        void CustomUpdate(float delta_time) override
        {
            if(!m_Application->GetResized() && !m_PragmaUpdated && !m_Image->GetPragmaUpdated() && !m_Label->GetPragmaUpdated()) return;

            UpdateSize();
            UpdatePositions();

            m_Image->SetColor(m_Color);

            m_Image->Update(delta_time);
            m_Label->Update(delta_time);
        }

        void Draw(int8_t layer = PHYSICS_LAYER_UI_2) override
        {
            m_Image->Draw(layer);
            m_Label->Draw(layer);
        }

        LabeledImage* SetShader(sf::Shader* shader)
        {
            m_Image->SetShader(shader);
            return this;
        }

        template <typename T>
        LabeledImage* SetUniform(const std::string& name, T value)
        {
            m_Image->SetUniform(name, value);
            return this;
        }

        inline Label* GetLabel() { return m_Label; }
        inline const Label* GetLabel() const { return m_Label; }

        inline Image* GetImage() { return m_Image; }
        inline const Image* GetImage() const { return m_Image; }
    private:
        void UpdatePositions()
        {
            m_Image->SetPosition(sf::Vector2f{m_Position.x, m_Position.y - m_Size.y / 2.0f + m_Image->GetSize().y / 2.0f});
            m_Label->SetPosition(sf::Vector2f{m_Position.x, m_Image->GetPosition().y + m_Image->GetSize().y / 2.0f + m_Image->GetMargin().y / 2.0f + m_Label->GetSize().y / 2.0f});
        }

        void UpdateSize()
        {
            m_Size.x = std::max(m_Image->GetSize().x, m_Label->GetSize().x);
            m_Size.y = m_Image->GetSize().y + m_Label->GetSize().y + m_Image->GetMargin().y / 2.0f + m_Label->GetMargin().y / 2.0f;
        }

        Image* m_Image;
        Label* m_Label;
    };
}