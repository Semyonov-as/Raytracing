#pragma once

#include <cmath>

#include "HittableObject.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"

template<typename T>
class Sphere : public HittableObject<T> {
public:
    Vector3<T> center;
    T radius;

    Sphere() {}
    Sphere(Vector3<T> _center, T _radius) : center(_center), radius(_radius) {}

    virtual bool hit(const Ray<T>&, T, T, HitRecord<T>&) const override;
};

template<typename T>
bool Sphere<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const {
    Vector3<T> oc = r.origin() - center;
    T a = r.direction().length_squared();
    T half_b = dot(oc, r.direction());
    T c = oc.length_squared() - radius*radius;

    T D = half_b*half_b - a*c;
    if (D < 0)
        return false;

    T root = (-half_b - sqrt(D))/a;
    if(root < t_min || root > t_max) {
        root = (- half_b + sqrt(D))/a;
        if(root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    rec.normal = (rec.p - center)/radius;

    return true;
}
