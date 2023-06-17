#include <Physics.hpp>
#include "TitleScreen.hpp"
#include "ShowcaseSelector.hpp"

class Demo : public physics::Application
{
public:
    void OnCreate() override
    {
        PushState(new ShowcaseSelector());
        PushState(new TitleScreen());
    }

    void OnUpdate(float dt) override {}
private:
};

int main()
{
    Demo application;
    return application.Start(sf::Vector2u{800, 600}, "");
}