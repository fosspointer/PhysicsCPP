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
            virtual void OnCreate() {}
            
            /// @brief State's update event, called every frame/window update
            /// @param delta_time Time elapsed since previous frame
            virtual void OnUpdate(float delta_time) {}
            
            /// @brief State's show event, called when state re-reaches the top of the stack, as well as after every OnCreate() call
            virtual void OnShow() {}
            
            /// @brief State's hide event, called when the state becomes innactive-when another state is shown
            virtual void OnHide() {}
            
            /// @brief State's destroy event, called before the state gets destructed
            virtual void OnDestroy() {}
            
            /// @brief Schedule state to be destroyed, takes effect on the next frame
            /// @param setting 
            void SetToBeDestroyed(bool setting = true) {m_ToBeDestroyed = setting;}
        private:
            bool m_ToBeDestroyed = false;
        protected:
            Application* m_Application = nullptr;
            friend class Application;
        };
        
        /// @brief Application's create event, override to handle
        inline virtual void OnCreate() {}
        
        /// @brief Application's update event, override to handle
        /// @param delta_time time elapsed since the previous frame
        inline virtual void OnUpdate(float delta_time) {}
        
        
        /// @brief Starts the application
        /// @param size The initial window size for the application
        /// @param title The title of the window
        /// @param style The sfml style properties the window accepts (i.e. sf::Style::Close)
        /// @return The application's return value - to be returned by main()
        [[nodiscard("The return value of Application::Start() is to be returned by main()")]]
        int Start(const sf::Vector2u& size, const sf::String& title, sf::Uint32 style = sf::Style::Default);
        
        /// @brief Renames the application window
        /// @param title The new title of the window
        void Rename(const sf::String& title);

        /// @brief Pushes application state to the state-stack, as well as calling the required functions 
        /// @param state the state to be appended, should be used with new(), delete handled by the application
        /// @return State* the state provided as a parameter, for chaining purposes
        State* PushState(State* state);
        State* ChangeState(State* state);

        /// @brief Calls the renderer's method to append a drawable to the specified layer, with no default one provided
        /// @param drawable mutable pointer to the SFML drawable (see the sf::Drawable class)
        /// @param layer the layer to draw to (e.g. PHYSICS_LAYER_FOREGROUND_0)
        void Draw(sf::Drawable* drawable, int8_t layer);

        /// @brief Pops the application state at the top of the state-stack, as well as calling the required function
        void PopState();
        
        /// @brief Get the Window object
        /// @return Mutable reference to the window
        inline sf::RenderWindow& GetWindow() { return m_Window; }
        
        /// @brief Get the State object at the top of the state-stack
        /// @return Mutable pointer to the state
        inline State* GetState() { return m_States.top(); }
        
        /// @brief Get the boolean corresponding to the window resize event
        /// @return Copy of the boolean
        inline bool GetResized() const { return m_Resized; }

        /// @brief Returns true if a 'floating' element is on screen, such as Dropdown lists, to discard any click events to
        // non floating elements
        /// @return Copy of the member boolean 
        inline bool HasFloating() const { return m_HasFloating; }
        
        /// @brief Get the Background Color object
        /// @return Immutable reference to the window's background color
        inline const sf::Color& GetBackgroundColor() const { return m_BackgroundColor; }

        /// @brief Get the Renderer object
        /// @return Immutable reference to the renderer
        inline const Renderer& GetRenderer() const { return m_Renderer; }

        /// @brief Set the background color of the application window
        /// @param color The color to use (SFML, 8-bit RGB)
        void SetBackgroundColor(const sf::Color& color);

        /// @brief Set the 'HasFloating' state, so that any floating elements can own any mouse events
        void SetFloating();
    private:
        Renderer m_Renderer;
        bool m_Resized{false}, m_HasFloating{false};
        std::stack<State*> m_States;
        sf::RenderWindow m_Window;
        std::chrono::steady_clock::time_point m_PreviousTime;
        std::chrono::steady_clock::time_point m_CurrentTime;
        float m_DeltaTime{0.0f};
        sf::Color m_BackgroundColor = sf::Color::Black;
    };
}