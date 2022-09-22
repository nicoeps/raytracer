#pragma once

#include <Eigen/Dense>
// #include <glm/glm.hpp>

using Vector3 = Eigen::Vector3d;
using Vector2 = Eigen::Vector2d;
// using Vector3 = glm::vec3;

double random_double(double min=0.0, double max=1.0);
int random_int(int min, int max);
Vector3 random_in_unit_sphere();
Vector3 random_unit_vector();
Vector3 random_in_hemisphere(const Vector3& normal);
