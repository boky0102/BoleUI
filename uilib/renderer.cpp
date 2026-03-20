#include "renderer.h"
#include "rect.h"
#include "types.h"
#include "uielement.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <queue>
#include <utility>

Renderer::Renderer(std::queue<UiElement*>& traverseBuffer) {}

auto Renderer::GetDrawables(UiTree* root) -> std::vector<std::pair<std::string, sf::Drawable*>>
{
    auto uiElements = root->GetAllDescendantsBreathFirst();

    assert(uiElements.size() > 0);

    // TODO: make it a buffer
    auto newRenderTree = std::vector<std::pair<std::string, sf::Drawable*>>{};
    for (const auto& elem : uiElements) {
        newRenderTree.emplace_back(std::move(CreateNewDrawable(elem)));
    }

    return newRenderTree;
}

auto Renderer::CreateNewDrawable(UiElement* element) -> std::pair<std::string, sf::Drawable*>
{
    switch (element->GetElementType()) {
    case ElemType::Box: {

        const auto elementSize = Components::Size{element->properties.width, element->properties.height};
        const auto elementborder =
            Components::Border{element->properties.border_radius_px, element->properties.border_width};
        const auto elementPositon = Pos{element->properties.position.x, element->properties.position.y};

        auto newElem = std::make_unique<Components::Rect>(elementSize, elementborder, elementPositon);
        const auto& elementColors = element->properties.color;
        newElem->GetUnderlayingShape()->setFillColor(
            {elementColors.red, elementColors.green, elementColors.blue, elementColors.alpha});

        newElem->GetUnderlayingShape()->setPosition({element->properties.position.x, element->properties.position.y});

        m_rectangles.emplace_back(std::move(newElem));

        return {element->GetName(), m_rectangles.back()->GetUnderlayingShape()};

        break;
    }

    case ElemType::Text:
        [[fallthrough]];

    case ElemType::Window:
        [[fallthrough]];

    default:
        return {"", nullptr};
    }
}

void Renderer::SyncProperties(UiElement* element, sf::Drawable* drawable)
{
    switch (element->GetElementType()) {
    case ElemType::Box: {
        auto rect = dynamic_cast<sf::RectangleShape*>(drawable);
        const auto& elementColors = element->properties.color;
        rect->setSize({float(element->properties.width), float(element->properties.height)});
        rect->setFillColor({elementColors.red, elementColors.green, elementColors.blue});
        rect->setPosition({element->properties.position.x, element->properties.position.y});
        break;
    }

    case ElemType::Text:
        [[fallthrough]];

    case ElemType::Window:
        [[fallthrough]];

    default:
        break;
    }
}
