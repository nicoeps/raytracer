#pragma once

#include <memory>

#include "ray.h"
#include "texture.h"

Vector3 reflect(const Vector3& direction, const Vector3& normal);
Vector3 refract(const Vector3& direction, const Vector3& normal, double refraction_ratio);
double reflectance(double cosine, double ref_idx);


class Material {
    public:
        Material() {}
        // ~Material();

        virtual bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) = 0;

        virtual Color emitted(Vector2& uv);

        bool get_normal(Vector3& normal, Vector2& uv);

        std::shared_ptr<Texture> normal_map;
};

class Lambertian : public Material {
    public:
        Lambertian(const Color albedo);
        Lambertian(const Color albedo, std::shared_ptr<Texture> normal_map);
        Lambertian(std::shared_ptr<Texture> texture);
        Lambertian(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normal_map);
        // ~Lambertian();

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered);

        std::shared_ptr<Texture> albedo;
};

class Metal : public Material {
    public:
        Metal(const Color albedo);
        Metal(const Color albedo, std::shared_ptr<Texture> normal_map);
        Metal(const Color albedo, double fuzz);
        // Metal(const Color albedo, std::shared_ptr<Texture> normal_map, double fuzz);
        Metal(std::shared_ptr<Texture> texture);
        Metal(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normal_map);
        Metal(std::shared_ptr<Texture> texture, double fuzz);
        // Metal(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normal_map, double fuzz);

        // ~Metal();

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered);

        std::shared_ptr<Texture> albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:
        Dielectric(double ior);
        Dielectric(const Color albedo, double ior);
        Dielectric(std::shared_ptr<Texture> texture, double ior);
        // ~Dielectric();

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered);

        std::shared_ptr<Texture> albedo;
        double ior;
};

class Emission : public Material {
    public:
        Emission(std::shared_ptr<Texture> texture);
        Emission(Color color);

        bool scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered);
        Color emitted(Vector2& uv);

        std::shared_ptr<Texture> emit;
};
