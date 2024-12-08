#include "elements.h"

void Camera::UpdateMat() {
    vec3 gxt = cross(g, t);
    Rv = {
            vec4{1, 0, 0, -e[0]/e[3]},
            vec4{0, 1, 0, -e[1]/e[3]},
            vec4{0, 0, 1, -e[2]/e[3]},
            vec4{0, 0, 0,          1},
    };
    Tv = {
            vec4{gxt[0], gxt[1], gxt[2], 0},
            vec4{t[0], t[1], t[2], 0},
            vec4{-g[0], -g[1], -g[2], 0},
            vec4{0, 0, 0, 1},
    };
    Mv = Rv * Tv;
    Mtrans = Mvp * Mpersp * Mv;
}

void Camera::InitProjMat(const camera_t& c) {
    mat4 A = diag({2/(c.r - c.l), 2/(c.t - c.b),2/(c.n - c.f), 1});
    mat4 B = identity();
    B[0][3] = -(c.r + c.l)/2;
    B[1][3] = -(c.t + c.b)/2;
    B[2][3] = -(c.n + c.f)/2;
    Morth = A*B;
    mat4 C = diag({c.n, c.n, c.n+c.f, 0});
    C[2][3] = -c.n * -c.f;
    C[3][2] = 1;
    Mpersp = Morth * C;
}

Camera::Camera(int w, int h) {
    UpdateMat();
    width = w;
    height = h;
    Mvp = diag({(float)width/2, -(float)height/2, 1, 1});
    Mvp[0][3] = (float)width/2;
    Mvp[1][3] = (float)height/2;
}
