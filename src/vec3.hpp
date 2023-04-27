#pragma once
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

#include "common.hpp"
inline float randomFloat();
float randomFloat(float min, float max);

using vec3 = glm::vec3;
using point3 = vec3;  // 3D point
using color = vec3;   // RGB color

inline bool near_zero(const vec3& vec) {
    const auto s = 1e-8;
    return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
}

inline vec3 randomVec() { return vec3(randomFloat(), randomFloat(), randomFloat()); }
inline vec3 randomVec(float min, float max) {
    return vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
}

// inline std::ostream& operator<<(std::ostream& out, const vec3& v) { return out << v.x << ' ' << v.y << ' ' << v.z; }

inline vec3 random_in_unit_sphere() {
    while (true) {
        vec3 p = randomVec(-1, 1);
        if (glm::length(p) * glm::length(p) >= 1) continue;
        return p;
    }
}

inline vec3 unit_vector(vec3 v) { return glm::normalize(v); }

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

        if (glm::length(p) * glm::length(p) >= 1) continue;
        return p;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) { return v - 2 * dot(v, n) * n; }

inline vec3 refract(const vec3& uv, const vec3& n, float refraction_ratio) {
    auto cos_theta = fmin(dot(-uv, n), 1.0f);
    vec3 r_out_perp = refraction_ratio * (uv + cos_theta * n);
    float length_squared = glm::length(r_out_perp) * glm::length(r_out_perp);
    vec3 r_out_parallel = -sqrt(fabs(1.0f - length_squared)) * n;
    return r_out_perp + r_out_parallel;
}