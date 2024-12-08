#include "maths.h"
#include <algorithm>
#include <iomanip>

vec3 operator*(const vec3 &v, float f) {
    return {v[0]*f, v[1]*f, v[2]*f};
}

vec3 operator/(const vec3 &v, float f) {
    return {v[0]/f, v[1]/f, v[2]/f};
}

vec4 operator+(const vec4 &a, const vec4 &b) {
    return {
        a[0] + b[0],
        a[1] + b[1],
        a[2] + b[2],
        a[3] + b[3],
    };
}

vec4 operator-(const vec4 &a, const vec4 &b) {
    return {
            a[0] - b[0],
            a[1] - b[1],
            a[2] - b[2],
            a[3] - b[3],
    };
}

vec4 operator*(float f, const vec4 &v) {
    return {v[0]*f, v[1]*f, v[2]*f, v[3]*f};
}

float operator*(const vec4 &a, const vec4 &b) {
    return
            a[0] * b[0] +
            a[1] * b[1] +
            a[2] * b[2] +
            a[3] * b[3]
    ;
}

mat4 transpose(const mat4 &A) {
    return {
            vec4{A[0][0], A[1][0], A[2][0], A[3][0]},
            vec4{A[0][1], A[1][1], A[2][1], A[3][1]},
            vec4{A[0][2], A[1][2], A[2][2], A[3][2]},
            vec4{A[0][3], A[1][3], A[2][3], A[3][3]},
    };
}
vec4 operator*(const mat4 &A, const vec4 &b) {
    return {
        A[0] * b,
        A[1] * b,
        A[2] * b,
        A[3] * b,
    };
}

mat4 operator*(float a, const mat4 &B) {
    return {
        a*B[0],
        a*B[1],
        a*B[2],
        a*B[3],
    };
}

mat4 operator*(const mat4 &A, const mat4 &B) {
    mat4 BT = transpose(B);
    return {
        BT*A[0],
        BT*A[1],
        BT*A[2],
        BT*A[3],
    };
}

mat4 operator+(const mat4 &A, const mat4 &B) {
    return {
      A[0] + B[0],
      A[1] + B[1],
      A[2] + B[2],
      A[3] + B[3],
    };
}

vec3 to3d(const vec4 &v) {
    return {v[0]/v[3], v[1]/v[3], v[2]/v[3]};
}

vec3 unit(const vec3 &n) {
    float l = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    if (l == 0) return {0, 0, 1};
    return {n[0]/l, n[1]/l, n[2]/l};
}

vec3 cross(const vec3 &a, const vec3 &b) {
    return {
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    };
}

// TODO: Homogenous
inline vec3 toVec3(const vec4 &v) {
    return {v[0], v[1], v[2]};
}

vec3 cross(const vec4 &a, const vec4 &b) {
    return cross(toVec3(a), toVec3(b));
}

mat4 diag(const vec4 &d) {
    return mat4{
            vec4{d[0], 0, 0, 0},
            vec4{0, d[1], 0, 0},
            vec4{0, 0, d[2], 0},
            vec4{0, 0, 0, d[3]},
    };
}

mat4 identity() {
    return diag({1, 1, 1, 1});
}

vec2d operator*(const mat4 &M, const vec2d &V) {
    size_t size = V.size();
    vec2d R(size);
    for (int i = 0; i < size; i++)
        R[i] = M * V[i];
    return R;
}

mat4 expand(const vec4 &v) {
    mat4 R;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            R[i][j] = v[i]*v[j];
    return R;
}

vec4 homogenous(const vec3 &v, float w) {
    return {v[0], v[1], v[2], w};
}

mat4 axisRotation(const vec3 &n, float t) {
    vec3 n1 = unit(n);
    vec4 n2 = homogenous(n1, 0);
    float c = cosf(t);
    float s = sinf(t);
    mat4 N = {
            vec4{0, -n1[2], n1[1], 0},
            vec4{n1[2], 0, -n1[0], 0},
            vec4{-n1[1], n1[0], 0, 0},
            vec4{0, 0, 0, 0}
    };
    mat4 R = c*identity() + (1-c)*expand(n2) + s * N;
    R[3][3] = 1;
    return R;
}

std::ostream& operator<<(std::ostream &out, const vec4& v) {
    //out << std::setprecision(2);
    out << '[' << v[0] << ' ' << v[1] << ' ' << v[2] << ' ' << v[3] << "]\n";
    return out;
}

std::ostream& operator<<(std::ostream &out, const mat4& M) {
    out << '[' << M[0] \
        << ' ' << M[1] \
        << ' ' << M[2] \
        << ' ' << '[' << M[3][0] << ' ' << M[3][1] << ' ' << M[3][2] << ' ' << M[3][3] << "]]\n";
    return out;
}
