#pragma once

enum ElemType { Text, Box, Window };

enum CircleSide { Top, Bottom };

struct Pos {
    float left;
    float top;
};

struct Range {
    double start;
    double end;
};
