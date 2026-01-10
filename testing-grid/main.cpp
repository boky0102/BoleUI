#include "grid.h"
#include "rect.h"
#include "types.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cstdint>
#include <iostream>

constexpr unsigned int SCREEN_WIDTH = 2400;
constexpr unsigned int SCREEN_HEIGHT = 1800;
constexpr uint32_t CELL_NUMBER = 150;
constexpr float CELL_WIDTH = static_cast<float>(SCREEN_WIDTH) / CELL_NUMBER;
constexpr unsigned int SCREEN_HEIGH_ADJUSTED = CELL_NUMBER * CELL_WIDTH;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGH_ADJUSTED}), "SFML window");
    window.setFramerateLimit(144);

    auto testGrid = TestGrid(CELL_NUMBER, CELL_WIDTH, window);

    auto testRect = Components::Rect({100, 50}, {10}, {30, 30});
    auto positions = testRect.GetPositions();
    auto transformedPos = std::vector<pos>{};
    for (const auto& p : positions) {
        std::cout << p.left << "  " << p.top << std::endl;
        transformedPos.push_back({p.left, p.top});
    }

    // auto circlePositions = std::vector<Components::Pos>();
    // for (int x = 25; x < 76; x++) {
    //     const auto pos = Components::GetCircleXPos(x, {50, 50}, CircleSide::Bottom, 25);
    //     circlePositions.push_back(pos);
    // }

    // for (int x = 25; x < 76; x++) {
    //     const auto pos = Components::GetCircleXPos(x, {50, 50}, CircleSide::Top, 25);
    //     circlePositions.push_back(pos);
    // }

    // auto transformedPos = std::vector<pos>{};
    // for (const auto& p : circlePositions) {
    //     std::cout << p.left << "  " << p.top << std::endl;
    //     transformedPos.push_back({p.left, p.top});
    // }

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        testGrid.Draw(transformedPos);

        // Update the window
        window.display();
    }
}
