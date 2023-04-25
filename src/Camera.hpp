#pragma once

#include "common.hpp"

class Camera {
   public:
    Camera(point3 lookfrom, point3 lookat, vec3 up, float vfov, float aspect_ratio, float aperature, float focus_dist);

    Ray getRay(float s, float t) const;

   private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};