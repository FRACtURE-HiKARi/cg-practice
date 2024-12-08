#pragma once
#include <array>
#include <vector>
#include <iostream>

typedef std::array<float, 4> vec4;
typedef std::array<float, 3> vec3;
typedef std::array<float, 2> vec2;
typedef std::array<vec4, 4> mat4;
typedef std::array<vec3, 3> mat3;
typedef std::vector<vec4> vec2d;
typedef std::vector<std::pair<int, int>> int_pairs;

typedef struct Quaternion {
    float x;
    float y;
    float z;
    float w;
}Quaternion;

vec3 operator*(const vec3 &v, float f);
vec3 operator/(const vec3 &v, float f);
vec4 operator+(const vec4 &a, const vec4 &b);
vec4 operator-(const vec4 &a, const vec4 &b);
vec4 operator*(float f, const vec4 &v);
float operator*(const vec4 &a, const vec4 &b);

vec4 operator*(const mat4 &A, const vec4 &b);
mat4 operator*(float a, const mat4 &B);
mat4 operator*(const mat4 &A, const mat4 &b);
mat4 operator+(const mat4 &A, const mat4 &b);
mat4 transpose(const mat4 &A);

vec3 to3d(const vec4 &v);
vec3 unit(const vec3 &n);
vec3 cross(const vec3 &a, const vec3 &b);
vec3 cross(const vec4 &a, const vec4 &b);
mat4 diag(const vec4 &d);
mat4 identity();
mat4 axisRotation(const vec3 &n, float t);

vec2d operator*(const mat4 &M, const vec2d &V);

std::ostream& operator<<(std::ostream &out, const vec4& v);
std::ostream& operator<<(std::ostream &out, const mat4& M);
