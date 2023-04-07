#pragma once

#include "ray.hpp"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
};

class Hittable {
   public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};