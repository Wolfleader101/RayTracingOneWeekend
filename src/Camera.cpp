#include "Camera.hpp"

Camera::Camera(point3 lookfrom, point3 lookat, vec3 up, float vfov, float aspect_ratio) {
    auto theta = degreesToRadians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0f * h;
    auto viewport_width = aspect_ratio * viewport_height;

    auto w = unit_vector(lookfrom - lookat);
    auto u = unit_vector(cross(up, w));
    auto v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
}

Ray Camera::getRay(float u, float v) const {
    return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}