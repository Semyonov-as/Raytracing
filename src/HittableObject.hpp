#pragma once

#include "Ray.hpp"
#include "Vector3DF.hpp"
#include "Point3.hpp"
#include "Materials.hpp"
#include "AABB.hpp"

#include <cstdlib>

template<typename T>
class HitRecord {
public:
    Vector3<T> p;
    Vector3<T> normal;
    std::shared_ptr<Material<T>> mat_ptr;
    T t;
    T u, v;
    bool front_face;

    void set_face_normal(const Ray<T>& r, const Vector3<T>& out_normal) noexcept {
        front_face = dot(r.direction(), out_normal) < 0;
        normal = front_face ? out_normal : -out_normal;
    }
};

template<typename T>
class HittableObject {
public:
    virtual bool bounding_box(T, T, AABB<T>&) const = 0;
    virtual bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept = 0;
};
