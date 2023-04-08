#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class Sphere : public Hittable {
   public:
    Sphere() {}
    Sphere(point3 cen, float r);

    virtual bool Hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

   private:
    point3 centre;
    float radius;
};