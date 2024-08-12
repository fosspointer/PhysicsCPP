#include <exception>
#include <Physics.hpp>
#include <Physics/System/Application.hpp>

namespace physics
{
    Application::Application()
        :m_backgroundColor(physics::Color::Black), m_deltaTime(0.0f), m_resized(true),
        m_renderer(m_window)
    {
        srand(time(0));
        Mouse::s_application = this;
        m_previousTime = m_currentTime = std::chrono::steady_clock::now();
    }

    Application::~Application()
    {
        while(!m_states.empty())
            popState();
    }

    int Application::start(const sf::Vector2u& size, const sf::String& title, sf::Uint32 style)
    {
        try
        {
            sf::ContextSettings settings;
            settings.antialiasingLevel = 8.0f;
            m_window.create(sf::VideoMode(size.x, size.y), title, style, settings);   

            Mouse::s_application = this;
            onCreate();

            while(m_window.isOpen()) [[likely]]
            {
                sf::Event e;
                while(m_window.pollEvent(e))
                {
                    if(e.type == sf::Event::Closed) [[unlikely]]
                        m_window.close();
                    else if(e.type == sf::Event::Resized)
                    {
                        sf::FloatRect visibleArea(0.0f, 0.0f, e.size.width, e.size.height);
                        m_window.setView(sf::View(visibleArea));
                        m_resized = true;
                    }
                    else [[likely]]
                        m_resized = false;
                }

                auto& mouse = Mouse::getInstance();

                mouse.previousState = mouse.currentState;

                mouse.currentState = sf::Mouse::isButtonPressed(sf::Mouse::Left) << 2
                    | sf::Mouse::isButtonPressed(sf::Mouse::Middle) << 1
                    | sf::Mouse::isButtonPressed(sf::Mouse::Right);

                mouse.clickState = mouse.previousState & ~mouse.currentState;

                m_previousTime = m_currentTime;
                m_currentTime = std::chrono::steady_clock::now();
                        
                m_deltaTime = std::chrono::duration<float>(m_currentTime - m_previousTime).count();    

                m_hasFloating = false;

                m_renderer.clear();
                m_window.clear(m_backgroundColor);
                
                onUpdate(m_deltaTime);
                
                if(!m_states.empty())
                {
                    if (m_states.top()->m_toBeDestroyed) [[unlikely]]
                        popState();
                    else [[likely]] 
                        m_states.top()->onUpdate(m_deltaTime);
                }

                m_renderer.draw();

                m_window.display();
            }
        }
        catch(const physics::Exception& e)
        {
            std::cerr << "[PHYSICS EXCEPTION] " << e << '\n';
            return RETURN_PHYSICS_EXCEPTION;
        }
        catch(const std::exception& e)
        {
            std::cerr << "[STANDARD EXCEPTION] " << e.what() << '\n';
            return RETURN_STD_EXCEPTION;
        }
        catch(...)
        {
            std::cerr << "[UNKNOWN EXCEPTION]\n";
            return RETURN_UNKNOWN_EXCEPTION;
        }
        
        return RETURN_SUCCESS;
    }

    auto Application::pushState(State* state) -> State*
    {
        m_statesUpdated = true;

        if(!m_states.empty())
            m_states.top()->onHide();
        m_states.push(state);
        state->m_application = this;
        state->onCreate();
        state->onShow();

        return state;
    }

    auto Application::changeState(State* state) -> State*
    {
        m_statesUpdated = true;

        if(!m_states.empty())
        {
            m_states.top()->onHide();
            m_states.top()->onDestroy();
            delete m_states.top();
            m_states.pop();
        }

        m_states.push(state);
        state->m_application = this;
        state->onCreate();
        state->onShow();

        return state;
    }

    void Application::popState()
    {
        m_statesUpdated = true;
        if(!m_states.empty())
        {
            m_states.top()->onDestroy();
            delete m_states.top();
            m_states.pop();
        }

        if(!m_states.empty())
            m_states.top()->onShow();
    }

    void Application::draw(sf::Drawable* drawable, int8_t layer)
    {
        m_renderer.append(drawable, layer);
    }

    void Application::setBackgroundColor(const sf::Color& color)
    {
        m_backgroundColor = color;
    }

    void Application::setFloating()
    {
        m_hasFloating = true;
    }

    void Application::rename(const sf::String& title)
    {
        m_window.setTitle(title);
    }
}