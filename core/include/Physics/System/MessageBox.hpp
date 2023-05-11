#pragma once
#include <SFML/Graphics.hpp>
#include <Physics/System/Colors.hpp>
#include <Physics/System/Font.hpp>

namespace physics
{
    class MessageBox
    {
    public:
        /**
         * @brief MessageBox class, creates window with message
         * @param main_window a reference to the application's window
         * @param title the title of the message box
         * @param message the string representing the message
         * @param margin margin to leave around the text, default recommended
         */
        MessageBox(sf::RenderWindow& main_window, const sf::String& title, const sf::String& message, const sf::Vector2f& margin = sf::Vector2f{50.0f, 50.0f})
            :m_MainWindow(main_window), m_MessageText(message, Font::GetDefault().Get(), 30), m_Margin(margin), m_Title(title)
        {}

        /**
         * @brief displays message box and "pauses" execution until it the message box is closed
         */
        void Show()
        {
            m_MessageBoxWindow.create(
                sf::VideoMode(m_MessageText.getGlobalBounds().width + m_Margin.x, m_MessageText.getGlobalBounds().height + m_Margin.y),
                m_Title, sf::Style::Close);
            
            m_MessageBoxWindow.setActive();
            m_MainWindow.setActive(false);

            m_MessageBoxWindow.setPosition(sf::Mouse::getPosition());

            m_MessageText.setOrigin(m_MessageText.getGlobalBounds().width / 2.0f, m_MessageText.getGlobalBounds().height / 2.0f);
            m_MessageText.setPosition((sf::Vector2f)m_MessageBoxWindow.getSize() / 2.0f);

            while(m_MessageBoxWindow.isOpen())
            {
                sf::Event event;
                while(m_MessageBoxWindow.pollEvent(event))
                {
                    if(event.type == sf::Event::Closed)
                        m_MessageBoxWindow.close();
                    else if(event.type == sf::Event::LostFocus)
                        m_MessageBoxWindow.requestFocus();
                }

                m_MessageBoxWindow.clear(Colors::Gray);
                m_MessageBoxWindow.draw(m_MessageText);
                m_MessageBoxWindow.display();
            }

            m_MainWindow.requestFocus();
            m_MainWindow.setActive();
        }
    private:
        sf::Vector2f m_Margin;
        sf::RenderWindow& m_MainWindow;
        sf::RenderWindow m_MessageBoxWindow;
        sf::Text m_MessageText;

        sf::String m_Title;
    };
}