#pragma once

#include "shape.h"
#include "scene.h"
#include "bvh.h"
#include "aabb.h"
#include "vector3.h"

AABB surrounding_box(AABB box0, AABB box1);

class BVH : public Shape {
    public:
        BVH() {}
        // ~BVH();

        BVH(const Scene& scene);

        BVH(const std::vector<std::shared_ptr<Shape>>& in_shapes, size_t start, size_t end);
        bool bounding_box(AABB& aabb);
        bool intersect(const Ray& ray, Shape*& found_shape, double& t_near);
        void getSurfaceProperties(const Vector3& hit,  Vector3& normal, Vector2& uv);

        AABB box;
        std::shared_ptr<Shape> left;
        std::shared_ptr<Shape> right;
};


// class BVH : public Shape {
//     public:
//         BVH() {}
//         // ~BVH();

//         BVH(const Scene& scene) : BVH(scene.shapes, 0, scene.shapes.size()) {}

//         BVH(const std::vector<std::shared_ptr<Shape>>& in_shapes, size_t start, size_t end) {
//             auto shapes = in_shapes;

//             int axis = random_int(0, 2);
//             auto comparator = (axis == 0) ? box_x_compare
//                             : (axis == 1) ? box_y_compare
//                                           : box_z_compare;

//             size_t shape_span = end - start;

//             if (shape_span == 1) {
//                 shape = shapes[start];
//             } else {
//                 std::sort(shapes.begin() + start, shapes.begin() + end, comparator);
//                 auto mid = start + shape_span/2;
//                 left = std::make_shared<BVH>(shapes, start, mid);
//                 right = std::make_shared<BVH>(shapes, mid, end);
//                 AABB box_left, box_right;
//                 if (!left->shape->bounding_box(box_left) || !right->shape->bounding_box(box_right)) {
//                     std::cout << "No bounding box in BVH constructor.\n";
//                 }
//                 shape = surrounding_box(box_left, box_right);
//             }
//         }

//         bool intersect(const Ray& ray, Shape*& found_shape, double& t_near) {
//             double t = t_near;
//             AABB bounding_box;
//             shape->bounding_box(bounding_box);

//             if (!bounding_box.intersect(ray, t)) {
//                 return false;
//             }

//             if (left == nullptr || right == nullptr) {
//                 if (shape->intersect(ray, t)) {
//                     if (t < t_near) {
//                         t_near = t;
//                         found_shape = shape.get();
//                         return true;
//                     }
//                 }
//                 return false;
//             }

//             double t0 = t_near;
//             double t1 = t_near;

//             Shape *found_shape0 = nullptr;
//             Shape *found_shape1 = nullptr;

//             bool hit_left = left->intersect(ray, found_shape0, t0);
//             bool hit_right = right->intersect(ray, found_shape1, t1);
//             bool found = false;
//             if (hit_left && t0 < t_near) {
//                 t_near = t0;
//                 found_shape = found_shape0;
//                 found = true;
//             }
//             if (hit_right && t1 < t_near) {
//                 t_near = t1;
//                 found_shape = found_shape1;
//                 found = true;
//             }

//             return found;
//         }

//         std::shared_ptr<Shape> shape;
//         std::shared_ptr<BVH> left;
//         std::shared_ptr<BVH> right;
// };

// class BVH {
//     public:
//         BVH();
//         // ~BVH();

//         BVH(const Scene& scene) : BVH(scene.shapes, 0, scene.shapes.size()) {}

//         BVH(const std::vector<std::shared_ptr<Shape>>& in_shapes, size_t start, size_t end) {
//             auto shapes = in_shapes;

//             int axis = random_int(0, 2);
//             auto comparator = (axis == 0) ? box_x_compare
//                             : (axis == 1) ? box_y_compare
//                                           : box_z_compare;

//             size_t shape_span = end - start;

//             if (shape_span == 1) {
//                 left = right = shapes[start];
//             } else if (shape_span == 2) {
//                 if (comparator(shapes[start], shapes[start+1])) {
//                     left = shapes[start];
//                     right = shapes[start+1];
//                 } else {
//                     left = shapes[start+1];
//                     right = shapes[start];
//                 }
//             } else {
//                 std::sort(shapes.begin() + start, shapes.begin() + end, comparator);
//                 auto mid = start + shape_span/2;
//                 left = std::make_shared<BVH>(shapes, start, mid);
//                 right = std::make_shared<BVH>(shapes, mid, end);
//             }

//             AABB box_left, box_right;

//             if (!left->bounding_box(box_left) || !right->bounding_box(box_right)) {
//                 std::cout << "No bounding box in BVH constructor.\n";
//             }

//             aabb = surrounding_box(box_left, box_right);
//         }

//         bool intersect(const Ray& ray, double& t) {
//             if (!aabb.intersect(ray)) {
//                 return false;
//             }

//             double t0 = t;
//             double t1 = t;

//             bool hit_left = left->intersect(ray, t0);
//             bool hit_right = right->intersect(ray, t1);

//             return hit_left || hit_right;
//         }

//         bool bounding_box(AABB& aabb) {
//             aabb = this->aabb;
//             return true;
//         }

//         void getSurfaceProperties(const Vector3& hit,  Vector3& normal, Vector3& uv) { }

//         std::shared_ptr<Shape> left;
//         std::shared_ptr<Shape> right;
// };
