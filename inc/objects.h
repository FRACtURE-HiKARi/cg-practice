#pragma once
#include "maths.h"
#include <graphics.h>

typedef std::vector<std::array<int, 3>> TripleArray;
typedef struct Grid {
    vec2d V;
    int_pairs E;
}Grid;

typedef struct Mesh {
    vec2d V;
    TripleArray S;
}Mesh;

Grid standardCube();
Mesh simpleTetra();