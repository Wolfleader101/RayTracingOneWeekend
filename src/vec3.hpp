#pragma once
#include <cmath>
#include <iostream>

#include "common.hpp"
inline float randomFloat();
float randomFloat(float min, float max);

class vec3 {
   public:
    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 operator-() const { return vec3(-x, -y, -z); }

    vec3& operator+=(const vec3& v) {
        x += x;
        y += y;
        z += z;
        return *this;
    }

    vec3& operator*=(const float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    vec3& operator/=(const float s) { return *this *= 1 / s; }

    float length() const { return std::sqrt(length_squared()); }

    float length_squared() const { return x * x + y * y + z * z; }

    bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    inline static vec3 random() { return vec3(randomFloat(), randomFloat(), randomFloat()); }
    inline static vec3 random(float min, float max) {
        return vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
    }

    float x;
    float y;
    float z;
};

using point3 = vec3;  // 3D point
using color = vec3;   // RGB color

inline std::ostream& operator<<(std::ostream& out, const vec3& v) { return out << v.x << ' ' << v.y << ' ' << v.z; }

inline vec3 operator+(const vec3& u, const vec3& v) { return vec3(u.x + v.x, u.y + v.y, u.z + v.z); }

inline vec3 operator-(const vec3& u, const vec3& v) { return vec3(u.x - v.x, u.y - v.y, u.z - v.z); }

inline vec3 operator*(const vec3& u, const vec3& v) { return vec3(u.x * v.x, u.y * v.y, u.z * v.z); }

inline vec3 operator*(float s, const vec3& v) { return vec3(s * v.x, s * v.y, s * v.z); }

inline vec3 operator*(const vec3& v, float s) { return s * v; }

inline vec3 operator/(const vec3& v, float s) { return (1 / s) * v; }

inline float dot(const vec3& u, const vec3& v) { return u.x * v.x + u.y * v.y + u.z * v.z; }

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }

inline vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0f)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(randomFloat(-1, 1), randomFloat(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) { return v - 2 * dot(v, n) * n; }

inline vec3 refract(const vec3& uv, const vec3& n, float refraction_ratio) {
    auto cos_theta = fmin(dot(-uv, n), 1.0f);
    vec3 r_out_perp = refraction_ratio * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}