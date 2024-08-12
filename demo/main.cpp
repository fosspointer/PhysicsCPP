#include <Physics.hpp>
#include "TitleScreen.hpp"
#include "ShowcaseSelector.hpp"

class Demo : public physics::Application
{
public:
    void onCreate() override
    {
        pushState(new ShowcaseSelector());
        pushState(new TitleScreen());
    }

    void onUpdate(float dt) override {}
private:
};

int main()
{
    Demo application;
    return application.start(sf::Vector2u{800, 600}, "");
}