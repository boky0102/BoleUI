#include "SFML/Graphics/RectangleShape.hpp"

#include "renderer.h"
#include "uielement.h"
#include "gtest/gtest.h"
#include <SFML/Graphics/Drawable.hpp>
#include <memory>
#include <queue>

class TestRenderer : public testing::Test {
  protected:
    TestRenderer()
    {
        m_child1 = std::make_unique<UiElement>("child-1", ElemType::Box);
        m_child2 = std::make_unique<UiElement>("child-2", ElemType::Box);
        m_child3 = std::make_unique<UiElement>("child-3", ElemType::Box);

        m_parent = std::make_unique<UiElement>("parent", ElemType::Box);

        m_traversalBuffer.reserve(MAX_ALL_CHILDREN);
    }

    void BasicUiTreeSetup()
    {
        auto child12 = std::make_unique<UiElement>("child-12", ElemType::Box);
        auto child13 = std::make_unique<UiElement>("child-13", ElemType::Box);
        auto child14 = std::make_unique<UiElement>("child-14", ElemType::Box);
        m_child1->AddChild(std::move(child12));
        m_child1->AddChild(std::move(child13));
        m_child1->AddChild(std::move(child14));

        auto child31 = std::make_unique<UiElement>("child-31", ElemType::Box);
        auto child32 = std::make_unique<UiElement>("child-32", ElemType::Box);
        m_child3->AddChild(std::move(child31));
        m_child3->AddChild(std::move(child32));

        m_parent->AddChild(std::move(m_child1));
        m_parent->AddChild(std::move(m_child2));
        m_parent->AddChild(std::move(m_child3));
    }

    std::unique_ptr<UiElement> m_child1;
    std::unique_ptr<UiElement> m_child2;
    std::unique_ptr<UiElement> m_child3;

    std::unique_ptr<UiElement> m_parent;
    std::vector<UiElement*> m_traversalBuffer;
    std::queue<UiElement*> m_traversalBufferQueue;
};

TEST_F(TestRenderer, TestRenderElements)
{
    BasicUiTreeSetup();

    auto renderer = std::make_unique<Renderer>(m_traversalBufferQueue);
    auto rendererVector = renderer->GetDrawables(m_parent.get());

    EXPECT_EQ(rendererVector.size(), size_t(9));

    for (const auto& [_, elem] : rendererVector) {
        auto isBox = dynamic_cast<sf::RectangleShape*>(elem);
        EXPECT_NE(isBox, nullptr);
    }
}

TEST_F(TestRenderer, TestRenderElements_PreviosFrameTreePartRemoved)
{
    // m_child3->RemoveImmediateChildren();
}
