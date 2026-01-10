#include "renderer.h"
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

    // for (const auto& elem : uiElements) {
    //     auto it = std::find_if(m_drawables.begin(), m_drawables.end(),
    //                            [elem](const auto& p) { return p.first == elem->GetName(); });

    //     if (it != m_drawables.end()) {
    //         auto& [name, drawable] = *it;
    //         SyncProperties(elem, drawable);
    //         newRenderTree.push_back(*it);
    //     }

    //     // not found create new element
    //     if (it == m_drawables.end()) {
    //         const auto& newDtawable = CreateNewDrawable(elem);
    //         newRenderTree.emplace_back(std::move(newDtawable));
    //     }
    // }

    // assert(newRenderTree.size() != 0);

    // m_drawables = newRenderTree;
}

auto Renderer::CreateNewDrawable(UiElement* element) -> std::pair<std::string, sf::Drawable*>
{
    switch (element->GetElementType()) {
    case ElemType::Box: {
        auto newElem = std::make_unique<sf::RectangleShape>(
            sf::Vector2f{float(element->properties.width), float(element->properties.height)});
        const auto& elementColors = element->properties.color;
        newElem->setFillColor(
            {elementColors.red, elementColors.green, elementColors.blue, elementColors.alpha});
        newElem->setPosition({element->properties.position.x, element->properties.position.y});
        m_rectangles.emplace_back(std::move(newElem));

        return {element->GetName(), m_rectangles.back().get()};

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
