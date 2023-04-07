#include <fstream>
#include <iostream>

#include "color.hpp"
#include "vec3.hpp"

int main() {
    // Image

    const int image_width = 256;
    const int image_height = 256;

    std::ofstream file("images/image.ppm");

    // Render

    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(float(i) / (image_height - 1), float(j) / (image_width - 1), 0.25);

            write_color(file, pixel_color);
        }
    }
    file.close();
    std::cout << std::endl << "Done!" << std::endl;
}