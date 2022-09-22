#include "ray.h"

Ray::Ray(const Vector3& origin, const Vector3& direction)
    : origin(origin), direction(direction) {}

Vector3 Ray::p(double t) const {
    return origin + t * direction;
}
