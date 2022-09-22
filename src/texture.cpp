#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

#include "texture.h"

SolidColor::SolidColor(Color color) : color(color) {}

Color SolidColor::value(Vector2& uv) {
    return color;
}

ImageTexture::ImageTexture() : data(nullptr), width(0), height(0) {}
ImageTexture::ImageTexture(const char* filename) {
    int components_per_pixel = bytes_per_pixel;
    data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

    if (!data) {
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;

}

ImageTexture::~ImageTexture() {
    delete data;
}

Color ImageTexture::value(Vector2& uv) {
    if (data == nullptr) {
        return Color(0, 1, 1);
    }

    double u = std::clamp(uv[0], 0.0, 1.0);
    double v = 1 - std::clamp(uv[1], 0.0, 1.0);

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
