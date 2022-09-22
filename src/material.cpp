#include "material.h"
#include "vector3.h"

#include <Eigen/Dense>

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

Color Material::emitted(Vector2& uv) {
    return Color(0, 0, 0);
}

bool Material::get_normal(Vector3& normal, Vector2& uv) {
    if (normal_map != nullptr) {
        Vector3 tangent(1, 1, (normal.x() + normal.y()) / (-normal.z()));
        tangent.normalize();
        Vector3 tangentspacenormal = normal_map->value(uv);

        tangentspacenormal = (2*tangentspacenormal).array()-1;
        double temp = tangentspacenormal[1];
        tangentspacenormal[1] = tangentspacenormal[2];
        tangentspacenormal[2] = temp;
        Vector3 z = tangent.normalized();
        Vector3 x = tangent.cross(normal);
        Vector3 y = x.cross(tangent);
        x.normalize();
        y.normalize();

        double x1 = tangentspacenormal.x()*x[0] + tangentspacenormal.y()*y[0] + tangentspacenormal.z()*z[0];
        double y1 = tangentspacenormal.x()*x[1] + tangentspacenormal.y()*y[1] + tangentspacenormal.z()*z[1];
        double z1 = tangentspacenormal.x()*x[2] + tangentspacenormal.y()*y[2] + tangentspacenormal.z()*z[2];

        Vector3 newnormal(x1, y1, z1);

        normal = newnormal;
        normal.normalize();

        return true;
    }

    return false;
}

Lambertian::Lambertian(const Color albedo) : albedo(std::make_shared<SolidColor>(albedo)) {}
Lambertian::Lambertian(const Color albedo, std::shared_ptr<Texture> normal_map) : albedo(std::make_shared<SolidColor>(albedo)) {
    this->normal_map = normal_map;
}
Lambertian::Lambertian(std::shared_ptr<Texture> texture) : albedo(texture) {}
Lambertian::Lambertian(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normal_map) : albedo(texture) {
    this->normal_map = normal_map;
}

bool Lambertian::scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) {
    // Vector3 scatter_direction = normal + random_unit_vector();
    Vector3 target = hit + random_in_hemisphere(normal);
    Vector3 scatter_direction = target - hit;
    // if ((scatter_direction.array().abs() < 1e-8).all()) {
        // scatter_direction = normal;
    // }
    scatter_direction.normalize();
    scattered = Ray(hit, scatter_direction);
    attenuation = albedo->value(uv);
    // double test = normal.dot(scatter_direction);
    // attenuation = albedo / M_PI * std::max(test, 0.0);
    return true;
}

Metal::Metal(const Color albedo) : albedo(std::make_shared<SolidColor>(albedo)), fuzz(0) {}
Metal::Metal(const Color albedo, std::shared_ptr<Texture> normal_map) : albedo(std::make_shared<SolidColor>(albedo)) {
    this->normal_map = normal_map;
}
Metal::Metal(const Color albedo, double fuzz) : albedo(std::make_shared<SolidColor>(albedo)), fuzz(fuzz) {}
Metal::Metal(std::shared_ptr<Texture> texture) : albedo(texture), fuzz(0) {}
Metal::Metal(std::shared_ptr<Texture> texture, double fuzz) : albedo(texture), fuzz(fuzz) {}
Metal::Metal(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normal_map) : albedo(texture) {
    this->normal_map = normal_map;
}

bool Metal::scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) {
    Vector3 reflected = reflect(ray.direction, normal);
    reflected.normalize();
    scattered = Ray(hit, reflected + fuzz*random_in_hemisphere(normal)); // + fuzz*random_in_unit_sphere());
    attenuation = albedo->value(uv);
    // attenuation = (normal.array() + 1) / 2;
    return (reflected.dot(normal) > 0);
}

Dielectric::Dielectric(double ior) : albedo(std::make_shared<SolidColor>(Vector3(1.0, 1.0, 1.0))), ior(ior) {}
Dielectric::Dielectric(const Color albedo, double ior) : albedo(std::make_shared<SolidColor>(albedo)), ior(ior) {}
Dielectric::Dielectric(std::shared_ptr<Texture> texture, double ior) : albedo(texture), ior(ior) {}

bool Dielectric::scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) {
    attenuation = albedo->value(uv);

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


Emission::Emission(std::shared_ptr<Texture> texture) : emit(texture) {}
Emission::Emission(Color color) : emit(std::make_shared<SolidColor>(color)) {}

bool Emission::scatter(const Ray& ray, Vector3& hit, Vector3& normal, Vector2& uv, Color& attenuation, Ray& scattered) {
    return false;
}

Color Emission::emitted(Vector2& uv) {
    return emit->value(uv);
}
