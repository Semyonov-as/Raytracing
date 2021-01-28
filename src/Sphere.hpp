#pragma once

#include "HittableObject.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"
#include "Materials.hpp"
#include "AABB.hpp"

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

    bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept override;
    bool bounding_box(T, T, AABB<T>&) const override;

    static void get_sphere_uv(const Vector3<T>&, T&, T&);
};

template<typename T>
bool Sphere<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    auto oc = r.orig - center;
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
    Vector3<T> out_norm = (rec.p - center)/radius;
    rec.set_face_normal(r, out_norm);
    get_sphere_uv(out_norm, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

template<class T>
bool Sphere<T>::bounding_box(T, T, AABB<T>& out) const {
    out = AABB<T>(center - Vector3<T>(radius, radius, radius), center + Vector3<T>(radius, radius, radius));
    return true;
}

template<class T>
void Sphere<T>::get_sphere_uv(const Vector3<T>& p, T& u, T& v) {
    T theta = acos(-p.y());
    T phi = atan2(-p.z(), p.x()) + pi;

    u = phi/(2*pi);
    v = theta/pi;
}
