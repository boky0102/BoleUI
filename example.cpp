#include "uitree.h"

void UiTree::RemoveChild(const std::string& elementName, bool added) {}

auto UiTree::GetChildren(const std::string& elementName) const -> std::vector<UiElement*> {}

auto UiTree::GetAllDescendantsDepthFirst() -> std::vector<UiElement*> {}

auto UiTree::GetAllDescendantsBreathFirst() -> std::vector<UiElement*> {}

bool UiTree::HasChild(const std::string& name) {}

auto UiTree::GetChild(const std::string& name) -> UiElement* {}

auto UiTree::GetElementType(const std::string& elementName) -> ElemType {}
