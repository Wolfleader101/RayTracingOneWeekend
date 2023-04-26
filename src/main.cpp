#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Camera.hpp"
#include "HittableList.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "color.hpp"
#include "common.hpp"

float hit_Sphere(const point3& centre, float radius, const Ray& r) {
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

HittableList random_scene() {
    HittableList world;

    auto ground_material = std::make_shared<Diffuse>(color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomFloat();
            point3 center(a + 0.9f * randomFloat(), 0.2f, b + 0.9f * randomFloat());

            if ((center - point3(4.f, 0.2f, 0.f)).length() > 0.9f) {
                std::shared_ptr<Material> Sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    Sphere_material = std::make_shared<Diffuse>(albedo);
                    world.Add(std::make_shared<Sphere>(center, 0.2, Sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randomFloat(0, 0.5);
                    Sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2, Sphere_material));
                } else {
                    // glass
                    Sphere_material = std::make_shared<Dielectric>(1.5);
                    world.Add(std::make_shared<Sphere>(center, 0.2, Sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Diffuse>(color(0.4f, 0.2f, 0.1f));
    world.Add(std::make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(color(0.7f, 0.6f, 0.5f), 0.0);
    world.Add(std::make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {
    // Image
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    std::ofstream file("images/image.ppm");
    std::stringstream ss;

    // World
    auto world = random_scene();

    // Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    float dist_to_focus = 10.0;
    float aperature = 0.1f;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperature, dist_to_focus);

    auto start = std::chrono::high_resolution_clock::now();

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

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();

    std::cout << std::endl << "Done in " << duration << "s" << std::endl;
}