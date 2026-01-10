#include "grid.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <stdexcept>

TestGrid::TestGrid(const uint64_t gridSize, const float cellSize, sf::RenderWindow& window)
    : m_window(&window)
    , m_size(gridSize)
    , m_cellSize(cellSize)
{
    auto outlineColor = sf::Color::White;
    outlineColor.a = 45;

    for (uint32_t y = 0; y < gridSize; y++) {
        auto tmpVector = std::vector<sf::RectangleShape>{};
        tmpVector.reserve(gridSize);
        for (uint32_t x = 0; x < gridSize; x++) {
            auto shape = sf::RectangleShape({m_cellSize, m_cellSize});
            shape.setPosition({x * m_cellSize, static_cast<float>(y * m_cellSize)});
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineThickness(1);
            shape.setOutlineColor(outlineColor);
            tmpVector.push_back(shape);
        }
        m_grid.push_back((tmpVector));
    }
}

void TestGrid::Draw(std::vector<pos> positions)
{
    assert(m_grid[0].size() == m_size && "Row size needs to equal grid size");
    assert(m_grid.size() == m_size && "Row count needs to equal grid size");

    m_window->clear();

    auto color = sf::Color::Cyan;
    for (const auto& pos : positions) {
        if (pos.y >= m_grid.size() || pos.x >= m_grid.size()) {
            throw std::runtime_error("Cant handle positions outside of the grid");
            continue;
        }
        m_grid[pos.y][pos.x].setFillColor(color);
    }

    for (const auto& row : m_grid) {
        for (const auto& elem : row) {
            m_window->draw(elem);
        }
    }

    counter++;
}
