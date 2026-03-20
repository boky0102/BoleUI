#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "types.h"

constexpr size_t MAX_CHILDREN = 100;
constexpr size_t MAX_ALL_CHILDREN = 10000;

enum LayoutDirection { Horizontal, Vertical };

struct BoundingBox {
    float left = 0;
    float top = 0;
    float right = 0;
    float bottom = 0;
};

struct Position {
    float x;
    float y;
};

struct Color {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t alpha = 255;
};

// clang-format off
struct Properties {
    float               width = 0;
    float               height = 0;
    float               border_radius_px = 0;
    float               border_width = 0;
    bool                border = false;
    bool                hidden = false;
    LayoutDirection     layout_children = LayoutDirection::Horizontal;
    Position            position = {0, 0};
    Color               color = {0, 0, 0};
};
// clang-format on

// TO ADD: background color,  border, etc ...
class UiElement {
  public:
    explicit UiElement(const std::string& name, ElemType isText);

    ~UiElement() {}

    // Get the name of the ui element
    auto GetName() const -> const std::string&;

    // Adds a child to this element
    void AddChild(std::unique_ptr<UiElement> child);

    // Sets parent of the element
    void SetParent(UiElement* parent);

    // Get parent of the lement
    auto GetParent() -> UiElement*;

    // removes a child and all of its descendants from the tree
    // returns false if child not found
    bool RemoveChild(std::vector<UiElement*>& traversalBuffer, const std::string& childName);

    // removes immediate child from the element
    // returns false if immediate child not found
    bool RemoveImmediateChild(const std::string& childName);

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
    auto GetAllDescendantsBreathFirst(std::queue<UiElement*>& traversalBuffer) -> std::vector<UiElement*>;

    // Returns true if element has a child with a name
    bool HasChild(std::vector<UiElement*>& traversalBuffer, const std::string name);

    // Get child with specific name, returns nullptr if not found
    auto GetChild(std::vector<UiElement*>& traversalBuffer, const std::string name) -> UiElement*;

    auto GetElementType() -> ElemType;

    Properties properties;

  private:
    void RearrangeChildren();

    bool IsText();
    ElemType m_elementType;

    std::vector<std::unique_ptr<UiElement>> m_children;
    UiElement* m_parent;
    std::string m_name;
};
