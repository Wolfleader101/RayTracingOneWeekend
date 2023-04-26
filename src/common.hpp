#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "ray.hpp"
#include "vec3.hpp"

const double INFIN = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline float degreesToRadians(float degrees) { return degrees * (float)PI / 180.0f; }

inline float randomFloat() {
    static thread_local std::uniform_real_distribution<float> distrubtion(0.0f, 1.0f);
    static thread_local std::mt19937 gen;
    return distrubtion(gen);
}

inline float randomFloat(float min, float max) {
    static thread_local std::uniform_real_distribution<float> distrubtion(min, max);
    static thread_local std::mt19937 gen;
    return distrubtion(gen);
}

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;

    return x;
}