#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>
#include <Physics/System/NonCopyable.hpp>
#include <Physics/System/Renderer.hpp>

#define RETURN_SUCCESS 0
#define RETURN_GENERIC_FAILURE -1
#define RETURN_PHYSICS_EXCEPTION -2
#define RETURN_STD_EXCEPTION -3
#define RETURN_UNKNOWN_EXCEPTION -4

namespace physics
{
    /// @brief Main application class, to be overridden by the client
    class Application : public NonCopyable
    {
    public:
        Application();
        ~Application();

        class State
        {
        public:
            /// @brief State's create event, called when state is added to the stack
            virtual void onCreate() {}
            
            /// @brief State's update event, called every frame/window update
            /// @param delta_time Time elapsed since previous frame
            virtual void onUpdate(float delta_time) {}
            
            /// @brief State's show event, called when state re-reaches the top of the stack, as well as after every onCreate() call
            virtual void onShow() {}
            
            /// @brief State's hide event, called when the state becomes innactive-when another state is shown
            virtual void onHide() {}
            
            /// @brief State's destroy event, called before the state gets destructed
            virtual void onDestroy() {}
            
            /// @brief Schedule state to be destroyed, takes effect on the next frame
            /// @param setting 
            void setToBeDestroyed(bool setting = true) {m_toBeDestroyed = setting;}
        private:
            bool m_toBeDestroyed = false;
        protected:
            Application* m_application = nullptr;
            friend class Application;
        };
        
        /// @brief Application's create event, override to handle
        inline virtual void onCreate() {}
        
        /// @brief Application's update event, override to handle
        /// @param delta_time time elapsed since the previous frame
        inline virtual void onUpdate(float delta_time) {}
        
        
        /// @brief Starts the application
        /// @param size The initial window size for the application
        /// @param title The title of the window
        /// @param style The sfml style properties the window accepts (i.e. sf::Style::Close)
        /// @return The application's return value - to be returned by main()
        [[nodiscard("The return value of Application::Start() is to be returned by main()")]]
        int start(const sf::Vector2u& size, const sf::String& title, sf::Uint32 style = sf::Style::Default);
        
        /// @brief Renames the application window
        /// @param title The new title of the window
        void rename(const sf::String& title);

        /// @brief Pushes application state to the state-stack, as well as calling the required functions 
        /// @param state the state to be appended, should be used with new(), delete handled by the application
        /// @return State* the state provided as a parameter, for chaining purposes
        State* pushState(State* state);

        /// @brief Pops the state stack, then pushes the new state to it, as well as calling th required functions
        /// @param state the state to be appended, should be used with new(), delete handled by the application
        /// @return State* the state provided as a parameter, for chaining purposes
        State* changeState(State* state);

        /// @brief Pops the application state at the top of the state-stack, as well as calling the required function
        void popState();

        /// @brief Calls the renderer's method to append a drawable to the specified layer, with no default one provided
        /// @param drawable mutable pointer to the SFML drawable (see the sf::Drawable class)
        /// @param layer the layer to draw to (e.g. PHYSICS_LAYER_FOREGROUND_0)
        void draw(sf::Drawable* drawable, int8_t layer);
        
        /// @brief Get the Window object
        /// @return Mutable reference to the window
        inline sf::RenderWindow& getWindow() { return m_window; }
        
        /// @brief Get the State object at the top of the state-stack
        /// @return Mutable pointer to the state
        inline State* getState() { return m_states.top(); }

        /// @brief Returns true if the application state has been updated within the last frame
        /// @return Copy of the member boolean
        inline bool getStatesUpdated() const { return m_statesUpdated; }

        /// @brief Get the boolean corresponding to the window resize event
        /// @return Copy of the boolean
        inline bool getResized() const { return m_resized; }

        /// @brief Returns true if a 'floating' element is on screen, such as Dropdown lists, to discard any click events to
        // non floating elements
        /// @return Copy of the member boolean 
        inline bool hasFloating() const { return m_hasFloating; }
        
        /// @brief Get the Background Color object
        /// @return Immutable reference to the window's background color
        inline const sf::Color& getBackgroundColor() const { return m_backgroundColor; }

        /// @brief Get the Renderer object
        /// @return Immutable reference to the renderer
        inline const Renderer& getRenderer() const { return m_renderer; }

        /// @brief Set the background color of the application window
        /// @param color The color to use (SFML, 8-bit RGB)
        void setBackgroundColor(const sf::Color& color);

        /// @brief Set the 'HasFloating' state, so that any floating elements can own any mouse events
        void setFloating();
    private:
        Renderer m_renderer;
        bool m_resized{false}, m_statesUpdated{false}, m_hasFloating{false};
        std::stack<State*> m_states;
        sf::RenderWindow m_window;
        std::chrono::steady_clock::time_point m_previousTime, m_currentTime;
        float m_deltaTime{0.0f};
        sf::Color m_backgroundColor{sf::Color::Black};
    };
}