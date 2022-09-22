#pragma once

#include <vector>
#include <memory>
#include <limits>

#include "shape.h"

class Scene {
    public:
        Scene();
        ~Scene();

        void add(std::shared_ptr<Shape> shape);
        bool raycast(const Ray& ray, Shape*& found_shape, double& t_near);

        std::vector<std::shared_ptr<Shape>> shapes;
};
