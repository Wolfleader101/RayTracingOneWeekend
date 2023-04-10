#include <fstream>
#include <iostream>
#include <sstream>

#include "Camera.hpp"
#include "HittableList.hpp"
#include "Material.hpp"
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

color ray_color(const Ray& r, const Hittable& world, int depth) {
    hit_record rec;
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    if (world.Hit(r, 0.001f, INFIN, rec)) {
        Ray scattered;
        color attenuation;

        if (rec.mat_ptr->Scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);

        return color(0, 0, 0);
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
    const int max_depth = 50;

    std::ofstream file("images/image.ppm");
    std::stringstream ss;

    // World
    HittableList world;
    auto material_ground = std::make_shared<Diffuse>(color(0.8f, 0.8f, 0.0f));
    // auto material_center = std::make_shared<Diffuse>(color(0.7f, 0.3f, 0.3f));
    // auto material_left = std::make_shared<Metal>(color(0.8f, 0.8f, 0.8f), 0.3f);
    auto material_center = std::make_shared<Dielectric>(1.5f);
    auto material_left = std::make_shared<Dielectric>(1.5f);
    auto material_right = std ::make_shared<Metal>(color(0.8f, 0.6f, 0.2f), 1.0f);

    world.Add(std ::make_shared<Sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
    world.Add(std ::make_shared<Sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f, material_center));
    world.Add(std ::make_shared<Sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
    world.Add(std ::make_shared<Sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));

    // Camera
    Camera cam;

    // Render
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cout << "\rRows remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + randomFloat()) / (image_width - 1);
                auto v = (j + randomFloat()) / (image_height - 1);

                Ray r = cam.getRay(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(ss, pixel_color, samples_per_pixel);
        }
    }

    file << ss.str();
    file.close();
    std::cout << std::endl << "Done!" << std::endl;
}