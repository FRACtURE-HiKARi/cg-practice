#include <graphics.h>
#include "render.h"
#define WIDTH 640
#define HEIGHT 640

int main() {
    initgraph(WIDTH, HEIGHT);
    Camera camera(WIDTH, HEIGHT);
    camera.InitProjMat({-1, 1, -1, 1, -1, -3});
    camera.e = {0.5, 0.5, 3, 1};
    camera.UpdateMat();
    Mesh tetra = simpleTetra();
    Grid cube = standardCube();
    BeginBatchDraw();
    float dt = 1.f/60;
    float omega = 0.002;
    std::stringstream ss;
    Watch w;
    Avg_n avg(10);
    RECT rect = {10, 10, WIDTH-1, HEIGHT-1};
    while(true) {
        w.start();
        mat4 Rotmat = axisRotation({1, 1, 1}, omega * dt);
        tetra.V = Rotmat * tetra.V;
        //cube.V = Rotmat * cube.V;
        cleardevice();
        rasterize(tetra, camera, 4, {88, 155, 181});
        //renderGrid(cube, camera);
        ss.str("");
        ss << "FPS: " << (1000.f / avg.getAverage());
        drawtext(ss.str().c_str(), &rect, DT_TOP | DT_BOTTOM);
        FlushBatchDraw();
        w.end();
        dt = (float)w.duration() / 1000;
        avg.push(dt);
    }
    return 0;
}