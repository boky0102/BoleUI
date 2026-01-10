#include "renderer.h"
#include "types.h"
#include "uielement.h"
#include "uitree.h"
#include "gtest/gtest.h"
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <queue>

class TestLayout : public testing::Test {
  protected:
    TestLayout()
        : m_uiTree(std::make_unique<UiTree>(Size(100, 100)))
        , m_renderQue()
        , m_renderer(std::make_unique<Renderer>(m_renderQue))
        , m_root(m_uiTree->GetRoot())
    {
    }

    void AddElement() { m_root->AddChild(std::make_unique<UiElement>("child", ElemType::Box)); }

    std::unique_ptr<UiTree> m_uiTree;
    std::queue<UiElement*> m_renderQue;
    std::unique_ptr<Renderer> m_renderer;
    UiElement* m_root;
};

TEST_F(TestLayout, TestLayoutSetup)
{
    ASSERT_EQ(m_root->properties.width, 100);
    ASSERT_EQ(m_root->properties.height, 100);
    ASSERT_EQ(m_root->properties.layout_children, LayoutDirection::Horizontal);
    ASSERT_EQ(m_root->properties.position.x, 0);
    ASSERT_EQ(m_root->properties.position.y, 0);
}

TEST_F(TestLayout, TestHoirzontalLayout_AddChild)
{
    m_root->properties.width = 30;

    AddElement();
    AddElement();
    AddElement();

    auto children = m_root->GetAllChildren();
    ASSERT_EQ(children.size(), 3);

    EXPECT_EQ(children[0]->properties.width, 10);
    EXPECT_EQ(children[1]->properties.width, 10);
    EXPECT_EQ(children[2]->properties.width, 10);

    EXPECT_EQ(children[0]->properties.position.x, 0);
    EXPECT_EQ(children[1]->properties.position.x, 10);
    EXPECT_EQ(children[2]->properties.position.x, 20);

    auto renderItems = m_renderer->GetDrawables(m_uiTree.get());
    ASSERT_EQ(renderItems.size(), 4);

    int i = 0;
    int pos = 0;
    for (const auto& [_, drawable] : renderItems) {
        if (i == 0)
            continue;

        auto rect = dynamic_cast<sf::RectangleShape*>(drawable);
        ASSERT_NE(rect, nullptr);

        EXPECT_EQ(rect->getPosition().x, pos);
        EXPECT_EQ(rect->getSize().x, 10);
        EXPECT_EQ(rect->getSize().y, 10);

        i++;
        pos += 10;
    }
}
