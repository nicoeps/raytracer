#include "color.h"

#include <iostream>
#include <algorithm>

using Color = Vector3;

void write_color(std::ostream &out, Color pixel_color, int samples) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Divide the color by the number of samples.
    double scale = 1.0 / samples;
    r = sqrt(scale*r);
    g = sqrt(scale*g);
    b = sqrt(scale*b);
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
}

void write_color2(int framebuffer[], Color pixel_color, int samples, int index) {
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // Divide the color by the number of samples.
    double scale = 1.0 / samples;
    r = sqrt(scale*r);
    g = sqrt(scale*g);
    b = sqrt(scale*b);
    index = index * 3;
    // std::cout << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
    //     << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
    //     << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
    framebuffer[index++] = static_cast<int>(256 * std::clamp(r, 0.0, 0.999));
    framebuffer[index++] = static_cast<int>(256 * std::clamp(g, 0.0, 0.999));
    framebuffer[index] = static_cast<int>(256 * std::clamp(b, 0.0, 0.999));
}
