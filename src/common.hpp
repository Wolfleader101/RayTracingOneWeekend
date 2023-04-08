#pragma once

#include <cmath>
#include <limits>
#include <memory>

#include "ray.hpp"
#include "vec3.hpp"

const double INFIN = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline float degreesToRadians(float degrees) { return degrees * PI / 180.0f; }