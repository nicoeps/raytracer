// #ifndef RAY_H
// #define RAY_H

#pragma once

#include "vector3.h"

class Ray {
    public:
        Ray() {}
        Ray(const Vector3& origin, const Vector3& direction)
            : origin(origin), direction(direction) {}

        Vector3 origin;
        Vector3 direction;

        Vector3 p(double t) const {
            return origin + t * direction;
        }
};

// #endif // RAY_H
