#include "Camera.hpp"

#include <glm/gtx/scalar_multiplication.hpp>

Camera::Camera(point3 lookfrom, point3 lookat, vec3 up, float vfov, float aspect_ratio, float aperature,
               float focus_dist) {
    float theta = degreesToRadians(vfov);
    float h = tan(theta / 2);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(up, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    vec3 half_hor = horizontal * 0.5;
    vec3 half_ver = vertical * 0.5;
    lower_left_corner = origin - half_hor - half_ver - focus_dist * w;

    lens_radius = aperature / 2;
}

Ray Camera::getRay(float s, float t) const {
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x + v * rd.y;
    return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
}