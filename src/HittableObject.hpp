#pragma once

#include "Ray.hpp"
#include "Vector3DF.hpp"
#include "Point3.hpp"
#include "Materials.hpp"
#include "AABB.hpp"

#include <cstdlib>
#include <cmath>


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

//Instances

template<class T>
class Translate : public HittableObject<T> {
public:
    std::shared_ptr<HittableObject<T>> ptr;
    Vector3<T> offset;

    Translate() {}
    Translate(std::shared_ptr<HittableObject<T>> _ptr, const Vector3<T>& _offset) : ptr(_ptr), offset(_offset) {}

    bool bounding_box(T t0, T t1, AABB<T>& out) const override {
        if(!ptr->bounding_box(t0, t1, out))
            return false;

        out = AABB<T>(out.minimum + offset, out.maximum + offset);
        return true;
    }

    bool hit(const Ray<T>& r, T t0, T t1, HitRecord<T>& rec) const noexcept override {
        Ray<T> moved(r.orig - offset, r.dir, r.time);

        if(!ptr->hit(moved, t0, t1, rec))
            return false;

        rec.p += offset;
        rec.set_face_normal(moved, rec.normal);

        return true;
    }
};

template<class T>
class RotateY : public HittableObject<T> {
public:
    std::shared_ptr<HittableObject<T>> ptr;
    T sin_theta, cos_theta;
    bool has_box;
    AABB<T> bbox;

    RotateY() {}
    RotateY(std::shared_ptr<HittableObject<T>> _ptr, T angle) : ptr(_ptr) {
        auto radians = degrees_to_radians(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        has_box = ptr->bounding_box(0, 1, bbox);

        Vector3<T> min(infinity, infinity, infinity);
        Vector3<T> max(-infinity, -infinity, -infinity);

        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                for(int k = 0; k < 2; k++) {
                    auto x = i*bbox.maximum.x() + (1 - i)*bbox.minimum.x();
                    auto y = j*bbox.maximum.y() + (1 - j)*bbox.minimum.y();
                    auto z = k*bbox.maximum.z() + (1 - k)*bbox.minimum.z();

                    auto newx = cos_theta*x + sin_theta*z;
                    auto newz = -sin_theta*x + cos_theta*z;

                    Vector3<T> tmp(newx, y, newz);

                    for(int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tmp[c]);
                        max[c] = fmax(max[c], tmp[c]);
                    }
                }
            }
        }

        bbox = AABB<T>(min, max);
    }

    bool bounding_box(T, T, AABB<T>& out) const override {
        out = bbox;
        return has_box;
    }

    bool hit(const Ray<T>& r, T t0, T t1, HitRecord<T>& rec) const noexcept override {
        auto origin = r.orig;
        auto direction = r.dir;

        origin[0] = cos_theta*r.orig[0] - sin_theta*r.orig[2];
        origin[2] = sin_theta*r.orig[0] + cos_theta*r.orig[2];

        direction[0] = cos_theta*r.dir[0] - sin_theta*r.dir[2];
        direction[2] = sin_theta*r.dir[0] + cos_theta*r.dir[2];

        Ray<T> rotated(origin, direction, r.time);

        if(!ptr->hit(rotated, t0, t1, rec))
            return false;

        auto p = rec.p;
        auto normal = rec.normal;

        p[0] =  cos_theta*rec.p[0] + sin_theta*rec.p[2];
        p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

        normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
        normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

        rec.p = p;
        rec.set_face_normal(rotated, normal);

        return true;
    }
};
