#pragma once

#include "vector3.h"
#include "color.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"


class Texture
{
public:
    Texture() {}

    virtual Color value(double u, double v) = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor() {}
    SolidColor(Color color) : color(color) {}


    Color value(double u, double v) {
        return color;
    }

    Color color;
};

class ImageTexture : public Texture
{
public:
    const static int bytes_per_pixel = 3;

    ImageTexture() : data(nullptr), width(0), height(0) {}
    ImageTexture(const char* filename) {
        int components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;

    }

    ~ImageTexture() {
        delete data;
    }

    Color value(double u, double v) {
        if (data == nullptr) {
            return Color(0, 1, 1);
        }

        u = std::clamp(u, 0.0, 1.0);
        v = 1 - std::clamp(v, 0.0, 1.0);

        int i = static_cast<int>(u*width);
        int j = static_cast<int>(v*height);

        if (i >= width) {
            i = width-1;
        }
        if (j >= height) {
            j = height-1;
        }

        double color_scale = 1.0 / 255.0;
        auto pixel = data + j*bytes_per_scanline +i*bytes_per_pixel;

        return Color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};

// class NormalTexture : public Texture
// {
// public:
//     NormalTexture() {}

//     Color value(double u, double v) {
//         double theta = (2 * M_PI) * u;
//         double phi = M_PI * v;

//         double dX = std::cos(theta) * std::sin(phi);
//         double dZ = std::sin(theta) * std::sin(phi);
//         double dY = std::cos(phi);
//         Color a(dX, dY, dZ);
//         a.normalize();
//         return a;
//     }
// };
