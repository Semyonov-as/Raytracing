#pragma once

#include "Ray.hpp"
#include "Vector3DF.hpp"
#include "Point3.hpp"

template<typename T>
class HitRecord {
public:
    Vector3<T> p;
    Vector3<T> normal;
    T t;
    bool front_face;

    void set_face_normal(const Ray<T>& r, const Vector3<T>& out_normal) noexcept {
        front_face = dot(r.direction(), out_normal) < 0;
        normal = front_face ? out_normal : -out_normal;
    }
};

template<typename T>
class HittableObject {
public:
    virtual bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept = 0;
};
