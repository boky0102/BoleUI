#include "uitree.h"
#include <cassert>
#include <memory>
#include <vector>
#include <ranges>
#include "format"

UiElement::UiElement(std::string name) : m_name(name) {
    m_children.reserve(MAX_CHILDREN);
};

auto UiElement::GetName() const -> const std::string&
{
    return m_name;
}

void UiElement::AddChild(std::unique_ptr<UiElement> child) {
    assert(m_children.size() <= MAX_CHILDREN);

    m_children.emplace_back(std::move(child));
}

auto UiElement::GetAllChildren() const -> std::vector<UiElement*> {
    if(m_children.size() == size_t(0)){
        return std::vector<UiElement*>{};
    }

    auto transformed = m_children | std::views::transform([](const auto& child){ return child.get();});
    return std::vector(transformed.begin(), transformed.end());
}

auto UiElement::GetAllDescendants(std::vector<UiElement*>& traversalBuffer) -> std::vector<UiElement*> {
    if(m_children.size() == size_t(0)){
        return std::vector<UiElement*>{};
    }

    if(traversalBuffer.size() > MAX_ALL_CHILDREN || traversalBuffer.capacity() != MAX_ALL_CHILDREN){
        throw std::runtime_error(std::format("Ui tree traversal buffer - too big allocation, max - {}, real - {}", MAX_ALL_CHILDREN, traversalBuffer.capacity()));
    }

    assert(traversalBuffer.capacity() == MAX_ALL_CHILDREN);
    traversalBuffer.clear();
    traversalBuffer.push_back(this);

    std::vector<UiElement*> elements;
    while(!traversalBuffer.empty()){
        auto elem = traversalBuffer.back();
        elements.push_back(elem);
        traversalBuffer.pop_back();

        auto children = elem->GetAllChildren();
        for (const auto& child : children){
            traversalBuffer.push_back(child);
        }
    }

    return elements;
}
