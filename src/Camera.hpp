#pragma once

#include "common.hpp"

class Camera {
   public:
    Camera(point3 lookfrom, point3 lookat, vec3 up, float vfov, float aspect_ratio);

    Ray getRay(float u, float v) const;

   private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};