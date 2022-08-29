#pragma once

#include <vector>
#include <memory>
#include <limits>

class Scene {
    public:
        Scene(/* args */);
        ~Scene();

        void add(std::shared_ptr<Shape> shape) {
            shapes.push_back(std::move(shape));
        }

        std::vector<std::shared_ptr<Shape>> shapes;

        bool raycast(const Ray& ray, Shape*& found_shape, double& t_near) {
            t_near = std::numeric_limits<double>::infinity();
            double t = t_near;
            for (auto& shape : shapes) {
                Shape* f_s;
                if (shape->intersect(ray, f_s, t) && t < t_near) {
                    found_shape = f_s;
                    t_near = t;
                }
            }

            return found_shape != nullptr;
        }

        // bool bounding_box(AABB& aabb) {
        //     if (shapes.empty()) return false;

        //     AABB temp;
        //     bool first_box = true;

        //     for (const auto& shape : shapes) {
        //         if (!shape->bounding_box(temp)) return false;
        //         aabb = first_box ? temp : surrounding_box(aabb, temp);
        //         first_box = false;
        //     }
        // }
};

Scene::Scene(/* args */)
{
}

Scene::~Scene()
{
}
