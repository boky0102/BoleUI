#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include "uielement.h"

class Renderer {

    // This class is responsible for storing sfml object representations of ui tree,
    // efficiently rendering, removing and adding parts of the tree
  public:
    Renderer(std::queue<UiElement*>& traversalBuffer);
    // Main api point of this class, it returns vector of drawables that
    // can be used to render sfml on the screen, idealy it should be
    // very efficient and not recreate elements when not necessary
    auto GetDrawables(UiElement* root) -> std::vector<std::pair<std::string, sf::Drawable*>>;

  private:
    auto CreateNewDrawable(UiElement* element) -> std::pair<std::string, sf::Drawable*>;

    std::queue<UiElement*>& m_pTraversalBuffer;

    // Cleared on each iteration since it is cheap
    // and no extra allocation / deallocation is needed to do so
    std::vector<std::pair<std::string, sf::Drawable*>> m_drawables;

    std::vector<std::unique_ptr<sf::RectangleShape>> m_rectangles;
    std::vector<std::unique_ptr<sf::Text>> m_textElements;
};
