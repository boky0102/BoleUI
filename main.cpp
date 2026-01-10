#include "SFML/Graphics.hpp"
#include "iostream"
#include "renderer.h"
#include "types.h"
#include "uielement.h"
#include "uitree.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <memory>
#include <queue>
#include <rect.h>
#include <utils.h>

int main()
{
    assert(__cplusplus == 202302);
    {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 16;

        auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Bole UI Test",
                                       sf::Style::Default, sf::State::Windowed, settings);
        window.setFramerateLimit(144);

        auto uiTree = std::make_unique<UiTree>(Size(window.getSize().x, window.getSize().y));
        uiTree->GetRoot()->properties.color = {50, 50, 50};
        uiTree->GetRoot()->properties.layout_children = LayoutDirection::Horizontal;

        auto renderQue = std::queue<UiElement*>();
        auto renderer = std::make_unique<Renderer>(renderQue);

        auto testRecet = Components::Rect({1300, 700}, {50}, {100, 150});
        auto testConvex = testRecet.GetUnderlayingShape();
        testConvex.setFillColor(sf::Color::Cyan);

        while (window.isOpen()) {
            window.clear();

            uiTree->GetRoot()->properties.width = window.getSize().x;
            uiTree->GetRoot()->properties.height = window.getSize().y;

            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
            }

            window.draw(testConvex);
            window.display();
        };

        return 0;
    }
}
