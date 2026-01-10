#include "uitree.h"
#include "types.h"
#include "uielement.h"
#include <memory>
#include <vector>

UiTree::UiTree(Size screenSize, const size_t traverseBufferCapacity)
    : m_root(std::make_unique<UiElement>("window", ElemType::Box))
    , m_traverseBuffer(traverseBufferCapacity)
    , m_traverseBufferQue()
{
    const auto& [width, height] = screenSize;
    m_root->properties.width = width;
    m_root->properties.height = height;
}

auto UiTree::GetRoot() -> UiElement* { return m_root.get(); }

void UiTree::RemoveChild(const std::string& elementName)
{
    m_root->RemoveChild(m_traverseBuffer, elementName);
}

auto UiTree::GetImmediateChildren(const std::string& elementName) -> std::vector<UiElement*>
{
    auto child = m_root->GetChild(m_traverseBuffer, "bla");
    if (child == nullptr) {
        return std::vector<UiElement*>{};
    }

    return child->GetAllChildren();
}

auto UiTree::GetAllDescendantsDepthFirst() -> std::vector<UiElement*>
{
    return m_root->GetAllDescendants(m_traverseBuffer);
}

auto UiTree::GetAllDescendantsBreathFirst() -> std::vector<UiElement*>
{
    return m_root->GetAllDescendantsBreathFirst(m_traverseBufferQue);
}

bool UiTree::HasChild(const std::string& name) { return m_root->HasChild(m_traverseBuffer, name); }

auto UiTree::GetChild(const std::string& name) -> UiElement*
{
    return m_root->GetChild(m_traverseBuffer, name);
}
