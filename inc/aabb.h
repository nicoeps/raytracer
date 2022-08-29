#pragma once

#include "vector3.h"
#include "ray.h"

class AABB {
    public:
        AABB() { }
        AABB(const Vector3& a, const Vector3& b) : minimum(a), maximum(b) {}
        // ~AABB();

        // bool intersect(const Ray& ray, double& t) {
        //     for (int i = 0; i < 3; ++i) {
        //         double invD = 1.0 / ray.direction[i];
        //         double t0 = (minimum[i] - ray.origin[i]) * invD;
        //         double t1 = (maximum[i] - ray.origin[i]) * invD;
        //         if (invD < 0.0) {
        //             std::swap(t0, t1);
        //         }
        //         double t_min = t0 > 0 ? t0 : 0;
        //         double t_max = t1 < std::numeric_limits<double>::infinity() ? t1 : std::numeric_limits<double>::infinity();
        //         if (t_max <= t_min) {
        //             return false;
        //         }
        //     }
        //     return true;
        // }

        bool intersect(const Ray& ray, double& t) const {
            // for (int i = 0; i < 3; ++i) {
            //     auto t1 = std::max((minimum[i] - ray.origin[i]) / ray.direction[i],
            //                    (maximum[i] - ray.origin[i]) / ray.direction[i]);
            //     if (t1 <= 0)
            //         return false;
            // }

            float tmin = (minimum.x() - ray.origin.x()) / ray.direction.x(); 
            float tmax = (maximum.x() - ray.origin.x()) / ray.direction.x(); 
        
            if (tmin > tmax) std::swap(tmin, tmax); 
        
            float tymin = (minimum.y() - ray.origin.y()) / ray.direction.y(); 
            float tymax = (maximum.y() - ray.origin.y()) / ray.direction.y(); 
        
            if (tymin > tymax) std::swap(tymin, tymax); 
        
            if ((tmin > tymax) || (tymin > tmax)) 
                return false; 
        
            if (tymin > tmin) 
                tmin = tymin; 
        
            if (tymax < tmax) 
                tmax = tymax; 
        
            float tzmin = (minimum.z() - ray.origin.z()) / ray.direction.z(); 
            float tzmax = (maximum.z() - ray.origin.z()) / ray.direction.z(); 
        
            if (tzmin > tzmax) std::swap(tzmin, tzmax); 
        
            if ((tmin > tzmax) || (tzmin > tmax)) 
                return false; 
        
            if (tzmin > tmin) 
                tmin = tzmin; 
        
            if (tzmax < tmax) 
                tmax = tzmax; 
  
            return true;
        }

        Vector3 minimum;
        Vector3 maximum;
};

inline bool box_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b, int axis) {
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
        std::cout << "No bounding box in BVH constructor.\n";

    return box_a.minimum[axis] < box_b.minimum[axis];
}


bool box_x_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<Shape> a, const std::shared_ptr<Shape> b) {
    return box_compare(a, b, 2);
}
