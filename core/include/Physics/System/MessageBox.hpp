#pragma once
#include <SFML/Graphics.hpp>
#include <Physics/System/Color.hpp>
#include <Physics/System/Font.hpp>

namespace physics
{
    /// @brief Message box class for displaying errors/warnings
    class MessageBox
    {
    public:
        
        /// @brief MessageBox class, creates window with message
        /// @param main_window a reference to the application's window
        /// @param title the title of the message box
        /// @param message the string representing the message
        /// @param margin margin to leave around the text, default recommended
         
        MessageBox(sf::RenderWindow& main_window, const sf::String& title, const sf::String& message, const sf::Vector2f& margin = sf::Vector2f{50.0f, 50.0f})
            :m_mainWindow(main_window), m_messageText(message, Font::getDefault().get(), 30), m_margin(margin), m_title(title)
        {}

        /// @brief displays message box and "pauses" execution until it the message box is closed
         
        void show()
        {
            m_messageBoxWindow.create(
                sf::VideoMode(m_messageText.getGlobalBounds().width + m_margin.x, m_messageText.getGlobalBounds().height + m_margin.y),
                m_title, sf::Style::Close);
            
            m_messageBoxWindow.setActive();
            m_mainWindow.setActive(false);

            m_messageBoxWindow.setPosition(sf::Mouse::getPosition());

            m_messageText.setOrigin(m_messageText.getGlobalBounds().width / 2.0f, m_messageText.getGlobalBounds().height / 2.0f);
            m_messageText.setPosition((sf::Vector2f)m_messageBoxWindow.getSize() / 2.0f);

            while(m_messageBoxWindow.isOpen())
            {
                sf::Event event;
                while(m_messageBoxWindow.pollEvent(event))
                {
                    if(event.type == sf::Event::Closed)
                        m_messageBoxWindow.close();
                    else if(event.type == sf::Event::LostFocus)
                        m_messageBoxWindow.requestFocus();
                }

                m_messageBoxWindow.clear(Color::Gray);
                m_messageBoxWindow.draw(m_messageText);
                m_messageBoxWindow.display();
            }

            m_mainWindow.requestFocus();
            m_mainWindow.setActive();
        }
    private:
        sf::Vector2f m_margin;
        sf::RenderWindow& m_mainWindow;
        sf::RenderWindow m_messageBoxWindow;
        sf::Text m_messageText;

        sf::String m_title;
    };
}