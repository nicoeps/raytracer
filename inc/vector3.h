// #ifndef VECTOR3_H
// #define VECTOR3_H

#pragma once

#include <Eigen/Dense>
#include <glm/glm.hpp>

#include <random>

using Vector3 = Eigen::Vector3d;
using Vector2 = Eigen::Vector2d;
// using Vector3 = glm::vec3;

inline double random_double(double min=0.0, double max=1.0) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1));
}

inline Vector3 random_in_unit_sphere() {
    while (true) {
        Vector3 p = Vector3::Random();
        if (p.squaredNorm() >= 1) continue;
        return p;
    }
}

Vector3 random_unit_vector() {
    return random_in_unit_sphere().normalized();
}


Vector3 random_in_hemisphere(const Vector3& normal) {
    Vector3 in_unit_sphere = random_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}

// #endif // VECTOR3_H
