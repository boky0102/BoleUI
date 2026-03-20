#pragma once
#include "types.h"

// Lagrange interpolation

auto GetInterpolatedPosY(Pos p1, Pos p2, Pos p3, float t) -> Pos;
