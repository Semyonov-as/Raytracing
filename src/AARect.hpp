#pragma once

#include "General.hpp"
#include "HittableObject.hpp"
#include "AABB.hpp"


template<class T>
class XYRect : public HittableObject<T> {
public:
    std::shared_ptr<Material<T>> mp;
    T x0, x1, y0, y1, k;

    XYRect() {}
    XYRect(T _x0, T _x1, T _y0, T _y1, T _k, std::shared_ptr<Material<T>> _mp)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(_mp) {}

    bool bounding_box(T, T, AABB<T>& output_box) const override {
        output_box = AABB<T>(Vector3<T>(x0, y0, k - 0.0001), Vector3<T>(x1, y1, k + 0.0001));
        return true;
    }
    bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept override;
};

template<class T>
bool XYRect<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    auto t = (k - r.orig.z())/r.dir.z();

    if(t < t_min || t > t_max)
        return false;

    auto x = r.orig.x() + t*r.dir.x();
    auto y = r.orig.y() + t*r.dir.y();

    if(x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.u = (x - x0)/(x1 - x0);
    rec.v = (y - y0)/(y1 - y0);
    rec.t = t;
    auto out_norm = Vector3<T>(0, 0, 1);
    rec.set_face_normal(r, out_norm);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

template<class T>
class XZRect : public HittableObject<T> {
public:
    std::shared_ptr<Material<T>> mp;
    T x0, x1, z0, z1, k;

    XZRect() {}
    XZRect(T _x0, T _x1, T _z0, T _z1, T _k, std::shared_ptr<Material<T>> _mp)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(_mp) {}

    bool bounding_box(T, T, AABB<T>& output_box) const override {
        output_box = AABB<T>(Vector3<T>(x0, k - 0.0001, z0), Vector3<T>(x1, k + 0.0001, z1));
        return true;
    }
    bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept override;
};

template<class T>
bool XZRect<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    auto t = (k - r.orig.y())/r.dir.y();

    if(t < t_min || t > t_max)
        return false;

    auto x = r.orig.x() + t*r.dir.x();
    auto z = r.orig.z() + t*r.dir.z();

    if(x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.u = (x - x0)/(x1 - x0);
    rec.v = (z - z0)/(z1 - z0);
    rec.t = t;
    auto out_norm = Vector3<T>(0, 1, 0);
    rec.set_face_normal(r, out_norm);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

template<class T>
class YZRect : public HittableObject<T> {
public:
    std::shared_ptr<Material<T>> mp;
    T z0, z1, y0, y1, k;

    YZRect() {}
    YZRect(T _y0, T _y1, T _z0, T _z1, T _k, std::shared_ptr<Material<T>> _mp)
        : z0(_z0), z1(_z1), y0(_y0), y1(_y1), k(_k), mp(_mp) {}

    bool bounding_box(T, T, AABB<T>& output_box) const override {
        output_box = AABB<T>(Vector3<T>(k - 0.0001, y0, z0), Vector3<T>(k + 0.0001, y1, z1));
        return true;
    }
    bool hit(const Ray<T>&, T, T, HitRecord<T>&) const noexcept override;
};

template<class T>
bool YZRect<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    auto t = (k - r.orig.x())/r.dir.x();

    if(t < t_min || t > t_max)
        return false;

    auto z = r.orig.z() + t*r.dir.z();
    auto y = r.orig.y() + t*r.dir.y();

    if(z < z0 || z > z1 || y < y0 || y > y1)
        return false;

    rec.v = (z - z0)/(z1 - z0);
    rec.u = (y - y0)/(y1 - y0);
    rec.t = t;
    auto out_norm = Vector3<T>(1, 0, 0);
    rec.set_face_normal(r, out_norm);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}


