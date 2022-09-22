#pragma once

#include <memory>

#include "vector3.h"
#include "ray.h"
#include "shape.h"


class AABB {
    public:
        AABB() {}
        AABB(const Vector3& a, const Vector3& b);
        // ~AABB();

        bool intersect(const Ray& ray, double& t) const;

        Vector3 minimum;
        Vector3 maximum;
};

inline bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axis);
bool box_x_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
bool box_y_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
bool box_z_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b);
