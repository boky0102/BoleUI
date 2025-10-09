#include "renderer.h"
#include "uitree.h"
#include <SFML/Graphics/Drawable.hpp>

auto Renderer::GetDrawables(UiElement* root) -> std::vector<sf::Drawable*>
{
    return std::vector<sf::Drawable*>{};
}
