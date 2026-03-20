#include "SFML/Graphics.hpp"
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
#include <cstdint>
#include <memory>
#include <queue>
#include <rect.h>
#include <utils.h>

#include <plot_util.h>

#include "iostream"
#include "plot_area.h"

int main()
{
    assert(__cplusplus == 202302);
    {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 16;

        auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Bole UI Test", sf::Style::Default,
                                       sf::State::Windowed, settings);
        window.setFramerateLimit(144);

        auto uiTree = std::make_unique<UiTree>(Size(window.getSize().x, window.getSize().y));
        uiTree->GetRoot()->properties.color = {20, 10, 2};
        uiTree->GetRoot()->properties.layout_children = LayoutDirection::Horizontal;

        auto renderQue = std::queue<UiElement*>();
        auto renderer = std::make_unique<Renderer>(renderQue);

        uint32_t prevPosition = 100;

        // auto data = std::vector<double>{100, 500, 100, 500, 100, 500, 100, 500, 100, 500};
        auto data = std::vector<double>{100, 100, 100};

        auto plotArea = std::make_unique<PlotArea>(Pos{100, 1000}, 700, data, 2, 0);
        auto vertices = plotArea->GetVertexArray();

        int i = 0;

        std::cout << "Y is : " << GetInterpolatedPosY({1, 1}, {2, 4}, {3, 9}, 2.5).top << std::endl;

        while (window.isOpen()) {
            window.clear();

            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                else if (event->is<sf::Event::KeyPressed>()) {

                    auto newElem = std::make_unique<UiElement>(std::format("elem-{}", i), ElemType::Box);
                    newElem->properties.border_radius_px = 20;
                    newElem->properties.color = GetRandomLocalColor();
                    newElem->properties.layout_children = LayoutDirection::Horizontal;
                    newElem->properties.height = 100;
                    newElem->properties.width = 200;
                    newElem->properties.position = {10, 20};

                    if (i < 4) {
                        uiTree->GetRoot()->AddChild(std::move(newElem));
                    }
                    else {
                        auto elem = uiTree->GetChild("elem-0");

                        newElem->properties.width = 20;
                        newElem->properties.border_radius_px = 3;
                        elem->AddChild(std::move(newElem));
                    }

                    i++;
                }
            }

            auto drawables = renderer->GetDrawables(uiTree.get());
            for (const auto& [_, drawable] : drawables) {
                window.draw(*drawable);
            }

            window.draw(vertices);

            window.display();
        };

        return 0;
    }
}
