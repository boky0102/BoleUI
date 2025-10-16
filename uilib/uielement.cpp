#include "format"
#include "types.h"
#include "uielement.h"
#include <cassert>
#include <memory>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <vector>

UiElement::UiElement(std::string name, ElemType elementType)
    : m_name(name)
    , m_elemetType(elementType)
{
    m_children.reserve(MAX_CHILDREN);
};

auto UiElement::GetName() const -> const std::string& { return m_name; }

void UiElement::AddChild(std::unique_ptr<UiElement> child)
{
    assert(m_children.size() <= MAX_CHILDREN);

    m_children.emplace_back(std::move(child));
}

auto UiElement::GetAllChildren() const -> std::vector<UiElement*>
{
    if (m_children.size() == size_t(0)) {
        return std::vector<UiElement*>{};
    }

    auto transformed =
        m_children | std::views::transform([](const auto& child) { return child.get(); });
    return std::vector(transformed.begin(), transformed.end());
}

auto UiElement::GetAllDescendants(std::vector<UiElement*>& traversalBuffer)
    -> std::vector<UiElement*>
{
    if (m_children.size() == size_t(0)) {
        return std::vector<UiElement*>{};
    }

    if (traversalBuffer.size() > MAX_ALL_CHILDREN ||
        traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(
            std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
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

auto UiElement::GetAllDescendantsBreathFirst(std::queue<UiElement*>& traversalBuffer)
    -> std::vector<UiElement*>
{

    if (m_children.size() == size_t(0)) {
        return std::vector<UiElement*>{};
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
    if (traversalBuffer.size() > MAX_ALL_CHILDREN ||
        traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(
            std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
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

auto UiElement::GetChild(std::vector<UiElement*>& traversalBuffer, const std::string name)
    -> UiElement*
{
    if (traversalBuffer.size() > MAX_ALL_CHILDREN ||
        traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(
            std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
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

auto UiElement::GetParent(std::vector<UiElement*>& traversalBuffer, UiElement* root) -> UiElement*
{
    if (traversalBuffer.size() > MAX_ALL_CHILDREN ||
        traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(
            std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
                        MAX_ALL_CHILDREN, traversalBuffer.capacity()));
    }

    traversalBuffer.clear();
    traversalBuffer.push_back(root);

    while (!traversalBuffer.empty()) {
        auto elem = traversalBuffer.back();
        traversalBuffer.pop_back();

        for (const auto& child : elem->GetAllChildren()) {
            if (child->GetName() == m_name) {
                return elem;
            }

            traversalBuffer.push_back(child);
        }
    }

    return nullptr;
}

bool UiElement::IsText() { return m_elemetType == ElemType::Text; }

auto UiElement::GetElementType() -> ElemType { return m_elemetType; }

void UiElement::RemoveChild(std::vector<UiElement*>& traversalBuffer, const std::string& childName)
{
    if (m_children.size() == size_t(0)) {
        return;
    }

    if (traversalBuffer.size() > MAX_ALL_CHILDREN ||
        traversalBuffer.capacity() != MAX_ALL_CHILDREN) {
        throw std::runtime_error(
            std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}",
                        MAX_ALL_CHILDREN, traversalBuffer.capacity()));
    }

    assert(traversalBuffer.capacity() == MAX_ALL_CHILDREN);
    traversalBuffer.clear();

    auto child = GetChild(traversalBuffer, childName);
}

void UiElement::RemoveImmediateChildren() { m_children.clear(); }
