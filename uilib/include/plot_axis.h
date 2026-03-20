#pragma once

#include "types.h"
#include <cstdint>

enum AxisType { X, Y };

class PlotAxis {
  public:
    explicit PlotAxis(Pos startPosition, uint32_t length, AxisType type);
    void SetRange(double start, double end);

  private:
    void Recalculate();

    AxisType m_type;
    Pos m_startPos;
    uint32_t m_length;
    Range m_range;
};
