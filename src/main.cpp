#include <atomic>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "Camera.hpp"
#include "HittableList.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "color.hpp"
#include "common.hpp"

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

void RenderPartial(int row_start, int row_end, int width, int height, int samples_per_pixel, int max_depth,
                   const Camera& cam, const Hittable& world, std::atomic<int>& completed_pixels, std::stringstream& ss,
                   std::mutex& ss_mutex) {
    for (int y = row_start; y < row_end; y++) {
        for (int x = 0; x < width; x++) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (x + randomFloat()) / (width - 1);
                auto v = (y + randomFloat()) / (height - 1);

                Ray r = cam.getRay(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            std::unique_lock<std::mutex> lock(ss_mutex);
            write_color(ss, pixel_color, samples_per_pixel);
            lock.unlock();

            completed_pixels++;
        }
    }
}

void RenderMultiThreaded(int width, int height, int num_threads, int samples_per_pixel, int max_depth,
                         const Camera& cam, const Hittable& world, std::stringstream& ss) {
    std::atomic<int> completed_pixels(0);
    std::mutex cout_mutex;
    std::mutex ss_mutex;

    std::vector<std::thread> threads;
    int rows_per_thread = height / num_threads;

    ss << "P3\n" << width << ' ' << height << "\n255\n";

    for (int i = 0; i < num_threads; i++) {
        int row_start = i * rows_per_thread;
        int row_end = (i + 1) * rows_per_thread;

        if (i == num_threads - 1) {
            row_end = height;
        }

        threads.push_back(std::thread(RenderPartial, row_start, row_end, width, height, samples_per_pixel, max_depth,
                                      std::ref(cam), std::ref(world), std::ref(completed_pixels), std::ref(ss),
                                      std::ref(ss_mutex)));
    }

    while (completed_pixels < width * height) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Sleep for 100ms to avoid excessive CPU usage

        // Lock the mutex and update the progress.
        std::unique_lock<std::mutex> lock(cout_mutex);
        double percentage = static_cast<double>(completed_pixels) / (width * height) * 100.0;
        std::cout << "\rRendering progress: " << std::fixed << std::setprecision(2) << percentage << "%";
        std::cout.flush();
        lock.unlock();
    }

    // Join the threads once they have finished rendering.
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << std::endl;
}

void RenderSingleThreaded(int width, int height, int samples_per_pixel, int max_depth, const Camera& cam,
                          const Hittable& world, std::stringstream& ss) {
    int completed_pixels = 0;
    ss << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + randomFloat()) / (width - 1);
                auto v = (j + randomFloat()) / (height - 1);

                Ray r = cam.getRay(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(ss, pixel_color, samples_per_pixel);
            completed_pixels++;
        }

        double percentage = static_cast<double>(completed_pixels) / (width * height) * 100.0;
        std::cout << "\rRendering progress: " << std::fixed << std::setprecision(2) << percentage << "%";
        std::cout.flush();
    }
}
int main() {
    // Image
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
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
    std::cout << "Starting Render" << std::endl;
    int num_threads = std::thread::hardware_concurrency();
    // RenderMultiThreaded(image_width, image_height, num_threads, samples_per_pixel, max_depth, cam, world, ss);
    RenderSingleThreaded(image_width, image_height, samples_per_pixel, max_depth, cam, world, ss);

    file << ss.str();
    file.close();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();

    std::cout << std::endl << "Done in " << duration << "s" << std::endl;
}