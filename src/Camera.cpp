#include "Camera.hpp"

Camera::Camera() {
    const float aspect_ratio = 16.0f / 9.0f;

    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0f;

    origin = point3(0, 0, 0);
    horizontal = vec3(viewport_width, 0, 0);
    vertical = vec3(0, viewport_height, 0);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
}

Ray Camera::getRay(float u, float v) const {
    return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}