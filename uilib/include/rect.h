#pragma once

#include "types.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <cmath>
namespace Components {

struct Size {
    float width;
    float height;
};

struct Border {
    float radius;
    float width;
};

class Rect {
  public:
    Rect(Size size, Border border, Pos pos);

    auto GetUnderlayingShape() -> sf::ConvexShape*;
    auto GetPositions() const -> std::vector<Pos>;

  private:
    auto CalculatePoints() const -> std::vector<Pos>;

    sf::ConvexShape m_convexShape;
    Size m_size;
    Border m_border;
    Pos m_pos;
};

inline auto GetCircleXPos(const float x, const Pos& circleCenter, CircleSide circleSide, const float radius) -> Pos
{
    const auto h = circleCenter.left;
    const auto k = circleCenter.top;
    const auto r2 = std::pow(radius, 2);

    const int xh = x - h;
    const int part = std::pow(xh, 2);
    const auto base = static_cast<float>(std::sqrt(r2 - part));

    if (circleSide == CircleSide::Top) {
        return {x, k + base};
    }

    return {x, k - base};
}

} // namespace Components
