#include <fstream>
#include <iostream>

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    float v = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - v) * color(1.0f, 1.0f, 1.0f) + v * color(0.5f, 0.7f, 1.0f);
}

int main() {
    // Image
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    std::ofstream file("images/image.ppm");

    // Camera
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0f;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Render
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = float(i) / (image_width - 1);
            auto v = float(j) / (image_height - 1);

            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

            color pixel_color = ray_color(r);

            write_color(file, pixel_color);
        }
    }
    file.close();
    std::cout << std::endl << "Done!" << std::endl;
}