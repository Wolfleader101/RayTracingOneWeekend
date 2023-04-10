#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class Sphere : public Hittable {
   public:
    Sphere() {}
    Sphere(point3 cen, float r, std::shared_ptr<Material> mat);

    virtual bool Hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;

   private:
    point3 centre;
    float radius;
    std::shared_ptr<Material> mat_ptr;
};