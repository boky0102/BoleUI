#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <cstdint>

struct pos {
    uint32_t x;
    uint32_t y;
};

class TestGrid {
  public:
    TestGrid(const uint64_t gridSize, const float cellSize, sf::RenderWindow& window);
    void Draw(std::vector<pos> positions);

  private:
    sf::RenderWindow* m_window;
    uint64_t m_size;
    std::vector<std::vector<sf::RectangleShape>> m_grid;
    float m_cellSize;
    int counter = 0;
};
