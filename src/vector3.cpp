#include <random>

#include "vector3.h"

double random_double(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1));
}

Vector3 random_in_unit_sphere() {
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
