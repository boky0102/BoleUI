#include "SFML/Graphics/RectangleShape.hpp"

#include "renderer.h"
#include "uielement.h"
#include "uitree.h"

#include "gtest/gtest.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <queue>
#include <utility>

class TestRenderer : public testing::Test {
  protected:
    TestRenderer()
    {
        m_tree = std::make_unique<UiTree>(Size{100, 200});

        auto m_child1 = std::make_unique<UiElement>("child-1", ElemType::Box);
        auto m_child2 = std::make_unique<UiElement>("child-2", ElemType::Box);
        auto m_child3 = std::make_unique<UiElement>("child-3", ElemType::Box);

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

        m_tree->GetRoot()->AddChild(std::move(m_child1));
        m_tree->GetRoot()->AddChild(std::move(m_child2));
        m_tree->GetRoot()->AddChild(std::move(m_child3));
    }

    std::unique_ptr<UiTree> m_tree;
    std::queue<UiElement*> m_rendererTraverseQue;
};

TEST_F(TestRenderer, TestRenderElements)
{
    ASSERT_EQ(m_tree->GetAllDescendantsBreathFirst().size(), 9);

    auto renderer = std::make_unique<Renderer>(m_rendererTraverseQue);
    auto rendererVector = renderer->GetDrawables(m_tree.get());

    EXPECT_EQ(rendererVector.size(), size_t(9));

    for (const auto& [_, elem] : rendererVector) {
        auto isBox = dynamic_cast<sf::RectangleShape*>(elem);
        EXPECT_NE(isBox, nullptr);
    }
}

TEST_F(TestRenderer, TestRenderElements_PreviosFrameTreePartRemoved)
{
    auto renderer = std::make_unique<Renderer>(m_rendererTraverseQue);

    // INITIAL FRAME
    auto rendererVector = renderer->GetDrawables(m_tree.get());
    ASSERT_EQ(rendererVector.size(), size_t(9));

    // NEXT FRAME
    m_tree->RemoveChild("child-1");
    rendererVector = renderer->GetDrawables(m_tree.get());
    EXPECT_EQ(rendererVector.size(), size_t(5));
}

TEST_F(TestRenderer, TestGetDrawables_MultipleFrames)
{
    auto renderer = std::make_unique<Renderer>(m_rendererTraverseQue);
    auto drawablesFirstFrame = renderer->GetDrawables(m_tree.get());

    const auto& child = m_tree->GetChild("child-1");
    child->properties.width = 999;

    auto drawablesSecondFrame = renderer->GetDrawables(m_tree.get());

    for (const auto& [name, drawable] : drawablesSecondFrame) {
        if (name == "child-1") {
            auto rect = dynamic_cast<sf::RectangleShape*>(drawable);
            ASSERT_NE(rect, nullptr);

            EXPECT_EQ(rect->getSize().x, 999);
        }
    }
}
