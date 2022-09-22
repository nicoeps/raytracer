#include "vector3.h"
#include "bvh.h"

AABB surrounding_box(AABB box0, AABB box1) {
    Vector3 small(
        std::min(box0.minimum.x(), box1.minimum.x()),
        std::min(box0.minimum.y(), box1.minimum.y()),
        std::min(box0.minimum.z(), box1.minimum.z())
    );

    Vector3 big(
        std::max(box0.maximum.x(), box1.maximum.x()),
        std::max(box0.maximum.y(), box1.maximum.y()),
        std::max(box0.maximum.z(), box1.maximum.z())
    );

    return AABB(small, big);
}

BVH::BVH(const Scene& scene) : BVH(scene.shapes, 0, scene.shapes.size()) {}

BVH::BVH(const std::vector<std::shared_ptr<Shape>>& in_shapes, size_t start, size_t end) {
    auto shapes = in_shapes;

    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                    : box_z_compare;

    size_t shape_span = end - start;

    if (shape_span == 1) {
        left = right = shapes[start];
    } else if (shape_span == 2) {
        if (comparator(shapes[start], shapes[start+1])) {
            left = shapes[start];
            right = shapes[start+1];
        } else {
            left = shapes[start+1];
            right = shapes[start]; 
        }
    } else {
        std::sort(shapes.begin() + start, shapes.begin() + end, comparator);

        auto mid = start + shape_span/2;
        left = std::make_shared<BVH>(shapes, start, mid);
        right = std::make_shared<BVH>(shapes, mid, end);
    }

    AABB box_left, box_right;

    if (!left->bounding_box(box_left)
        || !right->bounding_box(box_right)) {
            std::cout << "No bounding box in BVH constructor.\n";
    }
    box = surrounding_box(box_left, box_right);
}

bool BVH::bounding_box(AABB& aabb) {
    aabb = box;
    return true;
}

bool BVH::intersect(const Ray& ray, Shape*& found_shape, double& t_near) {
    double t = t_near;
    if (!box.intersect(ray, t)) {
        return false;
    }

    double t0 = t_near;
    double t1 = t_near;

    Shape *found_shape0 = nullptr;
    Shape *found_shape1 = nullptr;

    bool hit_left = left->intersect(ray, found_shape0, t0);
    bool hit_right = right->intersect(ray, found_shape1, t1);
    bool found = false;
    if (hit_left && t0 < t_near) {
        t_near = t0;
        found_shape = found_shape0;
        found = true;
    }
    if (hit_right && t1 < t_near) {
        t_near = t1;
        found_shape = found_shape1;
        found = true;
    }

    return found;
}

void BVH::getSurfaceProperties(const Vector3& hit,  Vector3& normal, Vector2& uv) { }
