#pragma once

#include "HittableObject.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"
#include "Materials.hpp"

#include <cmath>
#include <stdexcept>
#include <cstdlib>


template<typename T>
class Sphere : public HittableObject<T> {
public:
    Vector3<T> center;
    T radius;
    std::shared_ptr<Material<T>> mat_ptr;

    Sphere() noexcept {}
    Sphere(Vector3<T> _center, T _radius, std::shared_ptr<Material<T>> _mat_ptr) noexcept : center(_center), radius(_radius), mat_ptr(_mat_ptr) {}

    virtual bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept override;
};

template<typename T>
bool Sphere<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    auto oc = r.orig - center;
    auto a = r.direction().length_squared();
    if(a == 0)
        throw std::invalid_argument("wrong ray");
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
    rec.mat_ptr = mat_ptr;

    return true;
}
