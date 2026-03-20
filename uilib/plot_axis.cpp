#include "plot_axis.h"
#include <cstdint>

PlotAxis::PlotAxis(Pos startPosition, uint32_t length, AxisType axisType)
    : m_type{axisType}
    , m_startPos(startPosition)
    , m_length(length)
    , m_range()
{
}

void PlotAxis::SetRange(double start, double end)
{
    m_range = {start, end};
}
