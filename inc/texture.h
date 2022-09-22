#pragma once

#include "vector3.h"
#include "color.h"

class Texture {
    public:
        Texture() {}

        virtual Color value(Vector2& uv) = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor() {}
        SolidColor(Color color);

        Color value(Vector2& uv);

        Color color;
};

class ImageTexture : public Texture {
    public:
        const static int bytes_per_pixel = 3;

        ImageTexture();
        ImageTexture(const char* filename);
        ~ImageTexture();

        Color value(Vector2& uv);    

        unsigned char* data;
        int width, height;
        int bytes_per_scanline;
};
