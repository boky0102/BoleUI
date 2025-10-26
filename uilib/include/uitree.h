#include "uielement.h"
#include <memory>

// Convenience class for working with ui tree, makes a cleaner api
class UiTree {
  public:
    UiTree(const size_t traverseBufferCapacity);

    // removes a child and all of its descendants from the tree
    void RemoveChild(const std::string& elementName);

    // Get all children, returns empty vector if none present
    auto GetChildren(const std::string& elementName) const -> std::vector<UiElement*>;

    auto GetAllDescendantsDepthFirst() -> std::vector<UiElement*>;

    auto GetAllDescendantsBreathFirst() -> std::vector<UiElement*>;

    // Returns true if element has a child with a name
    bool HasChild(const std::string& name);

    // Get child with specific name
    auto GetChild(const std::string& name) -> UiElement*;

    auto GetElementType(const std::string& elementName) -> ElemType;

  private:
    std::unique_ptr<UiElement> m_root;
    std::vector<UiElement*> m_traverseBuffer;
};
