#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include "uielement.h"
#include "uitree.h"

// This class is responsible for storing sfml object representations of ui tree,
// efficiently rendering, removing and adding parts of the tree
// main purpose is to be the translation layer from ui element to the
// rendering framework.
//
// For example, data for bounding boxes is still kept separately in ui element, but
// here it is translated to the rendering libraries values, in this case sfml
// in the future it would be nice to make changing rendering libraries very
// easy by only providing transformation methods for necessary elements

class Renderer {

  public:
    Renderer(std::queue<UiElement*>& traversalBuffer);

    // Main api point of this class, it returns vector of drawables that
    // can be used to render sfml on the screen, idealy it should be
    // very efficient and not recreate elements when not necessary
    auto GetDrawables(UiTree* uiTree) -> std::vector<std::pair<std::string, sf::Drawable*>>;

  private:
    auto CreateNewDrawable(UiElement* element) -> std::pair<std::string, sf::Drawable*>;

    // Currently only sync position, size and color !
    void SyncProperties(UiElement* element, sf::Drawable* drawable);

    // Cleared on each iteration since it is cheap
    // and no extra allocation / deallocation is needed to do so
    std::vector<std::pair<std::string, sf::Drawable*>> m_drawables;

    // Owned ui elements which should not be re-allocated on each
    // render pass
    std::vector<std::unique_ptr<sf::RectangleShape>> m_rectangles;
    std::vector<std::unique_ptr<sf::Text>> m_textElements;
};
