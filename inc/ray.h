#pragma once

#include "vector3.h"

class Ray {
    public:
        Ray() {}
        Ray(const Vector3& origin, const Vector3& direction);

        Vector3 p(double t) const;

        Vector3 origin;
        Vector3 direction;
};
