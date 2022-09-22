#include "sphere.h"
#include "functions.h"

Sphere::Sphere(Vector3 origin, double radius, std::shared_ptr<Material> material) : origin(origin), radius(radius) {
    radius_squared = radius*radius;
    this->material = material;
}

bool Sphere::intersect(const Ray& ray, Shape*& found_shape, double& t) {
    Vector3 oo = ray.origin - origin;
    
    double t0;
    double t1;
    
    double a = ray.direction.dot(ray.direction); // = 1 (when normalized)
    double b = 2.0 * oo.dot(ray.direction);
    double c = oo.dot(oo) - radius_squared;
    if (!quadraticSolver(a, b, c, t0, t1)) {
        return false;
    }
    if (t0 < 0.001) {
        t0 = t1;
        if (t0 < 0.001) {
            return false;
        }
    }

    t = t0;

    found_shape = &*this;

    return true;
}

void Sphere::getSurfaceProperties(const Vector3& hit,  Vector3& normal, Vector2& uv) {
    normal = hit - origin;
    normal.normalize();
    if (radius < 0) {
        normal = -normal;
    }
    double theta = acos(-normal.y());
    double phi = atan2(-normal.z(), normal.x()) + M_PI;

    uv = Vector2(phi / (2*M_PI), theta / M_PI);

    if (material != nullptr) {
        material->get_normal(normal, uv);
    }
}

bool Sphere::bounding_box(AABB& aabb) {
    double r = abs(radius);
    aabb = AABB(
        origin - Vector3(r, r, r),
        origin + Vector3(r, r, r)
    );
    return true;
}
