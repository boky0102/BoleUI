#include "uitree.h"
#include "utils.h"
#include "gtest/gtest.h"
#include <chrono>
#include <cstdint>
#include <memory>
#include <queue>
#include <string>

class UiTreeTest : public testing::Test {
  protected:
    UiTreeTest()
    {
        m_child1 = std::make_unique<UiElement>("child-1");
        m_child2 = std::make_unique<UiElement>("child-2");
        m_child3 = std::make_unique<UiElement>("child-3");

        m_parent = std::make_unique<UiElement>("parent");

        m_traversalBuffer.reserve(MAX_ALL_CHILDREN);
    }

    void BasicUiTreeSetup()
    {
        auto child12 = std::make_unique<UiElement>("child-12");
        auto child13 = std::make_unique<UiElement>("child-13");
        auto child14 = std::make_unique<UiElement>("child-14");
        m_child1->AddChild(std::move(child12));
        m_child1->AddChild(std::move(child13));
        m_child1->AddChild(std::move(child14));

        auto child31 = std::make_unique<UiElement>("child-31");
        auto child32 = std::make_unique<UiElement>("child-32");
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
};

TEST_F(UiTreeTest, GetAllChildren)
{
    m_parent->AddChild(std::move(m_child1));
    m_parent->AddChild(std::move(m_child2));
    m_parent->AddChild(std::move(m_child3));

    auto children = m_parent->GetAllChildren();
    EXPECT_EQ(children.size(), size_t(3));
    EXPECT_EQ(children[0]->GetName(), "child-1");
    EXPECT_EQ(children[1]->GetName(), "child-2");
    EXPECT_EQ(children[2]->GetName(), "child-3");
}

TEST_F(UiTreeTest, GetAllDescendants)
{
    BasicUiTreeSetup();

    auto stackBuffer = std::vector<UiElement*>{};
    stackBuffer.reserve(MAX_ALL_CHILDREN);

    ASSERT_EQ(stackBuffer.capacity(), MAX_ALL_CHILDREN);

    auto allElements = m_parent->GetAllDescendants(stackBuffer);

    // check if all are unique
    uint16_t numberOfSameElements = 0;
    for (const auto& elem : allElements) {
        for (const auto& other : allElements) {
            if (elem == other) {
                numberOfSameElements++;
                continue;
            }

            EXPECT_NE(elem->GetName(), other->GetName());
        }
    }

    EXPECT_EQ(numberOfSameElements, 9);
    EXPECT_EQ(allElements.size(), 9);
}

TEST_F(UiTreeTest, GetAllDescendantsRandomized)
{
    uint16_t total = 1;

    auto currentChildren = std::vector<UiElement*>{m_parent.get()};

    while (total < MAX_ALL_CHILDREN - 101) {
        auto randomNum = generateRandomNum(1, 10);
        auto nextLevelChildren = std::vector<UiElement*>{};

        bool full = false;

        for (auto& child : currentChildren) {
            for (int i = 0; i < randomNum; i++) {
                auto newChild = std::make_unique<UiElement>("child");
                nextLevelChildren.push_back(newChild.get());
                child->AddChild(std::move(newChild));
                total++;

                if (total >= MAX_ALL_CHILDREN - 101) {
                    full = true;
                    break;
                }
            }

            if (full)
                break;
        }

        if (full)
            break;

        currentChildren = nextLevelChildren;
    }

    auto traverseBuffer = std::vector<UiElement*>{};
    traverseBuffer.reserve(MAX_ALL_CHILDREN);

    auto t1 = std::chrono::high_resolution_clock::now();
    auto allDescendants = m_parent->GetAllDescendants(traverseBuffer);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto totalDuration = std::chrono::duration<double>(t2 - t1);
    std::cout << std::format("Time taken ui tree traversal: {}",
                             std::chrono::duration_cast<std::chrono::microseconds>(totalDuration))
              << std::endl;
    EXPECT_EQ(allDescendants.size(), total);
}

TEST_F(UiTreeTest, GetAllDescendantsBreathFirst)
{
    BasicUiTreeSetup();

    auto stackBuffer = std::vector<UiElement*>{};
    stackBuffer.reserve(MAX_ALL_CHILDREN);

    ASSERT_EQ(stackBuffer.capacity(), MAX_ALL_CHILDREN);

    auto allElements = m_parent->GetAllDescendants(stackBuffer);

    // check if all are unique
    uint16_t numberOfSameElements = 0;
    for (const auto& elem : allElements) {
        for (const auto& other : allElements) {
            if (elem == other) {
                numberOfSameElements++;
                continue;
            }

            EXPECT_NE(elem->GetName(), other->GetName());
        }
    }

    EXPECT_EQ(numberOfSameElements, 9);
    EXPECT_EQ(allElements.size(), 9);

    // check the order
    uint16_t i = 0;
    std::vector<int> childrenNumbers{};
    for (const auto& elem : allElements) {
        if (i == 0) {
            i++;
            continue;
        }
        bool numStarted = false;
        std::string numString;

        for (const auto& c : elem->GetName()) {
            if (numStarted) {
                numString += c;
            }

            if (c == '-') {
                numStarted = true;
            }
        }

        i++;
    }
}

TEST_F(UiTreeTest, GetAllDescendantsBreathFirstRandomized)
{
    uint16_t total = 1;

    auto currentChildren = std::vector<UiElement*>{m_parent.get()};

    while (total < MAX_ALL_CHILDREN - 101) {
        auto randomNum = generateRandomNum(1, 10);
        auto nextLevelChildren = std::vector<UiElement*>{};

        bool full = false;

        for (auto& child : currentChildren) {
            for (int i = 0; i < randomNum; i++) {
                auto newChild = std::make_unique<UiElement>("child");
                nextLevelChildren.push_back(newChild.get());
                child->AddChild(std::move(newChild));
                total++;

                if (total >= MAX_ALL_CHILDREN - 101) {
                    full = true;
                    break;
                }
            }

            if (full)
                break;
        }

        if (full)
            break;

        currentChildren = nextLevelChildren;
    }

    auto traverseBuffer = std::queue<UiElement*>{};

    auto t1 = std::chrono::high_resolution_clock::now();
    auto allDescendants = m_parent->GetAllDescendantsBreathFirst(traverseBuffer);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto totalDuration = std::chrono::duration<double>(t2 - t1);
    std::cout << std::format("Time taken ui tree traversal: {}",
                             std::chrono::duration_cast<std::chrono::microseconds>(totalDuration))
              << std::endl;
    EXPECT_EQ(allDescendants.size(), total);
}

TEST_F(UiTreeTest, TestHasChild)
{
    BasicUiTreeSetup();

    EXPECT_TRUE(m_parent->HasChild(m_traversalBuffer, "child-31"));
    EXPECT_TRUE(m_parent->HasChild(m_traversalBuffer, "child-12"));
    EXPECT_FALSE(m_parent->HasChild(m_traversalBuffer, "child-4"));
}

TEST_F(UiTreeTest, TestSucessiveTraverse_NoInteference)
{
    BasicUiTreeSetup();

    std::vector<UiElement*> traverseBuffer;
    traverseBuffer.reserve(MAX_ALL_CHILDREN);
    auto elementsTree1 = m_parent->GetAllDescendants(traverseBuffer);
    EXPECT_EQ(elementsTree1.size(), size_t(9));

    auto secondRoot = std::make_unique<UiElement>("parent2");
    auto secondChild1 = std::make_unique<UiElement>("schild-2");
    auto secondChild2 = std::make_unique<UiElement>("schild-3");
    auto secondChild3 = std::make_unique<UiElement>("schild-4");

    secondRoot->AddChild(std::move(secondChild1));
    secondRoot->AddChild(std::move(secondChild2));
    secondRoot->AddChild(std::move(secondChild3));

    auto elementsTree2 = secondRoot->GetAllDescendants(traverseBuffer);
    EXPECT_EQ(elementsTree2.size(), size_t(4));
}

TEST_F(UiTreeTest, TestGetChild)
{
    BasicUiTreeSetup();

    auto traverseBuffer = std::vector<UiElement*>{};
    traverseBuffer.reserve(MAX_ALL_CHILDREN);

    EXPECT_NE(m_parent->GetChild(traverseBuffer, "child-12"), nullptr);
    EXPECT_EQ(m_parent->GetChild(traverseBuffer, "child-12")->GetName(), "child-12");
    EXPECT_NE(m_parent->GetChild(traverseBuffer, "child-31"), nullptr);
    EXPECT_EQ(m_parent->GetChild(traverseBuffer, "child-31")->GetName(), "child-31");
}
