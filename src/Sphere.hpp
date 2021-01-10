#pragma once

#include "HittableObject.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"

#include <cmath>

template<typename T>
class Sphere : public HittableObject<T> {
public:
    Vector3<T> center;
    T radius;

    Sphere() noexcept {}
    Sphere(Vector3<T> _center, T _radius) noexcept : center(_center), radius(_radius) {}

    virtual bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept override;
};

template<typename T>
bool Sphere<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    auto oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto D = half_b*half_b - a*c;
    if (D < 0)
        return false;

    auto root = (-half_b - sqrt(D))/a;
    if (root < t_min || root > t_max) {
        root = (- half_b + sqrt(D))/a;
        if (root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    rec.set_face_normal(r, (rec.p - center)/radius);

    return true;
}
