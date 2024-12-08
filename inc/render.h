#pragma once
#include <graphics.h>
#include "elements.h"
#include "objects.h"
#include "utils.h"

typedef vec3 color_t;
typedef struct Triangle {
    std::array<float, 3> xs;
    std::array<float, 3> ys;
    std::array<float, 3> zs;
    std::array<float, 3> ws; // extra values on vertices
    vec3 Vertex(int i) const;
} Triangle;

void renderGrid(const Grid &g, const Camera &c);
void rasterize(const Mesh &m, const Camera &c, int sr, const color_t& color); // super-resolution
