#include "uielement.h"
#include "format"
#include "types.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <print>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <vector>

UiElement::UiElement(const std::string& name, ElemType elementType)
    : m_name(name)
    , m_elementType(elementType)
    , m_parent(nullptr)
{
    m_children.reserve(MAX_CHILDREN);
};

auto UiElement::GetName() const -> const std::string&
{
    return m_name;
}

void UiElement::SetParent(UiElement* parent)
{
    m_parent = parent;
}

auto UiElement::GetParent() -> UiElement*
{
    return m_parent;
}

void UiElement::AddChild(std::unique_ptr<UiElement> child)
{
    assert(m_children.size() <= MAX_CHILDREN);
    child->SetParent(this);
    m_children.emplace_back(std::move(child));

    RearrangeChildren();
}

auto UiElement::GetAllChildren() const -> std::vector<UiElement*>
{
    if (m_children.size() == size_t(0)) {
        return std::vector<UiElement*>{};
    }

    auto transformed = m_children | std::views::transform([](const auto& child) { return child.get(); });
    return std::vector(transformed.begin(), transformed.end());
}

auto UiElement::GetAllDescendants(std::vector<UiElement*>& traversalBuffer) -> std::vector<UiElement*>
{
    if (m_children.size() == size_t(0)) {
        return std::vector<UiElement*>{};
    }

    if (traversalBuffer.size() > MAX_ALL_CHILDREN || traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
                                             MAX_ALL_CHILDREN, traversalBuffer.capacity()));
    }

    assert(traversalBuffer.capacity() == MAX_ALL_CHILDREN);
    traversalBuffer.clear();
    traversalBuffer.push_back(this);

    std::vector<UiElement*> elements;
    while (!traversalBuffer.empty()) {
        auto elem = traversalBuffer.back();
        elements.push_back(elem);
        traversalBuffer.pop_back();

        auto children = elem->GetAllChildren();
        for (const auto& child : children) {
            traversalBuffer.push_back(child);
        }
    }

    return elements;
}

auto UiElement::GetAllDescendantsBreathFirst(std::queue<UiElement*>& traversalBuffer) -> std::vector<UiElement*>
{
    if (m_children.size() == size_t(0)) {
        return std::vector<UiElement*>{this};
    }

    traversalBuffer.push(this);
    std::vector<UiElement*> elements;

    while (!traversalBuffer.empty()) {
        auto elem = traversalBuffer.front();
        elements.push_back(elem);
        traversalBuffer.pop();

        auto children = elem->GetAllChildren();
        auto reversedChildren = std::ranges::reverse_view(children);

        for (const auto& elem : reversedChildren) {
            traversalBuffer.push(elem);
        }
    }

    return elements;
}

bool UiElement::HasChild(std::vector<UiElement*>& traversalBuffer, const std::string childName)
{
    if (traversalBuffer.size() > MAX_ALL_CHILDREN || traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
                                             MAX_ALL_CHILDREN, traversalBuffer.capacity()));
    }

    assert(traversalBuffer.capacity() == MAX_ALL_CHILDREN);
    traversalBuffer.clear();
    traversalBuffer.push_back(this);

    while (!traversalBuffer.empty()) {
        auto elem = traversalBuffer.back();
        if (elem->GetName() == childName) {
            return true;
        }

        traversalBuffer.pop_back();

        auto children = elem->GetAllChildren();
        for (const auto& child : children) {
            traversalBuffer.push_back(child);
        }
    }

    return false;
}

auto UiElement::GetChild(std::vector<UiElement*>& traversalBuffer, const std::string name) -> UiElement*
{
    if (traversalBuffer.size() > MAX_ALL_CHILDREN || traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
                                             MAX_ALL_CHILDREN, traversalBuffer.capacity()));
    }

    traversalBuffer.clear();
    traversalBuffer.push_back(this);

    while (!traversalBuffer.empty()) {
        auto elem = traversalBuffer.back();
        traversalBuffer.pop_back();

        for (const auto& child : elem->GetAllChildren()) {
            if (child->GetName() == name) {
                return child;
            }

            traversalBuffer.push_back(child);
        }
    }

    return nullptr;
}

bool UiElement::IsText()
{
    return m_elementType == ElemType::Text;
}

auto UiElement::GetElementType() -> ElemType
{
    return m_elementType;
}

bool UiElement::RemoveChild(std::vector<UiElement*>& traversalBuffer, const std::string& childName)
{
    if (m_children.size() == size_t(0)) {
        return false;
    }

    auto element = GetChild(traversalBuffer, childName);
    auto parent = element->GetParent();

    return parent->RemoveImmediateChild(childName);
}

bool UiElement::RemoveImmediateChild(const std::string& childName)
{
    return std::erase_if(m_children,
                         [childName](std::unique_ptr<UiElement>& child) { return child->GetName() == childName; });
}

void UiElement::RemoveImmediateChildren()
{
    m_children.clear();
}

void UiElement::RearrangeChildren()
{
    if (properties.layout_children == LayoutDirection::Horizontal) {
        const auto parentPosition = properties.position;
        const auto nChildren = m_children.size();
        const auto totalSpace = properties.width;
        const auto childSpace = totalSpace / nChildren;

        int i = 0;
        for (const auto& child : m_children) {
            child->properties.width = childSpace;

            const auto childPosX = parentPosition.x + i * childSpace;
            child->properties.position = {childPosX, parentPosition.y};

            std::println("width : {} pos : {}", child->properties.width, childPosX);

            i++;
        }

        std::cout << "===================== " << std::endl;
    }
}
