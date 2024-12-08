#include <algorithm>
#include "render.h"

#define RGB_t(C) RGB(C[0], C[1], C[2])

typedef struct {
    int x;
    int y;
    int z;
} coord_t;

void Bresenham(const vec3 &v1, const vec3 &v2, std::vector<coord_t> &edge_pixels) {
    int x1 = floor(v1[0]), x2 = floor(v2[0]), y1 = floor(v1[1]), y2 = floor(v2[1]);
    int dx = abs(x1 - x2), sx = x2 < x1 ? 1 : -1;
    int dy = abs(y1 - y2), sy = y2 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    while(edge_pixels.push_back({x2, y2}), x2 != x1 || y2 != y1){
        int e2 = err;
        if(e2 > -dx) { err -= dy; x2 += sx;}
        if(e2 <  dy) { err += dx; y2 += sy;}
    }
}

void Bresenham(const vec4 &v1, const vec4 &v2, std::vector<coord_t> &edge_pixels) {
    Bresenham(to3d(v1), to3d(v2), edge_pixels);
}

void render_line(const vec4 &v1, const vec4 &v2) {
    line(
            (int)(v1[0]/v1[3]),
            (int)(v1[1]/v1[3]),
            (int)(v2[0]/v2[3]),
            (int)(v2[1]/v2[3])
    );
}

void renderGrid(const Grid &m, const Camera &c) {
    vec2d Vt = c.Mtrans * m.V;
    for (auto e: m.E)
        render_line(Vt[e.first], Vt[e.second]);
}

Triangle getTriangle(std::array<int, 3> indices, const vec2d &V) {
    const vec4 &v1 = V[indices[0]];
    const vec4 &v2 = V[indices[1]];
    const vec4 &v3 = V[indices[2]];
    return {
            vec3{v1[0]/v1[3], v2[0]/v2[3], v3[0]/v3[3]},
            vec3{v1[1]/v1[3], v2[1]/v2[3], v3[1]/v3[3]},
            vec3{v1[2]/v1[3], v2[2]/v2[3], v3[2]/v3[3]},
            {0}
    };
}

int inside(const Triangle &t, float x, float y) {
    float c1 = cross(vec3{t.xs[0] - x, t.ys[0] - y}, vec3{t.xs[0] - t.xs[1], t.ys[0] - t.ys[1]})[2];
    float c2 = cross(vec3{t.xs[1] - x, t.ys[1] - y}, vec3{t.xs[1] - t.xs[2], t.ys[1] - t.ys[2]})[2];
    float c3 = cross(vec3{t.xs[2] - x, t.ys[2] - y}, vec3{t.xs[2] - t.xs[0], t.ys[2] - t.ys[0]})[2];
    if (((c1 < 0) && (c2 < 0) && (c3 < 0)) ||
        ((c1 > 0) && (c2 > 0) && (c3 > 0)))
        return 1;
    else return 0;
}

float area(std::array<float, 3> xs, std::array<float, 3> ys) {
    return abs(cross(vec3{xs[0] - xs[1], ys[0] - ys[1]}, vec3{xs[0] - xs[2], ys[0] - ys[2]})[2]);
}

float triangleInterpolate2d(const Triangle& t, float x, float y) {
    std::array<float, 3> As = {0};
    float f = 0;
    for (int i = 0; i < 3; i++) {
        As[i] = area({t.xs[(i + 1) % 3], t.xs[(i + 2) % 3], x}, {t.ys[(i + 1) % 3], t.ys[(i + 2) % 3], y});
        f += As[i] * t.ws[i];
    }
    float A = area(t.xs, t.ys);
    if (A == 0) return 0;
    return f / A;
}

float superResolutionSample(const Triangle &t, int x, int y, int sr) {
    float d = 1/(float)(sr+1);
    float z = 0;
    for (int i = 1; i <= sr; i++) {
        for (int j = 1; j <= sr; j++) {
            z += (float)inside(t, (float)x + (float)i*d, (float)y + (float)j*d);
        }
    }
    float z1 = z / (float)sr / (float)sr;
    return z1 > 0 ? (z1 < 1 ? z1 : 1) : 0;
}

void drawTetra(const vec2d &V) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++)
            render_line(V[i], V[j]);
    }
}

void rasterize(const Mesh &m, const Camera &c, int sr, const color_t& color) {
    Watch w;
    std::vector<std::vector<float>> z_buffer(c.height, std::vector<float>(c.width, -INFINITY));
    // TODO: fill background
    // raster through all surfaces
    vec2d Vt = c.Mtrans * m.V;
    for (auto s: m.S) {
        Triangle t = getTriangle(s, Vt);
        t.ws = t.zs;
        std::vector<coord_t> edge_pixels;
        // get pixel on edge
        w.start();
        for (int i = 0; i < 3; i++)
            Bresenham(t.Vertex(i), t.Vertex(i+1), edge_pixels);
        // TODO: not sort
        std::sort(
                edge_pixels.begin(),
                edge_pixels.end(),
                [](coord_t c1, coord_t c2) -> bool {
                    return c1.y == c2.y ? c1.x < c2.x : c1.y < c2.y;
                }
        );
        w.tick();

        for (int i = 0; i < edge_pixels.size(); i++) {
            int x1 = edge_pixels[i].x;
            if (i < edge_pixels.size() - 1) {
                int x2 = edge_pixels[i + 1].x;
                if (x2 - x1 > 1) {
                    int y = edge_pixels[i].y;
                    for (int x = x1 + 1; x < x2; x++) {
                        float z_surface = triangleInterpolate2d(t, (float) x, (float) y);
                        if (z_surface > z_buffer[y][x]) {
                            z_buffer[y][x] = z_surface;
                            putpixel(x, y, RGB_t(color));
                        }
                    }
                }
            }
            int y = edge_pixels[i].y;
            if (edge_pixels[i].z > z_buffer[y][x1]){
                z_buffer[y][x1] = edge_pixels[i].z;
                color_t c1 = color * superResolutionSample(t, x1, y, sr);
                putpixel(x1, y, RGB_t(c1));
            }
        }
    }
    //drawTetra(Vt);
}

vec3 Triangle::Vertex(int i) const {
    if (i >= 3) i = i%3;
    return vec3{xs[i], ys[i], zs[i]};
}
