#pragma once

#include "material.h"
#include "color.h"
#include "vector3.h"

class Lambertian : public Material {
    public:
        Lambertian(const Color albedo) : albedo(albedo) {}
        // ~Lambertian();

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Color& attenuation, Ray& scattered) {
            // Vector3 scatter_direction = normal + random_unit_vector();
            Vector3 target = hit + random_in_hemisphere(normal);
            Vector3 scatter_direction = target - hit;
            scatter_direction.normalize();
            scattered = Ray(hit, scatter_direction);
            // double test = normal.dot(scatter_direction);
            // attenuation = albedo / M_PI * std::max(test, 0.0);
            attenuation = albedo;
            return true;
        }

        Color albedo;
};
