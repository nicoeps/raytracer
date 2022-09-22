#pragma once

#include "vector3.h"

#include <iostream>

using Color = Vector3;

void write_color(std::ostream &out, Color pixel_color, int samples);
void write_color2(int framebuffer[], Color pixel_color, int samples, int index);
