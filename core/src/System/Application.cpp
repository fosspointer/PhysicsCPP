#include <exception>
#include <Physics.hpp>
#include <Physics/System/Application.hpp>

namespace physics
{
    Application::Application()
        :m_BackgroundColor(physics::Color::Black), m_DeltaTime(0.0f), m_Resized(true),
        m_Renderer(m_Window)
    {
        srand(time(0));
        Mouse::s_Application = this;
        m_PreviousTime = m_CurrentTime = std::chrono::steady_clock::now();
    }

    Application::~Application()
    {
        while(!m_States.empty())
            PopState();
    }

    int Application::Start(const sf::Vector2u& size, const sf::String& title, sf::Uint32 style)
    {
        try
        {
            m_Window.create(sf::VideoMode(size.x, size.y), title, style);   

            Mouse::s_Application = this;
            OnCreate();

            while(m_Window.isOpen()) [[likely]]
            {
                sf::Event e;
                while(m_Window.pollEvent(e))
                {
                    if(e.type == sf::Event::Closed) [[unlikely]]
                        m_Window.close();
                    else if(e.type == sf::Event::Resized)
                    {
                        sf::FloatRect visibleArea(0.0f, 0.0f, e.size.width, e.size.height);
                        m_Window.setView(sf::View(visibleArea));
                        m_Resized = true;
                    }
                    else [[likely]]
                        m_Resized = false;
                }

                auto& mouse = Mouse::GetInstance();

                mouse.PreviousState = mouse.CurrentState;

                mouse.CurrentState = sf::Mouse::isButtonPressed(sf::Mouse::Left) << 2
                    | sf::Mouse::isButtonPressed(sf::Mouse::Middle) << 1
                    | sf::Mouse::isButtonPressed(sf::Mouse::Right);

                mouse.ClickState = mouse.PreviousState & ~mouse.CurrentState;

                m_PreviousTime = m_CurrentTime;
                m_CurrentTime = std::chrono::steady_clock::now();
                        
                m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_PreviousTime).count();    

                m_Renderer.Clear();
                m_Window.clear(m_BackgroundColor);
                
                OnUpdate(m_DeltaTime);
                
                if(!m_States.empty())
                {
                    if (m_States.top()->m_ToBeDestroyed) [[unlikely]]
                        PopState();
                    else [[likely]] 
                        m_States.top()->OnUpdate(m_DeltaTime);
                }

                m_Renderer.Draw();

                m_Window.display();
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

    auto Application::PushState(State* state) -> State*
    {
        if(!m_States.empty())
            m_States.top()->OnHide();
        m_States.push(state);
        state->m_Application = this;
        state->OnCreate();
        state->OnShow();
        return state;
    }

    auto Application::ChangeState(State* state) -> State*
    {
        if(!m_States.empty())
        {
            m_States.top()->OnHide();
            m_States.top()->OnDestroy();
            delete m_States.top();
            m_States.pop();
        }

        m_States.push(state);
        state->m_Application = this;
        state->OnCreate();
        state->OnShow();

        return state;
    }

    void Application::Draw(sf::Drawable* drawable, int8_t layer)
    {
        m_Renderer.Append(drawable, layer);
    }

    void Application::PopState()
    {
        if(!m_States.empty())
        {
            m_States.top()->OnDestroy();
            delete m_States.top();
            m_States.pop();
        }

        if(!m_States.empty())
            m_States.top()->OnShow();
    }

    void Application::SetBackgroundColor(const sf::Color& color)
    {
        m_BackgroundColor = color;
    }

    void Application::Rename(const sf::String& title)
    {
        m_Window.setTitle(title);
    }
}