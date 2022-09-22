#pragma once

#include <memory>

#include "vector3.h"
#include "ray.h"
// #include "aabb.h"

class AABB;

#include "material.h"

class Shape {
    public:
        virtual bool intersect(const Ray& ray, Shape*& found_shape, double& t) = 0;
        virtual void getSurfaceProperties(const Vector3& hit,  Vector3& normal, Vector2& uv) = 0;
        virtual bool bounding_box(AABB& aabb) = 0;

        Vector3 min;
        Vector3 max;
        std::shared_ptr<Material> material;
};
