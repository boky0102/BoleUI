#include "plot_util.h"
#include <sys/types.h>

// Lagrange foormula
auto GetInterpolatedPosY(Pos p0, Pos p1, Pos p2, float t) -> Pos
{
    float d1 = ((t - p1.left) * (t - p2.left)) / ((p0.left - p1.left) * (p0.left - p2.left));
    float d2 = ((t - p0.left) * (t - p2.left)) / ((p1.left - p0.left) * (p1.left - p2.left));
    float d3 = ((t - p0.left) * (t - p1.left)) / ((p2.left - p0.left) * (p2.left - p1.left));

    const auto y = p0.top * d1 + p1.top * d2 + p2.top * d3;

    return {t, y};
}
