#pragma once

#include "common.hpp"

class Camera {
   public:
    Camera();

    Ray getRay(float u, float v) const;

   private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};