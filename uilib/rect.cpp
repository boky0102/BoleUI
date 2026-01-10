#include "rect.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdint>
#include <sys/types.h>

namespace Components {

Rect::Rect(Size size, Border border, Pos pos)
    : m_convexShape()
    , m_size{size}
    , m_border{border}
    , m_pos{pos}
{
    const auto positions = CalculatePoints();
    m_convexShape.setPointCount(positions.size());
    int i = 0;
    for (const auto& pos : positions) {
        m_convexShape.setPoint(
            i, sf::Vector2f{static_cast<float>(pos.left), static_cast<float>(pos.top)});
        i++;
    }
}

auto Rect::GetUnderlayingShape() const -> sf::ConvexShape
{
    return m_convexShape;
}

auto Rect::CalculatePoints() const -> std::vector<Pos>
{
    Pos currentPos{0, 0};
    std::vector<Pos> positions;

    const auto& width = m_size.width;
    const auto& height = m_size.height;
    const auto& boderRadius = m_border.radius;
    const auto& startPos = m_pos;

    /*
     * ----- >
     */
    positions.push_back({startPos.left + boderRadius, startPos.top});
    positions.push_back({startPos.left + width - boderRadius, startPos.top});

    currentPos = positions.back();
    for (int x = currentPos.left; x < currentPos.left + boderRadius; x++) {
        const auto circleY = GetCircleXPos(x, {currentPos.left, currentPos.top + boderRadius},
                                           CircleSide::Bottom, boderRadius);
        positions.push_back(circleY);
    }

    positions.push_back({startPos.left + width, startPos.top + boderRadius});
    positions.push_back({startPos.left + width, startPos.top + height - boderRadius});

    currentPos = positions.back();
    for (int x = currentPos.left; x > currentPos.left - boderRadius; x--) {
        const auto circleY = GetCircleXPos(x, {currentPos.left - boderRadius, currentPos.top},
                                           CircleSide::Top, boderRadius);
        positions.push_back(circleY);
    }

    positions.push_back({startPos.left + width - boderRadius, startPos.top + height});
    positions.push_back({startPos.left + boderRadius, startPos.top + height});

    currentPos = positions.back();
    for (int x = currentPos.left; x > currentPos.left - boderRadius; x--) {
        const auto circleY = GetCircleXPos(x, {currentPos.left, currentPos.top - boderRadius},
                                           CircleSide::Top, boderRadius);
        positions.push_back(circleY);
    }

    positions.push_back({startPos.left, startPos.top + height - boderRadius});
    positions.push_back({startPos.left, startPos.top + boderRadius});

    currentPos = positions.back();
    for (int x = currentPos.left; x < currentPos.left + boderRadius; x++) {
        const auto circleY = GetCircleXPos(x, {currentPos.left + boderRadius, currentPos.top},
                                           CircleSide::Bottom, boderRadius);
        positions.push_back(circleY);
    }

    return positions;
}

auto Rect::GetPositions() const -> std::vector<Pos>
{
    return CalculatePoints();
}

} // namespace Components
