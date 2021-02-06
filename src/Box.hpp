#pragma once

#include "General.hpp"
#include "HittableList.hpp"
#include "HittableObject.hpp"
#include "AARect.hpp"


template<class T>
class Box : public HittableObject<T> {
public:
    Vector3<T> box_min;
    Vector3<T> box_max;
    HittableList<T> sides;

    Box() {}
    Box(const Vector3<T>&, const Vector3<T>&, std::shared_ptr<Material<T>>);

    bool hit(const Ray<T>& r, T t0, T t1, HitRecord<T>& rec) const noexcept override { return sides.hit(r, t0, t1, rec);}
    bool bounding_box(T, T, AABB<T>& output_box) const override {
        output_box = AABB<T>(box_min, box_max);
        return true;
    }

};

template<class T>
Box<T>::Box(const Vector3<T>& p0, const Vector3<T>& p1, std::shared_ptr<Material<T>> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(std::make_shared<XYRect<T>>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(std::make_shared<XYRect<T>>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(std::make_shared<XZRect<T>>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(std::make_shared<XZRect<T>>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(std::make_shared<YZRect<T>>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(std::make_shared<YZRect<T>>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}
