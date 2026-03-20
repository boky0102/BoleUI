#pragma once

#include "types.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

// This is a class that represents area on the plot
class PlotArea {
  public:
    // Axis scale represents how much pixels 1000 units of data represent
    explicit PlotArea(Pos startAxisPos, float axisWidth, const std::vector<double>& data, float axisScale,
                      uint32_t interpolationRate);

    auto GetVertexArray() -> sf::VertexArray;

  private:
    sf::VertexArray m_vertexArray;
};
