#pragma once

#include <iostream>

#include "vec3.hpp"

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    auto scale = 1.0f / samples_per_pixel;
    r = std::sqrtf(scale * r);
    g = std::sqrtf(scale * g);
    b = std::sqrtf(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0f, 0.999f)) << ' ' << static_cast<int>(256 * clamp(g, 0.0f, 0.999f))
        << ' ' << static_cast<int>(256 * clamp(b, 0.0f, 0.999f)) << '\n';
}