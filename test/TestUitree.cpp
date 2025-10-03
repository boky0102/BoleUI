#include "gtest/gtest.h"
#include <chrono>
#include <memory>
#include "uitree.h"
#include "utils.h"

class UiTreeTest : public testing::Test {
    protected:
    UiTreeTest() {
        m_child1 = std::make_unique<UiElement>("child1");
        m_child2 = std::make_unique<UiElement>("child2");
        m_child3 = std::make_unique<UiElement>("child3");

        m_parent = std::make_unique<UiElement>("parent");
    }

    std::unique_ptr<UiElement> m_child1;
    std::unique_ptr<UiElement> m_child2;
    std::unique_ptr<UiElement> m_child3;

    std::unique_ptr<UiElement> m_parent;
};

TEST_F(UiTreeTest, GetAllChildren){
    m_parent->AddChild(std::move(m_child1));
    m_parent->AddChild(std::move(m_child2));
    m_parent->AddChild(std::move(m_child3));

    auto children = m_parent->GetAllChildren();
    EXPECT_EQ(children.size(), size_t(3));
    EXPECT_EQ(children[0]->GetName(), "child1");
    EXPECT_EQ(children[1]->GetName(), "child2");
    EXPECT_EQ(children[2]->GetName(), "child3");
}

TEST_F(UiTreeTest, GetAllDescendants){
    auto child12 = std::make_unique<UiElement>("child12");
    auto child13 = std::make_unique<UiElement>("child13");
    auto child14 = std::make_unique<UiElement>("child14");
    m_child1->AddChild(std::move(child12));
    m_child1->AddChild(std::move(child13));
    m_child1->AddChild(std::move(child14));

    auto child31 = std::make_unique<UiElement>("child31");
    auto child32 = std::make_unique<UiElement>("child32");
    m_child3->AddChild(std::move(child31));
    m_child3->AddChild(std::move(child32));

    m_parent->AddChild(std::move(m_child1));
    m_parent->AddChild(std::move(m_child2));
    m_parent->AddChild(std::move(m_child3));

    auto stackBuffer = std::vector<UiElement*>{};
    stackBuffer.reserve(MAX_ALL_CHILDREN);

    ASSERT_EQ(stackBuffer.capacity(), MAX_ALL_CHILDREN);

    auto allElements = m_parent->GetAllDescendants(stackBuffer);

    // check if all are unique
    uint16_t numberOfSameElements = 0;
    for(const auto& elem: allElements){
        for(const auto& other: allElements){
            if(elem == other){
                numberOfSameElements++;
                continue;
            }

            EXPECT_NE(elem->GetName(), other->GetName());
        }
    }

    EXPECT_EQ(numberOfSameElements, 9);
    EXPECT_EQ(allElements.size(), 9);
}

TEST_F(UiTreeTest, GetAllDescendantsRandomized){
    uint16_t total = 1;

    auto currentChildren = std::vector<UiElement*>{m_parent.get()};

    while(total < MAX_ALL_CHILDREN - 101){
        auto randomNum = generateRandomNum(1, 10);
        auto nextLevelChildren = std::vector<UiElement*>{};

        bool full = false;

        for(auto& child: currentChildren){
            for(int i = 0; i < randomNum; i++){
                auto newChild = std::make_unique<UiElement>("child");
                nextLevelChildren.push_back(newChild.get());
                child->AddChild(std::move(newChild));
                total++;

                if(total >= MAX_ALL_CHILDREN - 101){
                    full = true;
                    break;
                }
            }

            if(full) break;
        }

        bla bla

        if(full) break;

        currentChildren = nextLevelChildren;
    }

    auto traverseBuffer = std::vector<UiElement*>{};
    traverseBuffer.reserve(MAX_ALL_CHILDREN);

    auto t1 = std::chrono::high_resolution_clock::now();
    auto allDescendants = m_parent->GetAllDescendants(traverseBuffer);
    auto t2 = std::chrono::high_resolution_clock::now();

    auto totalDuration = std::chrono::duration<double>(t2 - t1);
    std::cout << std::format("Time taken ui tree traversal: {}",std::chrono::duration_cast<std::chrono::microseconds>(totalDuration)) << std::endl;
    EXPECT_EQ(allDescendants.size(), total);
}
