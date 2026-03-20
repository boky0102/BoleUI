#include "SFML/Graphics/Color.hpp"
#include "uielement.h"
#include <cstdint>
#include <random>
#include <utility>

static auto GetRandomNumber(const std::pair<int, int> range) -> int
{
    const auto& [min, max] = range;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);

    return dist(gen);
}

static auto GetRandomColor() -> sf::Color
{
    const auto red = std::uint8_t(GetRandomNumber({0, 255}));
    const auto green = std::uint8_t(GetRandomNumber({0, 255}));
    const auto blue = std::uint8_t(GetRandomNumber({0, 255}));

    return sf::Color(red, green, blue);
}

static auto GetRandomLocalColor() -> Color
{
    const auto red = std::uint8_t(GetRandomNumber({0, 255}));
    const auto green = std::uint8_t(GetRandomNumber({0, 255}));
    const auto blue = std::uint8_t(GetRandomNumber({0, 255}));

    return {red, green, blue};
}
