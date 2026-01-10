#pragma once

#include "types.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <cmath>
#include <cstdint>
namespace Components {

struct Size {
    uint32_t width;
    uint32_t height;
};

struct Border {
    uint32_t radius;
};

struct Pos {
    uint32_t left;
    uint32_t top;
};

class Rect {
  public:
    Rect(Size size, Border border, Pos pos);

    auto GetUnderlayingShape() const -> sf::ConvexShape;
    auto GetPositions() const -> std::vector<Pos>;

  private:
    auto CalculatePoints() const -> std::vector<Pos>;

    sf::ConvexShape m_convexShape;
    Size m_size;
    Border m_border;
    Pos m_pos;
};

inline auto GetCircleXPos(const int32_t x, const Components::Pos& circleCenter,
                          CircleSide circleSide, const int32_t radius) -> Components::Pos
{
    const auto h = circleCenter.left;
    const auto k = circleCenter.top;
    const auto r2 = std::pow(radius, 2);

    const int xh = x - h;
    const int part = std::pow(xh, 2);
    const auto base = std::sqrt(r2 - part);

    if (circleSide == CircleSide::Top) {
        return {static_cast<uint32_t>(x), static_cast<uint32_t>(k + base)};
    }

    return {static_cast<uint32_t>(x), static_cast<uint32_t>(k - base)};
}

} // namespace Components
