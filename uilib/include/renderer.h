#pragma once

#include <memory>
#include <vector>

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include "uitree.h"

class Renderer {

    // This class is responsible for storing sfml object representations of ui tree,
    // efficiently rendering, removing and adding parts of the tree
  public:
    // Main api point of this class, it returns vector of drawables that
    // can be used to render sfml on the screen, idealy it should be
    // very efficient and not recreate elements when not necessary
    auto GetDrawables(UiElement* root) -> std::vector<sf::Drawable*>;

  private:
    // aggregation all elements -> must keep the same order of elements as UiElement
    std::vector<sf::Drawable*> m_drawables;

    std::vector<std::unique_ptr<sf::RectangleShape>> m_rectangles;
    std::vector<std::unique_ptr<sf::Text>> m_textElements;
};
