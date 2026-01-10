#include "rect.h"
#include "gtest/gtest.h"

namespace Components {

class TestComponents : public testing::Test {
  protected:
    TestComponents()
        : m_rect({100, 50}, {10}, {0, 0})
    {
    }

    Rect m_rect;
};

TEST_F(TestComponents, TestNonRoundedEdgesExistance)
{
    const auto positions = m_rect.GetPositions();
    bool leftTop = false;
    bool rightTop = false;
    bool rightBottom = false;
    bool leftBottom = false;

    for (const auto& pos : positions) {
        if (pos.left == 10 && pos.top == 0) {
            leftTop = true;
        }

        if (pos.left == 90 && pos.top == 0) {
            rightTop = true;
        }

        if (pos.left == 90 && pos.top == 50) {
            rightBottom = true;
        }

        if (pos.left == 10 && pos.top == 50) {
            leftBottom = true;
        }
    }

    EXPECT_TRUE(leftTop);
    EXPECT_TRUE(rightTop);
    EXPECT_TRUE(rightBottom);
    EXPECT_TRUE(leftBottom);
}

} // namespace Components
