#pragma once

#include <memory>
#include <queue>
#include <string>
#include <vector>

constexpr size_t MAX_CHILDREN = 100;
constexpr size_t MAX_ALL_CHILDREN = 10000;

class UiTree {};

// TO ADD: background color,  border, etc ...
class UiElement {
  public:
    explicit UiElement(std::string name);

    auto GetName() const -> const std::string&;

    // Adds a child to this element
    void AddChild(std::unique_ptr<UiElement> child);

    // Get all children, returns empty vector if none present
    auto GetAllChildren() const -> std::vector<UiElement*>;

    // travese the whole tree with calling element being a root
    // return an empty vector if there are no children
    auto GetAllDescendants(std::vector<UiElement*>& traverseBuffer) -> std::vector<UiElement*>;

    // travese the whole tree with calling element being a root
    // first goes into breadth then depth from left to right
    // if no children are found returns an empty vector
    auto GetAllDescendantsBreathFirst(std::queue<UiElement*>& traverseBuffer)
        -> std::vector<UiElement*>;

    // Returns true if element has a child with a name
    bool HasChild(std::vector<UiElement*>& traversalBuffer, const std::string name);

    // Get child with specific name
    auto GetChild(std::vector<UiElement*>& traversalBuffer, const std::string name) -> UiElement*;

  private:
    std::vector<std::unique_ptr<UiElement>> m_children;
    std::string m_name;
};
