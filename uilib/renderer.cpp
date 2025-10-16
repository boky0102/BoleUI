#include "renderer.h"
#include "types.h"
#include "uielement.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <memory>
#include <queue>
#include <utility>

Renderer::Renderer(std::queue<UiElement*>& traverseBuffer)
    : m_pTraversalBuffer{traverseBuffer}
{
}

auto Renderer::GetDrawables(UiElement* root) -> std::vector<std::pair<std::string, sf::Drawable*>>
{
    auto uiElements = root->GetAllDescendantsBreathFirst(m_pTraversalBuffer);

    // TODO: make it a buffer
    auto newRenderTree = std::vector<std::pair<std::string, sf::Drawable*>>{};

    for (const auto& elem : uiElements) {
        auto it = std::find_if(m_drawables.begin(), m_drawables.end(),
                               [elem](const auto& p) { return p.first == elem->GetName(); });

        // not found create new element
        if (it == m_drawables.end()) {
            const auto& newDtawable = CreateNewDrawable(elem);
            newRenderTree.emplace_back(std::move(newDtawable));
        }
    }

    m_drawables = newRenderTree;
    return newRenderTree;
}

auto Renderer::CreateNewDrawable(UiElement* element) -> std::pair<std::string, sf::Drawable*>
{
    switch (element->GetElementType()) {
    case ElemType::Box:
        auto newElem = std::make_unique<sf::RectangleShape>();
        m_rectangles.emplace_back(std::move(newElem));
        return {element->GetName(), m_rectangles.back().get()};
    }

    return {"", nullptr};
}
