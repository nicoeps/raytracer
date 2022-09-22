#pragma once

#include "ray.h"
#include "material.h"
#include "shape.h"
#include "aabb.h"

class Sphere : public Shape {
    public:
        // Sphere() {}
        Sphere(Vector3 origin, double radius, std::shared_ptr<Material> material);

        bool intersect(const Ray& ray, Shape*& found_shape, double& t);
        void getSurfaceProperties(const Vector3& hit,  Vector3& normal, Vector2& uv);
        bool bounding_box(AABB& aabb);

        Vector3 origin;
        double radius;
        double radius_squared;
};
