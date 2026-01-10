#pragma once

#include "uielement.h"
#include <X11/extensions/randr.h>
#include <memory>
#include <queue>

struct Size {
    uint16_t width;
    uint16_t height;
};

// Convenience class for working with ui tree elements, provides a cleaner api
class UiTree {
  public:
    explicit UiTree(Size screenSize, const size_t traverseBufferCapacity = MAX_ALL_CHILDREN);

    // returns root element of the ui tree
    auto GetRoot() -> UiElement*;

    // removes a child and all of its descendants from the tree
    void RemoveChild(const std::string& elementName);

    // Get all children, returns empty vector if none present
    auto GetImmediateChildren(const std::string& elementName) -> std::vector<UiElement*>;

    auto GetAllDescendantsDepthFirst() -> std::vector<UiElement*>;

    auto GetAllDescendantsBreathFirst() -> std::vector<UiElement*>;

    // Returns true if element has a child with a name
    bool HasChild(const std::string& name);

    // Get child with specific name, returns nullptr if not found
    auto GetChild(const std::string& name) -> UiElement*;

  private:
    std::unique_ptr<UiElement> m_root;
    std::vector<UiElement*> m_traverseBuffer;
    std::queue<UiElement*> m_traverseBufferQue;
};
