#pragma once
#include <vector>
#include <maths.h>

typedef std::vector<vec4> PointSet;
typedef struct camera_t {
    float l, r, b, t, n, f;
} camera_t;

class Camera {
public:
    vec4 e = {0}; // position
    vec3 g = {0, 0, -1}; // gaze direction
    vec3 t = {0, 1, 0}; // up direction
    int width, height;
    mat4 Rv={}, Tv={}, Mv={}; // transformation in view
    mat4 Morth={}, Mpersp={};
    mat4 Mvp = {}; // viewport matrix
    mat4 Mtrans = {}; // total transformation
    void UpdateMat();
    void InitProjMat(const camera_t& c);
    explicit Camera(int w=640, int h=480);
};
