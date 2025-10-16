#pragma once

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "types.h"

constexpr size_t MAX_CHILDREN = 100;
constexpr size_t MAX_ALL_CHILDREN = 10000;

class UiTree {};

// TO ADD: background color,  border, etc ...
class UiElement {
  public:
    explicit UiElement(std::string name, ElemType isText);

    ~UiElement() {}

    // Get the name of the ui element
    auto GetName() const -> const std::string&;

    // Adds a child to this element
    void AddChild(std::unique_ptr<UiElement> child);

    // removes a child and all of its descendants from the tree
    void RemoveChild(std::vector<UiElement*>& traversalBuffer, const std::string& childName);

    // remove immediate children of the ui element, immediate children need to
    // have 0 children, otherwise it throws
    void RemoveImmediateChildren();

    // Get all children, returns empty vector if none present
    auto GetAllChildren() const -> std::vector<UiElement*>;

    // travese the whole tree with calling element being a root
    // return an empty vector if there are no children
    auto GetAllDescendants(std::vector<UiElement*>& traversalBuffer) -> std::vector<UiElement*>;

    // travese the whole tree with calling element being a root
    // first goes into breadth then depth from left to right
    // if no children are found returns an empty vector
    auto GetAllDescendantsBreathFirst(std::queue<UiElement*>& traversalBuffer)
        -> std::vector<UiElement*>;

    // Returns true if element has a child with a name
    bool HasChild(std::vector<UiElement*>& traversalBuffer, const std::string name);

    // Get child with specific name
    auto GetChild(std::vector<UiElement*>& traversalBuffer, const std::string name) -> UiElement*;

    // Get parent of the element, returns null if no parent found (it should never be the case
    // except for the root element)
    auto GetParent(std::vector<UiElement*>& traversalBuffer, UiElement* root) -> UiElement*;

    auto GetElementType() -> ElemType;

  private:
    bool IsText();
    ElemType m_elemetType;

    std::vector<std::unique_ptr<UiElement>> m_children;
    std::string m_name;
};
