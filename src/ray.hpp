#pragma once

#include "vec3.hpp"

class Ray {
   public:
    Ray() {}
    Ray(const point3& origin, const vec3& dir) : orig(origin), dir(dir) {}

    const point3& origin() const { return orig; }
    const vec3& direction() const { return dir; }

    point3 at(float t) const { return orig + t * dir; }

   private:
    point3 orig;
    vec3 dir;
};