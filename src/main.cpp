#include <fstream>
#include <iostream>
#include <sstream>

#include "Camera.hpp"
#include "HittableList.hpp"
#include "color.hpp"
#include "common.hpp"
#include "sphere.hpp"

float hit_sphere(const point3& centre, float radius, const Ray& r) {
    vec3 oc = r.origin() - centre;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discrim = half_b * half_b - a * c;

    if (discrim < 0) {
        return -1.0f;
    } else {
        return (-half_b - std::sqrt(discrim)) / a;
    }
}

color ray_color(const Ray& r, const Hittable& world) {
    hit_record rec;
    if (world.Hit(r, 0, INFIN, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    std::ofstream file("images/image.ppm");
    std::stringstream ss;

    // World
    HittableList world;
    world.Add(std::make_shared<Sphere>(point3(0, 0, -1), 0.5));
    world.Add(std::make_shared<Sphere>(point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;

    // Render
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + randomFloat()) / (image_width - 1);
                auto v = (j + randomFloat()) / (image_height - 1);

                Ray r = cam.getRay(u, v);
                pixel_color += ray_color(r, world);
            }

            write_color(ss, pixel_color, samples_per_pixel);
        }
    }

    file << ss.str();
    file.close();
    std::cout << std::endl << "Done!" << std::endl;
}