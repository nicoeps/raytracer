#pragma once

#include <memory>

#include "texture.h"

Vector3 reflect(const Vector3& direction, const Vector3& normal) {
    return direction - 2*(direction.dot(normal))*normal;
}

Vector3 refract(const Vector3& direction, const Vector3& normal, double refraction_ratio) {
    // double test = uv.dot(normal);
    // double cos_theta = std::clamp(test, -1.0, 1.0);
    // double ior_a = 1;
    // double ior_b = ior;
    // if (cos_theta < 0) {
    //     cos_theta = -cos_theta;
    // } else {
    //     std::swap(ior_a, ior_b);
    //     // normal2 = -normal2;
    // }
    
    // double refraction_ratio = ior_a / ior_b;
    // return uv + (refraction_ratio * cos_theta - sqrt(1 - refraction_ratio*refraction_ratio*(1 - cos_theta*cos_theta))) * (cos_theta < 0 ? normal : -normal);

    double cos_theta = std::min((-direction).dot(normal), 1.0);
    Vector3 r_out_perp = refraction_ratio * (direction + cos_theta*normal);
    Vector3 r_out_parallel = -sqrt(abs(1.0 - r_out_perp.squaredNorm())) * normal;
    return r_out_perp + r_out_parallel;
}

double reflectance(double cosine, double ref_idx) {
    double r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine), 5);
}


class Material {
    public:
        Material() {}
        // ~Material();

        virtual bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Color albedo) : albedo(std::make_shared<SolidColor>(albedo)) {}
        Lambertian(std::shared_ptr<Texture> texture) : albedo(texture) {}
        // ~Lambertian();

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) {
            // Vector3 scatter_direction = normal + random_unit_vector();
            Vector3 target = hit + random_in_hemisphere(normal);
            Vector3 scatter_direction = target - hit;
            // if ((scatter_direction.array().abs() < 1e-8).all()) {
                // scatter_direction = normal;
            // }
            scatter_direction.normalize();
            scattered = Ray(hit, scatter_direction);
            attenuation = albedo->value(uv.x(), uv.y());
            // double test = normal.dot(scatter_direction);
            // attenuation = albedo / M_PI * std::max(test, 0.0);
            return true;
        }

        std::shared_ptr<Texture> albedo;
};

class Metal : public Material {
    public:
        Metal(const Color albedo) : albedo(std::make_shared<SolidColor>(albedo)), fuzz(0) {}
        Metal(const Color albedo, double fuzz) : albedo(std::make_shared<SolidColor>(albedo)), fuzz(fuzz) {}
        Metal(std::shared_ptr<Texture> texture) : albedo(texture), fuzz(0) {}
        Metal(std::shared_ptr<Texture> texture, double fuzz) : albedo(texture), fuzz(fuzz) {}
        // ~Metal();

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) {
            Vector3 reflected = reflect(ray.direction, normal);
            reflected.normalize();
            scattered = Ray(hit, reflected + fuzz*random_in_hemisphere(normal)); // + fuzz*random_in_unit_sphere());
            attenuation = albedo->value(uv.x(), uv.y());
            return (reflected.dot(normal) > 0);
        }

        std::shared_ptr<Texture> albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double ior) : albedo(std::make_shared<SolidColor>(Vector3(1.0, 1.0, 1.0))), ior(ior) {}
        Dielectric(const Color albedo, double ior) : albedo(std::make_shared<SolidColor>(albedo)), ior(ior) {}
        Dielectric(std::shared_ptr<Texture> texture, double ior) : albedo(texture), ior(ior) {}
        // ~Dielectric();

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) {
            attenuation = albedo->value(uv.x(), uv.y());

            double refraction_ratio = ior;
            Vector3 n = Vector3(normal);
            if (ray.direction.dot(n) < 0) {
                refraction_ratio = 1.0/refraction_ratio;
            } else {
                n = -n;
            }

            double cos_theta = std::min((-ray.direction).dot(n), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            Vector3 direction;
            if ((refraction_ratio * sin_theta > 1.0) || (reflectance(cos_theta, refraction_ratio) > random_double())) {
                direction = reflect(ray.direction, n);
            } else {
                direction = refract(ray.direction, n, refraction_ratio);
            }

            direction.normalize();
            scattered = Ray(hit, direction);
            return true;
        }

        std::shared_ptr<Texture> albedo;
        double ior;
};
