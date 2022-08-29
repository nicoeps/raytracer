#pragma once

#include "vector3.h"

class Camera {
    public:
        Camera(Vector3 position, Vector3 lookat, double fov, double aspect_ratio) {
            // double aspect_ratio = 16.0 / 9.0;
            // double theta = fov * M_PI / 180;
            // double h = tan(theta / 2);
            // double viewport_height = 2.0 * h;
            // double viewport_width = aspect_ratio * viewport_height;

            // Vector3 w = (position - lookat).normalized();
            // Vector3 u = Vector3(0, 1, 0).cross(w).normalized();
            // Vector3 v = w.cross(u);

            // origin = position;
            // horizontal = viewport_width * u;
            // vertical = viewport_height * v;
            // lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;

            // OLD
            aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            double focal_length = 1.0;
            origin = Vector3(0, 0, 0);
            horizontal = Vector3(viewport_width, 0, 0);
            vertical = Vector3(0, viewport_height, 0);
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);

        }
        // ~Camera();

        Ray get_ray(double u, double v) {
            Vector3 direction = lower_left_corner + u*horizontal + v*vertical - origin;
            direction.normalize();
            return Ray(origin, direction);
        }

        Vector3 origin;
        Vector3 horizontal;
        Vector3 vertical;
        Vector3 lower_left_corner;

};
