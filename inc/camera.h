#pragma once

#include "vector3.h"
#include "ray.h"

class Camera {
    public:
        Camera(Vector3 position, Vector3 lookat, double fov, double aspect_ratio);
        // ~Camera();
        Ray get_ray(double u, double v);

        Vector3 origin;
        Vector3 horizontal;
        Vector3 vertical;
        Vector3 lower_left_corner;
};
