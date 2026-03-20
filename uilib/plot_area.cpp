#include "plot_area.h"
#include "iostream"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <random>

constexpr uint32_t INTERPOLATION_RATE = 10;

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

static std::string PosStr(Pos ps)
{
    return std::format("({}, {})", ps.left, ps.top);
}

PlotArea::PlotArea(Pos startAxisPos, float axisWidth, const std::vector<double>& data, float axisScale,
                   uint32_t interpolationRate)
    : m_vertexArray(sf::PrimitiveType::TriangleFan, data.size() + 3)
{
    Pos centerAxis = {startAxisPos.left + axisWidth / 2, startAxisPos.top};
    Pos firstDataPos = {startAxisPos.left, startAxisPos.top + static_cast<uint32_t>(data[0])};
    Pos lastDataPos = {startAxisPos.left + static_cast<uint32_t>(axisWidth), static_cast<float>(data[data.size() - 1])};

    auto randColor = GetRandomColor();

    m_vertexArray[0].position = sf::Vector2f(centerAxis.left, centerAxis.top);
    m_vertexArray[0].color = GetRandomColor();

    m_vertexArray[1].position = sf::Vector2f(startAxisPos.left, startAxisPos.top);
    m_vertexArray[1].color = randColor;

    const double xStep = double(axisWidth) / double(data.size());

    int i = 2;
    double currentX = startAxisPos.left;
    for (const auto& elem : data) {
        // IMPORTANT SCALE THIS DATA TO PIXEL HEIGHT BASED ON AXIS
        m_vertexArray[i].position = sf::Vector2f(currentX, startAxisPos.top - static_cast<uint32_t>(data[i - 2]));
        m_vertexArray[i].color = randColor;
        currentX += xStep;
        i++;
    }

    m_vertexArray[m_vertexArray.getVertexCount() - 1].position =
        sf::Vector2f(startAxisPos.left + axisWidth, startAxisPos.top);
    m_vertexArray[m_vertexArray.getVertexCount() - 1].color = randColor;
}

auto PlotArea::GetVertexArray() -> sf::VertexArray
{
    return m_vertexArray;
}
