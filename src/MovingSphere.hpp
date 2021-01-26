#pragma once

#include "HittableObject.hpp"
#include "Vector3.hpp"
#include "General.hpp"
#include "Materials.hpp"

#include <memory>

template<class T>
class MovingSphere : public HittableObject<T> {
public:
    Vector3<T> center0, center1;
    T time0, time1, radius;
    std::shared_ptr<Material<T>> mat_ptr;

    MovingSphere() {}
    MovingSphere(Vector3<T> _center0, Vector3<T> _center1, T _time0, T _time1, T _radius, std::shared_ptr<Material<T>> _mat_ptr)
        : center0(_center0), center1(_center1), time0(_time0), time1(_time1), radius(_radius), mat_ptr(_mat_ptr) {}

    bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept override;
    bool bounding_box(T, T, AABB<T>&) const override;

    Vector3<T> center(T time) const { return center0 + (center1-center0)*((time-time0)/(time1-time0));}
};


template<class T>
bool MovingSphere<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    auto oc = r.orig - center(r.time);
    auto a = r.direction().length_squared();
    if (a == 0)
        return false;
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
    rec.set_face_normal(r, (rec.p - center(r.time))/radius);
    rec.mat_ptr = mat_ptr;

    return true;
}

template<class T>
bool MovingSphere<T>::bounding_box(T t0, T t1, AABB<T>& out) const {
    AABB<T> box0(center(t0) - Vector3<T>(radius, radius, radius), center(t0) + Vector3<T>(radius, radius, radius));
    AABB<T> box1(center(t1) - Vector3<T>(radius, radius, radius), center(t1) + Vector3<T>(radius, radius, radius));
    out = AABB<T>::surrounding_box(box1, box0);
    return true;
}
